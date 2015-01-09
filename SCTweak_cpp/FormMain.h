/*************************************************************************
    Copyright (C) 2015 trgs (https://github.com/trgs/)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************/

#pragma once
#include "PatchEngine.h"

namespace SCTweak {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Windows::Forms;
	using namespace System::Threading;
	using namespace System::Diagnostics;


	/// <summary>
	/// Summary for FormMain
	/// </summary>
	public ref class FormMain : System::Windows::Forms::Form
	{
	private: PatchEngine ^pPatchEngine;
	private: Thread ^hPatchWorkerThread;
	private: BackgroundWorker^ bwHotKeys;
	private: System::Windows::Forms::Timer^  timerStateUpdate;
	private: System::Windows::Forms::ComboBox^  cmBoxRoadType;
	private: System::Windows::Forms::TextBox^  txtA;
	private: System::Windows::Forms::TextBox^  txtB;
	private: System::Windows::Forms::TextBox^  txtC;
	private: System::Windows::Forms::CheckBox^  chkAlwaysOnTop;
	private: System::Windows::Forms::Button^  btnApply;
	private: System::Windows::Forms::TextBox^  txtLog;
	private: System::Windows::Forms::TabPage^  tabPage2;
	private: System::Windows::Forms::TextBox^  txtScript;
	private: System::Windows::Forms::Button^  btnRunScript;
	private: System::Windows::Forms::TabControl^  tabControl1;
	private: System::Windows::Forms::TabPage^  tabRoadMod;
	private: System::Windows::Forms::TabPage^  tabLog;
	private: System::Windows::Forms::TabPage^  tabSpeedMod;
	private: System::Windows::Forms::TabPage^  tabAbout;
	private: System::Windows::Forms::NumericUpDown^  numSpeed;
	private: System::Windows::Forms::Panel^  panelStatus;
	private: System::Windows::Forms::Label^  labelUpdatesLink;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::PictureBox^  pictureBox1;


	public:
		FormMain(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//

			LogWrite(L"starting hotkeys...", false);
			this->bwHotKeys = gcnew BackgroundWorker();
			this->bwHotKeys->DoWork += gcnew DoWorkEventHandler( this, &SCTweak::FormMain::bw_HotKeysPoll );
			this->bwHotKeys->ProgressChanged += gcnew ProgressChangedEventHandler( this, &SCTweak::FormMain::bw_HotKeysEvent );
			this->bwHotKeys->WorkerReportsProgress = true;
			this->bwHotKeys->RunWorkerAsync();
			LogWrite(L"ok");

			this->timerStateUpdate->Enabled = true;
		}

		void bw_HotKeysPoll(System::Object^  sender, System::ComponentModel::DoWorkEventArgs^  e)
		{
			// Do some long running task...
			for (int i=1; i<7; i++)
			{
				RegisterHotKey(NULL, i, MOD_CONTROL, 0x30 + i);
				//RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_NOREPEAT, '2');
			}

			RegisterHotKey(NULL, 8, MOD_CONTROL , 0x39); // z for speed mod
			RegisterHotKey(NULL, 9, MOD_CONTROL , 0x30); // x for speedmod

			MSG msg = {0};
			while (bwHotKeys->CancellationPending == false && msg.message != WM_QUIT)
			{
				while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE) > 0)
				{
					if (msg.message == WM_HOTKEY)
					{
						bwHotKeys->ReportProgress(msg.lParam); // sends event
					}
				}
			}
		}

		void bw_HotKeysEvent(System::Object^ sender, System::ComponentModel::ProgressChangedEventArgs^ e) {
			//this->LogWrite("WM_HOTKEY received " + e->ProgressPercentage);
			switch ((e->ProgressPercentage >> 0x10))
			{
			case 0x31: // CTRL + 1
			case 0x32:
			case 0x33:
			case 0x34:
			case 0x35:
			case 0x36:
			//case 0x37:
				   {
					   this->cmBoxRoadType->SelectedIndex = (e->ProgressPercentage >> 0x10) - 0x31;
					   this->LogWrite("hotkey (road type)");
				   } break;
			case 0x39: // CTRL + 9
				   {
					   if (this->numSpeed->Value > 0)
					   {
						   this->numSpeed->Value--;
						   this->LogWrite("hotkey (game speed)");
					   }
				   } break;
			case 0x30: // CTRL + 0
				   {
					   if (this->numSpeed->Value < 100)
					   {
						   this->numSpeed->Value++;
						   this->LogWrite("hotkey (game speed)");
					   }
				   } break;
			}
		}

		void LogWrite(String ^msg)
		{
			this->txtLog->AppendText(msg);
			this->txtLog->AppendText(L"\r\n");
		}

		void LogWrite(String ^msg, bool newLine)
		{
			this->txtLog->AppendText(msg);
			if (newLine == true)
				this->txtLog->AppendText(L"\r\n");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~FormMain()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnStart;
private: System::ComponentModel::IContainer^  components;
	protected: 


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(FormMain::typeid));
			this->btnStart = (gcnew System::Windows::Forms::Button());
			this->timerStateUpdate = (gcnew System::Windows::Forms::Timer(this->components));
			this->cmBoxRoadType = (gcnew System::Windows::Forms::ComboBox());
			this->txtA = (gcnew System::Windows::Forms::TextBox());
			this->txtB = (gcnew System::Windows::Forms::TextBox());
			this->txtC = (gcnew System::Windows::Forms::TextBox());
			this->chkAlwaysOnTop = (gcnew System::Windows::Forms::CheckBox());
			this->btnApply = (gcnew System::Windows::Forms::Button());
			this->txtLog = (gcnew System::Windows::Forms::TextBox());
			this->btnRunScript = (gcnew System::Windows::Forms::Button());
			this->txtScript = (gcnew System::Windows::Forms::TextBox());
			this->tabControl1 = (gcnew System::Windows::Forms::TabControl());
			this->tabRoadMod = (gcnew System::Windows::Forms::TabPage());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->tabSpeedMod = (gcnew System::Windows::Forms::TabPage());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->numSpeed = (gcnew System::Windows::Forms::NumericUpDown());
			this->tabLog = (gcnew System::Windows::Forms::TabPage());
			this->tabAbout = (gcnew System::Windows::Forms::TabPage());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->labelUpdatesLink = (gcnew System::Windows::Forms::Label());
			this->panelStatus = (gcnew System::Windows::Forms::Panel());
			this->tabControl1->SuspendLayout();
			this->tabRoadMod->SuspendLayout();
			this->tabSpeedMod->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numSpeed))->BeginInit();
			this->tabLog->SuspendLayout();
			this->tabAbout->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// btnStart
			// 
			this->btnStart->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->btnStart->BackColor = System::Drawing::SystemColors::Control;
			this->btnStart->Font = (gcnew System::Drawing::Font(L"Rod", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->btnStart->Location = System::Drawing::Point(7, 160);
			this->btnStart->Name = L"btnStart";
			this->btnStart->Size = System::Drawing::Size(246, 23);
			this->btnStart->TabIndex = 0;
			this->btnStart->Text = L"Start";
			this->btnStart->UseVisualStyleBackColor = false;
			this->btnStart->Click += gcnew System::EventHandler(this, &FormMain::btnStart_Click);
			// 
			// timerStateUpdate
			// 
			this->timerStateUpdate->Tick += gcnew System::EventHandler(this, &FormMain::timerStateUpdate_Tick);
			// 
			// cmBoxRoadType
			// 
			this->cmBoxRoadType->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->cmBoxRoadType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->cmBoxRoadType->Enabled = false;
			this->cmBoxRoadType->Items->AddRange(gcnew cli::array< System::Object^  >(6) {L"Default (Ctrl+1)", L"Tunnel (Ctrl+2)", L"OverPass Low (Ctrl+3)", 
				L"OverPass High (Ctrl+4)", L"Elevated (Ctrl+5)", L"Custom (Ctrl+6)"});
			this->cmBoxRoadType->Location = System::Drawing::Point(28, 35);
			this->cmBoxRoadType->Name = L"cmBoxRoadType";
			this->cmBoxRoadType->Size = System::Drawing::Size(211, 21);
			this->cmBoxRoadType->TabIndex = 2;
			this->cmBoxRoadType->SelectedValueChanged += gcnew System::EventHandler(this, &FormMain::cmBoxRoadType_SelectedValueChanged);
			this->cmBoxRoadType->TextChanged += gcnew System::EventHandler(this, &FormMain::cmBoxRoadType_TextChanged);
			// 
			// txtA
			// 
			this->txtA->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->txtA->Enabled = false;
			this->txtA->Location = System::Drawing::Point(28, 62);
			this->txtA->Name = L"txtA";
			this->txtA->Size = System::Drawing::Size(39, 20);
			this->txtA->TabIndex = 3;
			this->txtA->Text = L"-1024";
			this->txtA->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// txtB
			// 
			this->txtB->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->txtB->Enabled = false;
			this->txtB->Location = System::Drawing::Point(73, 62);
			this->txtB->Name = L"txtB";
			this->txtB->Size = System::Drawing::Size(39, 20);
			this->txtB->TabIndex = 4;
			this->txtB->Text = L"2048";
			this->txtB->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// txtC
			// 
			this->txtC->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->txtC->Enabled = false;
			this->txtC->Location = System::Drawing::Point(118, 62);
			this->txtC->Name = L"txtC";
			this->txtC->Size = System::Drawing::Size(41, 20);
			this->txtC->TabIndex = 5;
			this->txtC->Text = L"1024";
			this->txtC->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// chkAlwaysOnTop
			// 
			this->chkAlwaysOnTop->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->chkAlwaysOnTop->AutoSize = true;
			this->chkAlwaysOnTop->Checked = true;
			this->chkAlwaysOnTop->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkAlwaysOnTop->Location = System::Drawing::Point(7, 184);
			this->chkAlwaysOnTop->Name = L"chkAlwaysOnTop";
			this->chkAlwaysOnTop->Size = System::Drawing::Size(92, 17);
			this->chkAlwaysOnTop->TabIndex = 7;
			this->chkAlwaysOnTop->Text = L"Always on top";
			this->chkAlwaysOnTop->UseVisualStyleBackColor = true;
			this->chkAlwaysOnTop->CheckedChanged += gcnew System::EventHandler(this, &FormMain::chkAlwaysOnTop_CheckedChanged);
			// 
			// btnApply
			// 
			this->btnApply->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->btnApply->Enabled = false;
			this->btnApply->Location = System::Drawing::Point(165, 60);
			this->btnApply->Name = L"btnApply";
			this->btnApply->Size = System::Drawing::Size(74, 23);
			this->btnApply->TabIndex = 9;
			this->btnApply->Text = L"Apply";
			this->btnApply->UseVisualStyleBackColor = true;
			this->btnApply->Click += gcnew System::EventHandler(this, &FormMain::btnApply_Click);
			// 
			// txtLog
			// 
			this->txtLog->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtLog->Location = System::Drawing::Point(0, 0);
			this->txtLog->Margin = System::Windows::Forms::Padding(0);
			this->txtLog->Multiline = true;
			this->txtLog->Name = L"txtLog";
			this->txtLog->ReadOnly = true;
			this->txtLog->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->txtLog->Size = System::Drawing::Size(262, 119);
			this->txtLog->TabIndex = 1;
			this->txtLog->WordWrap = false;
			// 
			// btnRunScript
			// 
			this->btnRunScript->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->btnRunScript->Location = System::Drawing::Point(0, 73);
			this->btnRunScript->Name = L"btnRunScript";
			this->btnRunScript->Size = System::Drawing::Size(68, 23);
			this->btnRunScript->TabIndex = 1;
			this->btnRunScript->Text = L"Run Script";
			this->btnRunScript->UseVisualStyleBackColor = true;
			this->btnRunScript->Click += gcnew System::EventHandler(this, &FormMain::btnRunScript_Click);
			// 
			// txtScript
			// 
			this->txtScript->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->txtScript->Font = (gcnew System::Drawing::Font(L"Consolas", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->txtScript->Location = System::Drawing::Point(0, 0);
			this->txtScript->Multiline = true;
			this->txtScript->Name = L"txtScript";
			this->txtScript->Size = System::Drawing::Size(223, 73);
			this->txtScript->TabIndex = 0;
			this->txtScript->Text = L"# Example Script to set the default Player Camera offsets\r\nget 0x91eece40\r\nset 0x" 
				L"91eece40 float -1024\r\nget 0x95887ce7\r\nset 0x95887ce7 float 2048\r\nget 0xb8f6b4cf\r" 
				L"\nset 0xb8f6b4cf float 1024";
			this->txtScript->WordWrap = false;
			// 
			// tabControl1
			// 
			this->tabControl1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->tabControl1->Controls->Add(this->tabRoadMod);
			this->tabControl1->Controls->Add(this->tabSpeedMod);
			this->tabControl1->Controls->Add(this->tabLog);
			this->tabControl1->Controls->Add(this->tabAbout);
			this->tabControl1->Location = System::Drawing::Point(7, 6);
			this->tabControl1->Name = L"tabControl1";
			this->tabControl1->SelectedIndex = 0;
			this->tabControl1->Size = System::Drawing::Size(273, 148);
			this->tabControl1->TabIndex = 12;
			// 
			// tabRoadMod
			// 
			this->tabRoadMod->Controls->Add(this->label3);
			this->tabRoadMod->Controls->Add(this->cmBoxRoadType);
			this->tabRoadMod->Controls->Add(this->txtC);
			this->tabRoadMod->Controls->Add(this->btnApply);
			this->tabRoadMod->Controls->Add(this->txtB);
			this->tabRoadMod->Controls->Add(this->txtA);
			this->tabRoadMod->Location = System::Drawing::Point(4, 22);
			this->tabRoadMod->Name = L"tabRoadMod";
			this->tabRoadMod->Padding = System::Windows::Forms::Padding(3);
			this->tabRoadMod->Size = System::Drawing::Size(265, 122);
			this->tabRoadMod->TabIndex = 0;
			this->tabRoadMod->Text = L"Road Mod";
			this->tabRoadMod->UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this->label3->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Malgun Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label3->ForeColor = System::Drawing::SystemColors::Desktop;
			this->label3->Location = System::Drawing::Point(48, 86);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(175, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"Ctrl+1 > Ctrl+6 Select road type";
			// 
			// tabSpeedMod
			// 
			this->tabSpeedMod->Controls->Add(this->label1);
			this->tabSpeedMod->Controls->Add(this->numSpeed);
			this->tabSpeedMod->Location = System::Drawing::Point(4, 22);
			this->tabSpeedMod->Name = L"tabSpeedMod";
			this->tabSpeedMod->Size = System::Drawing::Size(265, 122);
			this->tabSpeedMod->TabIndex = 2;
			this->tabSpeedMod->Text = L"Speed Mod";
			this->tabSpeedMod->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Malgun Gothic", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label1->ForeColor = System::Drawing::SystemColors::Desktop;
			this->label1->Location = System::Drawing::Point(68, 72);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(123, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Ctrl+9=dec Ctrl+0=inc";
			// 
			// numSpeed
			// 
			this->numSpeed->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->numSpeed->DecimalPlaces = 3;
			this->numSpeed->Enabled = false;
			this->numSpeed->Location = System::Drawing::Point(70, 49);
			this->numSpeed->Name = L"numSpeed";
			this->numSpeed->Size = System::Drawing::Size(120, 20);
			this->numSpeed->TabIndex = 0;
			this->numSpeed->ValueChanged += gcnew System::EventHandler(this, &FormMain::numSpeed_ValueChanged);
			// 
			// tabLog
			// 
			this->tabLog->Controls->Add(this->txtLog);
			this->tabLog->Location = System::Drawing::Point(4, 22);
			this->tabLog->Name = L"tabLog";
			this->tabLog->Padding = System::Windows::Forms::Padding(3);
			this->tabLog->Size = System::Drawing::Size(265, 122);
			this->tabLog->TabIndex = 1;
			this->tabLog->Text = L"Log";
			this->tabLog->UseVisualStyleBackColor = true;
			// 
			// tabAbout
			// 
			this->tabAbout->Controls->Add(this->pictureBox1);
			this->tabAbout->Controls->Add(this->label2);
			this->tabAbout->Controls->Add(this->labelUpdatesLink);
			this->tabAbout->Location = System::Drawing::Point(4, 22);
			this->tabAbout->Name = L"tabAbout";
			this->tabAbout->Size = System::Drawing::Size(265, 122);
			this->tabAbout->TabIndex = 4;
			this->tabAbout->Text = L"About";
			this->tabAbout->UseVisualStyleBackColor = true;
			// 
			// pictureBox1
			// 
			this->pictureBox1->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->pictureBox1->BackgroundImage = (cli::safe_cast<System::Drawing::Image^  >(resources->GetObject(L"pictureBox1.BackgroundImage")));
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Stretch;
			this->pictureBox1->Location = System::Drawing::Point(111, 19);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(40, 40);
			this->pictureBox1->TabIndex = 2;
			this->pictureBox1->TabStop = false;
			// 
			// label2
			// 
			this->label2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Malgun Gothic", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(227, 109);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(38, 13);
			this->label2->TabIndex = 1;
			this->label2->Text = L"@trgs";
			// 
			// labelUpdatesLink
			// 
			this->labelUpdatesLink->Anchor = System::Windows::Forms::AnchorStyles::None;
			this->labelUpdatesLink->AutoSize = true;
			this->labelUpdatesLink->Cursor = System::Windows::Forms::Cursors::Hand;
			this->labelUpdatesLink->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, 
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->labelUpdatesLink->ForeColor = System::Drawing::SystemColors::HotTrack;
			this->labelUpdatesLink->Location = System::Drawing::Point(59, 66);
			this->labelUpdatesLink->Name = L"labelUpdatesLink";
			this->labelUpdatesLink->Size = System::Drawing::Size(152, 13);
			this->labelUpdatesLink->TabIndex = 0;
			this->labelUpdatesLink->Text = L"Official Form post @Simtropolis";
			this->labelUpdatesLink->Click += gcnew System::EventHandler(this, &FormMain::labelUpdatesLink_Click);
			// 
			// panelStatus
			// 
			this->panelStatus->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->panelStatus->Location = System::Drawing::Point(259, 160);
			this->panelStatus->Name = L"panelStatus";
			this->panelStatus->Size = System::Drawing::Size(27, 23);
			this->panelStatus->TabIndex = 13;
			this->panelStatus->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &FormMain::panelStatus_Paint);
			// 
			// FormMain
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(289, 205);
			this->Controls->Add(this->panelStatus);
			this->Controls->Add(this->tabControl1);
			this->Controls->Add(this->chkAlwaysOnTop);
			this->Controls->Add(this->btnStart);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(256, 163);
			this->Name = L"FormMain";
			this->Text = L"SCTweak R7";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &FormMain::FormMain_FormClosing);
			this->Load += gcnew System::EventHandler(this, &FormMain::FormMain_Load);
			this->tabControl1->ResumeLayout(false);
			this->tabRoadMod->ResumeLayout(false);
			this->tabRoadMod->PerformLayout();
			this->tabSpeedMod->ResumeLayout(false);
			this->tabSpeedMod->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->numSpeed))->EndInit();
			this->tabLog->ResumeLayout(false);
			this->tabLog->PerformLayout();
			this->tabAbout->ResumeLayout(false);
			this->tabAbout->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnStart_Click(System::Object^  sender, System::EventArgs^  e) 
		{
			if (this->btnStart->Text == L"Stop")
			{
				this->btnStart->Enabled = false;
				this->timerStateUpdate->Enabled = false;
				this->btnRunScript->Enabled = false;

				LogWrite(L"stopping patch engine...", false);
				pPatchEngine->Stop();
				while (pPatchEngine->IsRunning()) Sleep(1);
				LogWrite(L"done");

				SetFormState(false);
			}
			else
			{
				this->btnStart->Enabled = false;
				LogWrite(L"starting patch engine...", false);

				pPatchEngine = gcnew PatchEngine;
				ThreadStart ^patchThreadStart = gcnew ThreadStart(pPatchEngine, &PatchEngine::Run);
				hPatchWorkerThread = gcnew Thread(patchThreadStart);
				hPatchWorkerThread->Start();

				Sleep(100);

				if (!pPatchEngine || !pPatchEngine->IsRunning())
				{
					LogWrite(L"failed (forgot to start sc5?)");
					SetFormState(false);
					return;
				}

				LogWrite(L"ok");


				try {
					pPatchEngine->SetStreetHeight(Convert::ToInt32(this->txtA->Text), Convert::ToInt32(this->txtB->Text), Convert::ToInt32(this->txtC->Text), true);
				} catch (...) { }

				SetFormState(true);
			}
		}

private: System::Void SetFormState(bool is_attached)
		 {
			 if (is_attached == true)
			 {
				this->btnStart->Text = L"Stop";
				this->btnStart->Enabled = true;
				this->numSpeed->Enabled = true;
				this->cmBoxRoadType->Enabled = true;
				this->panelStatus->Invalidate();
				this->timerStateUpdate->Enabled = true;
			 }
			 else
			 {
				this->btnStart->Text = L"Start";
				this->btnStart->Enabled = true;
				this->numSpeed->Enabled = false;
				this->cmBoxRoadType->Enabled = false;
				this->panelStatus->Invalidate();
				this->timerStateUpdate->Enabled = false;
			 }
		 }

private: System::Void timerStateUpdate_Tick(System::Object^  sender, System::EventArgs^  e) 
		 {
			 String ^log;

			if (!pPatchEngine)
			{
				this->timerStateUpdate->Enabled = false;
				return;
			}

			// Update log string from PatchEngine
			log = gcnew String(pPatchEngine->LogGet());
			if (log->Length != 0)
			{
				LogWrite(log);
				pPatchEngine->LogClear();
			}

			// update Game Speed
			if (pPatchEngine->IsRunning())
			{
				this->numSpeed->Enabled = true;
				this->numSpeed->Value = System::Decimal(pPatchEngine->GetGameSpeed());
			}

			 // Check if PatchEngine is still operational
			try
			{
				if (!pPatchEngine->IsRunning() || !hPatchWorkerThread->IsAlive)
				{
					LogWrite("patch engine exited");
					LogWrite("did you open simcity 5?");
					SetFormState(false);
				}
			} catch (...) { }
		}
private: System::Void FormMain_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
			this->timerStateUpdate->Enabled = false;

			if (pPatchEngine && pPatchEngine->IsRunning())
			{
				pPatchEngine->Stop();
				Sleep(250);
				//hPatchWorkerThread->Abort();
			}
		 }

private: System::Void cmBoxRoadType_TextChanged(System::Object^  sender, System::EventArgs^  e) {
		 }

private: System::Void lblUrl_Click(System::Object^  sender, System::EventArgs^  e) {
			 Process::Start("http://community.simtropolis.com/topic/58059-sctweak/");
		 }

private: System::Void chkAlwaysOnTop_CheckedChanged(System::Object^  sender, System::EventArgs^  e) {
			 this->TopMost = this->chkAlwaysOnTop->Checked;
		 }

private: System::Void FormMain_Load(System::Object^  sender, System::EventArgs^  e) {
			 this->TopMost = this->chkAlwaysOnTop->Checked;
			 this->cmBoxRoadType->Text = L"Default (ctrl+1)";


		 }
private: System::Void cmBoxRoadType_SelectedValueChanged(System::Object^  sender, System::EventArgs^  e) {
			this->txtA->Enabled = false;
			this->txtB->Enabled = false;
			this->txtC->Enabled = false;
			this->btnApply->Enabled = false;

			/*if (this->cmBoxRoadType->Text->Contains(L"Tunnel 1")){ this->txtA->Text = L"-920"; this->txtB->Text = L"2100"; this->txtC->Text = L"1050"; } // A > -1024, B > 2048, C = 50% of B (iE  -928, 2144, 1072)
			else */
			if (this->cmBoxRoadType->Text->Contains(L"Tunnel")){ this->txtA->Text = L"-850"; this->txtB->Text = L"2225"; this->txtC->Text = L"1105"; }
			else if (this->cmBoxRoadType->Text->Contains(L"UnderPass")){ this->txtA->Text = L"-1024"; this->txtB->Text = L"2176"; this->txtC->Text = L"1024"; } // B > 2048, A = negative value of C. (iE -1024, 2176, 1024)
			else if (this->cmBoxRoadType->Text->Contains(L"OverPass Low")){ this->txtA->Text = L"-1120"; this->txtB->Text = L"1900"; this->txtC->Text = L"1024"; } // A needs to be < -1024, B needs to be < 2048. (iE -1120, 1950) C doesnt matter.
			else if (this->cmBoxRoadType->Text->Contains(L"OverPass High")){ this->txtA->Text = L"-1220"; this->txtB->Text = L"1775"; this->txtC->Text = L"1024"; }
			else if (this->cmBoxRoadType->Text->Contains(L"Elevated")){ this->txtA->Text = L"-1015"; this->txtB->Text = L"2030"; this->txtC->Text = L"1015"; } // B < 2048, C = 50% of B, A = the negative Value of C. (iE -1015, 2030, 1015)
			else if (this->cmBoxRoadType->Text->Contains(L"Custom"))
				{
					this->txtA->Enabled = true;
					this->txtB->Enabled = true;
					this->txtC->Enabled = true;
					this->btnApply->Enabled = true;
				}
			else { this->txtA->Text = L"-1024"; this->txtB->Text = L"2048"; this->txtC->Text = L"1024"; } // default game values

			// Updating values to worker...
			try {
				pPatchEngine->SetStreetHeight(Convert::ToInt32(this->txtA->Text), Convert::ToInt32(this->txtB->Text), Convert::ToInt32(this->txtC->Text), false);
			} catch (...) {}

			//LogWrite("draw a test piece to apply changes");
		 }
private: System::Void btnApply_Click(System::Object^  sender, System::EventArgs^  e) {
			try {
				pPatchEngine->SetStreetHeight(Convert::ToInt32(this->txtA->Text), Convert::ToInt32(this->txtB->Text), Convert::ToInt32(this->txtC->Text), false);
			} catch (...) {}
		 }
private: System::Void btnRunScript_Click(System::Object^  sender, System::EventArgs^  e) {
			 /*
				# Example Script to set the default Player Camera offsets
				getmem 0x41334de
				setmem 0x41334de float 9999
				setprop 0x91eece40 float -1024

			 */
			  // Loop through the script
			  for ( int counter = 0; counter < this->txtScript->Lines->Length; counter++ )
			  {
				  String ^line = gcnew String(this->txtScript->Lines[counter]->Trim());
				  //if (this->txtScript->Lines[counter]->Compare
				  if (line[0] != '#')
				  {
					  if (line->StartsWith("getmem"))
					  {
						  array<String^>^ params = line->Split(' ');
						  if (params->Length != 2)
						  {
							  LogWrite("getmem <address> invalid number of arguments on: " + line);
						  }
						  else
						  {
							  DWORD address = Convert::ToInt32(params[1], 16);
							  //String^ result;//t = gcnew String();
							  //result = result->Format("%x", address);
							  float test = pPatchEngine->ReadFloat(address);
							  LogWrite("@"+String::Format("0x{0,8:X0}",address)+":"+String::Format("{0}", pPatchEngine->ReadFloat(address)));
						  }
					  }
					  else if (line->StartsWith("setmem"))
					  {
						  array<String^>^ params = line->Split(' ');
						  if (params->Length != 4)
						  {
							  LogWrite("setmem <adress> <type> <value> invalid number of arguments on: " + line);
						  }
						  else
						  {
							  DWORD address = Convert::ToInt32(params[1], 16);
							  if (params[2] == "float") pPatchEngine->WriteFloat(address, (float)Convert::ToInt32(params[3]));
							  else if (params[2] == "bool") pPatchEngine->WriteByte(address, Convert::ToInt32(params[3]));
							  else if (params[2] == "integer") pPatchEngine->WriteByte(address, Convert::ToInt32(params[3]));
							  else LogWrite("unknown type: " + line + "valid types: float bool integer");
						  }
						  //pPatchEngine->
					  }
					  else if (line->StartsWith("setprop"))
					  {
						  array<String^>^ params = line->Split(' ');
						  if (params->Length != 4)
						  {
							  LogWrite("setprop <adress> <type> <value> invalid number of arguments on: " + line);
						  }
						  else
						  {
							  DWORD address = Convert::ToInt32(params[1], 16);
							  if (params[2] == "float") pPatchEngine->WriteFloat(address, (float)Convert::ToInt32(params[3]));
							  else if (params[2] == "bool") pPatchEngine->WriteByte(address, Convert::ToInt32(params[3]));
							  else if (params[2] == "integer") pPatchEngine->WriteByte(address, Convert::ToInt32(params[3]));
							  else LogWrite("unknown type: " + line + "valid types: float bool integer");
						  }
						  //pPatchEngine->
					  }
					  else
					  {
						  LogWrite("unknown command: " + line);
					  }
				  }
			  }
		 }

private: System::Void lblDonate_Click(System::Object^  sender, System::EventArgs^  e) {
			Process::Start("https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=SBME9LR8GT6P2");
		 }
private: System::Void numSpeed_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 if (pPatchEngine && pPatchEngine->IsRunning())
			 {
				 pPatchEngine->SetGameSpeed((float)(this->numSpeed->Value));
			 }
		 }
private: System::Void panelStatus_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 int posX = 0;
			 int posY = 3;
			 int size = 15;

			 if (pPatchEngine && pPatchEngine->IsRunning())
			 {
				 Graphics^ g = e->Graphics;
				 g->FillEllipse(Brushes::Lime, posX, posY, size, size);
				 g->DrawEllipse(Pens::DarkGreen, posX, posY, size, size);
			 }
			 else
			 {
				 Graphics^ g = e->Graphics;
				 g->FillEllipse(Brushes::Red, posX, posY, size, size);
				 g->DrawEllipse(Pens::DarkRed, posX, posY, size, size);
			 }
		 }
private: System::Void labelUpdatesLink_Click(System::Object^  sender, System::EventArgs^  e) {
			 Process::Start("http://community.simtropolis.com/topic/58059-sctweak/");
		 }
};
}

