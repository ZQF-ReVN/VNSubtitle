#pragma once
#include "Widget_Basic.h"


namespace RxUI::Win32::Widget
{
	class GroupBox : public Button
	{
	private:
		void UserStyle(uint32_t& uiStyle, uint32_t& uiStyleEx) override
		{
			uiStyle = BS_GROUPBOX;
			uiStyleEx = 0;
		}
	};
}