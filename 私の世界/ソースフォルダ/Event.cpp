#include "Event.h"
#include "main.h"
#include <fstream>
#include "BaseMenu.h"

Event::Event(MenuChanger *changer) :BaseMenu(changer){
}

Event::~Event() {
}

bool Event::Initialize(){
	if (g_MainData.mapID == 0) {
		DrawString(300, 200, "マップ先に開け", Black);
		WaitTimer(1500);
		pMenuChanger->Changemenu("スタート");
		return false;
	}

	Load(mapList[g_MainData.mapID - 1].name.c_str());

	fmf = new CFmfMap();
	fmf->Open(mapList[g_MainData.mapID - 1].fmf.c_str(), mapList[g_MainData.mapID - 1].png.c_str());
	DWORD wi = fmf->GetMapWidth();
	DWORD he = fmf->GetMapHeight();

	width = wi;
	height = he;

	px = g_MainData.px;
	py = g_MainData.py;

	cursor.x = 0;
	cursor.y = 0;
	connectcursor.x = 0;
	connectcursor.y = 0;
	fin = 0;

	if (cursor.x + px / 32 < 0)
		cursor.x = -px / 32;
	if (cursor.y + py / 32 < 0)
		cursor.y = -py / 32;

	for (int i = 0; i != mapList.size() - 1; i++)
		selectmap.push_back(mapList[i].name);
	passege.push_back("一方通行");
	passege.push_back("相互通行");

	MenuFlag = 0;

	movecursor = 0;
	isLoad = FALSE;
	
	return false;
}

void Event::Finalize(){
	delete fmf;
}

void Event::Update(){
	if (MenuFlag == 0) {
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			pMenuChanger->Changemenu("スタート");

		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5)
			if (cursor.y * 32 + 0 - py % 32 >= 0 + 32 && cursor.y + py / 32 > 0)
				cursor.y -= 1;
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5)
			if (cursor.y * 32 + 32 - py % 32 <= 480 - 32 && cursor.y + py / 32 + 1 < height)
				cursor.y += 1;
		if (g_MainData.key[KEY_INPUT_RIGHT] == 1 || g_MainData.key[KEY_INPUT_RIGHT] >= 10 || g_MainData.pad[INPUT_RIGHT] == 1 || g_MainData.pad[INPUT_RIGHT] >= 5)
			if (cursor.x * 32 + 32 - px % 32 <= 640 - 32 && cursor.x + px / 32 + 1 < width)
				cursor.x += 1;
		if (g_MainData.key[KEY_INPUT_LEFT] == 1 || g_MainData.key[KEY_INPUT_LEFT] >= 10 || g_MainData.pad[INPUT_LEFT] == 1 || g_MainData.pad[INPUT_LEFT] >= 5)
			if (cursor.x * 32 + 0 - px % 32 >= 0 + 32 && cursor.x + px / 32 > 0)
				cursor.x -= 1;

		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1)
			MenuFlag = 1;
	}

	else if (MenuFlag == 1) {
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1) {
			MenuFlag = 0;

			fmf->Open(mapList[g_MainData.mapID - 1].fmf.c_str(), mapList[g_MainData.mapID - 1].png.c_str());
			DWORD wi = fmf->GetMapWidth();
			DWORD he = fmf->GetMapHeight();

			width = wi;
			height = he;

			Load(mapList[g_MainData.mapID - 1].name.c_str());
		}

		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5) {
			if (movecursor == 0)
				movecursor = selectmap.size() - 1;
			else
				movecursor -= 1;
			isLoad = FALSE;
		}
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5) {
			if (movecursor + 1 > selectmap.size() - 1)
				movecursor = 0;
			else
				movecursor += 1;
			isLoad = FALSE;
		}

		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			width = (int)fmf->GetMapWidth();
			height = (int)fmf->GetMapHeight();

			Load(mapList[movecursor].name.c_str());

			MenuFlag = 2;
		}
	}

	else if (MenuFlag == 2) {

		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5)
			if (connectcursor.y - 1 >= 0)
				connectcursor.y -= 1;
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5)
			if (connectcursor.y + 1 < height)
				connectcursor.y += 1;
		if (g_MainData.key[KEY_INPUT_RIGHT] == 1 || g_MainData.key[KEY_INPUT_RIGHT] >= 10 || g_MainData.pad[INPUT_RIGHT] == 1 || g_MainData.pad[INPUT_RIGHT] >= 5)
			if (connectcursor.x + 1 < width)
				connectcursor.x += 1;
		if (g_MainData.key[KEY_INPUT_LEFT] == 1 || g_MainData.key[KEY_INPUT_LEFT] >= 10 || g_MainData.pad[INPUT_LEFT] == 1 || g_MainData.pad[INPUT_LEFT] >= 5)
			if (connectcursor.x - 1 >= 0)
				connectcursor.x -= 1;

		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1)
			MenuFlag = 3;
	
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			MenuFlag = 1;
	}

	else if (MenuFlag == 3) {
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			MenuFlag = 2;

		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5) {
			if (fin == 0)
				fin = 1;
			else
				fin = 0;
		}
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5) {
			if (fin == 1)
				fin = 0;
			else
				fin = 1;
		}

		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			t_event add = { cursor.x+px/32, cursor.y+py/32, movecursor+1, connectcursor.x, connectcursor.y };
			event.push_back(add);
			Save();
			pMenuChanger->Changemenu("スタート");
		}
	}
}

void Event::Draw(){

	for (int i = 1; i < width; i++) {
		DrawLine(i * 32 - g_MainData.px, -g_MainData.py, i * 32 - g_MainData.px, width * 32 - g_MainData.py, Black);
	}
	for (int i = 1; i < height; i++) {
		DrawLine(-g_MainData.px, i * 32 - g_MainData.py, height * 32 - g_MainData.px, i * 32 - g_MainData.py, Black);
	}
	DrawBox(cursor.x*32 + 0 -px%32,cursor.y*32 + 0-py%32,cursor.x*32 + 32 - px % 32,cursor.y*32 + 32 - py % 32, GetColor(255, 255, 0),FALSE);
	DrawBox(cursor.x * 32 + 1 -px % 32, cursor.y * 32 + 1 - py % 32, cursor.x * 32 + 31 - px % 32, cursor.y * 32 + 31 - py % 32, GetColor(255, 255, 0), FALSE);

	if (MenuFlag == 1) {
		SelectEvent();
		if (isLoad == FALSE) {	//カーソル移動のたびにマップをリロードしプレビュー表示
			fmf->Open(mapList[movecursor].fmf.c_str(), mapList[movecursor].png.c_str());
			isLoad = TRUE;
		}
		fmf->Preview();
	}

	if (MenuFlag == 2 || MenuFlag == 3) {
		DrawBox(0, 0, 640, 480, Black1, TRUE);
		fmf->Draw(connectcursor.x*32, connectcursor.y*32);
		for (int i = 1; i < width; i++) {
			DrawLine(i * 32, 0, i * 32, 480, Black);
		}
		for (int i = 1; i < height; i++) {
			DrawLine(0, i * 32, 640, i * 32, Black);
		}
		DrawBox(0, 0, 32, 32, GetColor(255, 255, 0), FALSE);
		DrawBox(0 + 1, 0 + 1, 31, 31, GetColor(255, 255, 0), FALSE);

		int size = event.size();
		int i = 0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		while (i < size - 1) {
			DrawBox(-connectcursor.x*32 + event[i].x * 32, -connectcursor.y * 32 + event[i].y * 32, -connectcursor.x * 32 + event[i].x * 32 + 32, -connectcursor.y * 32 + event[i].y * 32 + 32, GetColor(255, 255, 0), TRUE);
			i++;
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (MenuFlag == 3) {
		DrawBox1(50, 50, 1, passege, fin);
	}
	if (MenuFlag == 0) {
		int size = event.size();
		int i = 0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
		while (i < size - 1) {
			DrawBox(-px + event[i].x * 32, -py + event[i].y * 32, -px + event[i].x * 32 + 32, -py + event[i].y * 32 + 32, GetColor(255, 255, 0), TRUE);
			i++;
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}

void Event::Load(const char *name){
	event.clear();
	char fname[50];
	sprintf_s(fname, "data/event/%s.txt", name);
	fstream fs;
	fs.open(fname, ios::in);
	if (!fs)return;

	char xbuf[256];
	char ybuf[256];
	char mapIDbuf[256];
	char xxbuf[256];
	char yybuf[256];
	int i = 0;

	while (!fs.eof()) {
		fs.getline(xbuf, 256);
		fs.getline(ybuf, 256);
		fs.getline(mapIDbuf, 256);
		fs.getline(xxbuf, 256);
		fs.getline(yybuf, 256);
		t_event add = { atoi(xbuf), atoi(ybuf), atoi(mapIDbuf), atoi(xxbuf), atoi(yybuf) };
		event.push_back(add);
		i++;
	}

	fs.close();
}

void Event::Save(){
	char fname[50];
	sprintf_s(fname, "data/event/%s.txt", mapList[g_MainData.mapID - 1].name.c_str());
	fstream fs;
	fs.open(fname, ios::out | ios::app);
	if (!fs)
		fs.open(fname, ios::out);
	fs << event[event.size() - 1].x << endl;
	fs << event[event.size() - 1].y << endl;
	fs << event[event.size() - 1].mapid << endl;
	fs << event[event.size() - 1].xx << endl;
	fs << event[event.size() - 1].yy << endl;
	fs.close();

	if (fin == 1) {
		fstream fs2;
		sprintf_s(fname, "data/event/%s.txt", mapList[movecursor].name.c_str());
		fs2.open(fname, ios::out || ios::app);
		if (!fs2) {
			fs2.open(fname, ios::out);
		}
		fs2 << event[event.size() - 1].xx << endl;
		fs2 << event[event.size() - 1].yy << endl;
		fs2 << g_MainData.mapID << endl;
		fs2 << event[event.size() - 1].x << endl;
		fs2 << event[event.size() - 1].y << endl;
		fs2.close();
	}
	printfDx("%s保存完了\n", fname);
}

void Event::SelectEvent() {
	DrawBox1(50, 50, 1, selectmap, movecursor);
}

void Event::MapMove(){

}