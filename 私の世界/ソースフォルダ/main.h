#ifndef MAIN_H
#define MAIN_H

#include "DxLib.h"
#include <vector>
#include <string>

#define SCREEN_X 640
#define SCREEN_Y 480

//ディレクト内のファイルを読み込む
std::vector<std::string> getFileName(const std::string& dir_name, const std::string& extension);

//ゲーム全体で使用する変数
typedef struct {
	int key[256];
	int AllKeyCount;
	int pad[32];
	int mapID; //実際には１小さな値
	int px;
	int py;
}t_MainData;

//マップデータ
struct MapList {
	int ID;
	std::string fmf;
	std::string png;
	std::string name;
};

enum {
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_UP,
	INPUT_A,	//しかく
	INPUT_B,	//ばつ
	INPUT_C,	//まる
	INPUT_X,	//さんかく
	INPUT_Y,	//L1
	INPUT_Z,	//R1
	INPUT_L,	//L2
	INPUT_R,	//R2
	INPUT_START,//SHARE
	INPUT_M,	//OPTIONS
	INPUT_D,	//L3
	INPUT_F,	//R3
	INPUT_G,INPUT_H,INPUT_I,INPUT_J,INPUT_K,
	INPUT_LL,INPUT_N,INPUT_O,INPUT_P,INPUT_RR,
	INPUT_S,INPUT_T,INPUT_U,INPUT_V,INPUT_W,INPUT_XX
};

extern t_MainData g_MainData;
extern std::vector<MapList> mapList;

extern int Black;
extern int Black1;
extern int EndFlag;

#endif // !MAIN_H