#ifndef TITLE_H
#define TITLE_H

#include "BaseScene.h"

//タイトル管理クラス
class Title:public BaseScene {
private:
public:
	Title(SceneChanger * changer);
	~Title();
	bool Initialize();
	void Finalize();
	void Update();
	void Draw();
};

#endif //!TITLE_H