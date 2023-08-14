#include "../../lib/VNSubtitle/VNSubtitle.h"

using namespace VNSubtitle;

void Subtitle()
{
	HWND game_handle = game_handle = ::FindWindowW(L"GameWindowI", L"僜儔僐僀");
	if (game_handle == nullptr) { return; }

	HWND layer_handle = Layer::D2D::CreateLayerThread(L"VNSubtitle");
	if (layer_handle == nullptr) { return; }

	Player player(layer_handle, game_handle);
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


int main()
{
	Subtitle();
}