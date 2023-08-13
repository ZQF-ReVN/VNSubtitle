#pragma once
#include "../RxUI/Win32.h"

#include <d2d1.h>


namespace VNSubtitle
{
	class BaseLayer : public RxUI::Win32::Window::MainWindow
	{	
	private:
		int32_t m_iWidth, m_iHeigh;

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

		void CreateBaseLayer(int32_t iPosX = CW_USEDEFAULT, int32_t iPosY = CW_USEDEFAULT, int32_t iWidth = 1280, int32_t iHeight = 720);
		void DestroyBaseLayer();

		void SetPos(int32_t iPosX, int32_t iPosY);
		void SetSize(int32_t iWidth, int32_t iHeigh);

		void Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle, uint32_t uiStyleEx, HWND hParent) override;

	public:
		void SureRender();
		void SetWidth(uint32_t uiWidth) { m_iWidth = uiWidth; }
		void SetHeigh(uint32_t uiHeigh) { m_iHeigh = uiHeigh; }
		uint32_t GetWidth() const { return m_iWidth; }
		uint32_t GetHeight() const { return m_iHeigh; }
		ID2D1HwndRenderTarget* GetRender();
	};
}