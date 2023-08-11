#include "MainWindow.h"


namespace RxUI::Win32::Window
{
	void MainWindow::Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx , HWND hParent)
	{
		WNDCLASSEXW wcx = { 0 };
		wcx.cbSize = sizeof(wcx);
		wcx.hInstance = this->GetInstance();
		wcx.lpszClassName = wpClass;
		wcx.style = CS_HREDRAW | CS_HREDRAW;
		wcx.lpfnWndProc = Window_Basic::WndProc;
		wcx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		this->RegClass(wcx);
		this->CreateWnd(wpClass, wpTtile, uiStyle, uiStyleEx, hParent, (intptr_t)this);
	}

	void MainWindow::Loop()
	{
		MSG msg = { 0 };
		while (::GetMessageW(&msg, nullptr, 0, 0))
		{
			::TranslateMessage(&msg);
			::DispatchMessageW(&msg);
		}
	}

	LRESULT MainWindow::HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_PAINT:
		{
			this->OnPaint();
			return S_OK;
		}
		break;

		case WM_CREATE:
		{
			this->OnCreate(wParam, lParam);
			return S_OK;
		}
		break;

		case WM_SIZE:
		{
			this->OnSize(wParam, LOWORD(lParam), HIWORD(lParam));
			return S_OK;
		}
		break;

		case WM_COMMAND:
		{
			HWND handle = (HWND)lParam;
			if (handle) // Control
			{
				WidgetCommand(handle, HIWORD(wParam));
			}
			else if (HIWORD(wParam) == 1) // Accelerator
			{
				AcceleratorCommand(LOWORD(wParam));
			}
			else if (HIWORD(wParam) == 0) // Menu
			{
				MenuCommand(LOWORD(wParam));
			}
			return S_OK;
		}
		break;

		case WM_NCHITTEST:
		{
			return this->OnNCClick(uMsg, wParam, lParam);
		}
		break;

		case WM_DESTROY:
		{
			return this->OnDestroy(uMsg, wParam, lParam);
		}
		break;
		};

		return this->MsgDefault(uMsg, wParam, lParam);
	}

	void MainWindow::WidgetCommand(HWND hWnd, uint32_t uiID)
	{
		UI::UI_Basic* ui_ptr = (UI::UI_Basic*)::GetWindowLongPtrW(hWnd, GWLP_USERDATA);
		if (ui_ptr)
		{
			ui_ptr->OnEvent(hWnd, uiID);
		}
	}

	LRESULT MainWindow::OnNCClick(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		//return HTCAPTION
		return this->MsgDefault(uMsg, wParam, lParam);
	}

	LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		::PostQuitMessage(0);
		return S_OK;
	}
}