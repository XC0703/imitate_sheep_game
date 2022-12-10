#include <stdio.h>
#include "EasyXPng.h"
#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

int map[3][3];

//游戏初始化
void gameInit1() {
	for (int i = 0; i < 7; i++)
		chao[i] = 0;
	isStarted = 1;
	flag = 1;
	disChance = 1;//使用打乱工具的机会

	// 加载游戏的资源
	// 项目分析：背景图片、小方块
	loadimage(&imgBg, "resource/image/bg.png");
	loadimage(&imgTool, "resource/image/tool.png");

	char fileName[256];
	for (int i = 0; i < BLOCK_KINDS_1; i++) {
		sprintf_s(fileName,
			sizeof(fileName),
			"resource/image/UniversityBadge/%d.png", i + 1);
		loadimage(&imgBlocks[i], fileName);
	}
	loadimage(&imgResults[0], "resource/image/text/fail.png");
	loadimage(&imgResults[1], "resource/image/text/success.png");

	// 初始化地图并打乱
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			map[i][j] = i + 1; // 1  2  3
		}
	}
	disturbance1(map);

	//mciSendString("play res/sheep.mp3", 0, 0, 0);
}
//更新
void update1() {
	BeginBatchDraw();
	putimage(0, 0, &imgBg); //图片的左上角的坐标位置
	if (disChance){
		putimagePng(215, 450, &imgTool); //打乱工具图片坐标位置
		//setbkmode(TRANSPARENT);
		//settextcolor(RGB(0, 0, 0));
		//settextstyle(20, 0, _T("楷体"));
		//outtextxy(190, 500, _T("打乱工具"));
	}
	Sleep(10);
	//putimage(0, 0, &imgBg); //图片的左上角的坐标位置

	// 第一关，是比较简单的
	int off = 10;
	int marginX = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	int marginY = 120;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] == 0) continue;

			// 位置的计算
			int x = marginX + j * (BLOCK_W + off);
			int y = marginY + i * (BLOCK_H + off);
			// 到底画哪一个
			putimagePng(x, y, &imgBlocks[map[i][j] - 1]);
		}
	}

	marginX = 25;
	marginY = 630;
	for (int i = 0; i < 7; i++) {
		if (chao[i]) {
			int x = marginX + i * 62;
			int y = marginY + 5;
			putimagePng(x, y, &imgBlocks[chao[i] - 1]);
		}
	}

	EndBatchDraw();
}
// work(&loc);
void work1(struct location* loc) {
	int index = map[loc->row - 1][loc->col - 1];
	map[loc->row - 1][loc->col - 1] = 0;
	// 放到羊槽的合适位置
	for (int i = 0; i < 7; i++){
		if (i<6 && chao[i] == index&&chao[i + 1] != index){
			insertArr(chao, 7, i + 1, index);
			break;
		}
		else if (chao[i] == 0) {
			chao[i] = index;
			break;
		}
	}
}
//判断游戏结果
void getResult1(){
	flag = 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] != 0){
				flag = 0;
			}
		}
	}
	if (flag){
		update1();
		putimagePng(30, 200, &imgResults[1]);
		isStarted = 0;
	}
	if (!flag&&chao[6]){
		putimagePng(30, 200, &imgResults[0]);
		isStarted = 0;
	}
}
// 重玩这关
void rePlay1(){
	// 初始化地图并打乱
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			map[i][j] = i + 1; // 1  2  3
		}
	}
	disturbance1(map);
	for (int i = 0; i < 7; i++)
		chao[i] = 0;
	isStarted = 1;
	flag = 1;
	disChance = 1;//使用打乱工具的机会
}
int startGame1() {
	//gameInit(); //游戏的初始化
	struct location loc; //表示玩家有效点击的位置
	if (isStarted == 1){
		ExMessage msg;
		if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN) {
			int msgx = msg.x;
			int msgy = msg.y;
			//如果点击到打乱工具
			if (clickTool(msgx, msgy) && disChance){
				disturbance1(map);
				disChance = 0;
			}
			if (userClick1(&loc, msgx, msgy)){
				work1(&loc);
			}
		}
		//刷新游戏窗口
		update1();
		//清除卡片
		clear(chao);
		//判断游戏结果
		getResult1();
	}
		else if (isStarted == 0){
			if (_kbhit())			//	If a key is pressed
			{
				char key;
				key = _getch();		//	Obtain key info
				if (GetAsyncKeyState(VK_SPACE))     //空格
				{
					rePlay1();
				}
				else if (GetAsyncKeyState(VK_RETURN) && flag) //回车键--游戏过关时
				{
					rePlay1();
					return 1;
				}
				else if (GetAsyncKeyState(VK_RETURN) && !flag) //回车键--游戏未过关时
				{
					rePlay1();
					return -1;
				}
			}
		}
	//}
}
