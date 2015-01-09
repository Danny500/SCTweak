#pragma once
#include <windows.h>

ref class PatchEngine
{
private:
	bool m_terminate;
	bool m_running;
	wchar_t *m_log;

	DWORD m_processId;
	HANDLE m_hProcess;

	/* For street height hack */
	long m_streetA;// = -1024;
	long m_streetB;// = 2048;
	long m_streetC;// = 1024;

	/* For speed mod */
	DWORD m_speedPatchAddr;
	DWORD m_speedValueAddr;

	bool PatchEngine::EnableDebugPriv(void);
	DWORD PatchEngine::GetProcessID(wchar_t *exename);

public:
	PatchEngine();
	~PatchEngine();

	bool PatchEngine::IsRunning();
	wchar_t *PatchEngine::LogGet();
	void PatchEngine::LogClear();
	void PatchEngine::SpeedPatch(bool enable);
	void PatchEngine::SetGameSpeed(float value);
	float PatchEngine::GetGameSpeed();
	void PatchEngine::SetStreetHeight(long txtA, long txtB, long txtC, bool force);
	void PatchEngine::WriteInt(DWORD address, int value);
	void PatchEngine::WriteFloat(DWORD address, float value);
	void PatchEngine::WriteByte(DWORD address, byte new_optcode);
	float PatchEngine::ReadFloat(DWORD address);
	byte PatchEngine::ReadByte(DWORD address);
	int PatchEngine::ReadInt(DWORD address);
	DWORD PatchEngine::FindPattern(byte *pattern, char *mask, DWORD startAddr, DWORD stopAddr);

	void PatchEngine::Run();
	bool PatchEngine::Stop();

};