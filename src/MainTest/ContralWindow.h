#pragma once
#include "../../lib/RxUI/Win32.h"
#include "../../lib/VNSubtitle/VNSubtitle.h"


class ContralWindow : public RxUI::Win32::Window::MainWindow
{
private:
	VNSubtitle::Player player;
	RxUI::Win32::Font::Default m_fdFont;

	RxUI::Win32::Widget::Editor m_weText;
	RxUI::Win32::Widget::Button m_wbSend;


	void SendText(HWND hWnd, WPARAM wParam)
	{
		std::wstring text = m_weText.GetText();
		player.Trace();
		player.PutText(text);
	}

public:
	ContralWindow(HWND hLayer, HWND hGame)
	{
		player.Init(hLayer, hGame);
		player.EnableAlpha();
		//player.EnableThrough();
		player.SetSize(1200, 100);
		player.Trace();
		player.SetBackColor(VNSubtitle::MakeRGBA(0, 0, 0, 160));
		player.Show();

		player.SetFont(L"Î¢ÈíÑÅºÚ", 38);
		player.SetFontColor(VNSubtitle::MakeRGBA(255, 255, 255, 255));
	}

	void OnCreate(WPARAM wParam, LPARAM lParam) override
	{
		HWND hwnd = this->GetHandle();

		m_fdFont.Create(L"Î¢ÈíÑÅºÚ", 18);

		m_wbSend.Create(L"Send", hwnd);
		m_wbSend.SetFont(m_fdFont.GetHandle());
		m_wbSend.SetEvent(std::bind(&ContralWindow::SendText, this, std::placeholders::_1, std::placeholders::_2));
		m_wbSend.Run();

		m_weText.Create(L"Input Text Here", hwnd);
		m_weText.SetFont(m_fdFont.GetHandle());
		m_weText.Run();
	}

	void OnSize(WPARAM wType, uint32_t uiWidth, uint32_t uiHeigh) override
	{
		m_weText.SetRect(5, 05, uiWidth - 80, 25);
		m_wbSend.SetRect(uiWidth - 80 + 10, 03, 65, 30);
	}
};