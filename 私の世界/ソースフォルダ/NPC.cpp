#include "NPC.h"
#include <fstream>

NPC::NPC(MenuChanger * changer) :BaseMenu(changer){
}

NPC::~NPC() {
}

bool NPC::Initialize() {
	nowmap = 0;
	mode = 0;

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

	if (cursor.x + px / 32 < 0)
		cursor.x = -px / 32;
	if (cursor.y + py / 32 < 0)
		cursor.y = -py / 32;

	pngcursor = 0;
	isLoadpng = FALSE;
	isMakeHandle = FALSE;
	isNew = FALSE;
	newFlag = FALSE;
	gr = -1;

	selectFlagCursor = 0;

	newWarp = 0;
	warpSelect = 0;
	mode9 = 0;
	sentaku = 0;
	for (int i = 0; i != mapList.size() - 1; i++)
		warpList.push_back(mapList[i].name);

	Load(mapList[g_MainData.mapID-1].name.c_str());
	nowmap = g_MainData.mapID;

	select.push_back("グラフィック");
	select.push_back("名前");
	select.push_back("セリフ");
	select.push_back("フラグ入手後セリフ");
	select.push_back("Yes選択肢後セリフ");
	select.push_back("No選択肢後セリフ");
	select.push_back("くれるフラグ");
	select.push_back("必要なフラグ");
	select.push_back("会話ワープ");
	select.push_back("消えるフラグ");
	select.push_back("決定");
	return TRUE;
}

void NPC::Finalize() {

}

void NPC::Update() {
	if (nowmap != g_MainData.mapID) {
		nowmap = g_MainData.mapID;
		Load(mapList[g_MainData.mapID - 1].name.c_str());
	}

	if (mode == 0) {
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1) {
			pMenuChanger->Changemenu("スタート");
		}

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

		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			if (npc.size() == 0) {
				add = { cursor.x + px / 32,cursor.y + py / 32,"けつばん","・・・NBE","・・・NBE" ,{"はいNBE","いいえNBE"},-1,-1,{ 0 },"" ,-1 };
				strcpy_s(addflag.name, "フラグ名前");
				addNumber = 0;
				isNew = TRUE;
				mode = 100;
			}
			else
				for (int i = 0; i < npc.size() - 1; i++) {
					if (npc[i].x == cursor.x + px / 32 && npc[i].y == cursor.y + py / 32) {
						add = npc[i];
						if (add.giveFlag == -1)
							strcpy_s(addflag.name, "フラグ名前");
						else
							strcpy_s(addflag.name, flag[add.giveFlag].name);
						addNumber = i;
						isNew = FALSE;
						mode = 100;
						return;
					}
					else {
						add = { cursor.x+px/32,cursor.y+py/32,"けつばん","・・・NBE","・・・NBE" ,{ "はいNBE","いいえNBE" },-1,-1,{ 0 },"" ,-1 };
						addNumber = npc.size() - 1;
						strcpy_s(addflag.name, "フラグ名前");
						isNew = TRUE;
						mode = 100;
					}
				}
		}
	}
	else if (mode == 1) { //グラフィック
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			mode = 100;

		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5) {
			if (pngcursor != 0)
				pngcursor -= 1;
			else
				pngcursor = NPCgr.size() - 1;
			isLoadpng = FALSE;
		}
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5) {
			if (pngcursor != NPCgr.size() - 1)
				pngcursor += 1;
			else
				pngcursor = 0;
			isLoadpng = FALSE;
		}
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			add.gr = NPCgr[pngcursor];
			mode = 100;
		}
	}
	else if (mode == 2 || mode == 3 || mode == 4 || mode == 5 || mode == 6) { //名前.セリフ
		if (Key() == FALSE) {//キャンセル
			isMakeHandle = FALSE;
			mode = 100;
		}
		else if (Key() == TRUE) {//決定
			isMakeHandle = FALSE;
			if (mode == 2)
				GetKeyInputString(add.name, InputHandle);
			if (mode == 3)
				GetKeyInputString(add.serif, InputHandle);
			if (mode == 4)
				GetKeyInputString(add.changSerif, InputHandle);
			if(mode == 5)
				GetKeyInputString(add.seletserif[0], InputHandle);
			if(mode == 6)
				GetKeyInputString(add.seletserif[1], InputHandle);
			mode = 100;
		}
	}
	else if (mode == 7) {
		if (Key() == FALSE) {//キャンセル
			isMakeHandle = FALSE;
			mode = 100;
		}
		else if (Key() == TRUE) {//決定
			isMakeHandle = FALSE;
			if (add.giveFlag == -1) {
				GetKeyInputString(addflag.name, InputHandle);
				if (flag.size() == 0)
					flag.push_back(addflag);
				else if (add.giveFlag == -1)
					strcpy_s(flag[flag.size()-1].name, addflag.name);
				else
					strcpy_s(flag[add.giveFlag].name, addflag.name);
				add.giveFlag = flag.size()-1;
				newFlag = TRUE;
			}
			else {
				GetKeyInputString(flag[add.giveFlag].name, InputHandle);
				newFlag = FALSE;
			}
			mode = 100;
		}
	}
	else if (mode == 8) {
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			mode = 100;
		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1)
			if (selectFlagCursor != 0)
				selectFlagCursor--;
			else
				selectFlagCursor = Boxflag.size() - 2;
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1)
			if (selectFlagCursor != Boxflag.size() - 2)
				selectFlagCursor++;
			else
				selectFlagCursor = 0;
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			for (int i = 0; i < add.needFlag.size(); i++)
				if (add.needFlag[i] == selectFlagCursor) {
					add.needFlag.erase(add.needFlag.begin() + i);
					return;
				}
			add.needFlag.push_back(selectFlagCursor);
		}
	}
	else if (mode == 9) {
		if (newWarp == 0) {
			sentakusi.clear();
			if (warp.size() == 0) {
				addWarp = { addNumber,"",0,0 };
				sentakusi.push_back("追加");
				WarpAddNumber = 0;
				newWarp = 1;
			}
			else {
				/*for (int i = 0; i < warp.size() - 1; i++) {
					if (warp[i].npcNumber == addNumber) {
						addWarp = warp[i];
						sentakusi.push_back(warp[i].mapName);
						WarpAddNumber = i;
						newWarp = 2;
					}
					else */{
						addWarp = { addNumber, "", 0, 0 };
						WarpAddNumber = warp.size() - 1;
						newWarp = 1;
					}
					sentakusi.push_back("追加");
				//}
				if (newWarp == 2)
					for (int i = 0; i < warp.size() - 1; i++)
						if (strcmp(addWarp.mapName.c_str(), mapList[i].name.c_str()) == 0)
							warpSelect = i;
			}
		}
		if (mode9 == -1) {
			if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1)
				if (sentaku != 0)
					sentaku -= 1;
				else
					sentaku = sentakusi.size() - 1;
			if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1)
				if (sentaku != sentakusi.size() - 1)
					sentaku += 1;
				else
					sentaku = 0;
			if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
				mode = 100;
			if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
				/*if(sentaku == sentakusi.size())
					for (int i = 0; i < warp.size() - 1; i++)
						if (newWarp == 2 || sentakusi.size() - 1)
							if (sentakusi[sentaku] == warp[i].mapName && addNumber == warp[i].npcNumber && addWarp.x == warp[i].x && addWarp.y == warp[i].y)
								WarpAddNumber = i;*/
				mode9 = 0;
			}
		}
		else if (mode9 == 0) {
			if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1)
				if (warpSelect != 0)
					warpSelect -= 1;
				else
					warpSelect = warpList.size() - 1;
			if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1)
				if (warpSelect != warpList.size() - 1)
					warpSelect += 1;
				else
					warpSelect = 0;
			if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
				mode9 = -1;
			if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
				addWarp.mapName = mapList[warpSelect].name;
				fmf->Open(mapList[warpSelect].fmf.c_str(), mapList[warpSelect].png.c_str());
				
				DWORD wi = fmf->GetMapWidth();
				DWORD he = fmf->GetMapHeight();
				width = wi;
				height = he;

				mode9 = 1;
			}
		}
		else if (mode9 == 1) {
			if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5)
				if (addWarp.y - 1 >= 0)
					addWarp.y -= 1;
			if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5)
				if (addWarp.y + 1 < height)
					addWarp.y += 1;
			if (g_MainData.key[KEY_INPUT_RIGHT] == 1 || g_MainData.key[KEY_INPUT_RIGHT] >= 10 || g_MainData.pad[INPUT_RIGHT] == 1 || g_MainData.pad[INPUT_RIGHT] >= 5)
				if (addWarp.x + 1 < width)
					addWarp.x += 1;
			if (g_MainData.key[KEY_INPUT_LEFT] == 1 || g_MainData.key[KEY_INPUT_LEFT] >= 10 || g_MainData.pad[INPUT_LEFT] == 1 || g_MainData.pad[INPUT_LEFT] >= 5)
				if (addWarp.x - 1 >= 0)
					addWarp.x -= 1;

			if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {

				if (warp.size() == 0)
					warp.push_back(addWarp);
				else
					warp[WarpAddNumber] = addWarp;
				
				mode = 100;
				mode9 = -1;
				
				fmf->Open(mapList[g_MainData.mapID - 1].fmf.c_str(), mapList[g_MainData.mapID - 1].png.c_str());
				DWORD wi = fmf->GetMapWidth();
				DWORD he = fmf->GetMapHeight();
				width = wi;
				height = he;
			}
			if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
				mode9 = 0;
		}
	}
	else if (mode == 10) {
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			mode = 100;
		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1)
			if (add.walkFlag != 0)
				add.walkFlag--;
			else
				add.walkFlag = Boxflag.size() - 2;
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1)
			if (add.walkFlag != Boxflag.size() - 2)
				add.walkFlag++;
			else
				add.walkFlag = 0;
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) 
			mode = 100;
	}
	else if (mode == 11) {//決定セーブ
		add.npcNumber = addNumber;
		if (npc.size() == 0)
			npc.push_back(add);
		else
			npc[addNumber] = add;
		Save();
		mode = 0;
		pMenuChanger->Changemenu("スタート");
	}
	else if (mode == 100) {	//どれ編集
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			mode = 0;

		if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5) {
			if (selectCursor != 0)
				selectCursor -= 1;
			else
				selectCursor = select.size() - 1;
		}
		if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5) {
			if (selectCursor != select.size() - 1)
				selectCursor += 1;
			else
				selectCursor = 0;
		}
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1)
			mode = selectCursor + 1;
	}
}

void NPC::Draw() {
	for (int i = 1; i < width; i++) {
		DrawLine(i * 32 - g_MainData.px, -g_MainData.py, i * 32 - g_MainData.px, width * 32 - g_MainData.py, Black);
	}
	for (int i = 1; i < height; i++) {
		DrawLine(-g_MainData.px, i * 32 - g_MainData.py, height * 32 - g_MainData.px, i * 32 - g_MainData.py, Black);
	}
	DrawBox(cursor.x * 32 + 0 - px % 32, cursor.y * 32 + 0 - py % 32, cursor.x * 32 + 32 - px % 32, cursor.y * 32 + 32 - py % 32, GetColor(255, 255, 0), FALSE);
	DrawBox(cursor.x * 32 + 1 - px % 32, cursor.y * 32 + 1 - py % 32, cursor.x * 32 + 31 - px % 32, cursor.y * 32 + 31 - py % 32, GetColor(255, 255, 0), FALSE);
	if (mode == 1) {
		DrawBox1(50, 50, 1, NPCgr, pngcursor);
		if (isLoadpng == FALSE) {
			char grname[100];
			sprintf_s(grname, "data/character/%s", NPCgr[pngcursor].c_str());
			gr = LoadGraph(grname);
			isLoadpng = TRUE;
		}
		DrawGraph(300, 50, gr, FALSE);
	}
	else if (mode == 2 || mode == 3 || mode == 4 || mode == 5 || mode == 6 || mode == 7) {
		DrawBox(0, 65, 640, 480, Black1, TRUE);
		DrawKeyInputString(0, 65, InputHandle);
		DrawKeyInputModeString(600, 465);
		if(mode == 2)
			DrawString(200, 50, "NPCネームを入力してください", Black);
		if (mode == 3 || mode == 4 || mode == 5 || mode == 6) {
			DrawString(200, 50, "セリフを入力してください", Black);
			DrawString(0, 435, "　", Black);
			DrawString(0, 450, "フラグ入手:F 改行:N ボタン待ち:NB 文の終わり:NBE 選択肢開始:NBS(NBE不要)", Black);
		}
		if (mode == 7)
			DrawString(200, 50, "くれるフラグ名前を入力してください",Black);
		DrawString(0, 465, "決定はエンターキー キャンセルはEscキー", Black);
	}
	else if (mode == 8) {
		DrawBox1(50, 0, 1, Boxflag, selectFlagCursor);
		int j = 0;
		for (int i = 0; i < add.needFlag.size(); i++) {
			DrawFormatString(0, i * 20, Black, "%d　%d", add.needFlag[i],selectFlagCursor);
			j = add.needFlag[i];
			DrawBox(60, 20 * j + 7, 100, 20 * (j + 1) + 7, GetColor(255, 0, 0), FALSE);
		}
	}
	else if (mode == 9) {
		if (mode9 == -1)
			DrawBox1(50, 50, 1, sentakusi, sentaku);
		else if(mode9 == 0)
			DrawBox1(50, 50, 1, warpList, warpSelect);
		else if (mode9 == 1) {
			fmf->Draw(addWarp.x * 32, addWarp.y * 32);
			for (int i = 1; i < width; i++) {
				DrawLine(i * 32, 0, i * 32, 480, Black);
			}
			for (int i = 1; i < height; i++) {
				DrawLine(0, i * 32, 640, i * 32, Black);
			}
			DrawBox(0, 0, 32, 32, GetColor(255, 255, 0), FALSE);
			DrawBox(0 + 1, 0 + 1, 31, 31, GetColor(255, 255, 0), FALSE);

		}
	}
	else if (mode == 10) {
		DrawBox1(50, 0, 1, Boxflag, add.walkFlag);
	}
	else if (mode == 100)
		DrawBox1(50, 50, 1, select, selectCursor);
}

void NPC::Save() {
	char fname[50];
	sprintf_s(fname, "data/npc/%s.txt", mapList[g_MainData.mapID - 1].name.c_str());
	fstream fs;
	fs.open(fname, ios::out);
	if (!fs)
		fs.open(fname, ios::out);
	int size = 0;
	if (npc.size() >= 2 && isNew == FALSE)
		size = npc.size() - 1;
	else if (npc.size() >= 2 && isNew == TRUE)
		size = npc.size();
	else
		size = 1;
	for (int i = 0; i < size; i++) {
		fs << npc[i].x << endl;
		fs << npc[i].y << endl;
		fs << npc[i].name << endl;
		fs << npc[i].serif << endl;
		fs << npc[i].changSerif << endl;
		fs << npc[i].seletserif[0] << endl;
		fs << npc[i].seletserif[1] << endl;
		fs << npc[i].walkFlag << endl;
		fs << npc[i].giveFlag << endl;
		fs << npc[i].needFlag.size() << endl;
		for(int j = 0; j < npc[i].needFlag.size(); j++)
			fs << npc[i].needFlag[j] << endl;
		fs << npc[i].gr << endl;
		fs << npc[i].npcNumber << endl;
	}
	fs.close();
	printfDx("%s保存完了\n",fname);

	sprintf_s(fname, "data/flag/flaglist.txt");
	fs.open(fname, ios::out);
	if (flag.size() >= 2 && newFlag == FALSE)
		size = flag.size() - 1;
	else if (flag.size() >= 2 && newFlag == TRUE)
		size = flag.size();
	else
		size = 1;
	for (int j = 0; j < size; j++) {
		fs << flag[j].name << endl;
	}
	fs.close();

	if (newWarp > 0) {
		sprintf_s(fname, "data/event/WarpList%s.txt", mapList[g_MainData.mapID - 1].name.c_str());
		fs.open(fname, ios::out);
		if (warp.size() >= 2 && newWarp == 2)
			size = warp.size() - 1;
		else if (warp.size() >= 2 && newWarp == 1)
			size = warp.size();
		else
			size = 1;
		for (int z = 0; z < size; z++) {
			fs << warp[z].npcNumber << endl;
			fs << warp[z].mapName << endl;
			fs << warp[z].x << endl;
			fs << warp[z].y << endl;
		}
		fs.close();
	}
}

void NPC::Load(const char *name) {
	NPCgr = getFileName("data/character", "png");

	char fname[50];
	sprintf_s(fname, "data/npc/%s.txt", name);
	fstream fs;
	fs.open(fname, ios::in);
	if (!fs)return;

	char xbuf[256];
	char ybuf[256];
	char namebuf[1024];
	char serifbuf[1024];
	char changSerifbuf[1024];
	char selectSerifbuf[2][1024];
	char walkFlagbuf[256];
	char giveFlagbuf[256];
	char needFlagNum[256];
	char needFlag[256][256];
	char grbuf[256];
	char npcNumberbuf[256];
	npc.clear();
	int i = 0;

	while (!fs.eof()) {
		fs.getline(xbuf, 256);
		fs.getline(ybuf, 256);
		fs.getline(namebuf, 1024);
		fs.getline(serifbuf, 1024);
		fs.getline(changSerifbuf, 1024);
		fs.getline(selectSerifbuf[0], 1024);
		fs.getline(selectSerifbuf[1], 1024);
		fs.getline(walkFlagbuf, 256);
		fs.getline(giveFlagbuf, 256);
		fs.getline(needFlagNum, 256);
		for (int i = 0; i < atoi(needFlagNum); i++)
			fs.getline(needFlag[i], 256);
		fs.getline(grbuf, 256);
		fs.getline(npcNumberbuf, 256);

		t_npc a;
		a.x = atoi(xbuf);
		a.y = atoi(ybuf);
		strcpy_s(a.name, namebuf);
		strcpy_s(a.serif, serifbuf);
		strcpy_s(a.changSerif, changSerifbuf);
		strcpy_s(a.seletserif[0], selectSerifbuf[0]);
		strcpy_s(a.seletserif[1], selectSerifbuf[1]);
		a.walkFlag = atoi(walkFlagbuf);
		a.giveFlag = atoi(giveFlagbuf);
		for (int i = 0; i < atoi(needFlagNum); i++)
			a.needFlag.push_back(atoi(needFlag[i]));
		a.gr = grbuf;
		a.npcNumber = atoi(npcNumberbuf);
		npc.push_back(a);
		i++;
	}

	fs.close();

	sprintf_s(fname, "data/flag/flaglist.txt");
	fs.open(fname, ios::in);
	if (!fs)return;
	i = 0;
	char buf[1024];
	flag.clear();
	Boxflag.clear();
	while (!fs.eof()) {
		fs.getline(buf, 1024);
		t_flag b;
		strcpy_s(b.name, buf);
		flag.push_back(b);
		std::string a = buf;
		Boxflag.push_back(a);
	}
	fs.close();

	sprintf_s(fname, "data/event/WarpList%s.txt", mapList[g_MainData.mapID-1].name.c_str());
	fs.open(fname, ios::in);
	if (!fs)return;
	char wxbuf[256];
	char wybuf[256];
	char wNumbuf[256];
	char wmapnamebuf[256];
	warp.clear();
	while (!fs.eof()) {
		t_Warp w;
		fs.getline(wNumbuf, 256);
		w.npcNumber = atoi(wNumbuf);
		fs.getline(wmapnamebuf, 256);
		w.mapName = wmapnamebuf;
		fs.getline(wxbuf, 256);
		w.x = atoi(wxbuf);
		fs.getline(wybuf, 256);
		w.y = atoi(wybuf);
		warp.push_back(w);
	}
	fs.close();
}

int NPC::Key() {
	if (isMakeHandle == FALSE) {
		InputHandle = MakeKeyInput(1024, TRUE, FALSE, FALSE);
		SetActiveKeyInput(InputHandle);
		if(mode == 2)
			SetKeyInputString(add.name, InputHandle);
		if (mode == 3)
			SetKeyInputString(add.serif, InputHandle);
		if(mode == 4)
			SetKeyInputString(add.changSerif, InputHandle);
		if(mode == 5)
			SetKeyInputString(add.seletserif[0], InputHandle);
		if (mode == 6)
			SetKeyInputString(add.seletserif[1], InputHandle);
		if (mode == 7) 
			SetKeyInputString(addflag.name, InputHandle);
		isMakeHandle = TRUE;
	}
	if (CheckKeyInput(InputHandle) == 1)
		return TRUE;
	else if (CheckKeyInput(InputHandle) == 2)
		return FALSE;

	return -1;
}
