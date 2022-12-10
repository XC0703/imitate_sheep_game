#include "myprepare.h"
#include "tools.h"
#include "firstLevel.h"
#include "secondLevel.h"
#include "thirdLevel.h"

IMAGE bkimg_up, bkimg_down;//��Ϸ����󱳾�
IMAGE bkimg_up_st, bkimg_down_st;//��ʼ����󱳾�
IMAGE bkimg_up_option, bkimg_down_option;//ѡ����ڴ󱳾�
IMAGE option_openedlevel_img[2];//[�ѿ��Źؿ�]ѡ��
IMAGE option_closedlevel_img[2];//[δ���Źؿ�]ѡ��
IMAGE option_returnback_img[2];//[���ؿ�ʼ����]ѡ��
IMAGE option_gamestart_img[2];//[��ʼ��Ϸ]ѡ��
IMAGE option_gameinfo_img[2];//[��Ϸ˵��]ѡ��
IMAGE text_Title_img;//���������

const int MAX_HP_cnt = 10;//���HPֵ
const int MAX_level_cnt = 56;//��Ϸ�ؿ�����(���Ϊ56)
const int common_level_cnt = 3;//��ͨ�ؿ�����
const int playerstyle_cnt = 1;//�����ʽ����
int isPass = 0;//�ж�ĳ���Ƿ�ͨ��

const COLOR player_style_color[playerstyle_cnt + 1] = { 0, pink };//��������ɫ������style��
const COLOR player_style_color_line[playerstyle_cnt + 1] = { 0, brown };//��ұ߿���ɫ������style��
const COLOR player_shadow_color = darkgray;//�����Ӱ��ɫ
const COLOR default_option_line_color[2] = { darkgray, lightgray };//Ĭ��ѡ��߿���ɫ������״̬ �� ���ָ��״̬��
const COLOR default_option_text_color[2] = { lightcyan, white };//Ĭ��ѡ��������ɫ������״̬ �� ���ָ��״̬��
const COLOR level_opened_option_text_color[2] = { RGB(0, 255, 30), white };//�ѿ��Źؿ�ѡ��������ɫ������״̬ �� ���ָ��״̬��
const COLOR level_closed_option_text_color[2] = { lightgray, white };//δ���Źؿ�ѡ��������ɫ������״̬ �� ���ָ��״̬��
const COLOR default_debug_text_color = white;//Ĭ��debug����������ɫ

#define level_endless MAX_level_cnt //�޾�ģʽ�ؿ�

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
// ����ѡ��
namespace OPTION{
	struct Option{//ѡ��
		bool msin;//ѡ��״̬(����Ƿ�������ײ������)
		Range box; IMAGE bkimg;//��ײ�䡢ѡ���
		int height, weight; STR text;//����
		int line_weight;//�߿���
		COLOR line_color[2], text_color[2];

		Option(Range R = { 0, 0, 0, 0 }, Re H = 0, Re W = 0, STR T = _T(""),
			COLOR line_color_0 = default_option_line_color[0], COLOR line_color_1 = default_option_line_color[1],
			COLOR text_color_0 = default_option_text_color[0], COLOR text_color_1 = default_option_text_color[1],
			Re line_W = 3){//���캯��
			line_color[0] = line_color_0, line_color[1] = line_color_1;
			text_color[0] = text_color_0, text_color[1] = text_color_1;
			box = R, height = H, weight = W, text = T, line_weight = line_W, msin = 0;
			bkimg = H ? getimage(box.boost(1, 1)) : IMAGE(0, 0);
		}

		inline void refresh(){//ˢ��ѡ��
			Setbkmode(bkmode2), ClearRange(box.boost(1, 1), bkimg);//���û�ͼ͸������ѡ�������ÿ�
			Settextstyle(height, weight, text_color[msin], 0, msin), print(box, text);//������
		}

		inline bool updata_action(MOUSE m){//���ѡ��״̬
			Point ms = Point(m.x, m.y);
			if (PIR(ms, box) ^ msin)msin ^= 1, refresh();//��ѡ��洢��״̬�뵱ǰ״̬��ͬ����ˢ��״̬
			return (msin&&m.ms_info == ms_lefthit);//�������ѡ��
		}
	};
	struct Option_img{//ѡ��(��ͼ��)
		bool msin;//ѡ��״̬(����Ƿ�������ײ������)
		Range box; IMAGE img0, img1;//��ײ�䡢��ͼ��ѡ���

		Option_img(Range R = { 0, 0, 0, 0 }, IMAGE img0_ = IMAGE(0, 0), IMAGE img1_ = IMAGE(0, 0)){//���캯��
			img0 = img0_, img1 = img1_, box = R, msin = 0;
		}
		inline void refresh(){ putimage_transparancy(box.x1, box.y1, msin ? &img1 : &img0); }//ˢ��ѡ��
		inline bool updata_action(MOUSE m){//���ѡ��״̬
			Point ms = Point(m.x, m.y);
			if (PIR(ms, box) ^ msin)msin ^= 1, refresh();//��ѡ��洢��״̬�뵱ǰ״̬��ͬ����ˢ��״̬
			return (msin&&m.ms_info == ms_lefthit);//�������ѡ��
		}
	};
	struct Option_bkimg_text{//ѡ��
		bool msin;//ѡ��״̬(����Ƿ�������ײ������)
		Range box; IMAGE bkimg;//��ײ�䡢ѡ���
		int height, weight; STR text;//����
		IMAGE bkimg0, bkimg1;//������ͼ
		COLOR text_color[2];

		Option_bkimg_text(Range R = { 0, 0, 0, 0 }, Re H = 0, Re W = 0, STR T = _T(""),
			IMAGE img0_ = IMAGE(0, 0), IMAGE img1_ = IMAGE(0, 0),
			COLOR text_color_0 = default_option_text_color[0], COLOR text_color_1 = default_option_text_color[1]
			){//���캯��
			bkimg0 = img0_, bkimg1 = img1_;
			text_color[0] = text_color_0, text_color[1] = text_color_1;
			box = R, height = H, weight = W, text = T, msin = 0;
			bkimg = H ? getimage(box) : IMAGE(0, 0);
		}

		inline void refresh(){//ˢ��ѡ��
			Setbkmode(bkmode2), ClearRange(box, bkimg);//���û�ͼ͸������ѡ�������ÿ�
			putimage_transparancy(box.x1, box.y1, msin ? &bkimg1 : &bkimg0);//������
			Settextstyle(height, weight, text_color[msin], 0, msin), print(box, text);//������
		}

		inline bool updata_action(MOUSE m){//���ѡ��״̬
			Point ms = Point(m.x, m.y);
			if (PIR(ms, box) ^ msin)msin ^= 1, refresh();//��ѡ��洢��״̬�뵱ǰ״̬��ͬ����ˢ��״̬
			return (msin&&m.ms_info == ms_lefthit);//�������ѡ��
		}
	};
	struct Level_option{//��ѡ��ؿ���ѡ��
		Option_bkimg_text option; CH text[100];
		int levelid, levelopen;//�ùؿ��Ƿ��Ѿ�ͨ��(level�޶�)

		inline void refresh(){ option.refresh(); }//ˢ��ѡ��
		inline bool updata_action(MOUSE m){ return option.updata_action(m); }//���ѡ��״̬
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
			else if (id == level_endless)_stprintf(text, _T("�޾�ģʽ"));
			else _stprintf(text, _T("��������"));

			levelopen = (id == 1);//�ؿ�ȫ��ģʽ
			if (levelid>common_level_cnt)levelopen = 0;//�ؿ���δ����

			IMAGE bkimg[2]; COLOR text_color[2]; get_option_img_and_color(bkimg, text_color);
			option = Option_bkimg_text(R, 20, 200, text, bkimg[0], bkimg[1], text_color[0], text_color[1]);
		}
	};
}
using OPTION::Option;
using OPTION::Option_img;
using OPTION::Level_option;
//����Ϸ�����С�
namespace GAMERUNNING{
	Option_img returnback;//���ؿ�ʼ����

	inline void refresh(){//����Ϸ�����С�ˢ�½���
		Setmain(mainbox, bkimg_up);//��Ϸ����������
		printdebug(_T("����*�ء�"));
		returnback.msin = 0, returnback.refresh();
	}

	inline int updata(){//����Ϸ�����С������Ϊ
		if (_mshit()){
			MOUSE m = _getms();
			if (returnback.updata_action(m))return -1;
		}
		return 0;
	}

	inline void GameRunning(int level){//����Ϸ�����С�������
		isPass = 0;
		refresh();
		if (level == 1){
			gameInit1(); //��Ϸ�ĳ�ʼ��
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
			gameInit2(); //��Ϸ�ĳ�ʼ��
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
			gameInit3(); //��Ϸ�ĳ�ʼ��
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

	inline void init(){//����Ϸ�����С���ʼ��
		Setmain(mainbox, bkimg_up);//��Ϸ����������
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//��Ϸ�����������

		Setmain(mainbox, bkimg_up_option);//ѡ�����������
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//ѡ������������
	}
}
using GAMERUNNING::GameRunning;
//����Ϸ˵����
namespace GAMEINFO{

	Range textbox;//����
	Option_img returnback;//���ؿ�ʼ����
	STR gameinfo_text;

	inline void refresh(){//����Ϸ˵����ˢ�½���
		Setmain(mainbox, bkimg_up);//��Ϸ����������
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//��Ϸ�����������

		printdebug(_T("����Ϸ˵����"));

		returnback.msin = 0, returnback.refresh();

		print(textbox, gameinfo_text, lightred, 20, 100, 2);
	}

	inline int updata(){//����Ϸ˵���������Ϊ
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

	inline void GameInfo(){//����Ϸ˵����������
		refresh();
		settextcolor(RGB(0, 0, 255));
		settextstyle(20, 0, "����");
		outtextxy(10, 10, "�ٱ���ϷΪ�����˸�����Ϸ��Ŀǰ�����������ء�");
		outtextxy(10, 35, "�ڵ����ʼ�����ϵġ���Ϸ˵������ť���ɽ���");
		outtextxy(30, 60, "��Ϸ˵�����档");
		outtextxy(10, 85, "�۵����ʼ�����ϵġ���ʼ��Ϸ����ť���ɽ���");
		outtextxy(30, 110, "ѡ��ؿ����档");
		outtextxy(10, 135, "��ѡ��ؿ������У����ؿ���Ӧ�ľ���Ϊ��ɫ��");
		outtextxy(30, 160, "��˵���ùؿ����ڿ���״̬�������ڹر�״̬��");
		outtextxy(10, 185, "�ݵ�����ڿ���״̬�Ĺؿ����ɿ�ʼ�ùؿ�����Ϸ��");
		outtextxy(10, 210, "����Ϸ����Ϊ������Ϸ��Ŀ�Ƭ����Ƭ�ɳ�������");
		outtextxy(30, 235, "���Ŀ����У�������δ�����Ϸ���Ƭ��������ϣ�");
		outtextxy(30, 260, "��ͨ�سɹ�������ͨ��ʧ�ܡ�");
		outtextxy(10, 285,"��ͨ��ĳ�غ󣬽��Ὺ����һ�ء�");
		outtextxy(10, 310, "�࿨���Ϸ�Ϊ��Ƭ���ҹ��ߣ�����ɴ���ʣ��Ŀ�Ƭ��");
		outtextxy(30, 335, "ÿ�����ҽ���һ��ʹ�ô��ҹ��ߵĻ��ᡣ");
		outtextxy(10, 360,"����Ϸ����ʱ����ͨ�����ո����汾�أ�");
		outtextxy(30, 385, "����ͨ���س����ص�ѡ��ؿ����档");
		while (1){
			Re cmd = updata();
			if (cmd == -1)return;//����[ESC]����[���ؿ�ʼ����]
		}
	}

	inline void init(){//����Ϸ˵������ʼ��
		Setmain(mainbox, bkimg_up);//��Ϸ����������
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//��Ϸ�����������

		textbox = gameinfo_text_box;
		gameinfo_text = _T("");

		Setmain(mainbox, bkimg_up_option);//ѡ�����������
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//ѡ������������

		returnback = Option_img(returnback_box, option_returnback_img[0], option_returnback_img[1]);
		//returnback=Option({110,625,370,675},30,80,_T("���ؿ�ʼ����"));
		//{0,0,260,50}
	}
}
using GAMEINFO::GameInfo;
//��ѡ��ؿ���
namespace CHOOSELEVEL{

	Option_img returnback;
	Level_option level_option[MAX_level_cnt + 1];

	inline void refresh(){//��ѡ��ؿ���ˢ�½���

		Setmain(mainbox, bkimg_up);//��Ϸ����������
		Setdebug(debugbox, bkimg_down, 30, 100, default_debug_text_color);//��Ϸ�����������

		printdebug(_T("��ѡ��ؿ���"));

		returnback.msin = 0, returnback.refresh();
		_Begin();
		for (Re i = 1; i <= MAX_level_cnt; ++i)level_option[i].option.msin = 0, level_option[i].refresh();
		_End(), _Flush();
	}

	inline int updata(){//��ѡ��ؿ��������Ϊ
		if (_kbhit()){
			char c = _getch();
			if (c == kb_esc)return -1;
			if (c == 'O')return -1000 + 'O';//��[O]���ҡ��ؿ�ȫ����
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

	inline void ChooseLevel(){//��ѡ��ؿ���������
		Re warning_showtime = 0, warning_type = -1; refresh();
		while (1){
			Re cmd = updata();
			if (cmd == -1)return;//����[ESC]����[���ؿ�ʼ����]
			if (cmd == -1000 + 'O')controlermode_level_allopen = 1, CHOOSELEVEL::level_allopen(), refresh();//��[O]���ҡ��ؿ�ȫ����
			if (cmd >= 1 && cmd <= common_level_cnt){
				if (level_option[cmd].levelopen){//����[level *]
					warning_showtime = 0, refresh();
					//ѡ��ؿ���ʼ��Ϸ
					GameRunning(cmd), refresh();
					//����ͨ���ؿ�
					if (isPass)
						level_option[min(cmd + 1, common_level_cnt)].open();
				}
				else warning_showtime = (2000 * 10), warning_type = -1;//��㵽�˲��õ�Ķ���QAQ
			}
			if (warning_showtime){
				Re type = warning_showtime / (250 * 10) & 1;//Լÿ0.25s����һ����ɫ
				if (warning_type != type)printdebug(_T("��FBI Warning���������ǰһ�ء�"), (warning_type = type) ? lightred : lightgray, 25, 200);
				if (!--warning_showtime)printdebug(_T("��ѡ��ؿ���"));//Լ2s���ٷ���Warning
			}
		}
	}

	inline void init(){//��ѡ��ؿ�����ʼ��
		Setmain(mainbox, bkimg_up_option);//ѡ�����������
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//ѡ������������

		returnback = Option_img(returnback_box, option_returnback_img[0], option_returnback_img[1]);

		Re stx = 35 + 10, sty = 35 + 10;
		for (Re i = 1; i <= MAX_level_cnt; ++i){
			level_option[i].init({ stx, sty, stx + 90, sty + 30 }, i);//{0,0,90,30}
			stx += 100; if (stx == 445)stx = 45, sty += 40;
		}
	}
}
using CHOOSELEVEL::ChooseLevel;
//����ʼ���桿
namespace STARTGRAPH{

	Range Titlebox;//�����
	const int Title_shadow = 5;
	//Range Staffbox;IMAGE Staff_bkimg;//����

	Option_img gamestart, gameinfo;

	inline void refresh(){//����ʼ���桿ˢ�½���

		Setmain(mainbox, bkimg_up_st);//��ʼ����������
		Setdebug(debugbox, bkimg_down_st, 30, 100, default_debug_text_color);//��ʼ�����������

		printdebug(_T("����ʼ���桿"));

		putimage_transparancy(Titlebox.x1, Titlebox.y1, &text_Title_img);

		gamestart.msin = 0, gamestart.refresh();
		gameinfo.msin = 0, gameinfo.refresh();
	}

	inline int updata(){//����ʼ���桿�����Ϊ
		if (_mshit()){
			MOUSE m = _getms();
			if (gamestart.updata_action(m))return 1;
			if (gameinfo.updata_action(m))return 2;
		}
		return 0;
	}

	inline void StartGraph(){//����ʼ���桿������
		refresh();

		while (1){
			Re cmd = updata();
			if (cmd == 1)ChooseLevel(), refresh();//����[��ʼ��Ϸ]
			if (cmd == 2)GameInfo(), refresh();//����[��Ϸ˵��]
			Sleep(1);
		}
	}

	inline void init(){//����ʼ���桿��ʼ��

		//Setmain(mainbox,bkimg_up);//��Ϸ����������
		//Setdebug(debugbox,bkimg_down,30,100,default_debug_text_color);//��Ϸ�����������

		Titlebox = { 30, 100, 450, 280 }; //Title_bkimg=getimage(Titlebox);
		//{0,0,420,180}
		//Staffbox={30,340,450,370}; //Staff_bkimg=getimage(Staffbox);

		//Setmain(mainbox,bkimg_up_st);//��ʼ����������
		//Setdebug(debugbox,bkimg_down_st,30,100,default_debug_text_color);//��ʼ�����������

		Setmain(mainbox, bkimg_up_option);//ѡ�����������
		Setdebug(debugbox, bkimg_down_option, 30, 100, default_debug_text_color);//ѡ������������

		gamestart = Option_img(gamestart_box, option_gamestart_img[0], option_gamestart_img[1]);//{0,0,290,80}
		gameinfo = Option_img(gameinfo_box, option_gameinfo_img[0], option_gameinfo_img[1]);//{0,0,290,80}
		//gamestart=Option({95,480,385,560},50,100,_T("��ʼ��Ϸ"));
		//gameinfo=Option({95,610,385,690},50,100,_T("��Ϸ˵��"));
	}
}
using STARTGRAPH::StartGraph;
// ��Ϸ��ʼ��
inline void init(){

	Setgraphsize(480, 820);
	SetWindowstext(_T("�����ߣ�����ԱС�졿ѧ�˸�ѧ "));

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
	mciSendString(_T("open resource/video/sheep.mp3 alias bkmusic"), NULL, 0, NULL);//�򿪱�������
	mciSendString(_T("play bkmusic repeat"), NULL, 0, NULL);  // ѭ������
	CHOOSELEVEL::init();
	GAMEINFO::init();
	GAMERUNNING::init();
}
// ������
int main(){ init(), StartGraph(), CloseProgram(); }