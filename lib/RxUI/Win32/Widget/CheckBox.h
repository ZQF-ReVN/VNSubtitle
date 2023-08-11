#pragma once
#include "Button.h"


namespace RxUI::Win32::Widget
{
	class CheckBox : public Button
	{
	private:
		void UserStyle(uint32_t& uiStyle, uint32_t& uiStyleEx) override
		{
			uiStyle = BS_AUTOCHECKBOX;
			uiStyleEx = 0;
		}
	};
}
