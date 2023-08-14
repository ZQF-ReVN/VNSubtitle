#pragma once
#include "../Message.h"

#include <string>
#include <vector>


namespace VNSubtitle
{
	uint32_t MakeRGBA(uint8_t uR, uint8_t uG, uint8_t uB, uint8_t uA);

	class Player
	{
	private:
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

	private:
		HWND m_hGame;
		HWND m_hLayer;
		std::vector<Subtitle> m_vecSubtitle;

	public:
		Player()
		{

		}

		Player(HWND hLayer, HWND hGame);

		void Init(HWND hLayer, HWND hGame)
		{
			m_hLayer = hLayer;
			m_hGame = hGame;
		}

		void SetSize(uint32_t uiWidth, uint32_t uiHeigh)
		{
			SendMessageW(m_hLayer, VNSM_SET_SIZE, (WPARAM)uiWidth, (LPARAM)uiHeigh);
		}

		void SetBackColor(uint32_t uiRGBA)
		{
			SendMessageW(m_hLayer, VNSM_SET_BKCOLOR, (WPARAM)uiRGBA, 0);
		}

		void Show()
		{
			SendMessageW(m_hLayer, VNSM_SHOW, 0, 0);
		}

		void EnableAlpha()
		{
			SendMessageW(m_hLayer, VNSM_EN_ALPHA, 0, 0);
		}

		void EnableThrough()
		{
			SendMessageW(m_hLayer, VNSM_EN_THROUGH, 0, 0);
		}

		void AddSub(std::wstring wsText, uint32_t uiDuration);
		void PutText(std::wstring_view wsText);
		void SetFont(std::wstring_view wsFont, uint32_t uiSize);
		void SetFontColor(uint32_t uiRGBA);
		void Trace();
		void CloseLayer();
		void Play();
	};

}