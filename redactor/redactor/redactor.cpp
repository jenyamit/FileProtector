#include "stdafx.h"
#include "Form1.h"

using namespace redactor;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Application::Run(gcnew Form1());
	return 0;
}

void Form1::loadDir()
{
	folderBrowserDialog1->ShowDialog();
	String^ path = folderBrowserDialog1->SelectedPath;
	if (!Directory::Exists(path))
		return;
	dataGridView1->Tag = path;
	for each (String^ fileName in Directory::GetFiles(path))
	{
		if (Path::GetExtension(fileName) == ".txt")
			dataGridView1->Rows->Add(Path::GetFileName(fileName));
	}
}

void Form1::loadFile()
{
	String^ fileName = String::Concat(dataGridView1->Tag, dataGridView1->SelectedCells[0]->Value);
	RegistryKey^ key = Registry::CurrentUser;
	key = key->OpenSubKey("Software\\FileProtector", true);
	array<String^>^ namesList = (array<String^>^)key->GetValue("lockedFiles");
	for each(String^ name in namesList) {
		if (name == fileName)
			MessageBox::Show("Locked");
	}
	array<String^>^ file = File::ReadAllLines(fileName);
	TextBoxEditor->Lines = file;
}
