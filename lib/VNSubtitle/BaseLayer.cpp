#include "BaseLayer.h"

#include <stdexcept>

#pragma comment(lib, "d2d1.lib")


namespace VNSubtitle
{
	void BaseLayer::CreateFactory()
	{
		ReleaseFactory();
		HRESULT d2d_factory_hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pID2D1Factory);
		if (FAILED(d2d_factory_hr)) { throw std::runtime_error("CreateFactory Error!"); }
	}

	void BaseLayer::CreateRender()
	{
		if (m_isRenderValid) { return; }

		ReleaseRender();
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties
		(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);
		m_pID2D1Factory->CreateHwndRenderTarget(props, D2D1::HwndRenderTargetProperties(this->GetHandle(), D2D1::SizeU(m_uiWidth, m_uiHeight)), &m_pRenderTarget);
		if (m_pRenderTarget == nullptr) { throw std::runtime_error("Create RenderTarget Error!"); }

		m_isRenderValid = true;
	}

	void BaseLayer::ReleaseRender()
	{
		SafeRelease(m_pRenderTarget);
		m_isRenderValid = false;
	}

	void BaseLayer::ReleaseFactory()
	{
		SafeRelease(m_pID2D1Factory);
	}

	BaseLayer::BaseLayer()
	{
		ZeroThisOBJ();
	}

	BaseLayer::~BaseLayer()
	{
		DestroyBaseLayer();
	}

	void BaseLayer::ZeroThisOBJ()
	{
		memset(this, 0x0, sizeof(*this));
	}

	void BaseLayer::CreateBaseLayer(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight)
	{
		this->Create(L"VNSubtitle_Class", L"VNSubtitle", WS_POPUP, WS_EX_TOPMOST,nullptr);
		this->SetRect(uiPosX, uiPosY, uiWidth, uiHeight);
		RxUI::Win32::Tools::EnableAlphaCompositing(this->GetHandle());
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		CreateFactory();
		CreateRender();
	}

	void BaseLayer::DestroyBaseLayer()
	{
		ReleaseRender();
		ReleaseFactory();
		ZeroThisOBJ();
	}

	void BaseLayer::Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent)
	{
		WNDCLASSEXW wcx = { 0 };
		wcx.cbSize = sizeof(wcx);
		wcx.hInstance = this->GetInstance();
		wcx.lpszClassName = wpClass;
		wcx.lpfnWndProc = DefWindowProcW;
		this->RegClass(wcx);
		this->CreateWnd(wpClass, wpTtile, uiStyle, uiStyleEx, hParent, 0);
	}

	void BaseLayer::SetRenderInvalid()
	{
		this->m_isRenderValid = false;
	}

	void BaseLayer::SureRenderValid()
	{
		CreateRender();
	}

	ID2D1HwndRenderTarget* BaseLayer::GetRender()
	{
		if (m_pRenderTarget == nullptr) { CreateRender(); }
		return m_pRenderTarget;
	}
}