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
			::MessageBoxW(NULL, L"Not Find Layer", NULL, NULL);
		}

		return lanyer_hwnd;
	}

	LRESULT MainLayer::OnNCClick(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		POINT cur = { LOWORD(lParam), HIWORD(lParam) };
		::ScreenToClient(this->GetHandle(), &cur);

		int32_t px = (int32_t)cur.x;
		int32_t py = (int32_t)cur.y;
		int32_t cw = (int32_t)this->GetWidth(true);
		int32_t ch = (int32_t)this->GetHeigh(true);

		if (((cw - px) < 10) && ((ch - py) < 10))
		{
			return HTBOTTOMRIGHT;
		}
		else
		{
			return HTCAPTION;
		}
	}

	void MainLayer::OnMouseWheel(int16_t sDistance, uint16_t usFlag, uint16_t usCurX, uint16_t usCurY)
	{
		if (sDistance > 0)
		{
			this->SetFontSize(this->GetFontSize() - 2);
		}
		else
		{
			this->SetFontSize(this->GetFontSize() + 2);
		}

		this->Invalidate();
	}

	LRESULT MainLayer::AppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
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
			int32_t gw = rect.right - rect.left;
			int32_t lw = this->GetWidth(true);
			SetPos(point.x + ((gw - lw) / 2), point.y);
			return S_OK;
		}
		break;

		case VNSM_PUT_TEXT:
		{
			this->PutText(*(std::wstring_view*)wParam);
			return S_OK;
		}
		break;

		case VNSM_SET_FONT:
		{
			this->SetFont((const wchar_t*)wParam, (uint32_t)lParam);
			return S_OK;
		}
		break;

		case VNSM_SET_FONTCOLOR:
		{
			this->SetFontColor((uint32_t)wParam);
			return S_OK;
		}
		break;

		}

		return this->MsgDefault(uMsg, wParam, lParam);
	}
}