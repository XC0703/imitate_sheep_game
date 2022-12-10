#include <graphics.h>
#include <time.h>

#pragma comment(lib, "winmm.lib")
#define BLOCK_KINDS_1 9
#define WIN_W  480
#define WIN_H  820
#define BLOCK_W 60
#define BLOCK_H 60

// ������Դ
IMAGE imgBg; //������ʾ����ͼƬ
IMAGE imgTool;//������ʾ���ҹ���
IMAGE imgBlocks[BLOCK_KINDS_1];//��Ƭ
IMAGE imgResults[2];//��Ϸͨ�ػ���ʧ��

int chao[7] = { 0 };
int isStarted = 1; //��Ϸ�Ƿ�ʼ
int flag = 1;//��Ϸʧ�ܻ��߳ɹ���־
int disChance = 1;//ʹ�ô��ҹ��ߵĻ���

struct location {
	int row;
	int col;
};


/************�������õ��ĺ���************/
//�������ݵĺ���
void insertArr(int *arr, int n, int p, int k){//n�Ǽ���Ԫ�أ�p�ǲ���λ��, k�ǲ����Ԫ�� 
	for (int i = n; i >= p; i--)//���ſ�λ 
	{
		arr[i] = arr[i - 1];
	}
	arr[p - 1] = k;
}
//��������м�0�ĺ���
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


/************��Ƭ�����õ��ĺ���************/
//��Ƭ���Һ���
//���ҵ�һ�㡢������
void disturbance1(int map[][3]) {//��һ��
	srand(time(NULL)); //�����������

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
//���ҵڶ��㡢���Ĳ�
void disturbance2(int map[][4]) {//�ڶ���
	srand(time(NULL)); //�����������

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
// �����������ͬ������ͼƬ
void clear(int *chao) {
	for (int i = 0; chao[i] && i < 5; i++){
		if (chao[i] == chao[i + 1] && chao[i + 1] == chao[i + 2]){
			chao[i] = 0;
			chao[i + 1] = 0;
			chao[i + 2] = 0;
		}
	}
	//�ճ���λ�ò���
	CompactIntegers(chao, 7);
}
//���--�ж��Ƿ����ڵ�һ��
bool userClick1(struct location* loc, int msgx, int msgy) {
	// �жϾ����λ��
	int off = 10;
	int marginY = 120;
	int marginX = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	float tail;
	//��
	float x = (msgx - marginX)*1.0 / (BLOCK_W + off);
	int col = (x + 1) - 0.1;
	tail = (x + 1) - 0.1 - col;  //�õ�С������
	if (col < 1 || col > 3 || tail > 0.6) {
		return false;
	}
	//��
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
//���--�ж��Ƿ����ڵڶ��㡢���Ĳ�
bool userClick2(struct location* loc, int msgx, int msgy) {
	// �жϾ����λ��
	int off = 10;
	int marginY = 150;
	int marginX = (WIN_W - BLOCK_W * 4 - off * 3) / 2;
	float tail;
	//��
	float x = (msgx - marginX)*1.0 / (BLOCK_W + off);
	int col = (x + 1) - 0.1;
	tail = (x + 1) - 0.1 - col;  //�õ�С������
	if (col < 1 || col > 4 || tail > 0.6) {
		return false;
	}
	//��
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
//���--�ж��Ƿ����ڵ����㡢�����
bool userClick3(struct location* loc, int msgx, int msgy) {
	// �жϾ����λ��
	int off = 10;
	int marginY = 120;
	int marginX = (WIN_W - BLOCK_W * 3 - off * 2) / 2;
	float tail;
	//��
	float x = (msgx - marginX)*1.0 / (BLOCK_W + off);
	int col = (x + 1) - 0.1;
	tail = (x + 1) - 0.1 - col;  //�õ�С������
	if (col < 1 || col > 3 || tail > 0.6) {
		return false;
	}
	//��
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
//�ж�ĳ���ڶ��㿨Ƭ��һ����Χ�ĸ���Ƭ�Ƿ�Ϊ��
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
//�ж�ĳ�������㡢����㿨Ƭ��һ����Χ�ĸ���Ƭ�Ƿ�Ϊ��
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
//�ж�ĳ�����Ĳ㿨Ƭ��һ����Χ�ĸ���Ƭ�Ƿ�Ϊ��
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


/************���������õ��ĺ���************/
bool clickTool(int msgx, int msgy){
	if (msgx > 215 && msgx < 265 && msgy>450 && msgy < 500)
		return true;
	return false;
}