// Locker.cpp: главный файл проекта.

#include "stdafx.h"
#include <Windows.h>
#include <Vcclr.h>
#include <iostream>

#define MAX 1024

using namespace System;
using namespace Microsoft::Win32;
using namespace std;

HANDLE lockedFilesHandles[MAX];
LPCWSTR lockedFilesNames[MAX];
bool freeCells[MAX];

bool strCompare(LPCWSTR s1, LPCWSTR s2)
{
	int i = 0;
	while (s1[i] != 0 && s2[i] != 0)
		if (s1[i] != s2[i++])
			return false;
	return true;
}

int getFileNameIndex(LPCWSTR name)
{
	for (int i = 0; i < MAX; i++)
		if (strCompare(lockedFilesNames[i], name))
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

bool processFile(String^ filename, bool lock)
{
	int index = getFirstFreeIndex();
	pin_ptr<const wchar_t> name = PtrToStringChars(filename);
	OVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));
	const int lockSize = 10000;
	if (lock)
	{
		HANDLE hFile = hFile = CreateFile(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
		if (!LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, lockSize, 0, &overlapped))
		{
			DWORD err = GetLastError();
			printf("Not locked\t\"%s\"\n", filename);
			freeCells[index] = true;
			return false;
		}
		else
		{
			printf("Locked\t\"%s\"\n", filename);
			int index = getFirstFreeIndex();
			lockedFilesHandles[index] = hFile;
			lockedFilesNames[index] = name;
			freeCells[index] = false;
		}
	}
	else
	{
		LPCWSTR unlockName = name;
		int index = getFileNameIndex(unlockName);
		bool result = UnlockFileEx(lockedFilesHandles[index], 0, lockSize, 0, &overlapped);
		if (result)
			printf("Unlocked\t\"%s\"\n", filename);
		else
		{
			DWORD err = GetLastError();
			printf("Not unlocked\t\"%s\"\n", filename);
		}
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
	for each (String^ name in namesList)
		processFile(name, true);

	//UNLOCK

	namesList = (array<String^>^)key->GetValue("toUnlock");
	for each (String^ name in namesList)
		processFile(name, false);
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
