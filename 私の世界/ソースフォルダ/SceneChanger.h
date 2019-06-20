#ifndef SCENECHANGER_H
#define SCENECHANGER_H

typedef enum {
	TITLE, //ƒ^ƒCƒgƒ‹‰æ–Ê
	MAP, //’¬’†

}eScene;

class SceneChanger {
public:
	virtual ~SceneChanger();
	virtual void ChangeScene(eScene NextScene);
};

#endif //!SCENECHANGER_H