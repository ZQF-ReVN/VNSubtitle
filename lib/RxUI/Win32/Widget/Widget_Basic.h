#pragma once
#include "../UI/UI_Basic.h"


namespace RxUI::Win32::Widget
{
	class Widget_Basic : public UI::UI_Basic
	{
	protected:
		virtual const wchar_t* UserClass() = 0;
		virtual void UserStyle(uint32_t& uiStyle, uint32_t& uiStyleEx) = 0;

	public:
		void Create(const wchar_t* wpText, HWND hParent)
		{
			std::uint32_t style = 0;
			std::uint32_t style_ex = 0;
			this->UserStyle(style, style_ex);
			this->CreateWnd(this->UserClass(), wpText, WS_CHILD | style, style_ex, hParent, 0);
		}

		void NewGroup()
		{
			AddStyle(WS_GROUP);
		}
	};
}