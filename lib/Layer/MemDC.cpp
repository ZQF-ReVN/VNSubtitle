#include "MemDC.h"


namespace RiaLayer
{
	MemDC::MemDC()
	{
		ZeroMemDC();
	}

	MemDC::MemDC(HWND hWnd, uint32_t uiWidth, uint32_t uiHeight)
	{
		ZeroMemDC();
		Init(hWnd, uiWidth, uiHeight);
	}

	MemDC::~MemDC()
	{
		DeleteDC(m_hMemDC);
		DeleteObject(m_hBitMap);
		ZeroMemDC();
	}

	void MemDC::CreateMemDC()
	{
		if (m_hMemDC) { return; }
		m_hDC = GetDC(m_hMainWnd);
		m_hMemDC = CreateCompatibleDC(m_hDC);
		m_hBitMap = CreateCompatibleBitmap(m_hDC, m_uiWidth, m_uiHeight);
		SelectObject(m_hMemDC, m_hBitMap);
		ReleaseDC(m_hMainWnd, m_hDC);
	}

	void MemDC::ZeroMemDC()
	{
		memset(this, 0x0, sizeof(*this));
	}

	void MemDC::Init(HWND hWnd, uint32_t uiWidth, uint32_t uiHeight)
	{
		this->m_hMainWnd = hWnd;
		this->m_uiWidth = uiWidth;
		this->m_uiHeight = uiHeight;
		CreateMemDC();
	}
}