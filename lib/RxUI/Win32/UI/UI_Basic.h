#pragma once
#include <string>
#include <functional>
#include <Windows.h>


namespace RxUI::Win32::UI
{
	class UI_Basic
	{
	private:
		HWND m_hWnd;
		HWND m_hParentWnd;
		HINSTANCE m_hInstance;
		std::wstring m_wsClass;
		std::function<void(HWND, uint32_t)> m_fnEvent;

	public:
		UI_Basic();
		~UI_Basic();

		void Run();
		bool Destroy();
		bool Enable(bool isEnable);
		bool Update(bool isRepaint = true);
		bool Show(int32_t iShow = SW_NORMAL);
		HWND CreateWnd(const wchar_t* wpClass, const wchar_t* wpText, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent, intptr_t lParam);

		// Window Class Reg / Unreg
		bool RegClass(WNDCLASSEXW& refWCX);
		bool UnregClass(const wchar_t* wpClass, HINSTANCE hInstance);

		// Event Funcation
		void OnEvent(HWND hWnd, uint32_t uiID);
		void SetEvent(std::function<void(HWND, WPARAM)> fnEvent);

		// Move / Size
		bool SetRect(RECT& refRect, bool isRepaint = true);
		bool SetRect(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeigh, bool isRepaint = true);

		// Set / Get Window Data
		intptr_t GetWindowData(int iIndex);
		intptr_t SetWindowData(int iIndex, intptr_t iValue);

		// Msg
		LRESULT SendMsg(UINT uiMsg, WPARAM wParam = 0, LPARAM lParam = 0);
		LRESULT MsgDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

		// Font
		void SetFont(HFONT hFont);

		// Text
		uint32_t GetTextLen();
		std::wstring GetText();
		bool SetText(std::wstring_view wsText);
		uint32_t GetText(wchar_t* wpBuffer, uint32_t nChar);

		// Style
		uint32_t GetStyle();
		uint32_t GetExStyle();
		intptr_t SetStyle(uint32_t uiStyle);
		intptr_t SetExStyle(uint32_t uiExStyle);
		intptr_t AddStyle(uint32_t uiStyle);
		intptr_t AddExStyle(uint32_t uiExStyle);

		// UserData
		intptr_t GetUserData();
		intptr_t SetUserData(intptr_t pData);

		// Hanlde / Instance
		HWND GetHandle();
		void SetHandle(HWND hWnd);
		HWND GetParentHandle() const;
		HINSTANCE GetInstance();
		HINSTANCE GetParentInstance();
		HINSTANCE SetInstance(intptr_t hInstance);

		// Menu / Control ID
		uint16_t GetID();
		intptr_t SetID(uint16_t uiID = 0, bool isRandom = true);

		// Window Proc Address
		intptr_t GetWndProc();
		intptr_t SetWndProc(intptr_t pFunc);

	};

}