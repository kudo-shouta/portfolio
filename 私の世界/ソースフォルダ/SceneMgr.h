#ifndef SCENEMGR_H
#define SCENEMGR_H

#include "SceneChanger.h"
#include "BaseScene.h"

//シーンを管理するクラス
//ポリモーフィズムを採用
class SceneMgr : public SceneChanger{
private:
	BaseScene* pScene;
	bool initialized;
public:
	SceneMgr();
	~SceneMgr();
	void Initialize();
	void Finalize();
	void Update();
	void Draw();
	void ChangeScene(eScene NextScene);
};

#endif //SCENEMGR_H