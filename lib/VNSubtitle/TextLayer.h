#pragma once
#include "BaseLayer.h"

#include <string>
#include <dwrite.h>


namespace VNSubtitle
{
	class TextLayer : public BaseLayer
	{
	private:
		IDWriteFactory* m_pIDWriteFactory;
		IDWriteTextFormat* m_pIDWriteTextFormat;

		ID2D1SolidColorBrush* m_pID2D1FontColor;

	private:
		void CreateFactory();
		void ReleaseFactory();

	public:
		TextLayer();
		~TextLayer();

		void Create(int32_t iPosX = CW_USEDEFAULT, int32_t iPosY = CW_USEDEFAULT, int32_t iWidth = 1280, int32_t iHeight = 720);
		void PutSring(std::wstring_view wsText);

	public:
		void SetFont(std::wstring_view wsFont, uint32_t uiSize);
		void SetFontColor(uint32_t uiRGBA);

	private:
		LRESULT HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	};
}