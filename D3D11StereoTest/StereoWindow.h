#pragma once
#include <robmikh.common/DesktopWindow.h>

struct StereoWindow : robmikh::common::desktop::DesktopWindow<StereoWindow>
{
	static const std::wstring ClassName;
	static void RegisterWindowClass();
	StereoWindow(std::wstring const& titleString);
	LRESULT MessageHandler(UINT const message, WPARAM const wparam, LPARAM const lparam);

	wil::shared_event Closed() { return m_windowClosed; }

private:
	void CloseWindow() { m_windowClosed.SetEvent(); }
private:
	wil::shared_event m_windowClosed{ nullptr };
};