#include "ClientForm.h"
#include <msclr/marshal_cppstd.h>

using namespace AlohaClient;
using namespace System::Drawing;
using namespace System::Windows::Forms;

System::Void ClientForm::setState(ConnState st, System::String^ text)
{
    switch (st)
    {
    case ConnState::Online:
        labelStatus->ForeColor = Color::LimeGreen;
        buttonSendMsg->Enabled = true;
        textBoxSend->Enabled = true;
        textBoxServerIp->Enabled = false;
        textBoxPort->Enabled = false;
        buttonConnect->Text = L"УЙТИ";
        is_connected = true;
        break;
    case ConnState::Wait:
        labelStatus->ForeColor = Color::Goldenrod;
        buttonSendMsg->Enabled = false;
        textBoxSend->Enabled = false;
        textBoxServerIp->Enabled = false;
        textBoxPort->Enabled = false;
        buttonConnect->Text = L"ОТМЕНА";
        is_connected = false;
        break;
    case ConnState::Offline:
    default:
        labelStatus->ForeColor = Color::Red;
        buttonSendMsg->Enabled = false;
        textBoxSend->Enabled = false;
        textBoxServerIp->Enabled = true;
        textBoxPort->Enabled = true;
        buttonConnect->Text = L"ПОЙТИ";
        is_connected = false;
        break;
    }
    labelStatus->Text = L"● " + text;
}

System::Void ClientForm::SetOnlineState() {
    setState(ConnState::Online, L"Онлайн");
}

System::Void ClientForm::SetOfflineState() {
    setState(ConnState::Offline, L"Не подключено");
}

System::Void ClientForm::addLineLog(System::String^ line, System::Drawing::Color color)
{
    if (richTextChat->IsDisposed) return;
    richTextChat->SelectionStart = richTextChat->TextLength;
    richTextChat->SelectionLength = 0;
    richTextChat->SelectionColor = color;
    richTextChat->AppendText(line + System::Environment::NewLine);
    richTextChat->ScrollToCaret();
}

System::Void ClientForm::addLine(const std::string& text, System::Drawing::Color color)
{
    if (this->IsDisposed || !this->IsHandleCreated) return;
    System::String^ sysText = msclr::interop::marshal_as<System::String^>(text);
    if (this->InvokeRequired)
        this->BeginInvoke(gcnew log_update(this, &ClientForm::addLineLog), sysText, color);
    else
        addLineLog(sysText, color);
}

System::Void ClientForm::textBoxPort_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
{
    if (!Char::IsDigit(e->KeyChar) && e->KeyChar != '\b')
        e->Handled = true;
}

System::Void ClientForm::textBoxSend_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
{
    if (e->KeyCode == Keys::Enter) {
        buttonSendMsg_Click(sender, EventArgs::Empty);
        e->Handled = true;
        e->SuppressKeyPress = true;
    }
}

System::Void ClientForm::buttonSendMsg_Click(System::Object^ sender, System::EventArgs^ e) {
    if (!is_connected || client == nullptr) return;
    if (System::String::IsNullOrWhiteSpace(textBoxSend->Text)) return;

    msclr::interop::marshal_context context;
    std::string message = context.marshal_as<std::string>(textBoxSend->Text);

    try {
        client->send(message);
        addLineLog(L"[Вы] " + textBoxSend->Text, Color::White);
        textBoxSend->Clear();
        textBoxSend->Focus();
    }
    catch (const std::exception& ex) {
        addLineLog("[ошибка] " + msclr::interop::marshal_as<System::String^>(ex.what()), Color::Tomato);
    }
}

System::Void ClientForm::buttonConnect_Click(System::Object^ sender, System::EventArgs^ e) {
    if (is_connected || (worker != nullptr && worker->IsAlive)) {
        client_shutdown();
        setState(ConnState::Offline, L"Отключено");
        addLineLog(L"[система] Вы отключились", Color::Silver);
    }
    else {
        System::String^ ip = textBoxServerIp->Text->Trim();
        System::String^ port = textBoxPort->Text->Trim();
        if (ip->Length == 0 || port->Length == 0) {
            MessageBox::Show(L"Укажите IP и порт", L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
            return;
        }

        setState(ConnState::Wait, L"Подключение...");
        addLineLog(L"[система] Подключение к " + ip + L":" + port, Color::Silver);

        worker = gcnew System::Threading::Thread(
            gcnew System::Threading::ParameterizedThreadStart(this, &ClientForm::client_run));

        ConnectInfo^ info = gcnew ConnectInfo(ip, System::Convert::ToInt32(port), 8000);
        worker->IsBackground = true;
        worker->Start(info);
    }
}

System::Void ClientForm::client_run(System::Object^ obj) {
    ConnectInfo^ info = (ConnectInfo^)obj;
    WsaInitializer wsa;

    msclr::interop::marshal_context context;
    client = new ClientSocket(info->clientPort);

    try {
        client->connect(context.marshal_as<std::string>(info->host), info->serverPort);
        System::String^ connectionMsg = "Подключен к серверу " + info->host + ":" + System::Convert::ToString(info->serverPort);

        this->Invoke(gcnew log_update(this, &ClientForm::addLineLog), connectionMsg, Color::Silver);
        this->Invoke(gcnew Action(this, &ClientForm::SetOnlineState));
    }
    catch (const std::exception& e) {
        this->Invoke(gcnew log_update(this, &ClientForm::addLineLog),
            "[ошибка] " + msclr::interop::marshal_as<System::String^>(e.what()), Color::Tomato);
        this->Invoke(gcnew Action(this, &ClientForm::SetOfflineState));
        return;
    }

    while (is_connected) {
        try {
            std::string raw = client->receive(2048);
            if (raw.empty()) break;
            System::String^ msg = msclr::interop::marshal_as<System::String^>(raw);
            this->Invoke(gcnew log_update(this, &ClientForm::addLineLog), msg, Color::White);
        }
        catch (const NetworkException&) {
            this->Invoke(gcnew log_update(this, &ClientForm::addLineLog),
                "[система] Соединение закрыто сервером", Color::Silver);
            break;
        }
        catch (const std::exception& e) {
            this->Invoke(gcnew log_update(this, &ClientForm::addLineLog),
                "[ошибка] " + msclr::interop::marshal_as<System::String^>(e.what()), Color::Tomato);
            break;
        }
    }

    this->Invoke(gcnew Action(this, &ClientForm::SetOfflineState));
}

System::Void ClientForm::client_shutdown() {
    is_connected = false;

    if (client != nullptr) {
        delete client;
        client = nullptr;
    }

    if (worker != nullptr) {
        if (worker->IsAlive && worker != System::Threading::Thread::CurrentThread) {
            worker->Join(2000);
        }
        worker = nullptr;
    }
}

System::Void ClientForm::ClientForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
    client_shutdown();
}