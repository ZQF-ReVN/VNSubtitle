#pragma once
#include "Font_Basic.h"


namespace RxUI::Win32::Font
{
	class Default : public Font_Basic
	{
	public:
		Default()
		{

		}

		Default(const wchar_t* wpFont, uint32_t uiSize)
		{
			this->Create(wpFont, uiSize);
		}

		HFONT UserCreate(const wchar_t* wpFont, uint32_t uiSize) override
		{
			return ::CreateFontW
			(
				uiSize, 0, 0, 0, 0, FALSE, FALSE, 0,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
				DEFAULT_PITCH | FF_SWISS, wpFont
			);
		}
	};
}