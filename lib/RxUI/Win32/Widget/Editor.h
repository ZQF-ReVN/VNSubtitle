#pragma once
#include "Widget_Basic.h"


namespace RxUI::Win32::Widget
{
	class Editor : public Widget_Basic
	{
	private:
		const wchar_t* UserClass() override
		{
			return L"EDIT";
		}

		void UserStyle(uint32_t& uiStyle, uint32_t& uiStyleEx) override
		{
			uiStyle = ES_LEFT | ES_AUTOHSCROLL | ES_AUTOVSCROLL;
			uiStyleEx = WS_EX_CLIENTEDGE;
		}
	};

	class Editor_Multiline : public Editor
	{
	private:
		void UserStyle(uint32_t& uiStyle, uint32_t& uiStyleEx) override
		{
			uiStyle = ES_MULTILINE | ES_WANTRETURN | ES_AUTOHSCROLL | ES_AUTOVSCROLL | WS_HSCROLL | WS_VSCROLL;
			uiStyleEx = WS_EX_CLIENTEDGE;
		}
	public:
		void AddLine(std::wstring_view wsText)
		{
			uint32_t len = SendMsg(WM_GETTEXTLENGTH);
			this->SendMsg(EM_SETSEL, (WPARAM)len, (LPARAM)len);
			this->SendMsg(EM_REPLACESEL, FALSE, (LPARAM)wsText.data());
		}

		void AddNewLine(std::wstring_view wsText)
		{
			this->AddLine(wsText);
			this->AddLine(L"\r\n");
		}

		void Clear()
		{
			this->SetText(L"");
		}
	};

}