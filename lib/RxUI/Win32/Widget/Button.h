#pragma once
#include "Widget_Basic.h"


namespace RxUI::Win32::Widget
{
	class Button : public Widget_Basic
	{
	public:
		Button()
		{

		}

	private:
		const wchar_t* UserClass() override
		{
			return L"BUTTON";
		}

		void UserStyle(uint32_t& uiStyle, uint32_t& uiStyleEx) override
		{
			uiStyle = BS_DEFPUSHBUTTON;
			uiStyleEx = 0;
		}

	public:
		void SetChecked()
		{
			SendMsg(BM_SETCHECK, BST_CHECKED, 0);
		}

		void SetUnChecked()
		{
			SendMsg(BM_SETCHECK, BST_UNCHECKED, 0);
		}

		void SetNotClick()
		{
			SendMsg(BM_SETDONTCLICK, TRUE, 0);
		}

		bool IsCheck()
		{
			switch (SendMsg(BM_GETCHECK))
			{
			case BST_CHECKED: return true; break;
			case BST_UNCHECKED: return false; break;
			}
			return false;
		}
	};
}