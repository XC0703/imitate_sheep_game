#pragma once
#include<map>
#include<set>
#include<ctime>
#include<cmath>
#include<cctype>
#include<string>
#include<cstdio>
#include<sstream>
#include<cstdlib>
#include<cstring>
#include<iostream>
#include<algorithm>

#include<conio.h>
#include<easyx.h>
#include<windows.h>
#include<graphics.h>

#pragma comment(lib,"MSIMG32.LIB")
#pragma warning(disable:4996)

#define LD double
#define LL long long
#define Re register int

#define CH TCHAR
#define STR LPCTSTR
#define COLOR COLORREF
#define MOUSE MOUSEMSG

const int CH_N = 500;//��ʱ����ʹ�ô�С

namespace OTHERS_{/*�������Ӱˡ�*/

	/*����ɫ�趨��*/

#define red RED        //��
#define blue BLUE      //��
#define cyan CYAN      //��
#define green GREEN    //��
#define white WHITE    //��
#define black BLACK    //��
#define brown BROWN    //��ɫ
#define yellow YELLOW  //��
#define purple MAGENTA //��

#define lightred LIGHTRED        //����
#define lightgray LIGHTGRAY      //ǳ��
#define lightblue LIGHTBLUE      //����
#define lightcyan LIGHTCYAN      //����
#define lightgreen LIGHTGREEN    //����
#define lightpurple LIGHTMAGENTA //����
#define orange RGB(255,165,0)//��ɫ
#define pink RGB(253,64,195) //�ۺ�

#define darkgray DARKGRAY        //���
#define darkred RGB(170,0,9) //���
#define darkpurple RGB(66,9,65)

	/*�����ú�����*/

#define gettime _GETTIME_
	inline LD gettime(){ return (LD)clock() / (LD)CLOCKS_PER_SEC; }//������

#define Squre(a) ((a)*(a))
#define Cube(a) ((a)*(a)*(a)) 

#define abs _ABS_
	template<typename T>inline T abs(T a){ return a<0 ? -a : a; }

#define swap _SWAP_
	template<typename T>inline void swap(T &a, T &b){ T c = a; a = b, b = c; }

#define gcd _GCD_
	template<typename T>inline T gcd(T a, T b){ return b == 0 ? a : gcd(b, a%b); }

#define max _MAX_
	template<typename T>inline T max(T a, T b){ return (a>b) ? a : b; }
	template<typename T>inline T max(T a, T b, T c){ return max(max(a, b), c); }
	template<typename T>inline T max(T a, T b, T c, T d){ return max(max(max(a, b), c), d); }
	template<typename T>inline T max(T a, T b, T c, T d, T e){ return max(max(max(max(a, b), c), d), e); }
	template<typename T>inline T max(T a, T b, T c, T d, T e, T f){ return max(max(max(max(a, b), c), d), e); }

#define min _MIN_
	template<typename T>inline T min(T a, T b){ return (a<b) ? a : b; }
	template<typename T>inline T min(T a, T b, T c){ return min(min(a, b), c); }
	template<typename T>inline T min(T a, T b, T c, T d){ return min(min(min(a, b), c), d); }
	template<typename T>inline T min(T a, T b, T c, T d, T e){ return min(min(min(min(a, b), c), d), e); }
	template<typename T>inline T min(T a, T b, T c, T d, T e, T f){ return min(min(min(min(a, b), c), d), e); }

	/*------------------------------------------------------------------------------*/

	/*��������תΪ�ַ����顿*/

	inline void tostr(CH s[], Re a){ _stprintf(s, _T("%d"), a); }
	inline void tostr(CH s[], LL a){ _stprintf(s, _T("%lld"), a); }
	inline void tostr(CH s[], LD a){ _stprintf(s, _T("%lf"), a); }
	inline void tostr(CH s[], LD a, Re k){
		if (k == 0)_stprintf(s, _T("%.0lf"), a);
		if (k == 1)_stprintf(s, _T("%.1lf"), a);
		if (k == 2)_stprintf(s, _T("%.2lf"), a);
		if (k == 3)_stprintf(s, _T("%.3lf"), a);
		if (k == 4)_stprintf(s, _T("%.4lf"), a);
		if (k == 5)_stprintf(s, _T("%.5lf"), a);
		if (k == 6)_stprintf(s, _T("%.6lf"), a);
	}

	/*------------------------------------------------------------------------------*/

	inline void CloseProgram(){ closegraph(), exit(0); }//�رճ���

	/*------------------------------------------------------------------------------*/

	/*���ļ�����������*/
	inline void open_file_id(int id){
		freopen("CON", "r", stdin);
		freopen("CON", "w", stdout);
		std::string _id; std::stringstream ss; ss << id; ss >> _id;
		std::string filepath = "./data/level/level_" + _id + ".txt";
		freopen(filepath.c_str(), "w", stdout);
	}
	inline void close_file(){
		freopen("CON", "r", stdin);
		freopen("CON", "w", stdout);
	}

	inline void debuglog(char *s){//�����Ϣ���ļ�debug.log
		freopen("CON", "r", stdin);
		freopen("CON", "w", stdout);
		freopen("debuglog.txt", "w", stdout);
		printf("%s", s);
		freopen("CON", "r", stdin);
		freopen("CON", "w", stdout);
	}

	/*------------------------------------------------------------------------------*/

	/*�������С�*/
	inline void cmd_openfile(char *filepath){//�����·��filepath�µ��ļ���ת��STR����CH������б������⣬�ȹ�һ�£���char�պϴպϣ�
		char _cmd[1000] = "start ";
		_getcwd_dbg(_cmd + 6, sizeof(_cmd), _NORMAL_BLOCK, NULL, NULL);//��ȡ��ǰ���г������·��
		Re len = std::strlen(_cmd);
		for (Re i = 0; filepath[i]; ++i)_cmd[len++] = filepath[i];//�ں���������·��
		_cmd[len++] = 0, system(_cmd);
	}
};
using namespace OTHERS_;

namespace RANDOM_DATA{/*�������*/

	/*�����ߡ�*/
	int index, MT[624]; bool isInit;

	inline void MTsrand_(unsigned int seed){
		index = 0, isInit = 1, MT[0] = seed;
		for (Re i = 1; i<624; i++){
			Re t = 1812433253 * (MT[i - 1] ^ (MT[i - 1] >> 30)) + i;
			MT[i] = t & 0xffffffff;
		}
	}
	inline void generate_(){
		for (Re i = 0; i<624; i++){
			Re y = (MT[i] & 0x80000000) + (MT[(i + 1) % 624] & 0x7fffffff);
			MT[i] = MT[(i + 397) % 624] ^ (y >> 1);
			if (y % 2 == 1)MT[i] ^= 2087483647;
		}
	}
	inline int MTrand_(){
		if (!isInit)MTsrand_((int)time(NULL));
		if (index == 0)generate_();
		Re y = MT[index];
		y = y ^ (y >> 11);
		y = y ^ ((y << 7) & 1969275843);
		y = y ^ ((y << 15) & 2058296471);
		y = y ^ (y >> 18);
		index = (index + 1) % 624;
		return y;
	}
	inline int MTrand(Re a, Re b){
		if (a>b)swap(a, b);
		return (a == b) ? a : (MTrand_() % (b - a + 1) + a);
	}

	/*------------------------------------------------------------------------------*/

	/*����ʼ��������ӡ�*/

	int Flag_Srand;

	inline void Setseed(unsigned int seed){ Flag_Srand = 1, srand(seed), MTsrand_(seed); }//��ʼ���������
	inline void Setseed(){ Flag_Srand = 1, Setseed((unsigned int)(time(int(NULL)))); }//�Զ���ʼ���������

	/*------------------------------------------------------------------------------*/

	/*��������ݷ�������*/

	inline int Rand(Re L, Re R){//�������һ��int��Χ�ڵ���ֵ
		if (!Flag_Srand)Setseed();
		if (L>R)swap(L, R);
		LL len = R - L;
		if (len <= 32767)return (!len) ? L : (L + rand() % (len + 1));
		else return MTrand(L, R);
	}
	inline COLOR Rand_color(){//�������һ����ɫ
		if (!Flag_Srand)Setseed();
		return RGB(Rand(0, 255), Rand(0, 255), Rand(0, 255));
	}

	/*------------------------------------------------------------------------------*/
}
using RANDOM_DATA::Rand;
using RANDOM_DATA::Setseed;
using RANDOM_DATA::Rand_color;
#define randcolor (Rand_color())

namespace COLLISION_BOX{/*����ײ�䡿*/

	/*------------------------------------------------------------------------------*/

	/*�������Ϣ��*/

#define ms_move WM_MOUSEMOVE          //����ƶ���Ϣ
#define ms_wheel WM_MOUSEWHEEL        //�����ֲ�����Ϣ
#define ms_lefthit WM_LBUTTONDOWN     //���������Ϣ
#define ms_leftgo WM_LBUTTONUP        //���������Ϣ
#define ms_lefthitt WM_LBUTTONDBLCLK  //���˫����Ϣ
#define ms_midhit WM_MBUTTONDOWN      //�м�������Ϣ
#define ms_midgo WM_MBUTTONUP         //�м�������Ϣ
#define ms_midhitt WM_MBUTTONDBLCLK   //�м�˫����Ϣ
#define ms_righthit WM_RBUTTONDOWN    //�Ҽ�������Ϣ
#define ms_rightgo WM_RBUTTONUP       //�Ҽ�������Ϣ
#define ms_righthitt WM_RBUTTONDBLCLK //�Ҽ�˫����Ϣ

#define ms_info uMsg           //��ǰ�����Ϣ
#define hit_kb_ctrl mkCtrl     //Ctrl ���Ƿ���
#define hit_kb_shift mkShift   //Shift ���Ƿ���
#define hit_ms_left mkLButton  //�������Ƿ���
#define hit_ms_mid mkMButton   //����м��Ƿ���
#define hit_ms_right mkRButton //����Ҽ��Ƿ���

#define _mshit MouseHit
#define _getms GetMouseMsg
#define _peekms PeekMouseMsg
#define _Flushms FlushMouseMsgBuffer

	/*
	struct MOUSEMSG{
	UINT uMsg;      //��ǰ�����Ϣ
	bool mkCtrl;    //Ctrl ���Ƿ���
	bool mkShift;   //Shift ���Ƿ���
	bool mkLButton; //�������Ƿ���
	bool mkMButton; //����м��Ƿ���
	bool mkRButton; //����Ҽ��Ƿ���
	int x,y;        //��ǰ��� x,y ����
	int wheel;      //�����ֹ���ֵ
	};
	*/

	/*��������Ϣ��*/

#define kb_A 65
#define kb_D 68
#define kb_F 70
#define kb_G 71
#define kb_L 76	
#define kb_P 80
#define kb_S 83
#define kb_up 38
#define kb_esc 27
#define kb_left 37
#define kb_down 40
#define kb_right 39
#define kb_space 32
#define kb_caps_lock 20

	inline void press_kb_down(Re vk){ keybd_event(vk, 0, 0, 0); }//����ĳ��
	inline void press_kb_up(Re vk){ keybd_event(vk, 0, KEYEVENTF_KEYUP, 0); }//�ɿ�ĳ��
	inline void press_kb(Re vk){ press_kb_down(vk); }//��һ��ĳ����

	/*------------------------------------------------------------------------------*/

	/*����ײ�䶨�塿*/

	struct Point{
		int x, y; Point(Re X = 0, Re Y = 0){ x = X, y = Y; }
		inline bool operator==(const Point&P)const{ return x == P.x&&y == P.y; }
		inline bool operator!=(const Point&P)const{ return !(*this == P); }
	};
	struct DB_Point{
		LD x, y; DB_Point(LD X = 0, LD Y = 0){ x = X, y = Y; }
		inline DB_Point operator+(const DB_Point&P)const{ return DB_Point(x + P.x, y + P.y); }
		inline DB_Point operator+=(DB_Point &P){ return *this = (*this + P); }
		inline DB_Point operator-(const DB_Point &O)const{ return DB_Point(x - O.x, y - O.y); }
		inline DB_Point operator-=(DB_Point &O){ return *this = (*this - O); }
		inline DB_Point operator*(LD b)const{ return DB_Point(x*b, y*b); }
		inline DB_Point operator*=(LD b){ return *this = (*this*b); }
	};
	struct Line{
		int x1, y1, x2, y2;
		Line(Re X1 = 0, Re Y1 = 0, Re X2 = 0, Re Y2 = 0){ x1 = X1, y1 = Y1, x2 = X2, y2 = Y2; }
		Line(Point A = { 0, 0 }, Point B = { 0, 0 }){ x1 = A.x, y1 = A.y, x2 = B.x, y2 = B.y; }
	};
	struct Range{
		int x1, y1, x2, y2;
		Range(Re X1 = 0, Re Y1 = 0, Re X2 = 0, Re Y2 = 0){
			x1 = X1, y1 = Y1, x2 = X2, y2 = Y2;
			if (x1>x2)swap(x1, x1);
			if (y1>y2)swap(y1, y2);
		}
		inline Range cutmid(Re x, Re y)const{ return Range(x1 + x, y1 + y, x2 - x, y2 - y); }
		inline Range boost(Re x, Re y)const{ return Range(x1 - x, y1 - y, x2 + x, y2 + y); }
		inline Range transfer(Re d)const{ return Range(x1 + d, y1 + d, x2 + d, y2 + d); }
		inline Range transfer(Re _x1, Re _y1, Re _x2, Re _y2)const{ return Range(x1 + _x1, y1 + _y1, x2 + _x2, y2 + _y2); }
	};
	struct DB_Range{
		LD x1, y1, x2, y2;
		DB_Range(LD X1 = 0, LD Y1 = 0, LD X2 = 0, LD Y2 = 0){
			x1 = X1, y1 = Y1, x2 = X2, y2 = Y2;
			if (x1>x2)swap(x1, x1);
			if (y1>y2)swap(y1, y2);
		}
		inline DB_Range cutmid(LD x, LD y){ return DB_Range(x1 + x, y1 + y, x2 - x, y2 - y); }
		inline DB_Range boost(LD x, LD y){ return DB_Range(x1 - x, y1 - y, x2 + x, y2 + y); }
	};

	/*------------------------------------------------------------------------------*/

	/*����ײ���⡿*/

	inline bool PIR(Point P, Range R){ return P.x >= R.x1&&P.x <= R.x2&&P.y >= R.y1&&P.y <= R.y2; }//����P�Ƿ�������R��
	inline bool PIR(Re x, Re y, Range R){ return x >= R.x1&&x <= R.x2&&y >= R.y1&&y <= R.y2; }//����(x,y)�Ƿ�������R��
	inline bool RIR(Range R, Range O){ return PIR(R.x1, R.y1, O) && PIR(R.x2, R.y2, O); }//�������R�Ƿ�������R��

};
using namespace COLLISION_BOX;

namespace Computational_Geometry{
#define G_Point DB_Point
#define Vector DB_Point
#define G_Poly DB_Range

	const LD eps = 1e-8, Pi = acos(-1.0);

	/*һ������*/
	inline int dcmp(LD a){ return a<-eps ? -1 : (a>eps ? 1 : 0); };
	inline LD Abs(LD a){ return a*dcmp(a); }//ȡ����ֵ
	inline LD toPi(LD a){ return a / 180.0*Pi; }//������ת��
	inline LD toTheta(LD r){ return 180.0*r / Pi; }

	/*������������*/
	inline LD Dot(Vector a, Vector b){ return a.x*b.x + a.y*b.y; }//�������
	inline LD Cro(Vector a, Vector b){ return a.x*b.y - a.y*b.x; }//�������
	inline LD Len(Vector a){ return sqrt(Dot(a, a)); }//��ģ����
	inline LD Angle(Vector a, Vector b){ return acos(Dot(a, b) / Len(a) / Len(b)); }//���������нǡ�
	inline Vector Normal(Vector a){ return Vector(-a.y, a.x); }//����������
	inline bool operator==(DB_Point a, DB_Point b){ return !dcmp(a.x - b.x) && !dcmp(a.y - b.y); }//���������غ������

	/*�������㡢������λ�ñ任��*/

	/*1.���㡢��������ת��*/
	inline G_Point turn_P(G_Point a, LD theta){//����A\����A˳ʱ����תtheta(����)��
		LD x = a.x*cos(theta) + a.y*sin(theta);
		LD y = -a.x*sin(theta) + a.y*cos(theta);
		return G_Point(x, y);
	}
	inline G_Point turn_PP(G_Point a, G_Point b, LD theta){//������A�Ƶ�B˳ʱ����תtheta(����)��
		LD x = (a.x - b.x)*cos(theta) + (a.y - b.y)*sin(theta) + b.x;
		LD y = -(a.x - b.x)*sin(theta) + (a.y - b.y)*cos(theta) + b.y;
		return G_Point(x, y);
	}


	/*�ģ���ͼ����ͼ��֮��Ĺ�ϵ��*/

	/*1.�������߶Ρ�*/
	inline int pan_PL(G_Point p, G_Point a, G_Point b){//���жϵ�P�Ƿ����߶�AB�ϡ�
		return !dcmp(Cro(p - a, b - a)) && dcmp(Dot(p - a, p - b)) <= 0;//����һ
		//  return !dcmp(Cro(p-a,b-a))&&dcmp(min(a.x,b.x)-p.x)<=0&&dcmp(p.x-max(a.x,b.x))<=0&&dcmp(min(a.y,b.y)-p.y)<=0&&dcmp(p.y-max(a.y,b.y))<=0;//������
		//PA,AB������P��AB֮��(��ʵҲ������len(p-a)+len(p-b)==len(a-b)�жϣ����Ǿ�����ʧ�ϴ�)
	}
	inline LD dis_PL(G_Point p, G_Point a, G_Point b){//����P���߶�AB���롿
		if (a == b)return Len(p - a);//AB�غ�
		Vector x = p - a, y = p - b, z = b - a;
		if (dcmp(Dot(x, z))<0)return Len(x);//P����A����
		if (dcmp(Dot(y, z))>0)return Len(y);//P����B����
		return Abs(Cro(x, z) / Len(z));//������Եױ߳�
	}

	/*2.������ֱ�ߡ�*/
	inline int pan_PL_(G_Point p, G_Point a, G_Point b){//���жϵ�P�Ƿ���ֱ��AB�ϡ�
		return !dcmp(Cro(p - a, b - a));//PA,AB����
	}
	inline G_Point FootPoint(G_Point p, G_Point a, G_Point b){//����P��ֱ��AB�Ĵ��㡿
		Vector x = p - a, y = p - b, z = b - a;
		LD len1 = Dot(x, z) / Len(z), len2 = -1.0*Dot(y, z) / Len(z);//�ֱ����AP,BP��AB,BA�ϵ�ͶӰ
		return a + z*(len1 / (len1 + len2));//��A��������AF
	}
	inline G_Point Symmetry_PL(G_Point p, G_Point a, G_Point b){//����P����ֱ��AB�ĶԳƵ㡿
		return p + (FootPoint(p, a, b) - p) * 2;//��PF�ӳ�һ������
	}

	/*3.�������ߡ�*/
	inline G_Point cross_LL(G_Point a, G_Point b, G_Point c, G_Point d){//����ֱ��AB,CD�Ľ��㡿
		Vector x = b - a, y = d - c, z = a - c;
		return a + x*(Cro(y, z) / Cro(x, y));//��A��������AF
	}
	inline int pan_cross_L_L(G_Point a, G_Point b, G_Point c, G_Point d){//���ж�ֱ��AB���߶�CD�Ƿ��ཻ��
		return pan_PL(cross_LL(a, b, c, d), c, d);//ֱ��AB��ֱ��CD�Ľ������߶�CD��
	}
	inline int pan_cross_LL(G_Point a, G_Point b, G_Point c, G_Point d){//���ж����߶�AB,CD�Ƿ��ཻ��
		LD c1 = Cro(b - a, c - a), c2 = Cro(b - a, d - a);
		LD d1 = Cro(d - c, a - c), d2 = Cro(d - c, b - c);
		return dcmp(c1)*dcmp(c2)<0 && dcmp(d1)*dcmp(d2)<0;//�ֱ�������
	}

	/*4.������Բ��*/
	inline int cross_LC_(G_Point &A, G_Point &B, G_Point O, LD R){//����ֱ����Բ�Ľ��㡿
		if (A == B)return 0;//��Fucking Bug!!!��ֱ���˻�Ϊ���ˣ���
		if (A == O || B == O){//ֱ�߹�Բ��
			Vector x = A - B; LD lenx = Len(x);
			x = x*(R / lenx); A = O + x, B = O - x; return 2;//��������
		}
		Vector x = A - O, y = B - O; LD lenab = dis_PL(O, A, B); Re flag = dcmp(lenab - R);
		if (flag>0)return 0;//�޽���
		if (!flag){ G_Point F = FootPoint(O, A, B); A = F; return 1; }//����
		//�ཻ
		G_Point F = FootPoint(O, A, B); LD lenx = Len(F - O), len = sqrt(R*R - lenx*lenx);
		if (lenx == 0)lenx = eps;//�Է���һ���Ӹ�����
		Vector z = turn_P(F - O, Pi / 2.0)*(len / lenx);
		A = F + z, B = F - z; return 2;//��������
	}
	inline int cross_LC(G_Point &A, G_Point &B, G_Point O, LD R){//�����߶���Բ�Ľ��㡿
		G_Point A_ = A, B_ = B; Re cnt = cross_LC_(A_, B_, O, R);
		if (!cnt)return 0;
		if (cnt == 1){//ֱ��AB��Բ��һ������
			if (pan_PL(A_, A, B)){ A = A_; return 1; }
			return 0;
		}
		//ֱ��AB��Բ����������
		if (pan_PL(B_, A, B)){//�ڶ�����Ϸ�
			if (pan_PL(A_, A, B)){ A = A_, B = B_; return 2; }//��һ����Ϸ�
			A = B_; return 1;//��һ���㲻�Ϸ�
		}
		else{//�ڶ����㲻�Ϸ�
			if (pan_PL(A_, A, B)){ A = A_; return 1; }//��һ����Ϸ�
			return 0;//��һ���㲻�Ϸ�
		}
	}
#undef G_Point
#undef Vector
#undef G_Poly
};
#define GMT Computational_Geometry
using GMT::toTheta;
using GMT::dcmp;
using GMT::toPi;
using GMT::eps;

namespace SETTING_OPTION{/*������ѡ�*/

	/*�������趨��*/

	int MAXX = 1080, MAXY = 720;

	inline void Setgraphsize(){ initgraph(MAXX, MAXY); }
	inline void Setgraphsize(Re x, Re y){ initgraph(MAXX = x, MAXY = y); }//���ô��ڴ�С

	inline void SetWindowstext(STR s){ SetWindowText(GetHWnd(), s); }//���ô��ڱ���

	/*------------------------------------------------------------------------------*/

#define Setfillcolor setfillcolor
#define Setlinecolor setlinecolor
#define Setbkcolor setbkcolor
#define Settextcolor settextcolor
#define Setbkmode setbkmode

	/*------------------------------------------------------------------------------*/

	/*�����塢������ʽ��*/

	int __textheight, __textweight;//����߶ȡ���ϸ
	bool Flag_Italic = false, Flag_Underline = false, Flag_StrikeOut = false;//б�壬�»��ߣ�ɾ����

	inline void Settextstyle(Re height, Re weight, COLOR color, bool flag1 = 0, bool flag2 = 0, bool flag3 = 0){
		settextstyle(height, 0, _T("KaiTi"), 0, 0, weight, Flag_Italic = flag1, Flag_Underline = flag2, Flag_StrikeOut = flag3);
		Settextcolor(color);
	}

	//PS_SOLID 	����Ϊʵ�ߡ�
	//PS_DASH 	����Ϊ���ߣ�------------
	//PS_DOT 	����Ϊ���ߣ�������������������������
	inline void Setlinestyle(Re thickness){ setlinestyle(PS_SOLID, thickness); }
	inline void Setlinestyle(Re style, Re thickness){ setlinestyle(style, thickness); }

	/*------------------------------------------------------------------------------*/

	/*�����ģʽ�趨��*/

#define bkmode1 OPAQUE      //ͼ������ֱ���Ϊ��Ĭ��ʹ�õ�ǰbkcolor
#define bkmode2 TRANSPARENT //ͼ������ֱ���Ϊ��͸��

	/*------------------------------------------------------------------------------*/

	/*��ͼƬ���롢�����*/

	inline void putimage(Re x, Re y, IMAGE &img){ putimage(x, y, &img); }
	inline void putimage(Point P, IMAGE img){ putimage(P.x, P.y, &img); }
	inline IMAGE getimage(Re x1, Re y1, Re x2, Re y2){ IMAGE img; getimage(&img, x1, y1, x2 - x1 + 1, y2 - y1 + 1); return img; }
	inline IMAGE getimage(Range R){ return getimage(R.x1, R.y1, R.x2, R.y2); }
	inline IMAGE getimage(Point P){ return getimage(P.x, P.y, P.x, P.y); }

	/*------------------------------------------------------------------------------*/

	/*���������ǡ�*/

	inline void ClearRange(Re x1, Re y1, Re x2, Re y2, COLOR color){ setbkcolor(color), clearrectangle(x1, y1, x2, y2); }//��color����x1<=x<=x2,y1<=y<=y2������
	inline void ClearRange(Range R, COLOR color){ ClearRange(R.x1, R.y1, R.x2, R.y2, color); }//��color����R����
	inline void ClearRange(Re x, Re y, IMAGE img){ putimage(x, y, &img); }
	inline void ClearRange(Point P, IMAGE img){ putimage(P.x, P.y, &img); }
	inline void ClearRange(Range R, IMAGE img){ putimage(R.x1, R.y1, &img); }

	/*------------------------------------------------------------------------------*/

	/*��������ѡ�*/

	Range __mainbox; IMAGE __main_bkimg; bool __main_image;
	int __main_bkcolor = black, __main_color = white, __main_bkmode = bkmode1;
	inline void Clearmain(){
		if (__main_image)putimage(__mainbox.x1, __mainbox.y1, __main_bkimg);
		else ClearRange(__mainbox, __main_bkcolor);
	}
	inline void Setmain(Range R, COLOR bkcolor){
		__mainbox = R, __main_bkcolor = bkcolor, __main_image = 0, Clearmain();
	}
	inline void Setmain(Range R, IMAGE bkimg){
		__mainbox = R, __main_bkimg = bkimg, __main_image = 1, Clearmain();
	}

	/*------------------------------------------------------------------------------*/

	/*������ѡ�*/
	Range __debugbox; IMAGE __debug_bkimg;
	int __debug_bkcolor = black, __debug_color, __debug_bkmode, __debug_height, __debug_weight;
	bool debug_Italic = false, debug_Underline = false, debug_StrikeOut = false;//б�壬�»��ߣ�ɾ����
	inline void Cleardebug(){
		if (__debug_bkmode == bkmode2)putimage(__debugbox.x1, __debugbox.y1, __debug_bkimg);
		else ClearRange(__debugbox, __debug_bkcolor);
	}
	inline void Setdebug(Range R, COLOR bkcolor, Re height, Re weight, COLOR color = lightgray, bool flag1 = 0, bool flag2 = 0, bool flag3 = 0){
		__debugbox = R, __debug_bkcolor = bkcolor, __debug_bkmode = bkmode1, Cleardebug();
		__debug_height = height, __debug_weight = weight, __debug_color = color, debug_Italic = flag1, debug_Underline = flag2, debug_StrikeOut = flag3;
	}
	inline void Setdebug(Range R, IMAGE &bkimg, Re height, Re weight, COLOR color = lightgray, bool flag1 = 0, bool flag2 = 0, bool flag3 = 0){
		__debugbox = R, __debug_bkimg = bkimg, __debug_bkmode = bkmode2, Cleardebug();
		__debug_height = height, __debug_weight = weight, __debug_color = color, debug_Italic = flag1, debug_Underline = flag2, debug_StrikeOut = flag3;
	}

	/*------------------------------------------------------------------------------*/
}
using namespace SETTING_OPTION;

namespace DRAW_PICTURE{/*����ͼ�����֡�*/

	/*����ͼ���塿*/
#define _Begin BeginBatchDraw 
#define _End EndBatchDraw 
#define _Flush FlushBatchDraw 

	/*����ͼ�ϵ����á�*/

	inline void ST(){ BeginBatchDraw(); }
	inline void DO(){ FlushBatchDraw(); }
	inline void ED(){ EndBatchDraw(); }

	/*��ͼƬת͸��������*/

	inline void putimage_transparancy(Re x, Re y, IMAGE *img, IMAGE *dstimg = NULL){
		// ������ʼ��
		DWORD *dst = GetImageBuffer(dstimg);
		DWORD *src = GetImageBuffer(img);
		int src_width = img->getwidth();
		int src_height = img->getheight();
		int dst_width = (dstimg == NULL ? getwidth() : dstimg->getwidth());
		int dst_height = (dstimg == NULL ? getheight() : dstimg->getheight());

		// ������ͼ��ʵ�ʳ���
		int iwidth = (x + src_width > dst_width) ? dst_width - x : src_width;		// �������ұ߽�
		int iheight = (y + src_height > dst_height) ? dst_height - y : src_height;	// �������±߽�
		if (x < 0) { src += -x;				iwidth -= -x;	x = 0; }				// ��������߽�
		if (y < 0) { src += src_width * -y;	iheight -= -y;	y = 0; }				// �������ϱ߽�

		// ������ͼ��ʼλ��
		dst += dst_width * y + x;

		// ʵ��͸����ͼ
		for (int iy = 0; iy < iheight; iy++){
			for (int ix = 0; ix < iwidth; ix++){
				int sa = ((src[ix] & 0xff000000) >> 24);
				int sr = ((src[ix] & 0xff0000) >> 16);	// Դֵ�Ѿ��˹���͸��ϵ��
				int sg = ((src[ix] & 0xff00) >> 8);		// Դֵ�Ѿ��˹���͸��ϵ��
				int sb = src[ix] & 0xff;				// Դֵ�Ѿ��˹���͸��ϵ��
				int dr = ((dst[ix] & 0xff0000) >> 16);
				int dg = ((dst[ix] & 0xff00) >> 8);
				int db = dst[ix] & 0xff;

				dst[ix] = ((sr + dr * (255 - sa) / 255) << 16)
					| ((sg + dg * (255 - sa) / 255) << 8)
					| (sb + db * (255 - sa) / 255);
			}
			dst += dst_width;
			src += src_width;
		}
	}

	/*------------------------------------------------------------------------------*/

	/*������ͼ����*/

	inline void drawpoint(Point P, COLOR color){ putpixel(P.x, P.y, color); }//��һ����
	inline void drawpoint(Re x, Re y, COLOR color){ putpixel(x, y, color); }//��һ����

	inline void drawline(Line L, COLOR color, int thickness){ Setlinestyle(thickness), setlinecolor(color), line(L.x1, L.y1, L.x2, L.y2); }//��һ��ֱ��
	inline void drawline(Point P1, Point P2, COLOR color, int thickness){ Setlinestyle(thickness), drawline({ P1, P2 }, color, thickness); }//��һ��ֱ��
	inline void drawline(Re x1, Re y1, Re x2, Re y2, COLOR color, int thickness){ Setlinestyle(thickness), setlinecolor(color), line(x1, y1, x2, y2); }//��һ��ֱ��

	inline void drawrange(Range R, COLOR color, int thickness){//��һ�����ľ���
		if (thickness == 2)++R.x2, ++R.y2;
		drawline({ R.x1, R.y1, R.x2, R.y1 }, color, thickness);
		drawline({ R.x1, R.y1, R.x1, R.y2 }, color, thickness);
		drawline({ R.x2, R.y2, R.x2, R.y1 }, color, thickness);
		drawline({ R.x2, R.y2, R.x1, R.y2 }, color, thickness);
	}

	inline void drawrange(Range R, COLOR color){//��һ���ޱ߽��ߵ�ʵ�ľ���
		setfillcolor(color), solidrectangle(R.x1, R.y1, R.x2, R.y2);
	}

	inline void drawrange(Range R, COLOR color, COLOR color_, int thickness){//��һ���б߽��ߵ�ʵ�ľ���
		if (thickness == 2)++R.x2, ++R.y2;
		Setlinestyle(thickness), setfillcolor(color), setlinecolor(color_), fillrectangle(R.x1, R.y1, R.x2, R.y2);
	}

	inline void drawcircle(Point P, Re r, COLOR color, int thickness){//��һ������Բ
		Setlinestyle(thickness), setlinecolor(color), circle(P.x, P.y, r);
	}
	inline void drawcircle(Point P, Re r, COLOR color){//��һ���ޱ߽��ߵ�ʵ��Բ
		setfillcolor(color), solidcircle(P.x, P.y, r);
	}
	inline void drawcircle(Point P, Re r, COLOR color, COLOR color_, int thickness){//��һ���б߽��ߵ�ʵ��Բ
		Setlinestyle(thickness), setfillcolor(color), setlinecolor(color_), fillcircle(P.x, P.y, r);
	}

	/*------------------------------------------------------------------------------*/

	/*��������֡�*/

	/*��(x,y)���������*/
	inline void print(Point P, STR s){ outtextxy(P.x, P.y, s); }
	inline void print(Point P, Re a){ CH s[CH_N]; tostr(s, a), print(P, s); }
	inline void print(Point P, LL a){ CH s[CH_N]; tostr(s, a), print(P, s); }
	inline void print(Point P, LD a){ CH s[CH_N]; tostr(s, a), print(P, s); }
	inline void print(Point P, LD a, Re k){ CH s[CH_N]; tostr(s, a, k), print(P, s); }

	/*��x1<=x<=x2,y1<=y<=y2����������������*/
	inline void print(Range R, STR s, int format = 0){
		RECT r = { R.x1, R.y1, R.x2, R.y2 };
		if (format == 0)drawtext(s, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);//�������֣�ȫbox����
		if (format == 1)drawtext(s, &r, DT_CENTER | DT_WORDBREAK);//�������֣��Զ����У�ˮƽ���У���ֱ����
		if (format == 2)drawtext(s, &r, DT_LEFT | DT_WORDBREAK);//�������֣��Զ����У�ˮƽ������ֱ����
	}
	inline void print(Range R, Re a, Re format = 0){ CH s[CH_N]; tostr(s, a), print(R, s, format); }
	inline void print(Range R, LL a, Re format = 0){ CH s[CH_N]; tostr(s, a), print(R, s, format); }
	inline void print(Range R, LD a, Re format = 0){ CH s[CH_N]; tostr(s, a), print(R, s, format); }
	inline void print(Range R, LD a, Re k, Re format = 0){ CH s[CH_N]; tostr(s, a, k), print(R, s, format); }//����kλС�����ʵ��
	inline void print(Range R, STR s, COLOR color, Re height, Re weight, Re format = 0){ Settextstyle(height, weight, color, 0, 0, 0), print(R, s, format); }

	/*�ڵ������������*/
	inline void printdebug(STR s, COLOR color = __debug_color, Re height = __debug_height, Re weight = __debug_weight){
		Settextstyle(height, weight, color, debug_Italic, debug_Underline, debug_StrikeOut);
		setbkcolor(__debug_bkcolor), setbkmode(__debug_bkmode), Cleardebug(), print(__debugbox, s);
	}
	inline void printdebug(Re a, COLOR color = __debug_color, Re height = __debug_height, Re weight = __debug_weight){ CH s[CH_N]; tostr(s, a), printdebug(s, color, height, weight); }
	inline void printdebug(LL a, COLOR color = __debug_color, Re height = __debug_height, Re weight = __debug_weight){ CH s[CH_N]; tostr(s, a), printdebug(s, color, height, weight); }
	inline void printdebug(LD a, COLOR color = __debug_color, Re height = __debug_height, Re weight = __debug_weight){ CH s[CH_N]; tostr(s, a), printdebug(s, color, height, weight); }
	inline void printdebug(LD a, Re k, COLOR color = __debug_color, Re height = __debug_height, Re weight = __debug_weight){ CH s[CH_N]; tostr(s, a, k), printdebug(s, color, height, weight); }

	/*------------------------------------------------------------------------------*/
}
using namespace DRAW_PICTURE;