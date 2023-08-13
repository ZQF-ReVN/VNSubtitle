#pragma once
#include <cstdint>
#include <Windows.h>


namespace VNSubtitle
{
	static constexpr uint32_t VNSM_PLAY = WM_APP + 0x1;
	static constexpr uint32_t VNSM_FONT = WM_APP + 0x2;
	static constexpr uint32_t VNSM_FONTCOLOR = WM_APP + 0x3;

	static constexpr uint32_t VNSM_TRACE = WM_APP + 0x4;
}