#include "Tools.h"
#include <stdexcept>
#include <dwmapi.h>
#include <versionhelpers.h>
#pragma comment(lib, "dwmapi")


namespace RxUI::Win32::Tools
{
	void GetCenterRect(RECT& refRect, uint32_t uiWidth, uint32_t uiHeigh)
	{
		RECT rect_screen;
		GetWindowRect(GetDesktopWindow(), &rect_screen);
		refRect.left = (rect_screen.right - uiWidth) / 2;
		refRect.top = (rect_screen.bottom - uiHeigh) / 2;
		refRect.right = refRect.left + uiWidth;
		refRect.bottom = refRect.top + uiHeigh;
	}

	// https://github.com/glfw/glfw/blob/master/src/win32_window.c
	bool EnableAlphaCompositing(HWND hWnd)
	{
		if (!::IsWindowsVistaOrGreater()) { return false; }

		BOOL is_composition_enable = false;
		if (FAILED(::DwmIsCompositionEnabled(&is_composition_enable))) { throw std::runtime_error("Dwm Check Composition Error!"); }
		if (!is_composition_enable) { return true; }

		DWORD current_color = 0;
		BOOL is_opaque = false;
		if (FAILED(::DwmGetColorizationColor(&current_color, &is_opaque))) { throw std::runtime_error("Dwm Check Color Error!"); }

		if (!is_opaque || IsWindows8OrGreater())
		{
			HRGN region = ::CreateRectRgn(0, 0, -1, -1);
			DWM_BLURBEHIND bb = { 0 };
			bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
			bb.hRgnBlur = region;
			bb.fEnable = TRUE;
			::DwmEnableBlurBehindWindow(hWnd, &bb);
			::DeleteObject(region);
			return true;
		}
		else // For Window7
		{
			DWM_BLURBEHIND bb = { 0 };
			bb.dwFlags = DWM_BB_ENABLE;
			::DwmEnableBlurBehindWindow(hWnd, &bb);
			return false;
		}
	}
}