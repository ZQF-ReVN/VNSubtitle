#pragma once
#include <string>
#include <vector>
#include <d2d1.h>
#include <dwrite.h>
#include <Windows.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")


//namespace VNSubtitle
//{
//	struct VNSubtitleEntry
//	{
//		uint32_t uiPosX;
//		uint32_t uiPosY;
//		uint32_t uiWidth;
//		uint32_t uiHeight;
//		uint32_t uiBegTime;
//		uint32_t uiEndTime;
//		uint32_t uiFontSize;
//		uint32_t uiFonrColor;
//		uint32_t uiBackColor;
//		std::wstring wsFont;
//		std::wstring wsText;
//	};
//
//	class VNSubtitleWindow
//	{
//	private:
//		HWND m_hTextHwnd;
//		HWND m_hGameHwnd;
//		HFONT m_hTextFont;
//		HBRUSH m_hBackColor;
//		COLORREF m_cTextColor;
//
//		uint32_t m_uiBegTimerID;
//		uint32_t m_uiEndTimerID;
//
//		std::wstring* m_wsTextPtr;
//		std::vector<VNSubtitleEntry> m_vecText;
//
//	private:
//		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
//		{
//			VNSubtitleWindow* pOBJ = (VNSubtitleWindow*)GetWindowLongW(hWnd, GWLP_USERDATA);
//
//			if (pOBJ != nullptr)
//			{
//				switch (Msg)
//				{
//				case WM_PAINT: {if (pOBJ->OnPaint()) { return 0; }} break;
//				case WM_TIMER: {if (pOBJ->OnTimer(wParam)) { return 0; }} break;
//				}
//			}
//
//			return DefWindowProcW(hWnd, Msg, wParam, lParam);
//		}
//
//		HWND SetWindow(HINSTANCE hInstance)
//		{
//			WNDCLASSEX wcex = { 0 };
//			wcex.hInstance = hInstance;
//			wcex.lpszClassName = L"Subtitle Window Class";
//			wcex.lpfnWndProc = VNSubtitleWindow::WindowProc;
//
//			RegisterClassExW(&wcex);
//
//			HWND hwnd = CreateWindowExW
//			(
//				WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED,
//				wcex.lpszClassName,
//				L"Subtitle Window",
//				WS_POPUP,
//				0, 0,
//				0, 0,
//				nullptr,
//				nullptr,
//				wcex.hInstance,
//				nullptr
//			);
//
//			return hwnd;
//		}
//
//		void SetPos(VNSubtitleEntry& subEntry)
//		{
//			POINT game_clien_pos = { 0 };
//			ClientToScreen(m_hGameHwnd, &game_clien_pos);
//			MoveWindow(m_hTextHwnd, game_clien_pos.x + subEntry.uiPosX, game_clien_pos.y + subEntry.uiPosY, subEntry.uiWidth, subEntry.uiHeight, FALSE);
//		}
//
//		void SetBack(VNSubtitleEntry& subEntry)
//		{
//			SafeFree(m_hBackColor);
//			m_hBackColor = CreateSolidBrush(subEntry.uiBackColor);
//		}
//
//
//		bool OnTimer(WPARAM wParam)
//		{
//			static auto entry = m_vecText.begin();
//
//			if (entry == m_vecText.end())
//			{
//				KillTimer(m_hTextHwnd, m_uiBegTimerID);
//				KillTimer(m_hTextHwnd, m_uiEndTimerID);
//				PostMessageW(m_hTextHwnd, WM_QUIT, NULL, NULL);
//				return true;
//			}
//
//			if (wParam == m_uiBegTimerID)
//			{
//				KillTimer(m_hTextHwnd, m_uiBegTimerID);
//				UpdateState(*entry);
//
//				m_wsTextPtr = &entry->wsText;
//
//				InvalidateRect(m_hTextHwnd, NULL, FALSE);
//				SetTimer(m_hTextHwnd, m_uiEndTimerID, entry->uiEndTime, NULL);
//
//				entry++;
//				return true;
//			}
//
//			if (wParam == m_uiEndTimerID)
//			{
//				KillTimer(m_hTextHwnd, m_uiEndTimerID);
//
//				InvalidateRect(m_hTextHwnd, NULL, FALSE);
//				m_wsTextPtr = nullptr;
//
//				SetTimer(m_hTextHwnd, m_uiBegTimerID, entry->uiBegTime, NULL);
//				return true;
//			}
//
//			return true;
//		}
//
//		bool OnPaint()
//		{
//			PAINTSTRUCT ps = { 0 };
//			HDC hdc = BeginPaint(m_hTextHwnd, &ps);
//
//			//FillRect(hdc, &ps.rcPaint, m_hBackColor);
//
//			if (m_wsTextPtr != nullptr)
//			{
//				RECT rect = { 0 };
//				GetClientRect(m_hTextHwnd, &rect);
//				SelectObject(hdc, m_hTextFont);
//				SetTextColor(hdc, m_cTextColor);
//				SetBkMode(hdc, TRANSPARENT);
//
//				DrawTextW(hdc, m_wsTextPtr->c_str(), m_wsTextPtr->length(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
//			}
//
//			EndPaint(m_hTextHwnd, &ps);
//			return true;
//		}
//
//		void UpdateState(VNSubtitleEntry& subEntry)
//		{
//			SetPos(subEntry);
//			SetFont(subEntry);
//			SetBack(subEntry);
//		}
//
//		template <typename TOBJ>
//		static bool SafeFree(TOBJ tOBJ)
//		{
//			if (tOBJ)
//			{
//				if (DeleteObject(tOBJ))
//				{
//					tOBJ = NULL;
//					return true;
//				}
//
//				return false;
//			}
//
//			return false;
//		}
//
//	public:
//		VNSubtitleWindow(HINSTANCE hInstance, const wchar_t* wpClassName, const wchar_t* wpWindowName)
//		{
//			this->m_hTextHwnd = SetWindow(hInstance);
//			this->m_hGameHwnd = FindWindowW(wpClassName, wpWindowName);
//
//			this->m_hTextFont = 0;
//			this->m_hBackColor = 0;
//			this->m_cTextColor = 0;
//
//			this->m_uiEndTimerID = 1008600;
//			this->m_uiBegTimerID = 1008611;
//
//			this->m_wsTextPtr = nullptr;
//		}
//
//		~VNSubtitleWindow()
//		{
//			KillTimer(m_hTextHwnd, m_uiBegTimerID);
//			KillTimer(m_hTextHwnd, m_uiEndTimerID);
//			PostMessageW(m_hTextHwnd, WM_QUIT, NULL, NULL);
//			DestroyWindow(m_hTextHwnd);
//
//			SafeFree(m_hTextFont);
//			SafeFree(m_hBackColor);
//		}
//
//		void AddSubtitle(const VNSubtitleEntry& subEntry)
//		{
//			m_vecText.emplace_back(subEntry);
//		}
//
//		void Run()
//		{
//			if (m_hGameHwnd == NULL || m_vecText.size() == 0) { return; }
//
//			SetTimer(m_hTextHwnd, m_uiBegTimerID, m_vecText[0].uiBegTime, NULL);
//
//			ShowWindow(m_hTextHwnd, SW_NORMAL);
//
//			MSG msg = { 0 };
//			while (GetMessageW(&msg, this->m_hTextHwnd, NULL, NULL))
//			{
//				TranslateMessage(&msg);
//				DispatchMessageW(&msg);
//			}
//		}
//	};
//}

