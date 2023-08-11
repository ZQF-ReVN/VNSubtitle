#pragma once
#include <cstdint>
#include <stdexcept>
#include <assert.h>
#include <Windows.h>


namespace RxUI::Win32::Font
{
	class Font_Basic
	{
	protected:
		HFONT m_hFont;

		virtual HFONT UserCreate(const wchar_t* wpFont, uint32_t uiSize) = 0;

	public:
		Font_Basic() : m_hFont(nullptr)
		{

		}

		~Font_Basic()
		{
			Delete();
		}

		void Create(const wchar_t* wpFont, uint32_t uiSize)
		{
			assert(m_hFont == 0); // duplicate creation
			m_hFont = this->UserCreate(wpFont, uiSize);
			if (m_hFont == nullptr) { throw std::runtime_error("Create Font Error!"); }
		}

		void Delete()
		{
			if (m_hFont)
			{
				::DeleteObject(m_hFont);
				m_hFont = nullptr;
			}
		}

		HFONT GetHandle()
		{
			assert(m_hFont != nullptr); // not created
			return m_hFont;
		}

	};
}