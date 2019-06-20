#include "main.h"
#include "Map.h"
#include "MenuBox.h"

void Map::key(){
	if (menu->isOpen() == FALSE) {
		InputBuf = GetInputChar(TRUE);
		if (InputBuf != 0 && InputBuf >= CTRL_CODE_CMP) {
			if (InputCount != 5) {
				InputChar[InputCount] = InputBuf;
				InputCount++;
			}
			else {
				for (int i = 0; i < 5; i++)
					InputChar[i] = InputChar[i + 1];
				InputChar[InputCount] = InputBuf;
			}
			if (strcmp(InputChar, "henshu") == 0) {
				menu->Open();
			}
		}

		if (g_MainData.key[KEY_INPUT_SPACE] == 1 ||
			g_MainData.pad[INPUT_A] == 1)
			menu->Open();
	}
}

Map::Map(SceneChanger * changer) :BaseScene(changer) {
}

Map::~Map(){
	Finalize();
}

bool Map::Initialize(){
	menu = new MenuBox();
	nowMapID = 0;
	fmfMap = new CFmfMap();
	player = new Player(fmfMap);
	return false;
}

void Map::Finalize(){
	//delete menu;
}

void Map::Update(){
	key();
	menu->Update();
	player->Update(menu->isOpen());
	
	if (nowMapID != g_MainData.mapID) {	//現在のIDと違っていたらマップ変更
		nowMapID = g_MainData.mapID;
		fmfMap->Open(mapList[nowMapID - 1].fmf.c_str(), mapList[nowMapID - 1].png.c_str());
	}
}

void Map::Draw(){
	fmfMap->Draw(player->getpx(),player->getpy());
	player->Draw();
	menu->Draw();
}