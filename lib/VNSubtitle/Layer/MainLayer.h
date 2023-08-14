#pragma once
#include "D2D/TextLayer.h"
#include "../Message.h"


namespace VNSubtitle::Layer::D2D
{
	HWND CreateLayerThread(const wchar_t* wpTtile);

	class MainLayer : public TextLayer
	{
	public:
		LRESULT HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

	};
}