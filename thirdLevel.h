#include <stdio.h>
#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>
#include "EasyXPng.h"

#pragma comment(lib, "winmm.lib")

int l3_map1[3][3] = { { 1, 2, 3 }, { 4, 4, 6 }, { 7, 8, 9 } };
int l3_map2[3][4] = { { 1, 2, 3, 4 }, { 5, 6, 3, 8 }, { 9, 1, 2, 3 } };
int l3_map3[4][3] = { { 4, 5, 6 }, { 7, 8, 9 }, { 1, 2, 3 }, { 4, 5, 6 } };
int l3_map4[3][4] = { { 7, 8, 9, 1 }, { 2, 3, 4, 5 }, { 6, 7, 8, 9 } };
int l3_map5[4][3] = { { 1, 2, 3 }, { 5, 5, 6 }, { 7, 8, 9 }, { 7, 3, 3 } };

int l3_curLevel = 1;//判断当前点击的卡片层数

//游戏初始化
void gameInit3() {

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
	disturbance1(l3_map1);
	disturbance2(l3_map2);
	disturbance1(l3_map3);
	disturbance2(l3_map4);
	disturbance1(l3_map5);
}
//更新
void update3() {
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

	// 第一关，是比较简单的
	int off = 10;
	int marginX1 = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	int marginY1 = 120;
	int marginX2 = (WIN_W - BLOCK_W * 4 - off * 3) / 2;
	int marginY2 = 150;
	int marginX3 = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	int marginY3 = 120;
	int marginX4 = (WIN_W - BLOCK_W * 4 - off * 3) / 2;
	int marginY4 = 150;
	int marginX5 = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	int marginY5 = 120;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (l3_map5[i][j] == 0) continue;

			// 位置的计算
			int x = marginX5 + j * (BLOCK_W + off);
			int y = marginY5 + i * (BLOCK_H + off);
			// 到底画哪一个
			putimagePng(x, y, &imgBlocks[l3_map5[i][j] - 1]);
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (l3_map4[i][j] == 0) continue;

			// 位置的计算
			int x = marginX4 + j * (BLOCK_W + off);
			int y = marginY4 + i * (BLOCK_H + off);
			// 到底画哪一个
			putimagePng(x, y, &imgBlocks[l3_map4[i][j] - 1]);
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (l3_map3[i][j] == 0) continue;

			// 位置的计算
			int x = marginX3 + j * (BLOCK_W + off);
			int y = marginY3 + i * (BLOCK_H + off);
			// 到底画哪一个
			putimagePng(x, y, &imgBlocks[l3_map3[i][j] - 1]);
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (l3_map2[i][j] == 0) continue;

			// 位置的计算
			int x = marginX2 + j * (BLOCK_W + off);
			int y = marginY2 + i * (BLOCK_H + off);
			// 到底画哪一个
			putimagePng(x, y, &imgBlocks[l3_map2[i][j] - 1]);
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (l3_map1[i][j] == 0) continue;

			// 位置的计算
			int x = marginX1 + j * (BLOCK_W + off);
			int y = marginY1 + i * (BLOCK_H + off);
			// 到底画哪一个
			putimagePng(x, y, &imgBlocks[l3_map1[i][j] - 1]);
		}
	}
	int marginX = 25;
	int marginY = 630;
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
void work3(struct location* loc) {
	int index;
	if (l3_curLevel == 1){
		index = l3_map1[loc->row - 1][loc->col - 1];
		l3_map1[loc->row - 1][loc->col - 1] = 0;
	}
	else if (l3_curLevel == 2){
		index = l3_map2[loc->row - 1][loc->col - 1];
		l3_map2[loc->row - 1][loc->col - 1] = 0;
	}
	else if (l3_curLevel == 3){
		index = l3_map3[loc->row - 1][loc->col - 1];
		l3_map3[loc->row - 1][loc->col - 1] = 0;
	}
	else if (l3_curLevel == 4){
		index = l3_map4[loc->row - 1][loc->col - 1];
		l3_map4[loc->row - 1][loc->col - 1] = 0;
	}
	else{
		index = l3_map5[loc->row - 1][loc->col - 1];
		l3_map5[loc->row - 1][loc->col - 1] = 0;
	}
	//打乱
	//if (index){
	//disturbance1(l3_map1);
	//}
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
void getResult3(){
	flag = 1;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (l3_map1[i][j] != 0){
				flag = 0;
			}
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			if (l3_map2[i][j] != 0 || l3_map4[i][j] != 0){
				flag = 0;
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			if (l3_map3[i][j] != 0 || l3_map5[i][j] != 0){
				flag = 0;
			}
		}
	}
	if (flag){
		update3();
		putimagePng(30, 200, &imgResults[1]);
		isStarted = 0;
	}
	if (!flag&&chao[6]){
		putimagePng(30, 200, &imgResults[0]);
		isStarted = 0;
	}
}
// 重玩这关
void rePlay3(){
	// 初始化地图并打乱
	int map11[3][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 } };
	int map22[3][4] = { { 1, 2, 3, 4 }, { 5, 6, 3, 8 }, { 9, 1, 2, 3 } };
	int map33[4][3] = { { 4, 5, 6 }, { 7, 8, 9 }, { 1, 2, 3 }, { 4, 5, 6 } };
	int map44[3][4] = { { 7, 8, 9, 1 }, { 2, 3, 4, 5 }, { 6, 7, 8, 9 } };
	int map55[4][3] = { { 1, 2, 3 }, { 4, 5, 6 }, { 7, 8, 9 }, { 7, 3, 3 } };
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			l3_map1[i][j] = map11[i][j];
		}
	}
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 4; j++) {
			l3_map2[i][j] = map22[i][j];
			l3_map4[i][j] = map44[i][j];
		}
	}
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++) {
			l3_map3[i][j] = map33[i][j];
			l3_map5[i][j] = map55[i][j];
		}
	}
	disturbance1(l3_map1);
	disturbance2(l3_map2);
	disturbance1(l3_map3);
	disturbance2(l3_map4);
	disturbance1(l3_map5);
	for (int i = 0; i < 7; i++)
		chao[i] = 0;
	isStarted = 1;
	flag = 1;
	disChance = 1;
}
int startGame3() {
	struct location loc; //表示玩家有效点击的位置
	// 游戏主体循环
	//while (1) {
	if (isStarted == 1){
		ExMessage msg;
		if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN) {
			int msgx = msg.x;
			int msgy = msg.y;
			//如果点击到打乱工具
			if (clickTool(msgx, msgy) && disChance){
				disturbance1(l3_map1);
				disturbance2(l3_map2);
				disturbance1(l3_map3);
				disturbance2(l3_map4);
				disturbance1(l3_map5);
				disChance = 0;
			}
			// 如果点击的图片卡片属于第五层且上面没有被盖住
			if (userClick3(&loc, msgx, msgy) && isEmpty2(loc.row - 1, loc.col - 1, l3_map4)) {
				l3_curLevel = 5;
				work3(&loc);
			}
			// 如果点击的图片卡片属于第四层且上面没有被盖住
			else if (userClick2(&loc, msgx, msgy) && isEmpty3(loc.row - 1, loc.col - 1, l3_map3)) {
				l3_curLevel = 4;
				work3(&loc);
			}
			// 如果点击的图片卡片属于第三层且上面没有被盖住
			else if (userClick3(&loc, msgx, msgy) && isEmpty2(loc.row - 1, loc.col - 1, l3_map2)) {
				l3_curLevel = 3;
				work3(&loc);
			}
			// 如果点击的图片卡片属于第二层且上面没有被盖住
			else if (userClick2(&loc, msgx, msgy) && isEmpty1(loc.row - 1, loc.col - 1, l3_map1)) {
				l3_curLevel = 2;
				work3(&loc);
			}
			// 如果点击的图片卡片属于第一层
			else if (userClick1(&loc, msgx, msgy)){
				l3_curLevel = 1;
				work3(&loc);
			}
		}
		//刷新游戏窗口
		update3();
		//清除卡片
		clear(chao);
		//判断游戏结果
		getResult3();
	}
	else if (isStarted == 0){
		if (_kbhit())			//	If a key is pressed
		{
			char key;
			key = _getch();		//	Obtain key info
			if (GetAsyncKeyState(VK_SPACE))     //空格
			{
				rePlay3();
			}
			else if (GetAsyncKeyState(VK_RETURN) && flag) //回车键--游戏过关时
			{
				rePlay3();
				return 1;
			}
			else if (GetAsyncKeyState(VK_RETURN) && !flag) //回车键--游戏未过关时
			{
				rePlay3();
				return -1;
			}
		}
		//}
	}
}