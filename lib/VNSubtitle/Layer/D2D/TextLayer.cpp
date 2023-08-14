#include "TextLayer.h"
#include "../../Message.h"

#include <stdexcept>

#pragma comment(lib, "dwrite.lib")


namespace VNSubtitle::Layer::D2D
{
	D2D1::ColorF MakeD2DColor(uint32_t uiRGBA)
	{
		float r = (float)((uiRGBA & 0xFF000000) >> 0x18) / 255.0f;
		float g = (float)((uiRGBA & 0x00FF0000) >> 0x10) / 255.0f;
		float b = (float)((uiRGBA & 0x0000FF00) >> 0x08) / 255.0f;
		float a = (float)((uiRGBA & 0x000000FF) >> 0x00) / 255.0f;

		return D2D1::ColorF(r, g, b, a);
	}

	TextLayer::TextLayer()
		: m_pIDWriteFactory(nullptr), m_pIDWriteTextFormat(nullptr), m_pID2D1FontColor(nullptr), m_coBackColor(0.0f, 0.0f, 0.0f, 0.0f)
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

	void TextLayer::Create(const wchar_t* wpClass, const wchar_t* wpTtile)
	{
		CreateBaseLayer(wpClass, wpTtile);
		CreateFactory();
	}

	void TextLayer::PutText(std::wstring_view wsText)
	{
		SureRender();

		GetRender()->BeginDraw();

		GetRender()->Clear(m_coBackColor);
		if (wsText.length())
		{
			D2D1_RECT_F rect = { 0.0, 0.0, (float)GetWidth() , (float)GetHeigh() };
			GetRender()->DrawTextW(wsText.data(), wsText.length(), m_pIDWriteTextFormat, rect, m_pID2D1FontColor);
		}

		if (FAILED(GetRender()->EndDraw())) 
		{ 
			this->ReleaseRender();
		}
	}

	void TextLayer::SetBackColor(uint32_t uiRGBA)
	{
		m_coBackColor = MakeD2DColor(uiRGBA);
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

		GetRender()->CreateSolidColorBrush(MakeD2DColor(uiRGBA), &m_pID2D1FontColor);

		if (m_pID2D1FontColor == nullptr) { throw std::runtime_error("Create Font Color Brush Error!"); }
	}

}