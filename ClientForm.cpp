#include "ClientForm.h"
#include <msclr/marshal_cppstd.h>

using namespace AlohaClient;
using namespace System::Drawing;
using namespace System::Windows::Forms;

char specialKey = 'L';

std::string xorEnc(std::string text, char key) {
    std::string result = text;
    for (char& c : result) {
        c ^= key;
    }
    return result;
}

std::string encrypted(std::string text, char key) {
    std::string msg = xorEnc(text, key);
    return msg;
}
std::string decrypted(std::string text, char key) {
    std::string msg = xorEnc(text, key);
    return msg;
}

std::string get_broadcast_ip() {
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0)
        return "";
    hostent* host = gethostbyname(hostname);
    if (!host || !host->h_addr_list[0])
        return "";

    in_addr addr;
    memcpy(&addr, host->h_addr_list[0], sizeof(in_addr));
    std::string myIp = inet_ntoa(addr);

    size_t lastDot = myIp.find_last_of('.');
    if (lastDot == std::string::npos)
        return "";

    return myIp.substr(0, lastDot + 1) + "255";
}

std::string find_server(int timeoutMs) {
    std::string bcast = get_broadcast_ip();

    SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
        return "";

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char*)&opt, sizeof(opt));

    DWORD timeout = timeoutMs;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

    sockaddr_in addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8888);
    addr.sin_addr.s_addr = inet_addr(bcast.c_str());

    const char* request = "FIND_SERVER";
    if (sendto(sock, request, (int)strlen(request), 0, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
    {
        closesocket(sock);
        return "";
    }

    char buffer[128];
    sockaddr_in fromAddr;
    int fromLen = sizeof(fromAddr);

    int bytes = recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
        (sockaddr*)&fromAddr, &fromLen);

    closesocket(sock);

    if (bytes <= 0)
        return "";

    buffer[bytes] = '\0';

    if (strcmp(buffer, "SERVER_HERE") == 0) {
        return std::string(inet_ntoa(fromAddr.sin_addr));
    }

    return "";
}

System::Void ClientForm::find_server_thread() {
    WsaInitializer wsa;
    std::string ip = find_server(2000);

    if (!ip.empty()) {
        System::String^ foundIp = msclr::interop::marshal_as<System::String^>(ip);

        this->Invoke(gcnew log_update(this, &ClientForm::addLineLog), "Сервер найден: " + foundIp, Color::White);
    }
    else {
        this->Invoke(gcnew log_update(this, &ClientForm::addLineLog), "Сервер не найден", Color::Red);
    }
}

System::Void ClientForm::setState(ConnState st, System::String^ text)
{
    switch (st)
    {
    case ConnState::Online:
        labelStatus->ForeColor = Color::FromArgb(46, 160, 67);
        buttonSendMsg->Enabled = true;
        textBoxSend->Enabled = true;
        textBoxServerIp->Enabled = false;
        textBoxPort->Enabled = false;
        buttonConnect->Text = L"УЙТИ";
        is_connected = true;
        break;
    case ConnState::Wait:
        labelStatus->ForeColor = Color::FromArgb(196, 140, 40);
        buttonSendMsg->Enabled = false;
        textBoxSend->Enabled = false;
        textBoxServerIp->Enabled = false;
        textBoxPort->Enabled = false;
        buttonConnect->Text = L"ОТМЕНА";
        is_connected = false;
        break;
    case ConnState::Offline:
    default:
        labelStatus->ForeColor = Color::FromArgb(211, 47, 47);
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
    richTextChat->SelectionColor = AdjustColorForTheme(color);
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
    std::string crypt = xorEnc(message, specialKey);


    try {
        client->send(crypt);
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
            std::string decrypt = xorEnc(raw, specialKey);
            if (raw.empty()) break;
            System::String^ msg = msclr::interop::marshal_as<System::String^>(decrypt);
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

System::Void AlohaClient::ClientForm::historyClearBtn_Click(System::Object^ sender, System::EventArgs^ e) {
    this->richTextChat->Clear();
    this->richTextChat->ClearUndo();
}

System::Void AlohaClient::ClientForm::buttonFindServer_Click(System::Object^ sender, System::EventArgs^ e)
{
    addLineLog("Поиск сервера...", Color::Silver);

    System::Threading::Thread^ thread = gcnew System::Threading::Thread(
        gcnew System::Threading::ThreadStart(this, &ClientForm::find_server_thread)
    );
    thread->IsBackground = true;
    thread->Start();
}

System::String^ ClientForm::GetSettingsPath() {
    return System::IO::Path::Combine(
        System::IO::Path::GetDirectoryName(System::Windows::Forms::Application::ExecutablePath),
        L"client_settings.ini");
}

System::Void ClientForm::ApplyTheme(bool dark) {
    isDarkTheme = dark;

    if (dark) {
        richTextChat->BackColor = Color::FromArgb(28, 32, 38);
        richTextChat->ForeColor = Color::FromArgb(225, 230, 235);
    }
    else {
        richTextChat->BackColor = Color::White;
        richTextChat->ForeColor = Color::FromArgb(30, 30, 30);
    }

    radioThemeDark->CheckedChanged -= gcnew System::EventHandler(this, &ClientForm::radioTheme_CheckedChanged);
    radioThemeLight->CheckedChanged -= gcnew System::EventHandler(this, &ClientForm::radioTheme_CheckedChanged);

    radioThemeDark->Checked = dark;
    radioThemeLight->Checked = !dark;

    radioThemeDark->CheckedChanged += gcnew System::EventHandler(this, &ClientForm::radioTheme_CheckedChanged);
    radioThemeLight->CheckedChanged += gcnew System::EventHandler(this, &ClientForm::radioTheme_CheckedChanged);
}

System::Void ClientForm::InitSettings() {
    bool dark = true;

    try {
        System::String^ path = GetSettingsPath();
        if (System::IO::File::Exists(path)) {
            array<System::String^>^ lines = System::IO::File::ReadAllLines(path);
            for each (System::String ^ line in lines) {
                if (line->StartsWith(L"Theme=")) {
                    System::String^ val = line->Substring(6)->Trim()->ToLower();
                    dark = (val != L"light");
                }
            }
        }
    }
    catch (System::Exception^ ex) {
        addLineLog(L"[настройки] Не удалось загрузить конфиг: " + ex->Message, Color::Goldenrod);
    }

    ApplyTheme(dark);
}

System::Void ClientForm::SaveSettings() {
    try {
        System::String^ path = GetSettingsPath();
        System::String^ theme = isDarkTheme ? L"Dark" : L"Light";
        System::String^ content = L"[UI]\r\nTheme=" + theme + L"\r\n";
        System::IO::File::WriteAllText(path, content, System::Text::Encoding::UTF8);
        addLineLog(L"[настройки] Тема сохранена: " + (isDarkTheme ? L"Тёмная" : L"Светлая"), Color::Silver);
    }
    catch (System::Exception^ ex) {
        MessageBox::Show(L"Не удалось сохранить настройки: " + ex->Message,
            L"Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
    }
}

System::Void ClientForm::radioTheme_CheckedChanged(System::Object^ sender, System::EventArgs^ e) {
    if (radioThemeDark->Checked)
        ApplyTheme(true);
    else if (radioThemeLight->Checked)
        ApplyTheme(false);
}

System::Void ClientForm::buttonSaveSettings_Click(System::Object^ sender, System::EventArgs^ e) {
    SaveSettings();
}

System::Drawing::Color ClientForm::AdjustColorForTheme(System::Drawing::Color c) {
    if (!isDarkTheme) {
        if (c == Color::White || c == Color::Silver)
            return Color::FromArgb(30, 30, 30);
        if (c == Color::LimeGreen)
            return Color::FromArgb(46, 160, 67);
        if (c == Color::Goldenrod)
            return Color::FromArgb(180, 120, 20);
        if (c == Color::Tomato || c == Color::Red)
            return Color::FromArgb(200, 40, 40);
    }
    else {
        if (c == Color::Black) return Color::FromArgb(225, 230, 235);
        if (c == Color::DarkGreen) return Color::LimeGreen;
        if (c == Color::DarkGoldenrod) return Color::Goldenrod;
        if (c == Color::DarkRed) return Color::Tomato;
        if (c == Color::Red) return Color::FromArgb(255, 110, 110);
    }
    return c;
}