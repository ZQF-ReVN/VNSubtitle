#pragma once
#include "MemDC.h"

#include <d2d1.h>


namespace RiaLayer
{
	class BaseLayer
	{
	private:
		HWND m_hWnd;
		MemDC m_MemDC;
		uint32_t m_uiWidth;
		uint32_t m_uiHeight;
		bool m_isRenderValid;

		ID2D1Factory* m_pID2D1Factory;
		ID2D1DCRenderTarget* m_pRenderTarget;

	protected:
		template <typename PTR_TYPE>
		void SafeRelease(PTR_TYPE& pT)
		{
			if (pT) 
			{
				pT->Release();
				pT = nullptr;
			}
		}

	private:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
		void CreateWnd(uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight);
		void CreateMemDC(uint32_t uiWidth, uint32_t uiHeight);
		void CreateFactory();
		void CreateRender();
		void ReleaseRender();
		void ReleaseFactory();
		void ReleaseMemDC();
		void ReleaseWnd();

	public:
		BaseLayer();
		~BaseLayer();
		void ReleaseAll();
		void ZeroThisOBJ();

		void CreateBaseLayer(uint32_t uiPosX = CW_USEDEFAULT, uint32_t uiPosY = CW_USEDEFAULT, uint32_t uiWidth = 1280, uint32_t uiHeight = 720);
		void DestroyBaseLayer();
		void Update(uint32_t uiPosX, uint32_t uiPosY);
		void Show(uint32_t uiShow = SW_NORMAL);
		void Loop();

	public:
		void SetRenderInvalid();
		void SureRenderValid();
		void ReSizeBaseLayer(uint32_t uiPosX = CW_USEDEFAULT, uint32_t uiPosY = CW_USEDEFAULT, uint32_t uiWidth = 1280, uint32_t uiHeight = 720);
		uint32_t GetBaseLayerWidth() const { return m_uiWidth; }
		uint32_t GetBaseLayerHeight() const { return m_uiHeight; }
		ID2D1DCRenderTarget* GetRender();
	};
}