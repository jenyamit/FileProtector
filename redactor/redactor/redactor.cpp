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
	array<String^>^ namesList;
	getLines("lockedFiles", &namesList);
	for each(String^ name in namesList) {
		if (name == fileName)
		{
			appendLine("toUnlock", fileName);
			SetEvent(eventRedactor);
			WaitForSingleObject(eventLocker, INFINITE);
		}
	}
	array<String^>^ file = File::ReadAllLines(fileName);
	TextBoxEditor->Lines = file;
}

RegistryKey^ Form1::getLines(String^ valueName, array<String^>^* lines)
{
	RegistryKey^ key = Registry::CurrentUser;
	key = key->OpenSubKey("Software\\FileProtector", true);
	*lines = (array<String^>^)key->GetValue(valueName);
	return key;
}

void Form1::appendLine(String^ valueName, String^ line)
{
	array<String^>^ before;
	RegistryKey^ key = getLines("toUnlock", &before);
	array<String^>^ after = gcnew array<String^>(before->Length + 1);
	for (int i = 0; i < before->Length; i++)
		after[i] = before[i];
	after[before->Length] = line;
	key->SetValue(valueName, (Object^)after);
}
