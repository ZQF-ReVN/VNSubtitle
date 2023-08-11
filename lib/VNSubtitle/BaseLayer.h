#pragma once
#include "../RxUI/Win32.h"

#include <d2d1.h>


namespace VNSubtitle
{
	class BaseLayer : public RxUI::Win32::Window::MainWindow
	{	
	private:
		bool m_isRenderValid;
		uint32_t m_uiWidth, m_uiHeight;

		ID2D1Factory* m_pID2D1Factory;
		ID2D1HwndRenderTarget* m_pRenderTarget;

	protected:
		template <typename PTR_TYPE> void SafeRelease(PTR_TYPE& pT){ if (pT) { pT->Release(); pT = nullptr; }}

	private:
		void CreateFactory();
		void CreateRender();
		void ReleaseRender();
		void ReleaseFactory();

	public:
		BaseLayer();
		~BaseLayer();
		void ZeroThisOBJ();

		void CreateBaseLayer(uint32_t uiPosX = CW_USEDEFAULT, uint32_t uiPosY = CW_USEDEFAULT, uint32_t uiWidth = 1280, uint32_t uiHeight = 720);
		void DestroyBaseLayer();

		void Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent = nullptr) override;

	public:
		void SetRenderInvalid();
		void SureRenderValid();
		uint32_t GetBaseLayerWidth() const { return m_uiWidth; }
		uint32_t GetBaseLayerHeight() const { return m_uiHeight; }
		ID2D1HwndRenderTarget* GetRender();
	};
}