#include "myprepare.h"
#include "tools.h"
#include "firstLevel.h"
#include "secondLevel.h"
#include "thirdLevel.h"

IMAGE bkimg_up, bkimg_down;//游戏界面大背景
IMAGE bkimg_up_st, bkimg_down_st;//开始界面大背景
IMAGE bkimg_up_option, bkimg_down_option;//选项框内大背景
IMAGE option_openedlevel_img[2];//[已开放关卡]选项
IMAGE option_closedlevel_img[2];//[未开放关卡]选项
IMAGE option_returnback_img[2];//[返回开始界面]选项
IMAGE option_gamestart_img[2];//[开始游戏]选项
IMAGE option_gameinfo_img[2];//[游戏说明]选项
IMAGE text_Title_img;//大标题文字

const int MAX_HP_cnt = 10;//最大HP值
const int MAX_level_cnt = 56;//游戏关卡数量(最多为56)
const int common_level_cnt = 3;//普通关卡数量
const int playerstyle_cnt = 1;//玩家样式数量
int isPass = 0;//判断某关是否通过

const COLOR player_style_color[playerstyle_cnt + 1] = { 0, pink };//玩家填充颜色（若干style）
const COLOR player_style_color_line[playerstyle_cnt + 1] = { 0, brown };//玩家边框颜色（若干style）
const COLOR player_shadow_color = darkgray;//玩家阴影颜色
const COLOR default_option_line_color[2] = { darkgray, lightgray };//默认选项边框颜色（正常状态 与 鼠标指向状态）
const COLOR default_option_text_color[2] = { lightcyan, white };//默认选项文字颜色（正常状态 与 鼠标指向状态）
const COLOR level_opened_option_text_color[2] = { RGB(0, 255, 30), white };//已开放关卡选项文字颜色（正常状态 与 鼠标指向状态）
const COLOR level_closed_option_text_color[2] = { lightgray, white };//未开放关卡选项文字颜色（正常状态 与 鼠标指向状态）
const COLOR default_debug_text_color = white;//默认debug区域文字颜色

#define level_endless MAX_level_cnt //无尽模式关卡

#define UP_LINE 1
#define LEFT_LINE 2
#define RIGHT_LINE 3
#define DOWN_LINE 4
#define LEFT_UP_ARC 5
#define RIGHT_UP_ARC 6
#define LEFT_DOWN_ARC 7
#define RIGHT_DOWN_ARC 8

namespace SETBOX{
	const Range mainbox = { 0, 0, 25 - 1, 720 - 1 };
	const Range debugbox = { 0, 720, 480 - 1, 820 - 1 };
	const Range playerbox = { 0, 700 - 7, 480 - 1, 700 + 7 };
	const Range gamestart_box = { 95, 480, 385, 560 };
	const Range gameinfo_box = { 95, 610, 385, 690 };
	const Range returnback_box = { 110, 625, 370, 675 };
	const Range gameinfo_text_box = { 7, 7, 473 - 1, 650 };
}
using namespace SETBOX;

namespace CONTROLER{
	int controlermode_make_level;
	int controlermode_level_allopen;
	inline void init(){
		controlermode_make_level = controlermode_level_allopen = 0;
	}
};
using CONTROLER::controlermode_make_level;
using CONTROLER::controlermode_level_allopen;
// 功能选项
namespace OPTION{
	struct Option{//选项
		bool msin;//选项状态(鼠标是否在其碰撞箱以内)
		Range box; IMAGE bkimg;//碰撞箱、选项背景
		int height, weight; STR text;//文字
		int line_weight;//边框厚度
		COLOR line_color[2], text_color[2];

		Option(Range R = { 0, 0, 0, 0 }, Re H = 0, Re W = 0, STR T = _T(""),
			COLOR line_color_0 = default_option_line_color[0], COLOR line_color_1 = default_option_line_color[1],
			COLOR text_color_0 = default_option_text_color[0], COLOR text_color_1 = default_option_text_color[1],
			Re line_W = 3){//构造函数
			line_color[0] = line_color_0, line_color[1] = line_color_1;
			text_color[0] = text_color_0, text_color[1] = text_color_1;
			box = R, height = H, weight = W, text = T, line_weight = line_W, msin = 0;
			bkimg = H ? getimage(box.boost(1, 1)) : IMAGE(0, 0);
		}

		inline void refresh(){//刷新选项
			Setbkmode(bkmode2), ClearRange(box.boost(1, 1), bkimg);//设置绘图透明、用选项背景清除该块
			Settextstyle(height, weight, text_color[msin], 0, msin), print(box, text);//画文字
		}

		inline bool updata_action(MOUSE m){//检查选项状态
			Point ms = Point(m.x, m.y);
			if (PIR(ms, box) ^ msin)msin ^= 1, refresh();//若选项存储的状态与当前状态不同，则刷新状态
			return (msin&&m.ms_info == ms_lefthit);//鼠标点击了选项
		}
	};
	struct Option_img{//选项(贴图版)
		bool msin;//选项状态(鼠标是否在其碰撞箱以内)
		Range box; IMAGE img0, img1;//碰撞箱、贴图、选项背景

		Option_img(Range R = { 0, 0, 0, 0 }, IMAGE img0_ = IMAGE(0, 0), IMAGE img1_ = IMAGE(0, 0)){//构造函数
			img0 = img0_, img1 = img1_, box = R, msin = 0;
		}
		inline void refresh(){ putimage_transparancy(box.x1, box.y1, msin ? &img1 : &img0); }//刷新选项
		inline bool updata_action(MOUSE m){//检查选项状态
			Point ms = Point(m.x, m.y);
			if (PIR(ms, box) ^ msin)msin ^= 1, refresh();//若选项存储的状态与当前状态不同，则刷新状态
			return (msin&&m.ms_info == ms_lefthit);//鼠标点击了选项
		}
	};
	struct Option_bkimg_text{//选项
		bool msin;//选项状态(鼠标是否在其碰撞箱以内)
		Range box; IMAGE bkimg;//碰撞箱、选项背景
		int height, weight; STR text;//文字
		IMAGE bkimg0, bkimg1;//背景贴图
		COLOR text_color[2];

		Option_bkimg_text(Range R = { 0, 0, 0, 0 }, Re H = 0, Re W = 0, STR T = _T(""),
			IMAGE img0_ = IMAGE(0, 0), IMAGE img1_ = IMAGE(0, 0),
			COLOR text_color_0 = default_option_text_color[0], COLOR text_color_1 = default_option_text_color[1]
			){//构造函数
			bkimg0 = img0_, bkimg1 = img1_;
			text_color[0] = text_color_0, text_color[1] = text_color_1;
			box = R, height = H, weight = W, text = T, msin = 0;
			bkimg = H ? getimage(box) : IMAGE(0, 0);
		}

		inline void refresh(){//刷新选项
			Setbkmode(bkmode2), ClearRange(box, bkimg);//设置绘图透明、用选项背景清除该块
			putimage_transparancy(box.x1, box.y1, msin ? &bkimg1 : &bkimg0);//画背景
			Settextstyle(height, weight, text_color[msin], 0, msin), print(box, text);//画文字
		}

		inline bool updata_action(MOUSE m){//检查选项状态
			Point ms = Point(m.x, m.y);
			if (PIR(ms, box) ^ msin)msin ^= 1, refresh();//若选项存储的状态与当前状态不同，则刷新状态
			return (msin&&m.ms_info == ms_lefthit);//鼠标点击了选项
		}
	};
	struct Level_option{//【选择关卡】选项
		Option_bkimg_text option; CH text[100];
		int levelid, levelopen;//该关卡是否已经通过(level限定)

		inline void refresh(){ option.refresh(); }//刷新选项
		inline bool updata_action(MOUSE m){ return option.updata_action(m); }//检查选项状态
		inline void get_option_img_and_color(IMAGE img[], COLOR text_color[]){
			if (levelopen)
				img[0] = option_openedlevel_img[0], img[1] = option_openedlevel_img[1],
				text_color[0] = level_opened_option_text_color[0], text_color[1] = level_opened_option_text_color[1];
			else
				img[0] = option_closedlevel_img[0], img[1] = option_closedlevel_img[1],
				text_color[0] = level_closed_option_text_color[0], text_color[1] = level_closed_option_text_color[1];
		}
		inline void open(){
			levelopen = 1;
			IMAGE bkimg[2]; COLOR text_color[2]; get_option_img_and_color(bkimg, text_color);
			option.bkimg0 = bkimg[0], option.bkimg1 = bkimg[1];
			option.text_color[0] = text_color[0], option.text_color[1] = text_color[1];
		}
		inline void init(Range R, Re id){
			levelid = id;
			if (id <= common_level_cnt)_stprintf(text, _T("Level %d"), id);
			else if (id == level_endless)_stprintf(text, _T("无尽模式"));
			else _stprintf(text, _T("待开发中"));

			levelopen = (id == 1);//关卡全开模式
			if (levelid>common_level_cnt)levelopen = 0;//关卡还未开发

			IMAGE bkimg[2]; COLOR text_color[2]; get_option_img_and_color(bkimg, text_color);
			option = Option_bkimg_text(R, 20, 200, text, bkimg[0], bkimg[1], text_color[0], text_color[1]);
		}
	};
}
using OPTION::Option;
using OPTION::Option_img;
using OPTION::Level_option;
//【游戏进行中】
namespace GAMERUNNING{
	Option_img returnback;//返回开始界面

	inline void refresh(){//【游戏进行中】刷新界面
		Setmain(mainbox, bkimg_up);//游戏界面主区域
		printdebug(_T("【第*关】"));
		returnback.msin = 0, returnback.refresh();
	}

	inline int updata(){//【游戏进行中】检查行为
		if (_mshit()){
			MOUSE m = _getms();
			if (returnback.updata_action(m))return -1;
		}
		return 0;
	}

	inline void GameRunning(int level){//【游戏进行中】主程序
		isPass = 0;
		refresh();
		if (level == 1){
			gameInit1(); //游戏的初始化
			while (1){
				int result = startGame1();
				if (result == 1){
					isPass = 1;
					return;
				}
				else if (result == -1){
					isPass = 0;
					return;
				}
			}
		}
		else if (level == 2){
			gameInit2(); //游戏的初始化
			while (1){
				int result = startGame2();
				if (result == 1){
					isPass = 1;
					return;
				}
				else if (result == -1){
					isPass = 0;
					return;
				}
			}
		}
		else if (level == 3){
			gameInit3(); //游戏的初始化
			while (1){
				int result = startGame3();
				if (result == 1){
					isPass = 1;
					return;
				}
				else if (result == -1){
					isPass = 0;
					return;
				}
			}
		}
	}

	inline void init(){//【游戏进行中】初始化
		Setmain(mainbox, bkimg_up);//游戏界面主区域
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//游戏界面调试区域

		Setmain(mainbox, bkimg_up_option);//选项界面主区域
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//选项界面调试区域
	}
}
using GAMERUNNING::GameRunning;
//【游戏说明】
namespace GAMEINFO{

	Range textbox;//文字
	Option_img returnback;//返回开始界面
	STR gameinfo_text;

	inline void refresh(){//【游戏说明】刷新界面
		Setmain(mainbox, bkimg_up);//游戏界面主区域
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//游戏界面调试区域

		printdebug(_T("【游戏说明】"));

		returnback.msin = 0, returnback.refresh();

		print(textbox, gameinfo_text, lightred, 20, 100, 2);
	}

	inline int updata(){//【游戏说明】检查行为
		if (_kbhit()){
			char c = _getch();
			if (c == kb_esc)return -1;
		}
		if (_mshit()){
			MOUSE m = _getms();
			if (returnback.updata_action(m))return -1;
		}
		return 0;
	}

	inline void GameInfo(){//【游戏说明】主程序
		refresh();
		settextcolor(RGB(0, 0, 255));
		settextstyle(20, 0, "楷体");
		outtextxy(10, 10, "①本游戏为仿羊了个羊游戏，目前共开发出三关。");
		outtextxy(10, 35, "②点击开始界面上的“游戏说明”按钮，可进入");
		outtextxy(30, 60, "游戏说明界面。");
		outtextxy(10, 85, "③点击开始界面上的“开始游戏”按钮，可进入");
		outtextxy(30, 110, "选择关卡界面。");
		outtextxy(10, 135, "④选择关卡界面中，若关卡对应的矩形为绿色，");
		outtextxy(30, 160, "则说明该关卡处于开放状态，否则处于关闭状态。");
		outtextxy(10, 185, "⑤点击处于开放状态的关卡，可开始该关卡的游戏。");
		outtextxy(10, 210, "⑥游戏规则为，点击上方的卡片，卡片可出现在下");
		outtextxy(30, 235, "方的卡槽中，若卡槽未满且上方卡片已消除完毕，");
		outtextxy(30, 260, "则通关成功，否则通关失败。");
		outtextxy(10, 285,"⑦通过某关后，将会开启下一关。");
		outtextxy(10, 310, "⑧卡槽上方为卡片打乱工具，点击可打乱剩余的卡片。");
		outtextxy(30, 335, "每关有且仅有一次使用打乱工具的机会。");
		outtextxy(10, 360,"⑨游戏结束时，可通过按空格重玩本关，");
		outtextxy(30, 385, "或者通过回车键回到选择关卡界面。");
		while (1){
			Re cmd = updata();
			if (cmd == -1)return;//按下[ESC]或者[返回开始界面]
		}
	}

	inline void init(){//【游戏说明】初始化
		Setmain(mainbox, bkimg_up);//游戏界面主区域
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//游戏界面调试区域

		textbox = gameinfo_text_box;
		gameinfo_text = _T("");

		Setmain(mainbox, bkimg_up_option);//选项界面主区域
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//选项界面调试区域

		returnback = Option_img(returnback_box, option_returnback_img[0], option_returnback_img[1]);
		//returnback=Option({110,625,370,675},30,80,_T("返回开始界面"));
		//{0,0,260,50}
	}
}
using GAMEINFO::GameInfo;
//【选择关卡】
namespace CHOOSELEVEL{

	Option_img returnback;
	Level_option level_option[MAX_level_cnt + 1];

	inline void refresh(){//【选择关卡】刷新界面

		Setmain(mainbox, bkimg_up);//游戏界面主区域
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//游戏界面调试区域

		printdebug(_T("【选择关卡】"));

		returnback.msin = 0, returnback.refresh();
		_Begin();
		for (Re i = 1; i <= MAX_level_cnt; ++i)level_option[i].option.msin = 0, level_option[i].refresh();
		_End(), _Flush();
	}

	inline int updata(){//【选择关卡】检查行为
		if (_kbhit()){
			char c = _getch();
			if (c == kb_esc)return -1;
			if (c == 'O')return -1000 + 'O';//按[O]开挂【关卡全开】
		}
		if (_mshit()){
			MOUSE m = _getms();
			if (returnback.updata_action(m))return -1;
			for (Re i = 1; i <= MAX_level_cnt; ++i)if (level_option[i].updata_action(m))return i;
		}
		return 0;
	}

	inline void level_allopen(){
		for (Re i = 1; i <= common_level_cnt; ++i)level_option[i].open();
	}

	inline void ChooseLevel(){//【选择关卡】主程序
		Re warning_showtime = 0, warning_type = -1; refresh();
		while (1){
			Re cmd = updata();
			if (cmd == -1)return;//按下[ESC]或者[返回开始界面]
			if (cmd == -1000 + 'O')controlermode_level_allopen = 1, CHOOSELEVEL::level_allopen(), refresh();//按[O]开挂【关卡全开】
			if (cmd >= 1 && cmd <= common_level_cnt){
				if (level_option[cmd].levelopen){//按下[level *]
					warning_showtime = 0, refresh();
					//选择关卡后开始游戏
					GameRunning(cmd), refresh();
					//更新通过关卡
					if (isPass)
						level_option[min(cmd + 1, common_level_cnt)].open();
				}
				else warning_showtime = (2000 * 10), warning_type = -1;//你点到了不该点的东西QAQ
			}
			if (warning_showtime){
				Re type = warning_showtime / (250 * 10) & 1;//约每0.25s更新一次颜色
				if (warning_type != type)printdebug(_T("【FBI Warning】请先完成前一关。"), (warning_type = type) ? lightred : lightgray, 25, 200);
				if (!--warning_showtime)printdebug(_T("【选择关卡】"));//约2s后不再发出Warning
			}
		}
	}

	inline void init(){//【选择关卡】初始化
		Setmain(mainbox, bkimg_up_option);//选项界面主区域
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//选项界面调试区域

		returnback = Option_img(returnback_box, option_returnback_img[0], option_returnback_img[1]);

		Re stx = 35 + 10, sty = 35 + 10;
		for (Re i = 1; i <= MAX_level_cnt; ++i){
			level_option[i].init({ stx, sty, stx + 90, sty + 30 }, i);//{0,0,90,30}
			stx += 100; if (stx == 445)stx = 45, sty += 40;
		}
	}
}
using CHOOSELEVEL::ChooseLevel;
//【开始界面】
namespace STARTGRAPH{

	Range Titlebox;//大标题
	const int Title_shadow = 5;
	//Range Staffbox;IMAGE Staff_bkimg;//作者

	Option_img gamestart, gameinfo;

	inline void refresh(){//【开始界面】刷新界面

		Setmain(mainbox, bkimg_up_st);//开始界面主区域
		Setdebug(debugbox, bkimg_down_st, 30, 100, default_debug_text_color);//开始界面调试区域

		printdebug(_T("【开始界面】"));

		putimage_transparancy(Titlebox.x1, Titlebox.y1, &text_Title_img);

		gamestart.msin = 0, gamestart.refresh();
		gameinfo.msin = 0, gameinfo.refresh();
	}

	inline int updata(){//【开始界面】检查行为
		if (_mshit()){
			MOUSE m = _getms();
			if (gamestart.updata_action(m))return 1;
			if (gameinfo.updata_action(m))return 2;
		}
		return 0;
	}

	inline void StartGraph(){//【开始界面】主程序
		refresh();

		while (1){
			Re cmd = updata();
			if (cmd == 1)ChooseLevel(), refresh();//按下[开始游戏]
			if (cmd == 2)GameInfo(), refresh();//按下[游戏说明]
			Sleep(1);
		}
	}

	inline void init(){//【开始界面】初始化

		//Setmain(mainbox,bkimg_up);//游戏界面主区域
		//Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//游戏界面调试区域

		Titlebox = { 30, 100, 450, 280 }; //Title_bkimg=getimage(Titlebox);
		//{0,0,420,180}
		//Staffbox={30,340,450,370}; //Staff_bkimg=getimage(Staffbox);

		//Setmain(mainbox,bkimg_up_st);//开始界面主区域
		//Setdebug(debugbox,bkimg_down_st,30,100,default_debug_text_color);//开始界面调试区域

		Setmain(mainbox, bkimg_up_option);//选项界面主区域
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//选项界面调试区域

		gamestart = Option_img(gamestart_box, option_gamestart_img[0], option_gamestart_img[1]);//{0,0,290,80}
		gameinfo = Option_img(gameinfo_box, option_gameinfo_img[0], option_gameinfo_img[1]);//{0,0,290,80}
		//gamestart=Option({95,480,385,560},50,100,_T("开始游戏"));
		//gameinfo=Option({95,610,385,690},50,100,_T("游戏说明"));
	}
}
using STARTGRAPH::StartGraph;
// 游戏初始化
inline void init(){

	Setgraphsize(480, 820);
	SetWindowstext(_T("【作者：程序员小徐】学了个学 "));

	loadimage(&bkimg_up, _T("./resource/image/background/background_up.jpg"));
	loadimage(&bkimg_down, _T("./resource/image/background/background_down.jpg"));
	loadimage(&bkimg_up_st, _T("./resource/image/background/background_up_st.jpg"));
	loadimage(&bkimg_down_st, _T("./resource/image/background/background_down_st.jpg"));
	loadimage(&bkimg_up_option, _T("./resource/image/background/background_up_option.jpg"));
	loadimage(&bkimg_down_option, _T("./resource/image/background/background_down_option.jpg"));
	loadimage(&option_openedlevel_img[0], _T("./resource/image/option/openedlevel.png"));
	loadimage(&option_openedlevel_img[1], _T("./resource/image/option/openedlevel_.png"));
	loadimage(&option_closedlevel_img[0], _T("./resource/image/option/closedlevel.png"));
	loadimage(&option_closedlevel_img[1], _T("./resource/image/option/closedlevel_.png"));
	loadimage(&option_returnback_img[0], _T("./resource/image/option/returnback.png"));
	loadimage(&option_returnback_img[1], _T("./resource/image/option/returnback_.png"));
	loadimage(&option_gamestart_img[0], _T("./resource/image/option/gamestart.png"));
	loadimage(&option_gamestart_img[1], _T("./resource/image/option/gamestart_.png"));
	loadimage(&option_gameinfo_img[0], _T("./resource/image/option/gameinfo.png"));
	loadimage(&option_gameinfo_img[1], _T("./resource/image/option/gameinfo_.png"));
	loadimage(&text_Title_img, _T("./resource/image/text/title.png"));

	CONTROLER::init();
	STARTGRAPH::init();
	mciSendString(_T("open resource/video/sheep.mp3 alias bkmusic"), NULL, 0, NULL);//打开背景音乐
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);  // 循环播放
	CHOOSELEVEL::init();
	GAMEINFO::init();
	GAMERUNNING::init();
}
// 主函数
int main(){ init(), StartGraph(), CloseProgram(); }