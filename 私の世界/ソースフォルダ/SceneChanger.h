#ifndef SCENECHANGER_H
#define SCENECHANGER_H

typedef enum {
	TITLE, //�^�C�g�����
	MAP, //����

}eScene;

class SceneChanger {
public:
	virtual ~SceneChanger();
	virtual void ChangeScene(eScene NextScene);
};

#endif //!SCENECHANGER_H