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
			InitSettings();
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
	private: System::Windows::Forms::Button^ historyClearBtn;
	private: System::Windows::Forms::Button^ buttonFindServer;

	private: System::Windows::Forms::GroupBox^ groupBoxSettings;
	private: System::Windows::Forms::RadioButton^ radioThemeDark;
	private: System::Windows::Forms::RadioButton^ radioThemeLight;
	private: System::Windows::Forms::Label^ labelTheme;
	private: System::Windows::Forms::Button^ buttonSaveSettings;
	private: bool isDarkTheme;




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
			this->buttonFindServer = (gcnew System::Windows::Forms::Button());
			this->historyClearBtn = (gcnew System::Windows::Forms::Button());
			this->textBoxSend = (gcnew System::Windows::Forms::TextBox());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->groupBoxConnect = (gcnew System::Windows::Forms::GroupBox());
			this->buttonConnect = (gcnew System::Windows::Forms::Button());
			this->labelPort = (gcnew System::Windows::Forms::Label());
			this->labelServerIp = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->textBoxServerIp = (gcnew System::Windows::Forms::TextBox());
			this->richTextChat = (gcnew System::Windows::Forms::RichTextBox());
			this->groupBoxSettings = (gcnew System::Windows::Forms::GroupBox());
			this->radioThemeDark = (gcnew System::Windows::Forms::RadioButton());
			this->radioThemeLight = (gcnew System::Windows::Forms::RadioButton());
			this->labelTheme = (gcnew System::Windows::Forms::Label());
			this->buttonSaveSettings = (gcnew System::Windows::Forms::Button());
			this->panelRight->SuspendLayout();
			this->groupBoxConnect->SuspendLayout();
			this->groupBoxSettings->SuspendLayout();
			this->SuspendLayout();
			// 
			// labelStatus
			// 
			this->labelStatus->AutoSize = true;
			this->labelStatus->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.5F, System::Drawing::FontStyle::Bold));
			this->labelStatus->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(211)), static_cast<System::Int32>(static_cast<System::Byte>(47)),
				static_cast<System::Int32>(static_cast<System::Byte>(47)));
			this->labelStatus->Location = System::Drawing::Point(18, 16);
			this->labelStatus->Name = L"labelStatus";
			this->labelStatus->Size = System::Drawing::Size(150, 19);
			this->labelStatus->TabIndex = 0;
			this->labelStatus->Text = L"● Не подключено";
			// 
			// labelSend
			// 
			this->labelSend->AutoSize = true;
			this->labelSend->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular));
			this->labelSend->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(60)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(60)));
			this->labelSend->Location = System::Drawing::Point(0, 10);
			this->labelSend->Name = L"labelSend";
			this->labelSend->Size = System::Drawing::Size(80, 17);
			this->labelSend->TabIndex = 0;
			this->labelSend->Text = L"Сообщение";
			// 
			// panelRight
			// 
			this->panelRight->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->panelRight->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(245)), static_cast<System::Int32>(static_cast<System::Byte>(246)),
				static_cast<System::Int32>(static_cast<System::Byte>(248)));
			this->panelRight->Controls->Add(this->buttonFindServer);
			this->panelRight->Controls->Add(this->historyClearBtn);
			this->panelRight->Controls->Add(this->labelSend);
			this->panelRight->Controls->Add(this->textBoxSend);
			this->panelRight->Controls->Add(this->buttonSendMsg);
			this->panelRight->Controls->Add(this->groupBoxConnect);
			this->panelRight->Controls->Add(this->groupBoxSettings);
			this->panelRight->Location = System::Drawing::Point(380, 16);
			this->panelRight->Name = L"panelRight";
			this->panelRight->Size = System::Drawing::Size(320, 540);
			this->panelRight->TabIndex = 2;
			// 
			// buttonFindServer
			// 
			this->buttonFindServer->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(230)), static_cast<System::Int32>(static_cast<System::Byte>(236)),
				static_cast<System::Int32>(static_cast<System::Byte>(245)));
			this->buttonFindServer->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonFindServer->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Regular));
			this->buttonFindServer->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(90)));
			this->buttonFindServer->Location = System::Drawing::Point(180, 148);
			this->buttonFindServer->Name = L"buttonFindServer";
			this->buttonFindServer->Size = System::Drawing::Size(120, 34);
			this->buttonFindServer->TabIndex = 5;
			this->buttonFindServer->Text = L"Поиск сервера";
			this->buttonFindServer->UseVisualStyleBackColor = false;
			this->buttonFindServer->Click += gcnew System::EventHandler(this, &ClientForm::buttonFindServer_Click);
			// 
			// historyClearBtn
			// 
			this->historyClearBtn->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(230)), static_cast<System::Int32>(static_cast<System::Byte>(236)),
				static_cast<System::Int32>(static_cast<System::Byte>(245)));
			this->historyClearBtn->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->historyClearBtn->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Regular));
			this->historyClearBtn->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(40)), static_cast<System::Int32>(static_cast<System::Byte>(60)),
				static_cast<System::Int32>(static_cast<System::Byte>(90)));
			this->historyClearBtn->Location = System::Drawing::Point(180, 108);
			this->historyClearBtn->Name = L"historyClearBtn";
			this->historyClearBtn->Size = System::Drawing::Size(120, 34);
			this->historyClearBtn->TabIndex = 4;
			this->historyClearBtn->Text = L"Очистить историю";
			this->historyClearBtn->UseVisualStyleBackColor = false;
			this->historyClearBtn->Click += gcnew System::EventHandler(this, &ClientForm::historyClearBtn_Click);
			// 
			// textBoxSend
			// 
			this->textBoxSend->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxSend->Enabled = false;
			this->textBoxSend->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular));
			this->textBoxSend->Location = System::Drawing::Point(0, 32);
			this->textBoxSend->Name = L"textBoxSend";
			this->textBoxSend->Size = System::Drawing::Size(300, 25);
			this->textBoxSend->TabIndex = 0;
			this->textBoxSend->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &ClientForm::textBoxSend_KeyDown);
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(120)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)));
			this->buttonSendMsg->Enabled = false;
			this->buttonSendMsg->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonSendMsg->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Bold));
			this->buttonSendMsg->ForeColor = System::Drawing::Color::White;
			this->buttonSendMsg->Location = System::Drawing::Point(0, 64);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(160, 34);
			this->buttonSendMsg->TabIndex = 1;
			this->buttonSendMsg->Text = L"Отправить";
			this->buttonSendMsg->UseVisualStyleBackColor = false;
			this->buttonSendMsg->Click += gcnew System::EventHandler(this, &ClientForm::buttonSendMsg_Click);
			// 
			// groupBoxConnect
			// 
			this->groupBoxConnect->Controls->Add(this->buttonConnect);
			this->groupBoxConnect->Controls->Add(this->labelPort);
			this->groupBoxConnect->Controls->Add(this->labelServerIp);
			this->groupBoxConnect->Controls->Add(this->textBoxPort);
			this->groupBoxConnect->Controls->Add(this->textBoxServerIp);
			this->groupBoxConnect->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.5F, System::Drawing::FontStyle::Bold));
			this->groupBoxConnect->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(55)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->groupBoxConnect->Location = System::Drawing::Point(0, 212);
			this->groupBoxConnect->Name = L"groupBoxConnect";
			this->groupBoxConnect->Size = System::Drawing::Size(320, 150);
			this->groupBoxConnect->TabIndex = 3;
			this->groupBoxConnect->TabStop = false;
			this->groupBoxConnect->Text = L"Подключение";
			// 
			// buttonConnect
			// 
			this->buttonConnect->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(120)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)));
			this->buttonConnect->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonConnect->Font = (gcnew System::Drawing::Font(L"Segoe UI", 11.5F, System::Drawing::FontStyle::Bold));
			this->buttonConnect->ForeColor = System::Drawing::Color::White;
			this->buttonConnect->Location = System::Drawing::Point(160, 55);
			this->buttonConnect->Name = L"buttonConnect";
			this->buttonConnect->Size = System::Drawing::Size(140, 80);
			this->buttonConnect->TabIndex = 4;
			this->buttonConnect->Text = L"ПОЙТИ";
			this->buttonConnect->UseVisualStyleBackColor = false;
			this->buttonConnect->Click += gcnew System::EventHandler(this, &ClientForm::buttonConnect_Click);
			// 
			// labelPort
			// 
			this->labelPort->AutoSize = true;
			this->labelPort->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Regular));
			this->labelPort->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(90)), static_cast<System::Int32>(static_cast<System::Byte>(95)),
				static_cast<System::Int32>(static_cast<System::Byte>(105)));
			this->labelPort->Location = System::Drawing::Point(16, 96);
			this->labelPort->Name = L"labelPort";
			this->labelPort->Size = System::Drawing::Size(38, 17);
			this->labelPort->TabIndex = 7;
			this->labelPort->Text = L"Порт";
			// 
			// labelServerIp
			// 
			this->labelServerIp->AutoSize = true;
			this->labelServerIp->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Regular));
			this->labelServerIp->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(90)), static_cast<System::Int32>(static_cast<System::Byte>(95)),
				static_cast<System::Int32>(static_cast<System::Byte>(105)));
			this->labelServerIp->Location = System::Drawing::Point(16, 36);
			this->labelServerIp->Name = L"labelServerIp";
			this->labelServerIp->Size = System::Drawing::Size(66, 17);
			this->labelServerIp->TabIndex = 6;
			this->labelServerIp->Text = L"IP сервера";
			// 
			// textBoxPort
			// 
			this->textBoxPort->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxPort->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular));
			this->textBoxPort->Location = System::Drawing::Point(16, 115);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(125, 25);
			this->textBoxPort->TabIndex = 5;
			this->textBoxPort->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ClientForm::textBoxPort_KeyPress);
			// 
			// textBoxServerIp
			// 
			this->textBoxServerIp->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->textBoxServerIp->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular));
			this->textBoxServerIp->Location = System::Drawing::Point(16, 55);
			this->textBoxServerIp->Name = L"textBoxServerIp";
			this->textBoxServerIp->Size = System::Drawing::Size(125, 25);
			this->textBoxServerIp->TabIndex = 4;
			// 
			// richTextChat
			// 
			this->richTextChat->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(28)), static_cast<System::Int32>(static_cast<System::Byte>(32)),
				static_cast<System::Int32>(static_cast<System::Byte>(38)));
			this->richTextChat->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->richTextChat->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10));
			this->richTextChat->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(225)), static_cast<System::Int32>(static_cast<System::Byte>(230)),
				static_cast<System::Int32>(static_cast<System::Byte>(235)));
			this->richTextChat->Location = System::Drawing::Point(16, 44);
			this->richTextChat->Name = L"richTextChat";
			this->richTextChat->ReadOnly = true;
			this->richTextChat->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Vertical;
			this->richTextChat->Size = System::Drawing::Size(348, 512);
			this->richTextChat->TabIndex = 2;
			this->richTextChat->Text = L"";
			// 
			// groupBoxSettings
			// 
			this->groupBoxSettings->Controls->Add(this->buttonSaveSettings);
			this->groupBoxSettings->Controls->Add(this->radioThemeLight);
			this->groupBoxSettings->Controls->Add(this->radioThemeDark);
			this->groupBoxSettings->Controls->Add(this->labelTheme);
			this->groupBoxSettings->Font = (gcnew System::Drawing::Font(L"Segoe UI", 10.5F, System::Drawing::FontStyle::Bold));
			this->groupBoxSettings->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(55)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->groupBoxSettings->Location = System::Drawing::Point(0, 372);
			this->groupBoxSettings->Name = L"groupBoxSettings";
			this->groupBoxSettings->Size = System::Drawing::Size(320, 160);
			this->groupBoxSettings->TabIndex = 6;
			this->groupBoxSettings->TabStop = false;
			this->groupBoxSettings->Text = L"Настройки";
			// 
			// labelTheme
			// 
			this->labelTheme->AutoSize = true;
			this->labelTheme->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.5F, System::Drawing::FontStyle::Regular));
			this->labelTheme->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(90)), static_cast<System::Int32>(static_cast<System::Byte>(95)),
				static_cast<System::Int32>(static_cast<System::Byte>(105)));
			this->labelTheme->Location = System::Drawing::Point(16, 34);
			this->labelTheme->Name = L"labelTheme";
			this->labelTheme->Size = System::Drawing::Size(74, 17);
			this->labelTheme->TabIndex = 0;
			this->labelTheme->Text = L"Тема чата";
			// 
			// radioThemeDark
			// 
			this->radioThemeDark->AutoSize = true;
			this->radioThemeDark->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular));
			this->radioThemeDark->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(55)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->radioThemeDark->Location = System::Drawing::Point(16, 64);
			this->radioThemeDark->Name = L"radioThemeDark";
			this->radioThemeDark->Size = System::Drawing::Size(68, 21);
			this->radioThemeDark->TabIndex = 1;
			this->radioThemeDark->Text = L"Тёмная";
			this->radioThemeDark->UseVisualStyleBackColor = true;
			this->radioThemeDark->CheckedChanged += gcnew System::EventHandler(this, &ClientForm::radioTheme_CheckedChanged);
			// 
			// radioThemeLight
			// 
			this->radioThemeLight->AutoSize = true;
			this->radioThemeLight->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Regular));
			this->radioThemeLight->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(45)), static_cast<System::Int32>(static_cast<System::Byte>(55)),
				static_cast<System::Int32>(static_cast<System::Byte>(70)));
			this->radioThemeLight->Location = System::Drawing::Point(100, 64);
			this->radioThemeLight->Name = L"radioThemeLight";
			this->radioThemeLight->Size = System::Drawing::Size(74, 21);
			this->radioThemeLight->TabIndex = 2;
			this->radioThemeLight->Text = L"Светлая";
			this->radioThemeLight->UseVisualStyleBackColor = true;
			this->radioThemeLight->CheckedChanged += gcnew System::EventHandler(this, &ClientForm::radioTheme_CheckedChanged);
			// 
			// buttonSaveSettings
			// 
			this->buttonSaveSettings->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(52)), static_cast<System::Int32>(static_cast<System::Byte>(120)),
				static_cast<System::Int32>(static_cast<System::Byte>(196)));
			this->buttonSaveSettings->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->buttonSaveSettings->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9.75F, System::Drawing::FontStyle::Bold));
			this->buttonSaveSettings->ForeColor = System::Drawing::Color::White;
			this->buttonSaveSettings->Location = System::Drawing::Point(16, 110);
			this->buttonSaveSettings->Name = L"buttonSaveSettings";
			this->buttonSaveSettings->Size = System::Drawing::Size(284, 34);
			this->buttonSaveSettings->TabIndex = 3;
			this->buttonSaveSettings->Text = L"Сохранить настройки";
			this->buttonSaveSettings->UseVisualStyleBackColor = false;
			this->buttonSaveSettings->Click += gcnew System::EventHandler(this, &ClientForm::buttonSaveSettings_Click);
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(240)), static_cast<System::Int32>(static_cast<System::Byte>(242)),
				static_cast<System::Int32>(static_cast<System::Byte>(245)));
			this->ClientSize = System::Drawing::Size(716, 572);
			this->Controls->Add(this->labelStatus);
			this->Controls->Add(this->richTextChat);
			this->Controls->Add(this->panelRight);
			this->Font = (gcnew System::Drawing::Font(L"Segoe UI", 9));
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"ClientForm";
			this->Text = L"Aloha Messenger";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ClientForm::ClientForm_FormClosing);
			this->panelRight->ResumeLayout(false);
			this->panelRight->PerformLayout();
			this->groupBoxConnect->ResumeLayout(false);
			this->groupBoxConnect->PerformLayout();
			this->groupBoxSettings->ResumeLayout(false);
			this->groupBoxSettings->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: enum class ConnState { Wait, Online, Offline };
	private: System::Void setState(ConnState st, System::String^ text);
	private: System::Void SetOnlineState();
	private: System::Void find_server_thread();
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
	private: System::Void historyClearBtn_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void buttonFindServer_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void InitSettings();
	private: System::Void ApplyTheme(bool dark);
	private: System::Void SaveSettings();
	private: System::String^ GetSettingsPath();
	private: System::Drawing::Color AdjustColorForTheme(System::Drawing::Color c);
	private: System::Void radioTheme_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
	private: System::Void buttonSaveSettings_Click(System::Object^ sender, System::EventArgs^ e);
	};

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
}