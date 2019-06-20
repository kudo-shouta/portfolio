#include "DxLib.h"
#include "main.h"
#include "Fps.h"
#include "SceneMgr.h"
#include "Event.h"
#include <vector>
#include <string>

void DxInit();
void mainInit();
int GetHitKeyStateAll_2(int KeyStateBuf[], int PadStateBuf[]);
static void GetCheckHitKeyAll_2(int *);

int FPS = 60;
int Black = GetColor(255, 255, 255); //これ黒じゃない白だ
int Black1 = GetColor(0, 0, 0);
int EndFlag = 0;
t_MainData g_MainData;
std::vector<MapList> mapList;

int WINAPI WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow) {
	//初期化処理
	DxInit();
	mainInit();

	//全体のシーン管理マネージャー起動
	SceneMgr sceneMgr;
	sceneMgr.Initialize();

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && GetHitKeyStateAll_2(g_MainData.key, g_MainData.pad) == 0 && EndFlag == 0) {
		
		//キー入力受付　グローバル変数にて共有
		GetCheckHitKeyAll_2(&g_MainData.AllKeyCount);
		//FPSの維持
		WaitFpsConstant(FPS);

		//描写と更新
		sceneMgr.Draw();
		sceneMgr.Update();

		//FPSの数値表示
		//ＴｏＤｏ：最後には消す
		//DrawFps(FPS);
		ScreenFlip();
	}
	//終了処理
	DxLib_End();
	return 0;
}

//恒例の初期設定
void DxInit(void) {
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	DxLib_Init();
	SetLogDrawOutFlag(FALSE);
}

//なにに使うつもりだったんだ....
void mainInit() {
	g_MainData.mapID = 12;

	/*fstream fs;

	fs.open("data/map/maplist.txt", ios::in);
	if (!fs)return;

	char IDbuf[256];
	char fmfbuf[256];
	char pngbuf[256];
	char namebuf[256];
	mapList.clear();
	int i = 0;

	while (!fs.eof()) {
		fs.getline(IDbuf, 256);
		fs.getline(fmfbuf, 256);
		fs.getline(pngbuf, 256);
		fs.getline(namebuf, 256);
		MapList add = { atoi(IDbuf), fmfbuf, pngbuf, namebuf };
		mapList.push_back(add);
		i++;
	}*/
}

//全てのキーを個別管理
int GetHitKeyStateAll_2(int KeyStateBuf[] ,int PadStateBuf[])
{
	char GetHitKeyStateAll_Key[256];

	GetHitKeyStateAll(GetHitKeyStateAll_Key);
	for (int i = 0; i<256; i++) {
		if (GetHitKeyStateAll_Key[i] == 1) KeyStateBuf[i]++;
		else							KeyStateBuf[i] = 0;
	}

	int PadInput = GetJoypadInputState(DX_INPUT_PAD1);
	/*for (int i = 1, j = 1; i <= 2 ^ 32; i = i * 2, j++) {
		if (PadInput & i) PadStateBuf[j]++;
		else			PadStateBuf[j] = 0;
	}*/
	for (int i = 0; i < 32; i++) {
		if (PadInput & (1 << i)) PadStateBuf[i]++;
		else					PadStateBuf[i] = 0;
	}
	return 0;
}

//全てのキーを一括管理
static void GetCheckHitKeyAll_2(int *HitCount) {
	if (CheckHitKeyAll() > 0)
		(*HitCount)++;
	else
		(*HitCount) = 0;
}

//ファイル読み込み・あちこちで使います
//ファイルパス、拡張子を指定して使う
std::vector<std::string> getFileName(const std::string& dir_name, const std::string& extension) {
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	std::vector<std::string> file_names;

	std::string search_name = dir_name + "/*." + extension;

	hFind = FindFirstFile( search_name.c_str(), &win32fd);

	if(hFind == INVALID_HANDLE_VALUE) {
		printfDx("%s が見つかりませんでした\n",dir_name.c_str());
		return file_names;
	}

	do {
		if(win32fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
		}
		else {
			file_names.push_back(win32fd.cFileName);
		}
	}while(FindNextFile(hFind, &win32fd));

	FindClose(hFind);

	return file_names;
}