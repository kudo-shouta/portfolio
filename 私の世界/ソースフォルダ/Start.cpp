#include "main.h"
#include "Start.h"

Start::Start(MenuChanger* changer) : BaseMenu(changer) {
}

Start::~Start() {
}

bool Start::Initialize() {
	cursor = 0;
	select.push_back("ƒ}ƒbƒv"); select.push_back("ˆÚ“®Ý’u");
	select.push_back("‚m‚o‚b");

	return TRUE;
}

void Start::Finalize() {
	select.clear();
}

void Start::Update() {
	if(g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1) {
		if(cursor - 2 >= 0)
			cursor -= 2;
		else if (select.size() % 2 == 0)
			cursor = select.size() - 2 + cursor;
		else if (select.size() % 2 != 0)
			cursor = select.size() - 1  - cursor;
	}
	if(g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1) {
		if(cursor + 2 < select.size())
			cursor += 2;
		else
			cursor = cursor % 2;
	}
	if(g_MainData.key[KEY_INPUT_LEFT] == 1 || g_MainData.pad[INPUT_LEFT] == 1) {
		if(cursor % 2 != 0 && cursor + 1 < select.size())
			cursor -= 1;
		else if (cursor + 1 < select.size()) {
			cursor += 1;
		}
		
	}
	if(g_MainData.key[KEY_INPUT_RIGHT] == 1 || g_MainData.pad[INPUT_RIGHT] == 1) {
		if(cursor % 2 == 0 && cursor + 1 < select.size())
			cursor += 1;
		else if(cursor + 1 < select.size())
			cursor -= 1;
	}

	if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
		if(cursor == 0)
			pMenuChanger->Changemenu("ƒ}ƒbƒv");
		if (cursor == 1)
			pMenuChanger->Changemenu("ˆÚ“®Ý’u");
		if (cursor == 2)
			pMenuChanger->Changemenu("‚m‚o‚b");
	}

	if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
		pMenuChanger->Changemenu("‚Æ‚¶‚é");
}

void Start::Draw() {
	DrawBox1(50, 50, 2, select, cursor);
}