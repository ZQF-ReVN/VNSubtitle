#pragma once
#include <cstdint>
#include <Windows.h>


namespace VNSubtitle
{
	static constexpr uint32_t VNSM_TRACE = WM_APP + 0x2;
	static constexpr uint32_t VNSM_PUT_TEXT = WM_APP + 0x3;
	static constexpr uint32_t VNSM_SET_FONT = WM_APP + 0x4;
	static constexpr uint32_t VNSM_SET_FONTCOLOR = WM_APP + 0x5;
	static constexpr uint32_t VNSM_SET_POS = WM_APP + 0x6;
	static constexpr uint32_t VNSM_SET_SIZE = WM_APP + 0x7;
	static constexpr uint32_t VNSM_EN_ALPHA = WM_APP + 0x8;
	static constexpr uint32_t VNSM_EN_THROUGH = WM_APP + 0x9;
	static constexpr uint32_t VNSM_SHOW = WM_APP + 0xA;
	static constexpr uint32_t VNSM_SET_BKCOLOR = WM_APP + 0xB;
}