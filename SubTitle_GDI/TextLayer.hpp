#pragma once
#include <string>
#include <stdexcept>
#include <d2d1.h>
#include <dwrite.h>
#include <Windows.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


class MemDC
{
private:
	HDC m_hDC;
	HDC m_hMemDC;
	HWND m_hMainWnd;
	HBITMAP m_hBitMap;

	void CreateMemDC(HWND hWnd)
	{
		if (m_hMemDC) { return; }
		m_hMainWnd = hWnd;
		m_hDC = GetDC(m_hMainWnd);
		m_hMemDC = CreateCompatibleDC(m_hDC);
		m_hBitMap = CreateCompatibleBitmap(m_hDC, 1280, 720);
		SelectObject(m_hMemDC, m_hBitMap);
		ReleaseDC(m_hMainWnd, m_hDC);
	}

public:
	MemDC()
	{
		this->m_hDC = nullptr;
		this->m_hMemDC = nullptr;
		this->m_hBitMap = nullptr;
		this->m_hMainWnd = nullptr;
	}

	MemDC(HWND hWnd)
	{
		this->m_hDC = nullptr;
		this->m_hMemDC = nullptr;
		this->m_hBitMap = nullptr;
		this->m_hMainWnd = hWnd;
		CreateMemDC(hWnd);
	}

	~MemDC()
	{
		DeleteDC(m_hMemDC);
		DeleteObject(m_hBitMap);
	}

	void Init(HWND hWnd)
	{
		CreateMemDC(hWnd);
	}

	HDC GetHDC() const { return m_hDC; }
	HDC GetMemDC() const { return m_hMemDC; }
	HWND GetHWND() const { return m_hMainWnd; }
};

class TextLayer
{
private:
	HWND m_hWnd;
	MemDC m_MemDC;
	std::wstring m_wsLayer;

	bool m_isRenderValid;

	RECT m_rtText;

	ID2D1Factory* m_pID2D1Factory;
	ID2D1DCRenderTarget* m_pRenderTarget;
	ID2D1SolidColorBrush* m_pID2D1FontColor;

	IDWriteFactory* m_pIDWriteFactory;
	IDWriteTextFormat* m_pIDWriteTextFormat;

private:
	template <typename PTR_TYPE> void SafeRelease(PTR_TYPE& pT)
	{
		if (pT) {
			pT->Release();
			pT = nullptr;
		}
	}

	void CreateLayer(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight)
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
			L"Direct2D Transparency Class", m_wsLayer.c_str(),
			WS_POPUP,
			uiPosX, uiPosY, uiWidth, uiHeight, NULL, NULL, wcex.hInstance, NULL
		);
		if (m_hWnd == NULL) { throw std::runtime_error("Create Layer Window Error!"); }
	}

	void CreateFactory()
	{
		if (m_pIDWriteFactory || m_pID2D1Factory) { return; }
		HRESULT dwrite_factory_hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pIDWriteFactory);
		HRESULT d2d_factory_hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pID2D1Factory);
		if (FAILED(dwrite_factory_hr) || FAILED(d2d_factory_hr)) { throw std::runtime_error("CreateFactory Error!"); }
	}

	void CreateRender()
	{
		if (m_isRenderValid) { return; }

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

		SetFont(L"ºÚÌå", 25.0f);
		SetFontColor(D2D1::ColorF(1.0, 1.0, 1.0, 1.0f));

		m_isRenderValid = true;
	}

	void ReleaseRender()
	{
		SafeRelease(m_pRenderTarget);
		SafeRelease(m_pID2D1FontColor);
		SafeRelease(m_pIDWriteTextFormat);
		m_isRenderValid = false;
	}

	void ReleaseFactory()
	{
		SafeRelease(m_pID2D1Factory);
		SafeRelease(m_pIDWriteFactory);
	}

	void ReleaseLayer()
	{
		DestroyWindow(m_hWnd);
	}

public:
	TextLayer(const std::wstring& wsLayer)
	{
		this->m_hWnd = nullptr;
		this->m_wsLayer = wsLayer;

		this->m_rtText = { 0 };

		this->m_isRenderValid = false;

		this->m_pID2D1Factory = nullptr;
		this->m_pRenderTarget = nullptr;
		this->m_pID2D1FontColor = nullptr;

		this->m_pIDWriteFactory = nullptr;
		this->m_pIDWriteTextFormat = nullptr;
	}

	~TextLayer()
	{
		ReleaseLayer();
		ReleaseRender();
		ReleaseFactory();
	}

	void Create(uint32_t uiPosX = CW_USEDEFAULT, uint32_t uiPosY = CW_USEDEFAULT, uint32_t uiWidth = 1280, uint32_t uiHeight = 720)
	{
		m_rtText.left = 0;
		m_rtText.right = (LONG)uiWidth;
		m_rtText.top = 0;
		m_rtText.bottom = (LONG)uiHeight;

		CreateLayer(uiPosX, uiPosY, uiWidth, uiHeight);
		m_MemDC.Init(m_hWnd);
		CreateFactory();
		CreateRender();
	}

	void SetFont(const std::wstring& wsFont, const float flSize)
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

	void SetFontColor(const D2D1::ColorF& cColor)
	{
		SafeRelease(m_pID2D1FontColor);

		m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(cColor), &m_pID2D1FontColor);
		if (m_pID2D1FontColor == nullptr) { throw std::runtime_error("Create Font Color Brush Error!"); }
	}

	void Draw(const	std::wstring& wsText)
	{
		CreateRender();

		D2D1_RECT_F rect = { (float)m_rtText.left, (float)m_rtText.top, (float)m_rtText.right, (float)m_rtText.bottom };

		m_pRenderTarget->BeginDraw();
		{
			m_pRenderTarget->Clear();
			m_pRenderTarget->DrawTextW(wsText.c_str(), wsText.length(), m_pIDWriteTextFormat, rect, m_pID2D1FontColor);
		}

		if (SUCCEEDED(m_pRenderTarget->EndDraw())) { return; }

		ReleaseRender();
	}

	void Update(uint32_t uiPosX, uint32_t uiPosY)
	{
		POINT point_src = { 0, 0 };
		POINT point_dst = {(LONG)uiPosX, (LONG)uiPosY };
		SIZE size = { (LONG)m_rtText.right, (LONG)m_rtText.bottom };
		BLENDFUNCTION blend = { 0 };
		blend.BlendOp = AC_SRC_OVER;
		blend.BlendFlags = NULL;
		blend.SourceConstantAlpha = 255;
		blend.AlphaFormat = AC_SRC_ALPHA;
		UpdateLayeredWindow(m_MemDC.GetHWND(), m_MemDC.GetHDC(), &point_dst, &size, m_MemDC.GetMemDC(), &point_src, 0, &blend, ULW_ALPHA);
	}

	void Show(uint32_t uiShow = SW_NORMAL)
	{
		ShowWindow(m_hWnd, uiShow);
	}

	void Loop()
	{
		MSG msg = { 0 };
		while (GetMessageW(&msg, NULL, NULL, NULL))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
};