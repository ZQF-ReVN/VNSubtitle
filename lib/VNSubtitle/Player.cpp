#include "Player.h"
#include <stdexcept>


namespace VNSubtitle
{
	HWND GetGameHandle(const wchar_t* wpClass, const wchar_t* wpTitle)
	{
		HWND hwnd = FindWindowW(wpClass, wpTitle);
		if (hwnd == nullptr) { throw std::runtime_error("Not Find Game Window"); }
		return hwnd;
	}

	Player::Player(HWND hLayer, HWND hGame) : m_hGame(hGame), m_hLayer(hLayer)
	{

	}

	void Player::AddSub(std::wstring wsText, uint32_t uiDuration)
	{
		m_vecSubtitle.emplace_back(wsText, uiDuration);
	}

	void Player::ShowSub(Subtitle& rfSubtitle)
	{
		SendMessageW(m_hLayer, VNSM_PLAY, (WPARAM)&rfSubtitle, 0);
	}

	void Player::SetFont(std::wstring_view wsFont, uint32_t uiSize)
	{
		SendMessageW(m_hLayer, VNSM_FONT, (WPARAM)wsFont.data(), (LPARAM)uiSize);
	}

	void Player::SetFontColor(uint32_t uiRGBA)
	{
		SendMessageW(m_hLayer, VNSM_FONTCOLOR, (WPARAM)uiRGBA, 0);
	}

	void Player::TraceGameWindow()
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
			TraceGameWindow();
			ShowSub(subtitle);
		}

		CloseLayer();
	}
}