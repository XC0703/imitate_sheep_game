#include <stdio.h>
#include "EasyXPng.h"
#include <graphics.h>
#include <time.h>
#include <conio.h>
#include <mmsystem.h>

#pragma comment(lib, "winmm.lib")

int map[3][3];

//��Ϸ��ʼ��
void gameInit1() {
	for (int i = 0; i < 7; i++)
		chao[i] = 0;
	isStarted = 1;
	flag = 1;
	disChance = 1;//ʹ�ô��ҹ��ߵĻ���

	// ������Ϸ����Դ
	// ��Ŀ����������ͼƬ��С����
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

	// ��ʼ����ͼ������
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			map[i][j] = i + 1; // 1  2  3
		}
	}
	disturbance1(map);

	//mciSendString("play res/sheep.mp3", 0, 0, 0);
}
//����
void update1() {
	BeginBatchDraw();
	putimage(0, 0, &imgBg); //ͼƬ�����Ͻǵ�����λ��
	if (disChance){
		putimagePng(215, 450, &imgTool); //���ҹ���ͼƬ����λ��
		//setbkmode(TRANSPARENT);
		//settextcolor(RGB(0, 0, 0));
		//settextstyle(20, 0, _T("����"));
		//outtextxy(190, 500, _T("���ҹ���"));
	}
	Sleep(10);
	//putimage(0, 0, &imgBg); //ͼƬ�����Ͻǵ�����λ��

	// ��һ�أ��ǱȽϼ򵥵�
	int off = 10;
	int marginX = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	int marginY = 120;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (map[i][j] == 0) continue;

			// λ�õļ���
			int x = marginX + j * (BLOCK_W + off);
			int y = marginY + i * (BLOCK_H + off);
			// ���׻���һ��
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
	// �ŵ���۵ĺ���λ��
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
//�ж���Ϸ���
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
// �������
void rePlay1(){
	// ��ʼ����ͼ������
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
	disChance = 1;//ʹ�ô��ҹ��ߵĻ���
}
int startGame1() {
	//gameInit(); //��Ϸ�ĳ�ʼ��
	struct location loc; //��ʾ�����Ч�����λ��
	if (isStarted == 1){
		ExMessage msg;
		if (peekmessage(&msg) && msg.message == WM_LBUTTONDOWN) {
			int msgx = msg.x;
			int msgy = msg.y;
			//�����������ҹ���
			if (clickTool(msgx, msgy) && disChance){
				disturbance1(map);
				disChance = 0;
			}
			if (userClick1(&loc, msgx, msgy)){
				work1(&loc);
			}
		}
		//ˢ����Ϸ����
		update1();
		//�����Ƭ
		clear(chao);
		//�ж���Ϸ���
		getResult1();
	}
		else if (isStarted == 0){
			if (_kbhit())			//	If a key is pressed
			{
				char key;
				key = _getch();		//	Obtain key info
				if (GetAsyncKeyState(VK_SPACE))     //�ո�
				{
					rePlay1();
				}
				else if (GetAsyncKeyState(VK_RETURN) && flag) //�س���--��Ϸ����ʱ
				{
					rePlay1();
					return 1;
				}
				else if (GetAsyncKeyState(VK_RETURN) && !flag) //�س���--��Ϸδ����ʱ
				{
					rePlay1();
					return -1;
				}
			}
		}
	//}
}
