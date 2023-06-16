#include "VNSubtitle_GDI.hpp"

using namespace VNSubtitle;


int main()
{
	VNSubtitleWindow vn_sub(0, L"GameWindowI", L"僜儔僐僀");

	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"黑体", L"《赤壁赋》" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"微软雅黑", L"宋·苏轼" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"宋体", L"壬戌之秋，七月既望，苏子与客泛舟游于赤壁之下。" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"楷体", L"清风徐来，水波不兴。" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"黑体", L"举酒属客，诵明月之诗，歌窈窕之章。" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"黑体", L"少焉，月出于东山之上，徘徊于斗牛之间。" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"楷体", L"白露横江，水光接天。" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"黑体", L"纵一苇之所如，凌万顷之茫然。" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"楷体", L"浩浩乎如冯虚御风，而不知其所止；" });
	vn_sub.AddSubtitle(VNSubtitleEntry{ 0, 720, 1280, 50, 100, 2000, 25, 0xFFFFFF, 0x0,L"黑体", L"飘飘乎如遗世独立，羽化而登仙。" });

	vn_sub.Run();
}