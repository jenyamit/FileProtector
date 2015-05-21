// Locker.cpp: главный файл проекта.

#include "stdafx.h"
#include <Windows.h>
#include <Vcclr.h>
#include <iostream>

#define MAX 1024

using namespace System;
using namespace Microsoft::Win32;
using namespace std;

WCHAR fileNameBuffer[MAX];
HANDLE lockedFilesHandles[MAX];
LPCWSTR lockedFilesNames[MAX];
bool freeCells[MAX];

int getFileNameIndex(LPCWSTR name)
{
	for (int i = 0; i < MAX; i++)
		if (lockedFilesNames[i] == name)
			return i;
	return -1;
}

int getFirstFreeIndex()
{
	for (int i = 0; i < MAX; i++)
		if (freeCells[i])
			return i;
	return -1;
}

LPCWSTR StringToLPCWSTR(String^ str)
{
	int n = str->Length;
	for (int i = 0; i < n; i++)
	{
		fileNameBuffer[i] = str[i];
		fileNameBuffer[i + 1] = 0;
	}
	return fileNameBuffer;
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

bool processFile(String^ filename, bool lock)
{
	LPCWSTR name = StringToLPCWSTR(filename);
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	const int lockSize = 10000;
	if (lock)
	{
		HANDLE hFile = hFile = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (!LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, lockSize, 0, &overlapped))
		{
			DWORD err = GetLastError();
			printf("Error %i\n", err);
			return false;
		}
		else
		{
			printf("lock OK\n");
			int index = getFirstFreeIndex();
			lockedFilesHandles[index] = hFile;
			lockedFilesNames[index] = name;
			freeCells[index] = false;
		}
	}
	else
	{
		int index = getFileNameIndex(name);
		bool result = UnlockFileEx(lockedFilesHandles[index], 0, lockSize, 0, &overlapped);
		CloseHandle(lockedFilesHandles[index]);
		freeCells[index] = true;
	}
	return true;
}

void processAllFiles()
{
	RegistryKey^ key = Registry::CurrentUser;
	key = key->OpenSubKey("Software\\FileProtector", true);

	//LOCK

	array<String^>^ namesList = (array<String^>^)key->GetValue("toLock");
	array<String^>^ notLockedFiles = gcnew array<String^>(namesList->Length);
	array<String^>^ lockedFiles = gcnew array<String^>(namesList->Length);
	int notLockedCount = 0;
	int lockedCount = 0;
	for each (String^ name in namesList)
	{
		if (!processFile(name, true))
			notLockedFiles[notLockedCount++] = name;
		else
			lockedFiles[lockedCount++] = name;
	}

	//notLockedFiles
	array<String^>^ notLocked = gcnew array<String^>(notLockedCount);	//без пустых строк в конце
	for (int i = notLockedCount - 1; i >= 0; i--)
		notLocked[i] = notLockedFiles[i];
	key->SetValue("notLockedFiles", (Object^)notLocked, RegistryValueKind::MultiString);

	//lockedFiles
	array<String^>^ locked = gcnew array<String^>(lockedCount);	//без пустых строк в конце
	for (int i = lockedCount - 1; i >= 0; i--)
		locked[i] = lockedFiles[i];
	key->SetValue("lockedFiles", (Object^)locked, RegistryValueKind::MultiString);

	//UNLOCK

	namesList = (array<String^>^)key->GetValue("toUnlock");
	for each (String^ name in namesList)
	{
		processFile(name, false);
		removeExcessFileFromKeyValue("LockedFiles", name);
		removeExcessFileFromKeyValue("toUnlock", name);
	}
}

void work()
{
	HANDLE eventRedactor = CreateEvent(NULL, FALSE, FALSE, L"FileProtectorR");
	HANDLE eventLocker = CreateEvent(NULL, FALSE, FALSE, L"FileProtectorL");
	SetEvent(eventRedactor);
	bool firstLoop = true;
	while (true)
	{
		WaitForSingleObject(eventRedactor, INFINITE);
		processAllFiles();
		if (firstLoop)
			firstLoop = false;
		else
			SetEvent(eventLocker);
	}
}

void main()
{
	for (int i = 0; i < MAX; i++)
		freeCells[i] = true;
	work();
	getchar();
}
