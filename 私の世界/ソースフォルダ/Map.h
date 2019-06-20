#ifndef MAP_H
#define MAP_H

#include "BaseScene.h"
#include "MenuBox.h"
#include "fmfmap.h"
#include "Player.h"

//�^�C�g���Ǘ��N���X
class Map :public BaseScene {
private:
	void key();
	MenuBox* menu;
	Player* player;

	int nowMapID;	//���݂̃}�b�vID
	CFmfMap* fmfMap;

	char InputBuf;
	char InputChar[6];
	int InputCount;

public:
	Map(SceneChanger * changer);
	~Map();
	bool Initialize();
	void Finalize();
	void Update();
	void Draw();
};

#endif //!MAP_H