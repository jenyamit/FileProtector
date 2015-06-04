#include "stdafx.h"
#include <Vcclr.h>
#include "Form1.h"

using namespace redactor;
void removeExcessFileFromKeyValue(String^ KeyValue, String^ fileName);

#define MAX 1024

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

void Form1::lock()
{
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	const int lockSize = 10000;
	pin_ptr<const wchar_t> fileName = PtrToStringChars(openedFileName);
	lockedFileHandle = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	LockFileEx(lockedFileHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, lockSize, 0, &overlapped);
}

void Form1::unlock()
{
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	const int lockSize = 10000;
	UnlockFileEx(lockedFileHandle, 0, lockSize, 0, &overlapped);
	CloseHandle(lockedFileHandle);
}

void Form1::loadFile()
{
	closeFile();
	openedFileName = String::Concat(dataGridView1->Tag, dataGridView1->SelectedCells[0]->Value);
	array<String^>^ namesList;
	getLines("toLock", &namesList);
	for each(String^ name in namesList) {
		if (name == openedFileName)
		{
			appendLine("toUnlock", openedFileName);
			SetEvent(eventRedactor);
			WaitForSingleObject(eventLocker, INFINITE);
		}
	}
	array<String^>^ file = File::ReadAllLines(openedFileName);
	TextBoxEditor->Lines = file;

	lock();
}

void Form1::saveFile()
{
	if (!openedFileName)		//if nothing opened
		return;
	unlock();
	File::WriteAllLines(openedFileName, TextBoxEditor->Lines);
	lock();
}

void Form1::closeFile()
{
	if (!openedFileName)		//if nothing opened
		return;

	unlock();

	removeExcessFileFromKeyValue("toUnlock", openedFileName);
	SetEvent(eventRedactor);
	WaitForSingleObject(eventLocker, INFINITE);

	openedFileName = nullptr;
	TextBoxEditor->Clear();
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

void removeExcessFileFromKeyValue(String^ KeyValue, String^ fileName)
{
	//find excess file
	RegistryKey^ key = Registry::CurrentUser;
	key = key->OpenSubKey("Software\\FileProtector", true);
	array<String^>^ fileList = (array<String^>^)key->GetValue(KeyValue);
	bool excessFileFinded = false;
	for each (String^ lockedFileName in fileList)
		if (lockedFileName == fileName)
			excessFileFinded = true;
	//remove excess file
	if (excessFileFinded)
	{
		array<String^>^ temp = gcnew array<String^>(fileList->Length - 1);
		int i = 0;
		for each (String^ lockedFileName in fileList)
			if (lockedFileName != fileName)
				temp[i++] = lockedFileName;
		key->SetValue(KeyValue, (Object^)temp, RegistryValueKind::MultiString);
	}
}
