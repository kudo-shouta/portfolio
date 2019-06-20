#ifndef SCENEMGR_H
#define SCENEMGR_H

#include "SceneChanger.h"
#include "BaseScene.h"

//�V�[�����Ǘ�����N���X
//�|�����[�t�B�Y�����̗p
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