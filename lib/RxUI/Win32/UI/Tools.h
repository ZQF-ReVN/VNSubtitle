#pragma once
#include <cstdint>
#include <Windows.h>


namespace RxUI::Win32::Tools
{
	void GetCenterRect(RECT& refRect, int32_t iWidth, int32_t iHeigh);
	bool EnableAlphaCompositing(HWND hWnd);
	bool EnableMouseClickThrough(HWND hWnd);
}