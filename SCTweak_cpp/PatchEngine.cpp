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

#include "StdAfx.h"
#include "FormMain.h"
#include "PatchEngine.h"
#include <windows.h>
#include <string.h>
#include <time.h>
#include <tlhelp32.h>

static BYTE INT3_OPTCODE[] = {0xcc};
CONTEXT m_context;
DEBUG_EVENT m_de;

PatchEngine::PatchEngine(void)
{
	m_log = new wchar_t[512];
	m_log = L"";

	m_running = false;
	m_terminate = false;
	m_hProcess = NULL;

	m_streetA = -1024;
	m_streetB = 2048;
	m_streetC = 1024;

	m_speedPatchAddr = 0;
	m_speedValueAddr = 0;
}

PatchEngine::~PatchEngine(void)
{

}

float PatchEngine::ReadFloat(DWORD address)
{
	BYTE value[4];
	float result;

	ReadProcessMemory(m_hProcess, (void*)address, value, sizeof(value), 0);
	memcpy(&result, &value, sizeof(result));

	return result;
}

byte PatchEngine::ReadByte(DWORD address)
{
	byte value[1];
	byte result;

	ReadProcessMemory(m_hProcess, (void*)address, value, sizeof(value), 0);
	memcpy(&result, &value, sizeof(result));

	return result;
}

DWORD PatchEngine::FindPattern(byte *pattern, char *mask, DWORD startAddr, DWORD stopAddr)
{
	for (DWORD i=startAddr; i<stopAddr; i++)
	{
		int hits = 0;
		while (ReadByte(i+hits) == (byte)pattern[hits] || mask[hits] == '?')
		{
			hits++;
			if (hits == strlen(mask))
				return i;
		}
	}

	return 0;
}

int PatchEngine::ReadInt(DWORD address)
{
	BYTE value[4];
	int result;

	ReadProcessMemory(m_hProcess, (void*)address, value, sizeof(value), 0);
	memcpy(&result, &value, sizeof(result));

	return result;
}

void PatchEngine::WriteByte(DWORD address, byte new_optcode)
{
	WriteProcessMemory(m_hProcess,(void*)address, &new_optcode, sizeof(new_optcode), 0);
	FlushInstructionCache(m_hProcess, (void*)address, 1);
}

void PatchEngine::WriteFloat(DWORD address, float value)
{
	WriteProcessMemory(m_hProcess,(void*)address, &value, 4, 0);
	FlushInstructionCache(m_hProcess, (void*)address, 1);
}

void PatchEngine::WriteInt(DWORD address, int value)
{
	WriteProcessMemory(m_hProcess,(void*)address, &value, 4, 0);
	FlushInstructionCache(m_hProcess, (void*)address, 1);
}

bool PatchEngine::EnableDebugPriv(void)
{
    HANDLE              hToken;
    LUID                SeDebugNameValue;
    TOKEN_PRIVILEGES    TokenPrivileges;

    if(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        if(LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &SeDebugNameValue))
        {
            TokenPrivileges.PrivilegeCount              = 1;
            TokenPrivileges.Privileges[0].Luid          = SeDebugNameValue;
            TokenPrivileges.Privileges[0].Attributes    = SE_PRIVILEGE_ENABLED;

            if(AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL))
            {
                CloseHandle(hToken);
            }
            else
            {
                CloseHandle(hToken);
				m_log = L"warning: adjusting token privileges";
//                throw std::exception("Couldn't adjust token privileges!");              
				return false;
            }
        }
        else
        {
            CloseHandle(hToken);
			m_log = L"warning: looking up privilege value";
			return false;
        }
    }
    else
    {
//        throw std::exception("Couldn't open process token!");
		m_log = L"warning: could not open process token";
		return false;
    }
	return true;
}

private class Breakpoint
{
	public:
		DWORD address;
		BYTE original_optcode[1];
		bool hitonce;
		bool enabled;
		HANDLE m_hProcess;

		Breakpoint::Breakpoint()
		{
			enabled = false;
			hitonce = true;
			address = 0;
		}

		void Breakpoint::set(HANDLE hProcess, DWORD address, bool hitonce, bool enabled)
		{
			Breakpoint::address = address;
			Breakpoint::enabled = enabled;
			Breakpoint::hitonce = hitonce;
			m_hProcess = hProcess;

			ReadProcessMemory(m_hProcess, (void*)address, original_optcode, sizeof(original_optcode), 0);

			if (enabled)
				Breakpoint::enable();
		}

		void Breakpoint::enable()
		{
			::WriteProcessMemory(m_hProcess, (void*)address, INT3_OPTCODE, sizeof(INT3_OPTCODE), 0);
			FlushInstructionCache(m_hProcess, (void*)address, 1);

			Breakpoint::enabled = true;
		}

		void Breakpoint::disable()
		{
			::WriteProcessMemory(m_hProcess,(void*)address, original_optcode, sizeof(original_optcode), 0);
			FlushInstructionCache(m_hProcess, (void*)address, 1);

			enabled = false;
		}
};

DWORD PatchEngine::GetProcessID(wchar_t *exename)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if ( processesSnapshot == INVALID_HANDLE_VALUE )
	{
		m_log = L"error: getting process snapshot";
		return false;
	}
	
	Process32First(processesSnapshot, &processInfo);
	if ( wcscmp(processInfo.szExeFile, exename) == 0 )
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while ( Process32Next(processesSnapshot, &processInfo) )
	{
		if ( wcscmp(processInfo.szExeFile, exename) == 0 )
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}
	
	CloseHandle(processesSnapshot);
	return false;
}

wchar_t * PatchEngine::LogGet()
{
	return m_log;
}

void PatchEngine::LogClear()
{
	m_log = L"";
}

bool PatchEngine::IsRunning()
{
	return m_running;
}

bool PatchEngine::Stop()
{
	// Restore to default when stopping
	m_streetA = -1024; m_streetB = 2048; m_streetC = 1024; // default game values
	m_speedValueAddr = 0;

	try {
		SetStreetHeight(m_streetA, m_streetB, m_streetC, true);
		SpeedPatch(false);
	} catch (...) {}

	m_terminate = true;
	return true;
}

float PatchEngine::GetGameSpeed()
{
	if (m_running && m_speedValueAddr != 0)
	{
		return ReadFloat(m_speedValueAddr);
	}

	return 0;
}

void PatchEngine::SpeedPatch(bool enable)
{
	byte pattern[] = { 0x8d, 0x4c, 0x24, 0x1c, 0x77, 0x02 };

	if (enable == true)
	{
		if (m_speedPatchAddr == 0)
		{
			m_speedPatchAddr = FindPattern(pattern, "xxxxxx", 0x00400000, 0x00800000);
			if (m_speedPatchAddr > 0)
			{
				WriteByte(m_speedPatchAddr + 4, 0x90);
				WriteByte(m_speedPatchAddr + 5, 0x90);
			}
		}
	}
	else
	{
		if (m_speedPatchAddr != 0)
		{
			WriteByte(m_speedPatchAddr + 4, pattern[4]);
			WriteByte(m_speedPatchAddr + 5, pattern[5]);

			m_speedPatchAddr = 0;
		}
	}
}

void PatchEngine::SetGameSpeed(float value)
{
	if (m_running && m_speedValueAddr != 0)
	{
		WriteFloat(m_speedValueAddr, (float)value);
		m_log = L"speed mod: value set";
	}
}

void PatchEngine::SetStreetHeight(long txtA, long txtB, long txtC, bool force)
{
	// update realtime pointers to these values
	if (m_running)
	{
		if (m_streetA != txtA || m_streetB != txtB || m_streetC != txtC || force == true)
		{
			WriteFloat(0x10C6A10, (float)txtA);
			WriteFloat(0x10C6A0C, (float)txtB);
			WriteFloat(0x10C6A04, (float)txtC);
			m_log = L"road mod: value set";
		}
	}

	m_streetA = txtA;
	m_streetB = txtB;
	m_streetC = txtC;
}

void PatchEngine::Run()
{
	m_terminate = false;
	m_running = true;
	m_processId = GetProcessID(L"SimCity.exe");

	if (m_processId == 0)
	{
		m_running = false;
		return;
	}

	EnableDebugPriv();

	m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, m_processId);
	if (m_hProcess == NULL)
	{
		m_log = L"error: open process failed!";
		return;
	}

	if(DebugActiveProcess(m_processId) == 0) 
	{
		DWORD error = GetLastError();
		m_log = L"error: debug active process failed!";
		return;
	}

	/*
		// Find BP address to get the dynamic Speed value location
		CPU Disasm
		Address   Hex dump                 Command                                  Comments
		0074A6D2    F30F108678020000      MOVSS XMM0,DWORD PTR DS:[ESI+278]        ; FLOAT 0.2500000
	*/
	byte pattern[] = { 0xF3, 0x0F, 0x10, 0x86, 0x84, 0x02, 0x00, 0x00 };
	DWORD valueBpAddr = FindPattern(pattern, "xxxxxxxx", 0x00700000, 0x00900000);

	const int bps_length = 1;
	Breakpoint bps[bps_length];

	// patch for speedhack
	bps[0].set(m_hProcess, valueBpAddr, true, true); // To get dynamic Speed value address
	SpeedPatch(true);

	while(m_terminate == false) // Debugger loop, should be while(!)
	{
		DWORD ori_bp = 0;
		WaitForDebugEvent(&m_de, 1000); // INFINITE

		switch (m_de.dwDebugEventCode)
		{
			case EXCEPTION_DEBUG_EVENT: 
			switch(m_de.u.Exception.ExceptionRecord.ExceptionCode)
			{
				case EXCEPTION_BREAKPOINT:
				m_context.ContextFlags = CONTEXT_ALL;

				HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, false, m_de.dwThreadId);
				GetThreadContext(hThread, &m_context);

				m_context.Eip--; // Move back one byte
				SetThreadContext(m_hProcess, &m_context);

				int lastBp = -1;
				for(int i=0; i< bps_length; i++)
				{
					if (bps[i].address == m_context.Eip)
					{
						bps[i].disable();	// restores command & flushes cache
						lastBp = i;
					}
				}

				switch(lastBp)
				{
					case 0:
						{
							m_speedValueAddr = m_context.Esi + 0x284;
							m_log = L"got hit on speed value address";
						} break;
					case 3: // Load script values
						{
							/* old method (slow)
							DWORD test = ReadInt((m_context.Edx*8)+m_context.Esi);
							if (test == 0x91eece40)
							{
								WriteFloat((m_context.Edx*8)+m_context.Esi, (float)-1024);
							}
							*/
							if (m_context.Edx == 0x91eece40)
							{
								//WriteFloat(m_context.Eax, (float)-1220);
							}
							else if (m_context.Edx == 0x95887ce7)
							{
								//WriteFloat(m_context.Eax, (float)1850);
							}
							else if (m_context.Edx == 0xb8f6b4cf)
							{
								//WriteFloat(m_context.Eax, (float)1024);
							}
						} break;

				}
			} break;
		}

		ContinueDebugEvent(m_de.dwProcessId, m_de.dwThreadId, DBG_CONTINUE);

		//Sleep(1); // To avoid an instant re-break

		if (m_terminate == false)
		{
			for(int i=0; i< bps_length ; i++) // check if all bp's are enabled..
			{
				if (bps[i].hitonce == false && bps[i].enabled == false)
					bps[i].enable();
			}
		}
	}

	// unset all breakpoints
	for(int i=0; i< bps_length; i++)
		bps[i].disable();

	DebugActiveProcessStop(m_processId); // Detach...
	m_running = false;

	return;
}