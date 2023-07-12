#pragma once
#include <Windows.h>
#include <cstdint>


namespace RiaLayer
{
	class MemDC
	{
	private:
		HDC m_hDC;
		HDC m_hMemDC;
		HWND m_hMainWnd;
		HBITMAP m_hBitMap;
		uint32_t m_uiWidth;
		uint32_t m_uiHeight;

		void CreateMemDC();

	public:
		MemDC();
		MemDC(HWND hWnd, uint32_t uiWidth, uint32_t uiHeight);
		~MemDC();
		void ZeroMemDC();

		void Init(HWND hWnd, uint32_t uiWidth, uint32_t uiHeight);

	public:
		HDC GetHDC() const { return m_hDC; }
		HDC GetMemDC() const { return m_hMemDC; }
		HWND GetHWND() const { return m_hMainWnd; }
	};
}