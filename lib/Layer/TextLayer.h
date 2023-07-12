#pragma once
#include "Layer.h"

#include <string>
#include <dwrite.h>


namespace RiaTextLayer
{
	class TextLayer : public RiaLayer::BaseLayer
	{
	private:
		IDWriteFactory* m_pIDWriteFactory;
		IDWriteTextFormat* m_pIDWriteTextFormat;

		ID2D1SolidColorBrush* m_pID2D1FontColor;

	private:
		void CreateDWriteFactory();
		void ReleaseDWriteFactory();

	public:
		TextLayer();
		~TextLayer();

		void Create(uint32_t uiPosX = CW_USEDEFAULT, uint32_t uiPosY = CW_USEDEFAULT, uint32_t uiWidth = 1280, uint32_t uiHeight = 720);
		void Draw(const	std::wstring& wsText);

	public:
		void SetFont(const std::wstring& wsFont, const float flSize);
		void SetFontColor(const D2D1::ColorF& cColor);
	};
}