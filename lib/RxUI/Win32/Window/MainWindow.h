#pragma once
#include "Window_Basic.h"


namespace RxUI::Win32::Window
{
	class MainWindow : public Window_Basic<MainWindow>
	{
	public:
		MainWindow();
		virtual ~MainWindow();

		void Create(const wchar_t* wpClass, const wchar_t* wpTtile, uint32_t uiStyle = WS_OVERLAPPEDWINDOW, uint32_t uiStyleEx = 0, HWND hParent = nullptr);
		void Loop();

	protected:
		// All Msg
		virtual LRESULT HandleMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

		// App Msg
		virtual LRESULT AppMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);

		// OnCommand
		virtual void MenuCommand(uint32_t uiID) { }
		virtual void AcceleratorCommand(uint32_t uiID) { }
		virtual void WidgetCommand(HWND hWnd, uint32_t uiID);

		virtual void OnPaint() { }
		virtual void OnCreate(WPARAM wParam, LPARAM lParam) { }
		virtual void OnSize(WPARAM wType, uint32_t uiWidth, uint32_t uiHeigh) { }
		virtual void OnMouseWheel(int16_t sDistance, uint16_t usFlag, uint16_t usCurX, uint16_t usCurY) {};

		virtual LRESULT OnNCClick(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	};
}