#include <graphics.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
#define BLOCK_KINDS_1 9
#define WIN_W  480
#define WIN_H  820
#define BLOCK_W 60
#define BLOCK_H 60

// 定义资源
IMAGE imgBg; //用来表示背景图片
IMAGE imgTool;//用来表示打乱工具
IMAGE imgBlocks[BLOCK_KINDS_1];//卡片
IMAGE imgResults[2];//游戏通关或者失败

int chao[7] = { 0 };
int isStarted = 1; //游戏是否开始
int flag = 1;//游戏失败或者成功标志
int disChance = 1;//使用打乱工具的机会

struct location {
	int row;
	int col;
};


/************槽区域用到的函数************/
//插入数据的函数
void insertArr(int *arr, int n, int p, int k){//n是几个元素，p是插入位置, k是插入的元素 
	for (int i = n; i >= p; i--)//倒着空位 
	{
		arr[i] = arr[i - 1];
	}
	arr[p - 1] = k;
}
//清除数据中间0的函数
void CompactIntegers(int *arr, int n)
{
	int i, j, count = 0, allCount = n;
	for (i = 0; i<n; i++){
		if (arr[i] == 0){
			for (j = i; j<n - 1; j++)
				arr[j] = arr[j + 1];
			i--;
			n--;
		}
		else{
			count++;
		}
	}
	for (i = count; i<allCount; i++){
		arr[i] = 0;
	}
}


/************卡片区域用到的函数************/
//卡片打乱函数
//打乱第一层、第三层
void disturbance1(int map[][3]) {//第一层
	srand(time(NULL)); //配置随机种子

	for (int i = 0; i < 20; i++) {
		int row1 = rand() % 3; //0..2
		int col1 = rand() % 3;
		int row2 = rand() % 3;
		int col2 = rand() % 3;

		int tmp = map[row1][col1];
		map[row1][col1] = map[row2][col2];
		map[row2][col2] = tmp;
	}
}
//打乱第二层、第四层
void disturbance2(int map[][4]) {//第二层
	srand(time(NULL)); //配置随机种子

	for (int i = 0; i < 20; i++) {
		int row1 = rand() % 3; //0..2
		int col1 = rand() % 4; //0..3
		int row2 = rand() % 3;
		int col2 = rand() % 4;

		int tmp = map[row1][col1];
		map[row1][col1] = map[row2][col2];
		map[row2][col2] = tmp;
	}
}
// 清除槽连续相同的三张图片
void clear(int *chao) {
	for (int i = 0; chao[i] && i < 5; i++){
		if (chao[i] == chao[i + 1] && chao[i + 1] == chao[i + 2]){
			chao[i] = 0;
			chao[i + 1] = 0;
			chao[i + 2] = 0;
		}
	}
	//空出的位置补上
	CompactIntegers(chao, 7);
}
//点击--判断是否属于第一层
bool userClick1(struct location* loc, int msgx, int msgy) {
	// 判断具体的位置
	int off = 10;
	int marginY = 120;
	int marginX = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	float tail;
	//列
	float x = (msgx - marginX)*1.0 / (BLOCK_W + off);
	int col = (x + 1) - 0.1;
	tail = (x + 1) - 0.1 - col;  //得到小数部分
	if (col < 1 || col > 3 || tail > 0.6) {
		return false;
	}
	//行
	float y = (msgy - marginY)*1.0 / (BLOCK_H + off);
	int row = y + 1 - 0.1;
	tail = y + 1 - 0.1 - row;
	if (row < 1 || row > 3 || tail > 0.6) {
		return false;
	}
	loc->row = row;
	loc->col = col;

	printf("[%d,%d]", row, col);
	return true;
}
//点击--判断是否属于第二层、第四层
bool userClick2(struct location* loc, int msgx, int msgy) {
	// 判断具体的位置
	int off = 10;
	int marginY = 150;
	int marginX = (WIN_W - BLOCK_W * 4 - off * 3) / 2;
	float tail;
	//列
	float x = (msgx - marginX)*1.0 / (BLOCK_W + off);
	int col = (x + 1) - 0.1;
	tail = (x + 1) - 0.1 - col;  //得到小数部分
	if (col < 1 || col > 4 || tail > 0.6) {
		return false;
	}
	//行
	float y = (msgy - marginY)*1.0 / (BLOCK_H + off);
	int row = y + 1 - 0.1;
	tail = y + 1 - 0.1 - row;
	if (row < 1 || row > 3 || tail > 0.6) {
		return false;
	}

	loc->row = row;
	loc->col = col;
	printf("[%d,%d]", row, col);
	return true;
}
//点击--判断是否属于第三层、第五层
bool userClick3(struct location* loc, int msgx, int msgy) {
	// 判断具体的位置
	int off = 10;
	int marginY = 120;
	int marginX = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	float tail;
	//列
	float x = (msgx - marginX)*1.0 / (BLOCK_W + off);
	int col = (x + 1) - 0.1;
	tail = (x + 1) - 0.1 - col;  //得到小数部分
	if (col < 1 || col > 3 || tail > 0.6) {
		return false;
	}
	//行
	float y = (msgy - marginY)*1.0 / (BLOCK_H + off);
	int row = y + 1 - 0.1;
	tail = y + 1 - 0.1 - row;
	if (row < 1 || row > 4 || tail > 0.6) {
		return false;
	}
	loc->row = row;
	loc->col = col;

	printf("[%d,%d]", row, col);
	return true;
}
//判断某个第二层卡片上一层周围四个卡片是否为空
bool isEmpty1(int x, int y, int arr[][3]){
	if (x > 1 && y <= 0){
		if (!arr[x][y]){
			return true;
		}
	}
	if (x>1 && y >= 3){
		if (!arr[x][y - 1]){
			return true;
		}
	}
	if (x>1){
		if (!arr[x][y] && !arr[x][y - 1]){
			return true;
		}
	}
	if (y <= 0){
		if (!arr[x][y] && !arr[x + 1][y]){
			return true;
		}
	}
	if (y >= 3){
		if (!arr[x][y - 1] && !arr[x + 1][y - 1]){
			return true;
		}
	}
	if (!arr[x][y] && !arr[x + 1][y] && !arr[x][y - 1] && !arr[x + 1][y - 1]){
		return true;
	}
	return false;
}
//判断某个第三层、第五层卡片上一层周围四个卡片是否为空
bool isEmpty2(int x, int y, int arr[][4]){
	if (x <= 0){
		if (!arr[x][y] && !arr[x][y + 1]){
			return true;
		}
	}
	if (x >= 3){
		if (!arr[x - 1][y] && !arr[x - 1][y + 1]){
			return true;
		}
	}
	if (!arr[x][y] && !arr[x][y + 1] && !arr[x - 1][y] && !arr[x - 1][y + 1]){
		return true;
	}
	return false;
}
//判断某个第四层卡片上一层周围四个卡片是否为空
bool isEmpty3(int x, int y, int arr[][3]){
	if (y <= 0){
		if (!arr[x][y] && !arr[x + 1][y]){
			return true;
		}
	}
	if (y >= 3){
		if (!arr[x][y - 1] && !arr[x + 1][y - 1]){
			return true;
		}
	}
	if (!arr[x][y] && !arr[x + 1][y] && !arr[x][y - 1] && !arr[x + 1][y - 1]){
		return true;
	}
	return false;
}


/************工具区域用到的函数************/
bool clickTool(int msgx, int msgy){
	if (msgx > 215 && msgx < 265 && msgy>450 && msgy < 500)
		return true;
	return false;
}