#pragma once
#include <Windows.h>

namespace redactor {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace System::Text;
	using namespace Microsoft::Win32;

	/// <summary>
	/// ������ ��� Form1
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			eventRedactor = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"FileProtectorR");
			eventLocker = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"FileProtectorL");
		}

		void loadDir();
		void loadFile();
		RegistryKey^ getLines(String^ valueName, array<String^>^* lines);
		void appendLine(String^ valueName, String^ line);
		HANDLE eventRedactor;
		HANDLE eventLocker;

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	protected:
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog1;
	private: System::Windows::Forms::TextBox^  TextBoxEditor;
	private: System::Windows::Forms::MenuStrip^  menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^  ����ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �����_��������ToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  ���������ToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  �����ToolStripMenuItem;
	private: System::Windows::Forms::FolderBrowserDialog^  folderBrowserDialog1;
	private: System::Windows::Forms::DataGridView^  dataGridView1;

	private: System::Windows::Forms::SplitContainer^  splitContainer1;
	private: System::Windows::Forms::DataGridViewTextBoxColumn^  fileName;
	private: System::Windows::Forms::ToolStripMenuItem^  ������������ToolStripMenuItem;




	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// ������������ ����� ��� ��������� ������������ - �� ���������
		/// ���������� ������� ������ ��� ������ ��������� ����.
		/// </summary>
		void InitializeComponent(void)
		{
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->TextBoxEditor = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����_��������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->���������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->������������ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->�����ToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->folderBrowserDialog1 = (gcnew System::Windows::Forms::FolderBrowserDialog());
			this->dataGridView1 = (gcnew System::Windows::Forms::DataGridView());
			this->fileName = (gcnew System::Windows::Forms::DataGridViewTextBoxColumn());
			this->splitContainer1 = (gcnew System::Windows::Forms::SplitContainer());
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->BeginInit();
			this->splitContainer1->Panel1->SuspendLayout();
			this->splitContainer1->Panel2->SuspendLayout();
			this->splitContainer1->SuspendLayout();
			this->SuspendLayout();
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &Form1::openFileDialog1_FileOk);
			// 
			// TextBoxEditor
			// 
			this->TextBoxEditor->Dock = System::Windows::Forms::DockStyle::Fill;
			this->TextBoxEditor->Location = System::Drawing::Point(0, 0);
			this->TextBoxEditor->Multiline = true;
			this->TextBoxEditor->Name = L"TextBoxEditor";
			this->TextBoxEditor->Size = System::Drawing::Size(391, 276);
			this->TextBoxEditor->TabIndex = 1;
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->����ToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(569, 24);
			this->menuStrip1->TabIndex = 2;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// ����ToolStripMenuItem
			// 
			this->����ToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {
				this->�����_��������ToolStripMenuItem,
					this->���������ToolStripMenuItem, this->������������ToolStripMenuItem, this->�����ToolStripMenuItem
			});
			this->����ToolStripMenuItem->Name = L"����ToolStripMenuItem";
			this->����ToolStripMenuItem->Size = System::Drawing::Size(53, 20);
			this->����ToolStripMenuItem->Text = L"����";
			// 
			// �����_��������ToolStripMenuItem
			// 
			this->�����_��������ToolStripMenuItem->Name = L"�����_��������ToolStripMenuItem";
			this->�����_��������ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->�����_��������ToolStripMenuItem->Text = L"����� ��������";
			this->�����_��������ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::�����_��������ToolStripMenuItem_Click);
			// 
			// ���������ToolStripMenuItem
			// 
			this->���������ToolStripMenuItem->Name = L"���������ToolStripMenuItem";
			this->���������ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->���������ToolStripMenuItem->Text = L"���������";
			this->���������ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::���������ToolStripMenuItem_Click);
			// 
			// ������������ToolStripMenuItem
			// 
			this->������������ToolStripMenuItem->Name = L"������������ToolStripMenuItem";
			this->������������ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->������������ToolStripMenuItem->Text = L"��������� ���";
			// 
			// �����ToolStripMenuItem
			// 
			this->�����ToolStripMenuItem->Name = L"�����ToolStripMenuItem";
			this->�����ToolStripMenuItem->Size = System::Drawing::Size(162, 22);
			this->�����ToolStripMenuItem->Text = L"�����";
			this->�����ToolStripMenuItem->Click += gcnew System::EventHandler(this, &Form1::�����ToolStripMenuItem_Click);
			// 
			// dataGridView1
			// 
			this->dataGridView1->AllowUserToAddRows = false;
			this->dataGridView1->AllowUserToDeleteRows = false;
			this->dataGridView1->ColumnHeadersHeightSizeMode = System::Windows::Forms::DataGridViewColumnHeadersHeightSizeMode::AutoSize;
			this->dataGridView1->Columns->AddRange(gcnew cli::array< System::Windows::Forms::DataGridViewColumn^  >(1) { this->fileName });
			this->dataGridView1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->dataGridView1->Location = System::Drawing::Point(0, 0);
			this->dataGridView1->Name = L"dataGridView1";
			this->dataGridView1->ReadOnly = true;
			this->dataGridView1->RowHeadersVisible = false;
			this->dataGridView1->Size = System::Drawing::Size(150, 276);
			this->dataGridView1->TabIndex = 3;
			this->dataGridView1->CellDoubleClick += gcnew System::Windows::Forms::DataGridViewCellEventHandler(this, &Form1::dataGridView1_CellDoubleClick);
			// 
			// fileName
			// 
			this->fileName->AutoSizeMode = System::Windows::Forms::DataGridViewAutoSizeColumnMode::Fill;
			this->fileName->HeaderText = L"��� �����";
			this->fileName->Name = L"fileName";
			this->fileName->ReadOnly = true;
			// 
			// splitContainer1
			// 
			this->splitContainer1->Location = System::Drawing::Point(12, 27);
			this->splitContainer1->Name = L"splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this->splitContainer1->Panel1->Controls->Add(this->dataGridView1);
			// 
			// splitContainer1.Panel2
			// 
			this->splitContainer1->Panel2->Controls->Add(this->TextBoxEditor);
			this->splitContainer1->Size = System::Drawing::Size(545, 276);
			this->splitContainer1->SplitterDistance = 150;
			this->splitContainer1->TabIndex = 4;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(569, 315);
			this->Controls->Add(this->splitContainer1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"Form1";
			this->Text = L"FileProtector";
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->dataGridView1))->EndInit();
			this->splitContainer1->Panel1->ResumeLayout(false);
			this->splitContainer1->Panel2->ResumeLayout(false);
			this->splitContainer1->Panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->splitContainer1))->EndInit();
			this->splitContainer1->ResumeLayout(false);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void openFileDialog1_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e) {
		RegistryKey^ key = Registry::CurrentUser;
		key = key->OpenSubKey("Software\\FileProtector", true);
		array<String^>^ namesList = (array<String^>^)key->GetValue("lockedFiles");
		array<String^>^ file = File::ReadAllLines(openFileDialog1->FileName, Encoding::Default);
		for each(String^ nameFile in namesList) {
			if (nameFile == openFileDialog1->FileName)
				MessageBox::Show("���");
		}
		TextBoxEditor->Lines = file;
	}

	private: System::Void �����_��������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		loadDir();
	}

	private: System::Void �����ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void ���������ToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
		File::WriteAllLines(openFileDialog1->FileName, TextBoxEditor->Lines, Encoding::Default);
	}

	private: System::Void dataGridView1_CellDoubleClick(System::Object^  sender, System::Windows::Forms::DataGridViewCellEventArgs^  e) {
		loadFile();
	}
	};
}

