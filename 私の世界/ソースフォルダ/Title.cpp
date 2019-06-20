#include "main.h"
#include "Title.h"

Title::Title(SceneChanger* changer):BaseScene(changer){
}

Title::~Title(){
}

bool Title::Initialize(){
	return false;
}

void Title::Finalize(){
}

void Title::Update() {
	if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
		pSceneChanger->ChangeScene(MAP);
	}
}

void Title::Draw() {
	static int gr = LoadGraph("data/ƒ^ƒCƒgƒ‹.png");
	static int i = 1;
	static int a = 10;
	DrawExtendGraph(0, 0, 640, 480, gr, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ADD, i);
	DrawString(450, 230, "Click to Strat", Black);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	i += a;
	if (i > 255)
		a = -a;
	if (i < 0)
		a = -a;
}