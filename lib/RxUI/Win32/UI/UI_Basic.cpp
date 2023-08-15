#include "UI_Basic.h"
#include <stdexcept>
#include <assert.h>

#include "Tools.h"


namespace RxUI::Win32::UI
{
	UI_Basic::UI_Basic() : m_hWnd(nullptr), m_hParentWnd(nullptr)
	{
		m_hInstance = ::GetModuleHandleW(NULL);
	}

	UI_Basic::~UI_Basic()
	{
		this->Destroy();
		m_hWnd = nullptr;
	}

	void UI_Basic::OnEvent(HWND hWnd, uint32_t uiID)
	{
		if (m_fnEvent)
		{
			m_fnEvent(hWnd, uiID);
		}
	}

	void UI_Basic::SetEvent(std::function<void(HWND, WPARAM)> fnEvent)
	{
		m_fnEvent = fnEvent;
		this->SetUserData((intptr_t)this);
	}

	bool UI_Basic::EnableAlpha()
	{
		return Tools::EnableAlphaCompositing(this->GetHandle());
	}

	bool UI_Basic::EnableThrough()
	{
		return Tools::EnableMouseClickThrough(this->GetHandle());
	}

	void UI_Basic::SetFont(HFONT hFont)
	{
		this->SendMsg(WM_SETFONT, (WPARAM)hFont, true);
	}

	void UI_Basic::Run()
	{
		this->Update();
		this->Show();
	}

	void UI_Basic::SetHandle(HWND hWnd)
	{
		m_hWnd = hWnd;
	}

	HWND UI_Basic::GetHandle()
	{
		assert(m_hWnd != nullptr);
		return m_hWnd;
	}

	HWND UI_Basic::GetParentHandle() const
	{
		return m_hParentWnd;
	}

	HINSTANCE UI_Basic::GetInstance()
	{
		return m_hInstance;
	}

	HWND UI_Basic::CreateWnd(const wchar_t* wpClass, const wchar_t* wpText, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent, intptr_t lParam)
	{
		m_wsClass = wpClass;
		m_hParentWnd = hParent;
		m_hWnd = ::CreateWindowExW(uiStyleEx, wpClass, wpText, uiStyle, 0, 0, 0, 0, hParent, nullptr, m_hInstance, (LPVOID)lParam);
		if (m_hWnd == nullptr) { throw std::runtime_error("Create Window Error!"); }
		return m_hWnd;
	}

	bool UI_Basic::RegClass(WNDCLASSEXW& refWCX)
	{
		return ::RegisterClassExW(&refWCX) ? true : false;
	}

	bool UI_Basic::UnregClass()
	{
		return ::UnregisterClassW(m_wsClass.c_str(), m_hInstance);
	}

	bool UI_Basic::Show(int32_t iShow)
	{
		return ::ShowWindow(this->GetHandle(), iShow);
	}

	bool UI_Basic::Update()
	{
		return ::UpdateWindow(this->GetHandle());
	}

	bool UI_Basic::Invalidate(RECT* pRect, bool isErase)
	{
		return ::InvalidateRect(this->GetHandle(), pRect, isErase);
	}

	bool UI_Basic::Validate(RECT* pRect)
	{
		return ::ValidateRect(this->GetHandle(), pRect);
	}

	void UI_Basic::Destroy()
	{
		::PostQuitMessage(0);
	}

	bool UI_Basic::Enable(bool isEnable)
	{
		return ::EnableWindow(this->GetHandle(), isEnable);
	}

	std::wstring UI_Basic::GetText()
	{
		uint32_t len = this->GetTextLen();
		std::wstring text;
		text.resize(len);
		this->GetText(text.data(), len);
		return text;
	}

	uint32_t UI_Basic::GetText(wchar_t* wpBuffer, uint32_t nChar)
	{
		return ::GetWindowTextW(this->GetHandle(), wpBuffer, nChar + 1);
	}

	uint32_t UI_Basic::GetTextLen()
	{
		return ::GetWindowTextLengthW(this->GetHandle());;
	}

	bool UI_Basic::SetText(std::wstring_view wsText)
	{
		return ::SetWindowTextW(this->GetHandle(), wsText.data());
	}

	bool UI_Basic::SetRect(RECT& refRect, bool isRepaint)
	{
		return this->SetRect(refRect.left, refRect.top, refRect.right - refRect.left, refRect.bottom - refRect.top, isRepaint);
	}

	bool UI_Basic::SetRect(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeigh, bool isRepaint)
	{
		return ::MoveWindow(this->GetHandle(), iPosX, iPosY, iWidth, iHeigh, isRepaint);
	}

	bool UI_Basic::SetSize(int32_t iWidth, int32_t iHeigh)
	{
		return ::SetWindowPos(this->GetHandle(), HWND_TOP, 0, 0, iWidth, iHeigh, SWP_NOMOVE | SWP_NOZORDER);
	}

	bool UI_Basic::SetCenter(int32_t iWidth, int32_t iHeigh)
	{
		if (iWidth == 0 && iHeigh == 0)
		{
			RECT rect = { 0 };
			this->GetRect(rect);
			iWidth = rect.right - rect.left;
			iHeigh = rect.bottom - rect.top;
		}
		RECT rect = { 0 };
		Tools::GetCenterRect(rect, iWidth, iHeigh);
		return SetRect(rect);
	}

	uint32_t UI_Basic::GetHeigh(bool isClient)
	{
		RECT rect = { 0 };
		this->GetRect(rect, isClient);
		return (rect.bottom - rect.top);
	}

	uint32_t UI_Basic::GetWidth(bool isClient)
	{
		RECT rect = { 0 };
		this->GetRect(rect, isClient);
		return (rect.right - rect.left);
	}

	bool UI_Basic::GetRect(RECT& rfRect, bool isClient)
	{
		if (isClient)
		{
			return ::GetClientRect(this->GetHandle(), &rfRect);
		}
		else
		{
			return ::GetWindowRect(this->GetHandle(), &rfRect);
		}
	}

	bool UI_Basic::SetPos(int32_t iPosX, int32_t iPosY)
	{
		return ::SetWindowPos(this->GetHandle(), HWND_TOP, iPosX, iPosY, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	intptr_t UI_Basic::GetWindowData(int iIndex)
	{
		return (intptr_t)::GetWindowLongPtrW(this->GetHandle(), iIndex);
	}

	intptr_t UI_Basic::SetWindowData(int iIndex, intptr_t iValue)
	{
		return (intptr_t)::SetWindowLongPtrW(this->GetHandle(), iIndex, iValue);
	}

	LRESULT UI_Basic::PostMsg(UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::PostMessageW(this->GetHandle(), uiMsg, wParam, lParam);
	}

	LRESULT UI_Basic::SendMsg(UINT uiMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::SendMessageW(this->GetHandle(), uiMsg, wParam, lParam);
	}

	LRESULT UI_Basic::MsgDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return ::DefWindowProc(this->GetHandle(), uMsg, wParam, lParam);
	}

	intptr_t UI_Basic::SetUserData(intptr_t pData)
	{
		return this->SetWindowData(GWLP_USERDATA, pData);
	}

	intptr_t UI_Basic::GetUserData()
	{
		return this->GetWindowData(GWLP_USERDATA);
	}

	HINSTANCE UI_Basic::SetInstance(intptr_t hInstance)
	{
		return (HINSTANCE)this->SetWindowData(GWLP_HINSTANCE, hInstance);
	}

	HINSTANCE UI_Basic::GetParentInstance()
	{
		return (HINSTANCE)this->GetWindowData(GWLP_HINSTANCE);
	}

	uint32_t UI_Basic::GetStyle()
	{
		return (uint32_t)this->GetWindowData(GWL_STYLE);
	}

	uint32_t UI_Basic::GetExStyle()
	{
		return (uint32_t)this->GetWindowData(GWL_EXSTYLE);
	}

	intptr_t UI_Basic::SetStyle(uint32_t uiStyle)
	{
		return this->SetWindowData(GWL_STYLE, (intptr_t)uiStyle);
	}

	intptr_t UI_Basic::SetExStyle(uint32_t uiExStyle)
	{
		return this->SetWindowData(GWL_EXSTYLE, (intptr_t)uiExStyle);
	}

	intptr_t UI_Basic::AddStyle(uint32_t uiStyle)
	{
		return this->SetStyle(this->GetStyle() | uiStyle);
	}

	intptr_t UI_Basic::AddExStyle(uint32_t uiExStyle)
	{
		return this->SetExStyle(this->GetExStyle() | uiExStyle);
	}

	intptr_t UI_Basic::SetID(uint16_t uiID, bool isRandom)
	{
		if (isRandom)
		{
			while (true)
			{
				uiID = rand() & 0x0000FFFF;
				if (uiID != 0)
				{ 
					break;
				}
			}
		}

		return this->SetWindowData(GWLP_ID, uiID);
	}

	uint16_t UI_Basic::GetID()
	{
		return (uint16_t)GetWindowData(GWLP_ID);
	}

	intptr_t UI_Basic::SetWndProc(intptr_t pFunc)
	{
		return SetWindowData(GWLP_WNDPROC, pFunc);
	}

	intptr_t UI_Basic::GetWndProc()
	{
		return this->GetWindowData(GWLP_WNDPROC);
	}
}