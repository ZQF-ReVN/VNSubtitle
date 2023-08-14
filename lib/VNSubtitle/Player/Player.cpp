#include "Player.h"
#include <stdexcept>


namespace VNSubtitle
{
	uint32_t MakeRGBA(uint8_t uR, uint8_t uG, uint8_t uB, uint8_t uA)
	{
		return ((uint32_t)uR << 0x18) | ((uint32_t)uG << 0x10) | ((uint32_t)uB << 0x08) | ((uint32_t)uA << 0x00);
	}


	Player::Player(HWND hLayer, HWND hGame) : m_hGame(hGame), m_hLayer(hLayer)
	{

	}

	void Player::AddSub(std::wstring wsText, uint32_t uiDuration)
	{
		m_vecSubtitle.emplace_back(wsText, uiDuration);
	}

	void Player::PutText(std::wstring_view wsText)
	{
		SendMessageW(m_hLayer, VNSM_PUT_TEXT, (WPARAM)&wsText, 0);
	}

	void Player::SetFont(std::wstring_view wsFont, uint32_t uiSize)
	{
		SendMessageW(m_hLayer, VNSM_SET_FONT, (WPARAM)wsFont.data(), (LPARAM)uiSize);
	}

	void Player::SetFontColor(uint32_t uiRGBA)
	{
		SendMessageW(m_hLayer, VNSM_SET_FONTCOLOR, (WPARAM)uiRGBA, 0);
	}

	void Player::Trace()
	{
		SendMessageW(m_hLayer, VNSM_TRACE, (WPARAM)m_hGame, 0);
	}

	void Player::CloseLayer()
	{
		SendMessageW(m_hLayer, WM_DESTROY, 0, 0);
	}

	void Player::Play()
	{
		for (auto& subtitle : m_vecSubtitle)
		{
			Trace();
			PutText(subtitle.m_wsText);
			Sleep(subtitle.m_uiDuration);
		}

		CloseLayer();
	}

}