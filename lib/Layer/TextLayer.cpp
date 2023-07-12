#include "TextLayer.h"

#include <stdexcept>

#pragma comment(lib, "dwrite.lib")


namespace RiaTextLayer
{
	void TextLayer::CreateDWriteFactory()
	{
		ReleaseDWriteFactory();
		HRESULT dwrite_factory_hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pIDWriteFactory);
	}

	void TextLayer::ReleaseDWriteFactory()
	{
		SafeRelease(m_pIDWriteFactory);
	}

	TextLayer::TextLayer()
	{
		this->m_pIDWriteFactory = nullptr;
		this->m_pIDWriteTextFormat = nullptr;
		this->m_pID2D1FontColor = nullptr;
	}

	TextLayer::~TextLayer()
	{
		SafeRelease(m_pID2D1FontColor);
		SafeRelease(m_pIDWriteTextFormat);
		ReleaseDWriteFactory();
	}

	void TextLayer::Create(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight)
	{
		CreateBaseLayer(uiPosX, uiPosY, uiWidth, uiHeight); 
		CreateDWriteFactory();
	}

	void TextLayer::Draw(const	std::wstring& wsText)
	{
		SureRenderValid();

		D2D1_RECT_F rect = { 0.0, 0.0, (float)GetBaseLayerWidth() , (float)GetBaseLayerHeight()};

		GetRender()->BeginDraw();
		{
			GetRender()->Clear();
			GetRender()->DrawTextW(wsText.c_str(), wsText.length(), m_pIDWriteTextFormat, rect, m_pID2D1FontColor);
		}

		if (FAILED(GetRender()->EndDraw())) { SetRenderInvalid(); }
	}

	void TextLayer::SetFont(const std::wstring& wsFont, const float flSize)
	{
		SafeRelease(m_pIDWriteTextFormat);

		m_pIDWriteFactory->CreateTextFormat
		(
			wsFont.c_str(),
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			flSize,
			L"",
			&m_pIDWriteTextFormat
		);
		if (m_pIDWriteTextFormat == nullptr) { throw std::runtime_error("Create TextFormat Error!"); }
	}

	void TextLayer::SetFontColor(const D2D1::ColorF& cColor)
	{
		SafeRelease(m_pID2D1FontColor);

		GetRender()->CreateSolidColorBrush(D2D1::ColorF(cColor), &m_pID2D1FontColor);
		if (m_pID2D1FontColor == nullptr) { throw std::runtime_error("Create Font Color Brush Error!"); }
	}
}