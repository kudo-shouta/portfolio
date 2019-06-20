#ifndef SCENECHANGER_H
#define SCENECHANGER_H

typedef enum {
	TITLE, //タイトル画面
	MAP, //町中

}eScene;

class SceneChanger {
public:
	virtual ~SceneChanger();
	virtual void ChangeScene(eScene NextScene);
};

#endif //!SCENECHANGER_H