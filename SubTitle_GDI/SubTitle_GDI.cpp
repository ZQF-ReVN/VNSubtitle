#include <string>
#include <vector>
#include <Windows.h>


class SubtitleWindow
{
private:
	HWND m_hWnd;
	HFONT m_hFont;
	HBRUSH m_hColorBK;
	COLORREF m_cColorFont;
	HWND m_targ_game_hwnd;
	uint32_t m_flush_time;
	uint32_t m_sub_timer_id;
	uint32_t m_width_window;
	uint32_t m_height_window;

	std::wstring m_wsText;
	std::vector<std::pair<std::size_t, std::wstring>> m_vecText;

private:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		SubtitleWindow* pOBJ = (SubtitleWindow*)GetWindowLongW(hWnd, GWLP_USERDATA);

		if (pOBJ != nullptr) 
		{
			switch (Msg)
			{
			case WM_PAINT: {if (pOBJ->OnPaint()) { return 0; }} break;
			case WM_TIMER: {if (pOBJ->OnTimer(wParam)) { return 0; }} break;
			}
		}

		return DefWindowProcW(hWnd, Msg, wParam, lParam);
	}

	HFONT MakeFont(const wchar_t* wpFontName, uint32_t uiSizeFont = 0)
	{
		LOGFONTW logfont = { 0 };

		logfont.lfHeight = uiSizeFont;
		logfont.lfWidth = 0;
		logfont.lfEscapement = 0;
		logfont.lfOrientation = 0;
		logfont.lfWeight = 0;
		logfont.lfItalic = FALSE;
		logfont.lfUnderline = FALSE;
		logfont.lfStrikeOut = NULL;
		logfont.lfCharSet = GB2312_CHARSET;
		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = PROOF_QUALITY;
		logfont.lfPitchAndFamily = DEFAULT_PITCH;
		wcscpy_s(logfont.lfFaceName, 32, wpFontName);

		return CreateFontIndirectW(&logfont);
	}

	HWND MakeWindow(HINSTANCE hInstance, uint32_t uiPosX, uint32_t uiPosY, uint32_t uiWidth, uint32_t uiHeight)
	{
		WNDCLASS wcx = { 0 };
		wcx.hInstance = hInstance;
		wcx.lpszClassName = L"Subtitle Window Class";
		wcx.lpfnWndProc = SubtitleWindow::WindowProc;

		RegisterClassW(&wcx);

		HWND hwnd = CreateWindowExW
		(
			WS_EX_TOPMOST | WS_EX_TOOLWINDOW | WS_EX_TRANSPARENT | WS_EX_LAYERED,
			wcx.lpszClassName,
			L"Subtitle Window",
			WS_POPUP,
			uiPosX, uiPosY,
			uiWidth, uiHeight,
			nullptr,
			nullptr,
			wcx.hInstance,
			nullptr
		);

		SetWindowLongW(hwnd, GWLP_USERDATA, (LONG)this);
		SetTimer(hwnd, m_sub_timer_id, m_flush_time, NULL);
		SetLayeredWindowAttributes(hwnd, RGB(255, 255, 255), 200, LWA_ALPHA);
		return hwnd;
	}

	void Move()
	{
		RECT game_rect = { 0 };
		POINT game_clien_pos = { 0 };
		GetClientRect(m_targ_game_hwnd, &game_rect);
		ClientToScreen(m_targ_game_hwnd, &game_clien_pos);
		MoveWindow(m_hWnd, game_clien_pos.x, game_clien_pos.y, game_rect.right, m_height_window, FALSE);
	}

	template <typename TOBJ>
	static bool SafeFree(TOBJ tOBJ)
	{
		return tOBJ ? DeleteObject(tOBJ) : false;
	}

public:
	void SetFont(const wchar_t* wpFontName, uint32_t uiSizeFont)
	{
		SafeFree(m_hFont);
		m_hFont = MakeFont(wpFontName, uiSizeFont);
	}

	void SetBKColor(COLORREF cBKColor)
	{
		SafeFree(m_hColorBK);
		m_hColorBK = CreateSolidBrush(cBKColor);
	}

	void SetFontColor(COLORREF cFontColor)
	{
		m_cColorFont = cFontColor;
	}



	bool OnPaint()
	{
		RECT rect = { 0 };
		PAINTSTRUCT ps = { 0 };
		GetClientRect(m_hWnd, &rect);
		HDC hdc = BeginPaint(m_hWnd, &ps);

		SelectObject(hdc, m_hFont);
		SetTextColor(hdc, m_cColorFont);
		SetBkMode(hdc, TRANSPARENT);

		FillRect(hdc, &ps.rcPaint, m_hColorBK);

		if (m_wsText.length())
		{
			DrawTextW(hdc, m_wsText.c_str(), m_wsText.length(), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		EndPaint(m_hWnd, &ps);
		return true;
	}

	bool OnTimer(WPARAM wParam)
	{
		if (wParam == m_sub_timer_id)
		{
			Move();
			static uint32_t time_milliseconds = 0;
			static auto ite_text = m_vecText.begin();
			time_milliseconds += m_flush_time;

			if (ite_text == m_vecText.end())
			{
				PostMessageW(m_hWnd, WM_QUIT, NULL, NULL);
			}

			if (time_milliseconds >= ite_text->first)
			{
				m_wsText = ite_text->second;
				InvalidateRect(m_hWnd, NULL, FALSE);
				ite_text++;
			}
		}

		return true;
	}

	void Show()
	{
		Move();
		ShowWindow(m_hWnd, SW_NORMAL);
	}

	void Run()
	{
		if (m_targ_game_hwnd == NULL)
		{
			return;
		}

		MSG msg = { 0 };
		while (GetMessageW(&msg, this->m_hWnd, NULL, NULL))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}


	void AddSubtitle(std::pair<std::size_t, std::wstring> pairSub)
	{
		m_vecText.emplace_back(pairSub);
	}

	SubtitleWindow(HINSTANCE hInstance, const wchar_t* wpClassName, const wchar_t* wpWindowName, uint32_t uiWidth, uint32_t uiHeight)
	{
		m_targ_game_hwnd = FindWindowW(wpClassName, wpWindowName);

		m_width_window = uiWidth;
		m_height_window = uiHeight;

		m_flush_time = 200;
		m_sub_timer_id = 1008611;

		m_hWnd = MakeWindow(hInstance, 0, 0, uiWidth, uiHeight);
		m_hFont = MakeFont(L"黑体", 25);
		m_hColorBK = CreateSolidBrush(RGB(64, 0, 255));
		m_cColorFont = RGB(255, 255, 255);
	}

	~SubtitleWindow()
	{
		SafeFree(m_hFont);
		SafeFree(m_hColorBK);
	}
};


INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
	SubtitleWindow sub(hInstance, L"GameWindowI", L"僜儔僐僀", 1000, 50);

	sub.AddSubtitle(std::make_pair(1000 * 0, L"《赤壁赋》"));
	sub.AddSubtitle(std::make_pair(1000 * 1, L""));
	sub.AddSubtitle(std::make_pair(1000 * 2, L"宋·苏轼"));
	sub.AddSubtitle(std::make_pair(1000 * 3, L"壬戌之秋，七月既望，苏子与客泛舟游于赤壁之下。"));
	sub.AddSubtitle(std::make_pair(1000 * 4, L""));
	sub.AddSubtitle(std::make_pair(1000 * 5, L"清风徐来，水波不兴。"));
	sub.AddSubtitle(std::make_pair(1000 * 6, L""));
	sub.AddSubtitle(std::make_pair(1000 * 7, L"举酒属客，诵明月之诗，歌窈窕之章。"));
	sub.AddSubtitle(std::make_pair(1000 * 8, L""));
	sub.AddSubtitle(std::make_pair(1000 * 9, L"少焉，月出于东山之上，徘徊于斗牛之间。"));
	sub.AddSubtitle(std::make_pair(1000 * 12, L"白露横江，水光接天。"));
	sub.AddSubtitle(std::make_pair(1000 * 15, L"纵一苇之所如，凌万顷之茫然。"));
	sub.AddSubtitle(std::make_pair(1000 * 17, L"浩浩乎如冯虚御风，而不知其所止；"));
	sub.AddSubtitle(std::make_pair(1000 * 19, L"飘飘乎如遗世独立，羽化而登仙。"));

	sub.Show();
	sub.Run();
}