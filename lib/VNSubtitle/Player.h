#pragma once
#include "Message.h"

#include <string>
#include <vector>


namespace VNSubtitle
{
	HWND GetGameHandle(const wchar_t* wpClass, const wchar_t* wpTitle);

	class Subtitle
	{
	public:
		uint32_t m_uiDuration;
		std::wstring m_wsText;

		Subtitle(std::wstring wsText, uint32_t uiDuration)
		{
			m_wsText = wsText;
			m_uiDuration = uiDuration;
		}
	};

	class Player
	{
	private:
		HWND m_hGame;
		HWND m_hLayer;
		std::vector<Subtitle> m_vecSubtitle;

	public:
		Player(HWND hLayer, HWND hGame);

		void AddSub(std::wstring wsText, uint32_t uiDuration);
		void ShowSub(Subtitle& rfSubtitle);
		void SetFont(std::wstring_view wsFont, uint32_t uiSize);
		void SetFontColor(uint32_t uiRGBA);
		void TraceGameWindow();
		void CloseLayer();
		void Play();
	};

}