#include <iostream>
#include "ContralWindow.h"
#include "../../lib/VNSubtitle/VNSubtitle.h"

using namespace VNSubtitle;


void PlayMode(HWND hLayer, HWND hGame)
{
	Player player(hLayer, hGame);
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

		player.EnableAlpha();
		player.EnableThrough();
		player.SetSize(1600, 80);
		player.SetBackColor(MakeRGBA(0, 0, 0, 160));
		player.Show();

		player.SetFont(L"方正准圆_GBK", 55);
		player.SetFontColor(MakeRGBA(255, 255, 255, 255));

		player.Play();
	}
}

void RuntimeMode(HWND hLayer, HWND hGame)
{
	ContralWindow demo_wnd(hLayer, hGame);
	demo_wnd.Create(L"ContralClass", L"Contral", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, WS_EX_TOPMOST);
	demo_wnd.SetCenter(500, 80);
	demo_wnd.Run();
	demo_wnd.Loop();
}

void TargetWindow()
{
	RxUI::Win32::Window::MainWindow target_window;
	target_window.Create(L"TragetWindowClass", L"TargetWinodw", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME);
	target_window.SetCenter(1280, 720);
	target_window.Show();
	target_window.Loop();
}

void Subtitle()
{
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)TargetWindow, nullptr, 0, nullptr);
	Sleep(100);

	HWND game_handle = game_handle = ::FindWindowW(L"TragetWindowClass", L"TargetWinodw");
	if (game_handle == nullptr) { MessageBoxW(NULL, L"Not Find Game Window", NULL, NULL); return; }

	HWND layer_handle = Layer::D2D::CreateLayerThread(L"VNSubtitle");
	if (layer_handle == nullptr) { return; }

	//PlayMode(layer_handle, game_handle);
	RuntimeMode(layer_handle, game_handle);
}

int main()
{
	Subtitle();
}