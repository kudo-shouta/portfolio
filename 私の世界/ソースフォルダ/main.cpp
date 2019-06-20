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
int Black = GetColor(255, 255, 255); //���ꍕ����Ȃ�����
int Black1 = GetColor(0, 0, 0);
int EndFlag = 0;
t_MainData g_MainData;
std::vector<MapList> mapList;

int WINAPI WinMain(HINSTANCE hInstace, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow) {
	//����������
	DxInit();
	mainInit();

	//�S�̂̃V�[���Ǘ��}�l�[�W���[�N��
	SceneMgr sceneMgr;
	sceneMgr.Initialize();

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0 && GetHitKeyStateAll_2(g_MainData.key, g_MainData.pad) == 0 && EndFlag == 0) {
		
		//�L�[���͎�t�@�O���[�o���ϐ��ɂċ��L
		GetCheckHitKeyAll_2(&g_MainData.AllKeyCount);
		//FPS�̈ێ�
		WaitFpsConstant(FPS);

		//�`�ʂƍX�V
		sceneMgr.Draw();
		sceneMgr.Update();

		//FPS�̐��l�\��
		//�s���c���F�Ō�ɂ͏���
		//DrawFps(FPS);
		ScreenFlip();
	}
	//�I������
	DxLib_End();
	return 0;
}

//�P��̏����ݒ�
void DxInit(void) {
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);
	DxLib_Init();
	SetLogDrawOutFlag(FALSE);
}

//�ȂɂɎg�����肾������....
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

//�S�ẴL�[���ʊǗ�
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

//�S�ẴL�[���ꊇ�Ǘ�
static void GetCheckHitKeyAll_2(int *HitCount) {
	if (CheckHitKeyAll() > 0)
		(*HitCount)++;
	else
		(*HitCount) = 0;
}

//�t�@�C���ǂݍ��݁E���������Ŏg���܂�
//�t�@�C���p�X�A�g���q���w�肵�Ďg��
std::vector<std::string> getFileName(const std::string& dir_name, const std::string& extension) {
	HANDLE hFind;
	WIN32_FIND_DATA win32fd;
	std::vector<std::string> file_names;

	std::string search_name = dir_name + "/*." + extension;

	hFind = FindFirstFile( search_name.c_str(), &win32fd);

	if(hFind == INVALID_HANDLE_VALUE) {
		printfDx("%s ��������܂���ł���\n",dir_name.c_str());
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