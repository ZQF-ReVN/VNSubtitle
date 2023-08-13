#include "../../lib/VNSubtitle/Player.h"
#include "../../lib/VNSubtitle/TextLayer.h"


void PlayerThread(VNSubtitle::Player* pPlayer)
{
	pPlayer->Play();
}

void SubtitleThread()
{
	try
	{
		HWND game_handle = VNSubtitle::GetGameHandle(L"GameWindowI", L"僜儔僐僀");

		VNSubtitle::TextLayer layer;
		{
			layer.Create(0, 0, 1600, 100);
			layer.EnableAlpha();
			layer.EnableThrough();
			layer.Show();
		}

		VNSubtitle::Player player(layer.GetHandle(), game_handle);
		{
			player.AddSub(L"壬戌之秋", 2000);
			player.AddSub(L"七月既望", 2000);
			player.AddSub(L"苏子与客泛舟游于赤壁之下", 2000);
			player.AddSub(L"清风徐来", 2000);
			player.AddSub(L"水波不兴", 2000);
			player.AddSub(L"举酒属客", 2000);
			player.AddSub(L"诵明月之诗", 2000);
			player.AddSub(L"歌窈窕之章", 2000);
			player.AddSub(L"少焉", 2000);
			player.AddSub(L"月出于东山之上", 2000);
			player.AddSub(L"徘徊于斗牛之间", 2000);
			player.AddSub(L"白露横江", 2000);
			player.AddSub(L"水光接天", 2000);

			player.SetFont(L"方正准圆_GBK", 55);
			player.SetFontColor(0xFFFFFFFF);
		}

		HANDLE player_th = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)PlayerThread, &player, 0, nullptr);

		layer.Loop();

		if (player_th) { WaitForSingleObject(player_th, INFINITE); }
	}
	catch (const std::runtime_error& err)
	{
		MessageBoxA(nullptr, err.what(), "ERROR", MB_OK);
		return;
	}
}

int main()
{
	HANDLE subtitle_handle = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)SubtitleThread, nullptr, 0, nullptr);
	if (subtitle_handle) { WaitForSingleObject(subtitle_handle, INFINITE); }
}