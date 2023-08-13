#include "Player.h"
#include "Message.h"
#include "TextLayer.h"

#include <stdexcept>

#pragma comment(lib, "dwrite.lib")


namespace VNSubtitle
{
	TextLayer::TextLayer() : m_pIDWriteFactory(nullptr), m_pIDWriteTextFormat(nullptr), m_pID2D1FontColor(nullptr)
	{

	}

	TextLayer::~TextLayer()
	{
		SafeRelease(m_pID2D1FontColor);
		SafeRelease(m_pIDWriteTextFormat);
		ReleaseFactory();
	}

	void TextLayer::CreateFactory()
	{
		ReleaseFactory();
		HRESULT dwrite_factory_hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pIDWriteFactory);
	}

	void TextLayer::ReleaseFactory()
	{
		SafeRelease(m_pIDWriteFactory);
	}

	void TextLayer::Create(int32_t iPosX, int32_t iPosY, int32_t iWidth, int32_t iHeight)
	{
		CreateBaseLayer(iPosX, iPosY, iWidth, iHeight); 
		CreateFactory();
	}

	void TextLayer::PutSring(std::wstring_view wsText)
	{
		SureRender();

		GetRender()->BeginDraw();

		GetRender()->Clear(D2D1::ColorF(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.8f)));
		if (wsText.length())
		{
			GetRender()->DrawTextW(wsText.data(), wsText.length(), m_pIDWriteTextFormat, { 0.0, 0.0, (float)GetWidth() , (float)GetHeight() }, m_pID2D1FontColor);
		}

		if (FAILED(GetRender()->EndDraw())) 
		{ 
			this->ReleaseRender();
		}
	}

	void TextLayer::SetFont(std::wstring_view wsFont, uint32_t uiSize)
	{
		if (wsFont.empty()) { return; }

		SafeRelease(m_pIDWriteTextFormat);

		m_pIDWriteFactory->CreateTextFormat
		(
			wsFont.data(),
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(float)uiSize,
			L"",
			&m_pIDWriteTextFormat
		);
		if (m_pIDWriteTextFormat == nullptr) { throw std::runtime_error("Create TextFormat Error!"); }

		m_pIDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pIDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	void TextLayer::SetFontColor(uint32_t uiRGBA)
	{
		SafeRelease(m_pID2D1FontColor);

		float r = (float)((uiRGBA & 0xFF000000) >> 0x18) / 255.0f;
		float g = (float)((uiRGBA & 0x00FF0000) >> 0x10) / 255.0f;
		float b = (float)((uiRGBA & 0x0000FF00) >> 0x08) / 255.0f;
		float a = (float)((uiRGBA & 0x000000FF) >> 0x00) / 255.0f;

		GetRender()->CreateSolidColorBrush(D2D1::ColorF(r, g, b, a), &m_pID2D1FontColor);
		if (m_pID2D1FontColor == nullptr) { throw std::runtime_error("Create Font Color Brush Error!"); }
	}

	LRESULT TextLayer::HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return S_OK;
		}
		break;

		case VNSM_TRACE:
		{
			RECT rect = { 0 };
			POINT point = { 0 };
			HWND game_hwnd = (HWND)wParam;
			GetClientRect(game_hwnd, &rect);
			ClientToScreen(game_hwnd, &point);
			SetPos(point.x, point.y);
			return S_OK;
		}

		case VNSM_PLAY:
		{
			Subtitle* sub_ptr = (Subtitle*)wParam;
			PutSring(sub_ptr->m_wsText);
			Sleep(sub_ptr->m_uiDuration);
			return S_OK;
		}
		break;

		case VNSM_FONT:
		{
			SetFont((const wchar_t*)wParam, (uint32_t)lParam);
			return S_OK;
		}

		case VNSM_FONTCOLOR:
		{
			SetFontColor((uint32_t)wParam);
			return S_OK;
		}

		}

		return this->MsgDefault(uMsg, wParam, lParam);
	}
}