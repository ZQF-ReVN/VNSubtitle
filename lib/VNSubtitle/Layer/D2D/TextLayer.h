#pragma once
#include "BaseLayer.h"

#include <string>
#include <dwrite.h>


namespace VNSubtitle::Layer::D2D
{
	class TextLayer : public BaseLayer
	{
	private:
		IDWriteFactory* m_pIDWriteFactory;
		IDWriteTextFormat* m_pIDWriteTextFormat;

		D2D1::ColorF m_coBackColor;
		ID2D1SolidColorBrush* m_pID2D1FontColor;

	private:
		void CreateFactory();
		void ReleaseFactory();

	public:
		TextLayer();
		~TextLayer();

		void Create(const wchar_t* wpClass, const wchar_t* wpTtile);

		void SetBackColor(uint32_t uiRGBA);
		void PutText(std::wstring_view wsText);
		void SetFont(std::wstring_view wsFont, uint32_t uiSize);
		void SetFontColor(uint32_t uiRGBA);

	};
}