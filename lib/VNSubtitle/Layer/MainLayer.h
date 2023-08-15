#pragma once
#include "D2D/BaseLayer.h"
#include "../Message.h"


namespace VNSubtitle::Layer::D2D
{
	HWND CreateLayerThread(const wchar_t* wpTtile);

	class MainLayer : public BaseLayer
	{
	private:
		LRESULT OnNCClick(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
		void OnMouseWheel(int16_t sDistance, uint16_t usFlag, uint16_t usCurX, uint16_t usCurY) override;

	public:
		LRESULT AppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	};
}