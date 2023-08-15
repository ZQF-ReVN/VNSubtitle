#pragma once
#include "../../../RxUI/Win32.h"

#include <d2d1.h>
#include <dwrite.h>


namespace VNSubtitle::Layer::D2D
{
	class BaseLayer : public RxUI::Win32::Window::MainWindow
	{	
	private:
		ID2D1Factory* m_pID2D1Factory;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		ID2D1SolidColorBrush* m_pID2D1FontBrush;

		IDWriteFactory* m_pIDWriteFactory;
		IDWriteTextFormat* m_pIDWriteTextFormat;

		uint32_t m_uiBackColorRGBA;
		uint32_t m_uiFontColorRGBA;
		uint32_t m_uiFontSize;
		std::wstring m_wsText;
		std::wstring m_wsFont;

	private:
		template <typename PTR_TYPE> void SafeRelease(PTR_TYPE& pT){ if (pT) { pT->Release(); pT = nullptr; }}

		void CreateFactory();
		void ReleaseFactory();
		void CreateResouces();
		void ReleaseResouces();

		void CreateRenderTarget();
		void CreateFontBrush();
		void CreateTextFormatter();

		void ZeroThisOBJ();

	public:
		BaseLayer();
		~BaseLayer();

		void Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle = WS_POPUP, uint32_t uiStyleEx = WS_EX_TOPMOST, HWND hParent = nullptr) override;

		void OnCreate(WPARAM wParam, LPARAM lParam) override;
		void OnSize(WPARAM wType, uint32_t uiWidth, uint32_t uiHeigh) override;
		void OnPaint() override;
		void DrawBack(ID2D1HwndRenderTarget* pTarget);
		void DrawStr(ID2D1HwndRenderTarget* pTarget);

		void PutText(std::wstring_view wsText);
		void SetFont(std::wstring_view wsFont, uint32_t uiSize);
		void SetFontSize(uint32_t uiSize);
		void SetFontColor(uint32_t uiRGBA);
		void SetBackColor(uint32_t uiRGBA);

		uint32_t GetFontSize();

	public:
		ID2D1HwndRenderTarget* GetRender();
	};
}