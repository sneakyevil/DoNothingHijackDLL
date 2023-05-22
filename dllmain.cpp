#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include "VersionHijack.hpp"

DWORD __stdcall SuspendAllThreads(void* m_Reserved)
{
	DWORD m_CurrentProcessID = GetCurrentProcessId();
	DWORD m_CurrentThreadID = GetCurrentThreadId();

	HANDLE m_SnapshotHandle = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (m_SnapshotHandle != INVALID_HANDLE_VALUE)
	{
		THREADENTRY32 m_ThreadEntry;
		m_ThreadEntry.dwSize = sizeof(THREADENTRY32);

		if (Thread32First(m_SnapshotHandle, &m_ThreadEntry))
		{
			while (1)
			{
				if (m_ThreadEntry.th32OwnerProcessID == m_CurrentProcessID && m_ThreadEntry.th32ThreadID != m_CurrentThreadID)
				{
					// Priority
					{
						HANDLE m_ThreadHandle = OpenThread(THREAD_SET_INFORMATION, 0, m_ThreadEntry.th32ThreadID);
						if (m_ThreadHandle)
						{
							SetThreadPriority(m_ThreadHandle, THREAD_PRIORITY_IDLE);
							CloseHandle(m_ThreadHandle);
						}
					}

					// Suspend
					{
						HANDLE m_ThreadHandle = OpenThread(THREAD_SUSPEND_RESUME, 0, m_ThreadEntry.th32ThreadID);
						if (m_ThreadHandle)
						{
							SuspendThread(m_ThreadHandle);
							CloseHandle(m_ThreadHandle);
						}
					}
				}

				if (!Thread32Next(m_SnapshotHandle, &m_ThreadEntry))
					break;
			}
		}

		CloseHandle(m_SnapshotHandle);
	}

	TerminateThread(GetCurrentThread(), 0x0);
	SuspendThread(GetCurrentThread()); // When fails to Terminate

	return 0x0;
}

int __stdcall DllMain(HMODULE m_Module, DWORD m_Reason, void* m_Reserved)
{
    if (m_Reason == DLL_PROCESS_ATTACH)
    {
        sneakyevil_DllHijack::Initialize();

		HANDLE m_ThreadHandle = CreateThread(0, 0, SuspendAllThreads, 0, 0, 0);
		if (m_ThreadHandle)
			CloseHandle(m_ThreadHandle);

    }

    return 1;
}
