#pragma once
#include <cstdint>
#include <Windows.h>


namespace RxUI::Win32::Tools
{
	void GetCenterRect(RECT& refRect, uint32_t uiWidth, uint32_t uiHeigh);
	bool EnableAlphaCompositing(HWND hWnd);
}