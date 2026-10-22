#pragma once
#include <string>
#include <SocketWrapper/Client.h>

namespace AlohaClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public delegate void log_update(System::String^ text, System::Drawing::Color);

	public ref class ConnectInfo : public System::Object {
	public:
		System::String^ host;
		int serverPort;
		int clientPort;
		ConnectInfo(System::String^ host, int sPort, int cPort)
			: host(host), serverPort(sPort), clientPort(cPort) {
		}
	};

	/// <summary>
	/// Сводка для ClientForm
	/// </summary>
	public ref class ClientForm : public System::Windows::Forms::Form
	{
	public:
		ClientForm(void)
		{
			InitializeComponent();
			worker = nullptr;
			is_connected = false;
			client = nullptr;
			setState(ConnState::Offline, L"Не подключено");
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~ClientForm()
		{
			if (components)
			{
				delete components;
			}
			client_shutdown();
		}
	private: System::Windows::Forms::Label^ labelStatus;
	private: System::Windows::Forms::Label^ labelSend;
	private: System::Windows::Forms::Panel^ panelRight;
	private: System::Windows::Forms::TextBox^ textBoxSend;
	private: System::Windows::Forms::Button^ buttonSendMsg;
	private: System::Windows::Forms::RichTextBox^ richTextChat;
	private: System::Windows::Forms::GroupBox^ groupBoxConnect;
	private: System::Windows::Forms::Label^ labelServerIp;
	private: System::Windows::Forms::TextBox^ textBoxPort;
	private: System::Windows::Forms::TextBox^ textBoxServerIp;
	private: System::Windows::Forms::Button^ buttonConnect;
	private: System::Windows::Forms::Label^ labelPort;

	private: System::Threading::Thread^ worker;
	private: bool is_connected;
	private: ClientSocket* client;
	protected:

	protected:


	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->labelStatus = (gcnew System::Windows::Forms::Label());
			this->labelSend = (gcnew System::Windows::Forms::Label());
			this->panelRight = (gcnew System::Windows::Forms::Panel());
			this->textBoxSend = (gcnew System::Windows::Forms::TextBox());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->richTextChat = (gcnew System::Windows::Forms::RichTextBox());
			this->groupBoxConnect = (gcnew System::Windows::Forms::GroupBox());
			this->buttonConnect = (gcnew System::Windows::Forms::Button());
			this->labelPort = (gcnew System::Windows::Forms::Label());
			this->labelServerIp = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->textBoxServerIp = (gcnew System::Windows::Forms::TextBox());
			this->panelRight->SuspendLayout();
			this->groupBoxConnect->SuspendLayout();
			this->SuspendLayout();
			// 
			// labelStatus
			// 
			this->labelStatus->AutoSize = true;
			this->labelStatus->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10, System::Drawing::FontStyle::Bold));
			this->labelStatus->Location = System::Drawing::Point(14, 12);
			this->labelStatus->Name = L"labelStatus";
			this->labelStatus->TabIndex = 0;
			this->labelStatus->Text = L"● Не подключено";
			this->labelStatus->ForeColor = System::Drawing::Color::Red;
			// 
			// panelRight
			// 
			this->panelRight->Anchor = System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right;
			this->panelRight->Location = System::Drawing::Point(365, 12);
			this->panelRight->Name = L"panelRight";
			this->panelRight->Size = System::Drawing::Size(320, 500);
			this->panelRight->TabIndex = 2;
			// 
			// labelSend
			// 
			this->labelSend->AutoSize = true;
			this->labelSend->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 10));
			this->labelSend->Location = System::Drawing::Point(0, 4);
			this->labelSend->Name = L"labelSend";
			this->labelSend->Text = L"Сообщение:";
			this->labelSend->TabIndex = 0;
			// 
			// textBoxSend
			// 
			this->textBoxSend->Location = System::Drawing::Point(0, 28);
			this->textBoxSend->Name = L"textBoxSend";
			this->textBoxSend->Size = System::Drawing::Size(220, 20);
			this->textBoxSend->TabIndex = 0;
			this->textBoxSend->Enabled = false;
			this->textBoxSend->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ClientForm::textBoxSend_KeyDown);
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->buttonSendMsg->Location = System::Drawing::Point(228, 27);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(92, 26);
			this->buttonSendMsg->TabIndex = 1;
			this->buttonSendMsg->Text = L"ПОСЛАТЬ";
			this->buttonSendMsg->UseVisualStyleBackColor = true;
			this->buttonSendMsg->Enabled = false;
			this->buttonSendMsg->Click += gcnew System::EventHandler(this, &ClientForm::buttonSendMsg_Click);
			this->panelRight->Controls->Add(this->labelSend);
			this->panelRight->Controls->Add(this->textBoxSend);
			this->panelRight->Controls->Add(this->buttonSendMsg);
			// 
			// richTextChat
			// 
			this->richTextChat->BackColor = System::Drawing::Color::FromArgb(11, 61, 11);
			this->richTextChat->ForeColor = System::Drawing::Color::White;
			this->richTextChat->Location = System::Drawing::Point(11, 38);
			this->richTextChat->Name = L"richTextChat";
			this->richTextChat->ReadOnly = true;
			this->richTextChat->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->richTextChat->Size = System::Drawing::Size(340, 490);
			this->richTextChat->TabIndex = 2;
			this->richTextChat->Text = L"";
			// 
			// groupBoxConnect
			// 
			this->groupBoxConnect->Controls->Add(this->buttonConnect);
			this->groupBoxConnect->Controls->Add(this->labelPort);
			this->groupBoxConnect->Controls->Add(this->labelServerIp);
			this->groupBoxConnect->Controls->Add(this->textBoxPort);
			this->groupBoxConnect->Controls->Add(this->textBoxServerIp);
			this->groupBoxConnect->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->groupBoxConnect->Location = System::Drawing::Point(0, 262);
			this->groupBoxConnect->Name = L"groupBoxConnect";
			this->groupBoxConnect->Size = System::Drawing::Size(320, 194);
			this->groupBoxConnect->TabIndex = 3;
			this->groupBoxConnect->TabStop = false;
			this->groupBoxConnect->Text = L"Подключение";
			this->panelRight->Controls->Add(this->groupBoxConnect);
			// 
			// buttonConnect
			// 
			this->buttonConnect->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->buttonConnect->Location = System::Drawing::Point(143, 60);
			this->buttonConnect->Name = L"buttonConnect";
			this->buttonConnect->Size = System::Drawing::Size(110, 108);
			this->buttonConnect->TabIndex = 4;
			this->buttonConnect->Text = L"ПОЙТИ";
			this->buttonConnect->UseVisualStyleBackColor = true;
			this->buttonConnect->Click += gcnew System::EventHandler(this, &ClientForm::buttonConnect_Click);
			// 
			// labelPort
			// 
			this->labelPort->AutoSize = true;
			this->labelPort->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelPort->Location = System::Drawing::Point(25, 110);
			this->labelPort->Name = L"labelPort";
			this->labelPort->Size = System::Drawing::Size(40, 16);
			this->labelPort->TabIndex = 7;
			this->labelPort->Text = L"Порт";
			// 
			// labelServerIp
			// 
			this->labelServerIp->AutoSize = true;
			this->labelServerIp->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelServerIp->Location = System::Drawing::Point(25, 49);
			this->labelServerIp->Name = L"labelServerIp";
			this->labelServerIp->Size = System::Drawing::Size(77, 16);
			this->labelServerIp->TabIndex = 6;
			this->labelServerIp->Text = L"IP сервера";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(18, 129);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(119, 26);
			this->textBoxPort->TabIndex = 5;
			this->textBoxPort->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ClientForm::textBoxPort_KeyPress);
			// 
			// textBoxServerIp
			// 
			this->textBoxServerIp->Location = System::Drawing::Point(18, 72);
			this->textBoxServerIp->Name = L"textBoxServerIp";
			this->textBoxServerIp->Size = System::Drawing::Size(119, 26);
			this->textBoxServerIp->TabIndex = 4;
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(701, 547);
			this->Controls->Add(this->labelStatus);
			this->Controls->Add(this->richTextChat);
			this->Controls->Add(this->panelRight);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ClientForm";
			this->Text = L"ClientForm";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ClientForm::ClientForm_FormClosing);
			this->panelRight->ResumeLayout(false);
			this->panelRight->PerformLayout();
			this->groupBoxConnect->ResumeLayout(false);
			this->groupBoxConnect->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: enum class ConnState { Wait, Online, Offline };
	private: System::Void setState(ConnState st, System::String^ text);
	private: System::Void SetOnlineState();
	private: System::Void SetOfflineState();
	private: System::Void client_shutdown();
	private: System::Void client_run(System::Object^ obj);
	private: System::Void addLine(const std::string& text, System::Drawing::Color color);
	private: System::Void addLineLog(System::String^ line, System::Drawing::Color color);
	private: System::Void buttonConnect_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void buttonSendMsg_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void textBoxPort_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
	private: System::Void textBoxSend_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e);
	private: System::Void ClientForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
	};
}