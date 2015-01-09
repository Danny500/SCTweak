#pragma once
#include <windows.h>

ref class HotKeys
{
private:
	bool m_terminate;
	bool m_running;
	wchar_t *m_log;

public:
	HotKeys();
	~HotKeys();

	bool HotKeys::IsRunning();
	wchar_t * HotKeys::LogGet();
	void HotKeys::LogClear();

	void HotKeys::Run();
	bool HotKeys::Stop();
};