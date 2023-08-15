#include "BaseLayer.h"

#include <stdexcept>

#pragma comment(lib, "d2d1.lib")
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


	void BaseLayer::CreateFactory()
	{
		HRESULT dwrite_factory_hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pIDWriteFactory);
		HRESULT d2d_factory_hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pID2D1Factory);

		if (FAILED(d2d_factory_hr) || FAILED(dwrite_factory_hr)) { throw std::runtime_error("CreateFactory Error!"); }
	}

	void BaseLayer::ReleaseFactory()
	{
		SafeRelease(m_pID2D1Factory);
		SafeRelease(m_pIDWriteFactory);
	}

	void BaseLayer::CreateResouces()
	{
		CreateRenderTarget();
		CreateFontBrush();;
	}

	void BaseLayer::ReleaseResouces()
	{
		SafeRelease(m_pRenderTarget);
		SafeRelease(m_pID2D1FontBrush);
	}

	void BaseLayer::CreateRenderTarget()
	{
		if (m_pRenderTarget) { return; }

		D2D1_SIZE_U size = { this->GetWidth(true), this->GetHeigh(true) };
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties
		(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);
		m_pID2D1Factory->CreateHwndRenderTarget(props, D2D1::HwndRenderTargetProperties(this->GetHandle(), size), &m_pRenderTarget);

		if (m_pRenderTarget == nullptr) { throw std::runtime_error("Create RenderTarget Error!"); }
	}

	void BaseLayer::CreateFontBrush()
	{
		if (m_pID2D1FontBrush) { return; }

		GetRender()->CreateSolidColorBrush(MakeD2DColor(m_uiFontColorRGBA), &m_pID2D1FontBrush);

		if (m_pID2D1FontBrush == nullptr) { throw std::runtime_error("Create Font Color Brush Error!"); }
	}

	void BaseLayer::CreateTextFormatter()
	{
		if (m_wsFont.empty()) { return; }

		SafeRelease(m_pIDWriteTextFormat);

		m_pIDWriteFactory->CreateTextFormat
		(
			m_wsFont.data(),
			nullptr,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			(float)m_uiFontSize,
			L"",
			&m_pIDWriteTextFormat
		);
		if (m_pIDWriteTextFormat == nullptr) { throw std::runtime_error("Create TextFormat Error!"); }

		m_pIDWriteTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		m_pIDWriteTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}


	BaseLayer::BaseLayer()
	{
		ZeroThisOBJ();
		m_uiFontColorRGBA = -1;
	}

	BaseLayer::~BaseLayer()
	{
		ReleaseResouces();
		ReleaseFactory();
		SafeRelease(m_pIDWriteTextFormat);
		ZeroThisOBJ();
	}

	void BaseLayer::ZeroThisOBJ()
	{
		memset(this, 0x0, sizeof(*this));
	}


	void BaseLayer::Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent)
	{
		WNDCLASSEXW wcx = { 0 };
		wcx.cbSize = sizeof(wcx);
		wcx.hInstance = this->GetInstance();
		wcx.lpszClassName = wpClass;
		wcx.lpfnWndProc = Window_Basic::WndProc;
		//wcx.style = CS_HREDRAW | CS_HREDRAW;
		//wcx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);

		this->RegClass(wcx);
		this->CreateWnd(wpClass, wpTtile, uiStyle, uiStyleEx, hParent, (intptr_t)this);
	}


	void BaseLayer::OnCreate(WPARAM wParam, LPARAM lParam)
	{
		CreateFactory();
		CreateResouces();
	}

	void BaseLayer::OnSize(WPARAM wType, uint32_t uiWidth, uint32_t uiHeigh)
	{
		GetRender()->Resize(D2D1::SizeU(uiWidth, uiHeigh));
		this->Invalidate();
	}

	void BaseLayer::OnPaint()
	{
		ID2D1HwndRenderTarget* target = GetRender();

		target->BeginDraw();

		PaintBack(target);
		PaintText(target);

		if (FAILED(target->EndDraw()))
		{
			this->ReleaseResouces();
		}
		else
		{
			this->Validate();
		}
	}

	void BaseLayer::PaintBack(ID2D1HwndRenderTarget* pTarget)
	{
		pTarget->Clear(MakeD2DColor(m_uiBackColorRGBA));
	}

	void BaseLayer::PaintText(ID2D1HwndRenderTarget* pTarget)
	{
		if (m_wsText.length())
		{
			D2D1_RECT_F rect = { 0.0, 0.0, (float)GetWidth(true) , (float)GetHeigh(true) };
			pTarget->DrawTextW(m_wsText.data(), m_wsText.length(), m_pIDWriteTextFormat, rect, m_pID2D1FontBrush);
		}
	}

	void BaseLayer::PutText(std::wstring_view wsText)
	{
		m_wsText = wsText;
		this->Invalidate();
	}

	void BaseLayer::SetFont(std::wstring_view wsFont, uint32_t uiSize)
	{
		m_wsFont = wsFont;
		m_uiFontSize = uiSize;
		this->CreateTextFormatter();
	}

	void BaseLayer::SetFontSize(uint32_t uiSize)
	{
		m_uiFontSize = uiSize;
		this->CreateTextFormatter();
	}

	void BaseLayer::SetFontColor(uint32_t uiRGBA)
	{
		m_uiFontColorRGBA = uiRGBA;
		this->CreateFontBrush();
	}

	void BaseLayer::SetBackColor(uint32_t uiRGBA)
	{
		m_uiBackColorRGBA = uiRGBA;
	}

	uint32_t BaseLayer::GetFontSize()
	{
		if (m_pIDWriteTextFormat)
		{
			return (uint32_t)m_pIDWriteTextFormat->GetFontSize();
		}
		else
		{
			return 50;
		}
	}


	ID2D1HwndRenderTarget* BaseLayer::GetRender()
	{
		if (m_pRenderTarget == nullptr) { CreateResouces(); }
		return m_pRenderTarget;
	}


}