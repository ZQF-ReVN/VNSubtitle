#include "BaseLayer.h"

#include <stdexcept>

#pragma comment(lib, "d2d1.lib")


namespace VNSubtitle::Layer::D2D
{
	void BaseLayer::CreateFactory()
	{
		ReleaseFactory();
		HRESULT d2d_factory_hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pID2D1Factory);
		if (FAILED(d2d_factory_hr)) { throw std::runtime_error("CreateFactory Error!"); }
	}

	void BaseLayer::CreateRender()
	{
		if (m_pRenderTarget) { return; }
		ReleaseRender();

		D2D1_SIZE_U size = { this->GetWidth(), this->GetHeigh() };
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties
		(
			D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
		);
		m_pID2D1Factory->CreateHwndRenderTarget(props, D2D1::HwndRenderTargetProperties(this->GetHandle(), size), &m_pRenderTarget);

		if (m_pRenderTarget == nullptr) { throw std::runtime_error("Create RenderTarget Error!"); }
	}

	void BaseLayer::ReleaseRender()
	{
		SafeRelease(m_pRenderTarget);
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

	void BaseLayer::CreateBaseLayer(const wchar_t* wpClass, const wchar_t* wpTtile)
	{
		Create(wpClass, wpTtile, WS_POPUP, WS_EX_TOPMOST, nullptr);
		CreateFactory();
	}

	void BaseLayer::DestroyBaseLayer()
	{
		ReleaseRender();
		ReleaseFactory();
		ZeroThisOBJ();
	}

	void BaseLayer::SetSize(int32_t iWidth, int32_t iHeigh)
	{
		Window_Basic::SetSize(iWidth, iHeigh);
		ReleaseRender();
		CreateRender();
	}

	void BaseLayer::Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent)
	{
		WNDCLASSEXW wcx = { 0 };
		wcx.cbSize = sizeof(wcx);
		wcx.hInstance = this->GetInstance();
		wcx.lpszClassName = wpClass;
		//wcx.style = CS_HREDRAW | CS_HREDRAW;
		wcx.lpfnWndProc = Window_Basic::WndProc;;
		//wcx.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
		this->RegClass(wcx);
		this->CreateWnd(wpClass, wpTtile, uiStyle, uiStyleEx, hParent, (intptr_t)this);
	}

	void BaseLayer::SureRender()
	{
		CreateRender();
	}

	ID2D1HwndRenderTarget* BaseLayer::GetRender()
	{
		SureRender();
		return m_pRenderTarget;
	}
}