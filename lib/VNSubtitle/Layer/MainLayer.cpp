#include "MainLayer.h"


namespace VNSubtitle::Layer::D2D
{
	void LayerThread(const wchar_t* sg_wpTtile)
	{
		Layer::D2D::MainLayer handler;
		handler.Create(L"VNSubtitle_Class", sg_wpTtile);
		handler.Loop();
	}

	HWND CreateLayerThread( const wchar_t* wpTtile)
	{
		HANDLE handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)LayerThread, (LPVOID)wpTtile, 0, nullptr);
		if (handle == nullptr)
		{
			MessageBoxW(NULL, L"Create Layer Thread Failed!", NULL, NULL);
			return nullptr;
		}

		uint32_t wait_time = 500;
		HWND lanyer_hwnd = nullptr;
		while (wait_time-- > 0)
		{
			lanyer_hwnd = ::FindWindowW(L"VNSubtitle_Class", wpTtile);
			if (lanyer_hwnd) { break; }
			Sleep(1);
		}

		if (lanyer_hwnd == nullptr) 
		{ 
			MessageBoxW(NULL, L"Not Find Layer", NULL, NULL);
		}

		return lanyer_hwnd;
	}


	LRESULT MainLayer::HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return S_OK;
		}
		break;

		case VNSM_SET_BKCOLOR:
		{
			this->SetBackColor((uint32_t)wParam);
			return S_OK;
		}

		case VNSM_SHOW:
		{
			this->Show();
			return S_OK;
		}
		break;

		case VNSM_EN_ALPHA:
		{
			this->EnableAlpha();
			return S_OK;
		}
		break;

		case VNSM_EN_THROUGH:
		{
			this->EnableThrough();
			return S_OK;
		}
		break;

		case VNSM_SET_SIZE:
		{
			SetSize((uint32_t)wParam, (uint32_t)lParam);
			return S_OK;
		}
		break;

		case VNSM_TRACE:
		{
			RECT rect = { 0 };
			POINT point = { 0 };
			HWND game_hwnd = (HWND)wParam;
			GetClientRect(game_hwnd, &rect);
			ClientToScreen(game_hwnd, &point);
			SetPos(point.x, point.y);
			return S_OK;
		}
		break;

		case VNSM_PUT_TEXT:
		{
			PutText(*(std::wstring_view*)wParam);
			return S_OK;
		}
		break;

		case VNSM_SET_FONT:
		{
			SetFont((const wchar_t*)wParam, (uint32_t)lParam);
			return S_OK;
		}
		break;

		case VNSM_SET_FONTCOLOR:
		{
			SetFontColor((uint32_t)wParam);
			return S_OK;
		}
		break;

		}

		return this->MsgDefault(uMsg, wParam, lParam);
	}
}