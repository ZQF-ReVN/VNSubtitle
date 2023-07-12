#include "Layer.h"

#include <stdexcept>

#pragma comment(lib, "d2d1.lib")


namespace RiaLayer
{
	void BaseLayer::CreateWnd(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight)
	{
		WNDCLASSEX wcex = { 0 };
		wcex.cbSize = sizeof(wcex);
		wcex.hInstance = GetModuleHandleW(NULL);
		wcex.lpszClassName = L"Direct2D Transparency Class";
		wcex.lpfnWndProc = DefWindowProcW;

		RegisterClassExW(&wcex);

		m_hWnd = CreateWindowExW
		(
			WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_NOACTIVATE,
			L"Direct2D Transparency Class", L"BaseLayer",
			WS_POPUP,
			uiPosX, uiPosY, uiWidth, uiHeight, NULL, NULL, wcex.hInstance, NULL
		);
		if (m_hWnd == NULL) { throw std::runtime_error("Create Layer Window Error!"); }

		SetWindowLongW(m_hWnd, GWLP_USERDATA, (LONG)this);
	}

	void BaseLayer::CreateMemDC(uint32_t uiWidth, uint32_t uiHeight)
	{
		m_MemDC.Init(m_hWnd, uiWidth, uiHeight);
	}

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
		m_pID2D1Factory->CreateDCRenderTarget(&props, &m_pRenderTarget);
		if (m_pRenderTarget == nullptr) { throw std::runtime_error("Create RenderTarget Error!"); }

		RECT rc = { 0 };
		GetClientRect(m_MemDC.GetHWND(), &rc);
		m_pRenderTarget->BindDC(m_MemDC.GetMemDC(), &rc);

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

	void BaseLayer::ReleaseMemDC()
	{
		m_MemDC.~MemDC();
	}

	void BaseLayer::ReleaseWnd()
	{
		DestroyWindow(m_hWnd);
	}

	BaseLayer::BaseLayer()
	{
		ZeroThisOBJ();
	}

	BaseLayer::~BaseLayer()
	{
		DestroyBaseLayer();
	}

	void BaseLayer::ReleaseAll()
	{
		ReleaseRender();
		ReleaseFactory();
		ReleaseMemDC();
		ReleaseWnd();
	}

	void BaseLayer::ZeroThisOBJ()
	{
		memset(this, 0x0, sizeof(*this));
	}

	void BaseLayer::CreateBaseLayer(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight)
	{
		m_uiWidth = uiWidth;
		m_uiHeight = uiHeight;
		CreateWnd(uiPosX, uiPosY, m_uiWidth, m_uiHeight);
		CreateMemDC(m_uiWidth, m_uiHeight);
		CreateFactory();
		CreateRender();
	}

	void BaseLayer::DestroyBaseLayer()
	{
		ReleaseAll();
		ZeroThisOBJ();
	}

	void BaseLayer::Update(uint32_t uiPosX, uint32_t uiPosY)
	{
		POINT point_src = { 0, 0 };
		POINT point_dst = { (LONG)uiPosX, (LONG)uiPosY };
		SIZE size = { (LONG)m_uiWidth, (LONG)m_uiHeight };
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = NULL;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;
		UpdateLayeredWindow(m_MemDC.GetHWND(), m_MemDC.GetHDC(), &point_dst, &size, m_MemDC.GetMemDC(), &point_src, 0, &blend, ULW_ALPHA);
	}

	void BaseLayer::Show(uint32_t uiShow)
	{
		ShowWindow(m_hWnd, uiShow);
	}

	void BaseLayer::Loop()
	{
		MSG msg = { 0 };
		while (GetMessageW(&msg, NULL, NULL, NULL))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}

	void BaseLayer::SetRenderInvalid()
	{
		this->m_isRenderValid = false;
	}

	void BaseLayer::SureRenderValid()
	{
		CreateRender();
	}

	ID2D1DCRenderTarget* BaseLayer::GetRender()
	{
		if (m_pRenderTarget == nullptr) { CreateRender(); }
		return m_pRenderTarget;
	}
}