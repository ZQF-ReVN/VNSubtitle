#pragma once
#include "../../../RxUI/Win32.h"

#include <d2d1.h>


namespace VNSubtitle::Layer::D2D
{
	class BaseLayer : public RxUI::Win32::Window::MainWindow
	{	
	private:
		ID2D1Factory* m_pID2D1Factory;
		ID2D1HwndRenderTarget* m_pRenderTarget;

	protected:
		template <typename PTR_TYPE> void SafeRelease(PTR_TYPE& pT){ if (pT) { pT->Release(); pT = nullptr; }}

		void CreateFactory();
		void CreateRender();
		void ReleaseRender();
		void ReleaseFactory();

	public:
		BaseLayer();
		~BaseLayer();
		void ZeroThisOBJ();

		void CreateBaseLayer(const wchar_t* wpClass, const wchar_t* wpTtile);
		void DestroyBaseLayer();

		void SetSize(int32_t iWidth, int32_t iHeigh);

		void Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent) override;

	public:
		void SureRender();
		ID2D1HwndRenderTarget* GetRender();
	};
}