#include "StdAfx.h"
#include "FormMain.h"
#include "HotKeys.h"
#include <windows.h>
#include <string.h>

HotKeys::HotKeys(void)
{
	m_log = new wchar_t[512];
	m_log = L"";
}

HotKeys::~HotKeys(void)
{
	m_running = false;
	m_terminate = false;
}

wchar_t * HotKeys::LogGet()
{
	return m_log;
}

void HotKeys::LogClear()
{
	m_log = L"";
}

bool HotKeys::IsRunning()
{
	return m_running;
}

bool HotKeys::Stop()
{
	m_terminate = true;
	return true;
}

BOOL GetMessageWithTimeout(MSG *msg, UINT timeout)
{
    BOOL res;
    SetTimer(NULL, 11, timeout, NULL); // make sure timer id is unique!
    res = GetMessage(msg, NULL, 0, 0);
    KillTimer(NULL, 11);

	if (!res)
        return false;

    return true;
}

void HotKeys::Run()
{
	m_terminate = false;
	m_running = true;

	RegisterHotKey(NULL, 1, MOD_CONTROL | MOD_NOREPEAT, '1');

	MSG msg = {0};
	while (m_terminate == false && GetMessageWithTimeout(&msg, 1000) != 0)
	{
		if (msg.message == WM_HOTKEY)
		{
			m_log = L"WM_HOTKEY received";
		}
	}

	m_running = false;

	return;
}