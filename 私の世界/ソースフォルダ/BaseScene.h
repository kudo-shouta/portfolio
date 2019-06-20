#ifndef BASESCENE_H
#define BASESCENE_H

#include "SceneChanger.h"

//シーンの基礎クラス
class BaseScene {
private:
protected:
	SceneChanger* pSceneChanger;
public:
	BaseScene(SceneChanger * changer);
	virtual ~BaseScene() = 0;
	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};

#endif //!BASESCENE_H