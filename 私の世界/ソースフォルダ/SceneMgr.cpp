#include "SceneMgr.h"
#include "Title.h"
#include "Map.h"

SceneMgr::SceneMgr() {
	pScene = (BaseScene*)new Title(this);
}

SceneMgr::~SceneMgr() {
	Finalize();
}
void SceneMgr::Initialize() {
	pScene->Initialize();
}

void SceneMgr::Finalize() {
	pScene->Finalize();
}

void SceneMgr::Update() {
	pScene->Update();
}

void SceneMgr::Draw() {
	pScene->Draw();
}

void SceneMgr::ChangeScene(eScene scene) {
	pScene->Finalize();
	delete pScene;

	switch(scene) {
	case TITLE:
		pScene = (BaseScene*) new Title(this);
		break;
	case MAP:
		pScene = (BaseScene*) new Map(this);
		break;
	}
	pScene->Initialize();
}