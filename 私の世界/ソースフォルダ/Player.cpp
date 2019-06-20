#include "Player.h"
#include <fstream>

Player::Player(CFmfMap *fmf) {
	Initialize();
	fmfMap = fmf;
}

bool Player::Initialize(){
	oneLoad = FALSE;

	LoadDivGraph("data/character/勇者2.png", 3 * 4, 3, 4, 20, 28, PlayerGr);
	LoadDivGraph("data/character/Player.png", 3 * 4, 3, 4, 32, 32, PlayerGr2);
	Anime = 1, AnimeDire = 1;
	Dire = 0;
	FrameCount = 0;
	px = 336;
	py = 408;
	speed = 2;

	TalkMan = -1;
	NowisTalk = FALSE;
	Ncount = 0;
	mojisize = 17;
	nowMoji = 0;
	TalkMode = 0;
	TalkOn = FALSE;
	selectStart = FALSE;
	YesNo = {"はい","いいえ"};
	cursor = 0;

	LoadEvent();
	LoadNPC();

	nowmap = 0;

	chang = FALSE;

	for (int i = 0; i < 100; i++)
		haveFlag[i] = FALSE;

	end = FALSE;
	Endscene = 1;

	return TRUE;
}

void Player::Finalize(){
}

void Player::Update(bool menuisOpen){

	if (haveFlag[1] == TRUE && end == FALSE && NowisTalk == FALSE)
		end = TRUE;

	if (nowmap != g_MainData.mapID) {
		nowmap = g_MainData.mapID;
		LoadEvent();
		LoadNPC();
	}
	if(chang == FALSE && g_MainData.mapID == 5)
		chang = TRUE;
	FrameCount++;
	if (FrameCount == 10) {	//10フレームごとに歩行
		FrameCount = 0;
		if (Anime == 1) {
			Anime += AnimeDire;		//直立→右足前→直立→左足前→直立　を実装
			AnimeDire = -AnimeDire;
		}
		else
			Anime = 1;
	}
	if (menuisOpen == FALSE && NowisTalk == FALSE && end == FALSE) {
		int chei = fmfMap->GetChipHeight() * 2;
		if (g_MainData.key[KEY_INPUT_LCONTROL] > 0)
			speed = 8;
		else
			speed = 4;
		
		

		if (g_MainData.key[KEY_INPUT_UP] >= 1) {
			Dire = 3;
			for (int i = 0; i < npc.size(); i++)
				if(npc[i].walkFlag == -1 || (npc[i].walkFlag != -1 && haveFlag[npc[i].walkFlag] == FALSE))
					if (npc[i].x * 32 - (640 / 2 - 32 / 2) < px + chei - 1 && px < npc[i].x * 32 + chei - 1 - (640 / 2 - 32 / 2) &&
						npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py - speed && npc[i].y * 32 - (480 / 2 - 32 / 2) < py)
						return;
			if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py - speed) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1) / 32, (480 / 2 - chei / 2 + py - speed) / chei) != 2049 &&
				(480 / 2 - chei / 2 + py - speed) > 0) {
				py -= speed;
				g_MainData.py = py;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py - speed) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1) / 32, (480 / 2 - chei / 2 + py - speed) / chei) == 2049 &&
				(480 / 2 - chei / 2 + py - speed) > 0) {
				px -= speed;
				Dire = 1;
				g_MainData.px = px;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py - speed) / chei) == 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1) / 32, (480 / 2 - chei / 2 + py - speed) / chei) != 2049 &&
				(480 / 2 - chei / 2 + py - speed) > 0) {
				px += speed;
				Dire = 2;
				g_MainData.px = px;
				HitEvent();
			}
		}
		else if (g_MainData.key[KEY_INPUT_DOWN] >= 1) {
			Dire = 0;
			for (int i = 0; i < npc.size(); i++)
				if (npc[i].walkFlag == -1 || (npc[i].walkFlag != -1 && haveFlag[npc[i].walkFlag] == FALSE))
					if (npc[i].x * 32 - (640 / 2 - 32 / 2) < px + chei - 1 && px < npc[i].x * 32 + chei - 1 - (640 / 2 - 32 / 2) &&
						npc[i].y * 32 - (480 / 2 - 32 / 2) < py + chei + speed && npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py + chei)
						return;
			if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1) / 32, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != -1) {
				py += speed;
				g_MainData.py = py;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1) / 32, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) == 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != -1) {
				px -= speed;
				Dire = 1;
				g_MainData.px = px;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) == 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1) / 32, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px) / chei, (480 / 2 - chei / 2 + py + chei - 1 + speed) / chei) != -1) {
				px += speed;
				Dire = 2;
				g_MainData.px = px;
				HitEvent();
			}
		}
		else if (g_MainData.key[KEY_INPUT_LEFT] >= 1) {
			Dire = 1;
			for (int i = 0; i < npc.size(); i++)
				if (npc[i].walkFlag == -1 || (npc[i].walkFlag != -1 && haveFlag[npc[i].walkFlag] == FALSE))
					if (npc[i].x * 32 + chei - (640 / 2 - 32 / 2) > px - speed && px > npc[i].x * 32 - (640 / 2 - 32 / 2) &&
						npc[i].y * 32 - (480 / 2 - 32 / 2) < py + chei && npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py)
						return;
			if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px - speed) / chei, (480 / 2 - chei / 2 + py) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px - speed) / chei, (480 / 2 - chei / 2 + py + chei - 1) / chei) != 2049 &&
				(640 / 2 - chei / 2 + px - speed) > 0) {
				px -= speed;
				g_MainData.px = px;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px - speed) / chei, (480 / 2 - chei / 2 + py) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px - speed) / chei, (480 / 2 - chei / 2 + py + chei - 1) / chei) == 2049 &&
				(640 / 2 - chei / 2 + px - speed) > 0) {
				py -= speed;
				Dire = 3;
				g_MainData.py = py;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px - speed) / chei, (480 / 2 - chei / 2 + py) / chei) == 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px - speed) / chei, (480 / 2 - chei / 2 + py + chei - 1) / chei) != 2049 &&
				(640 / 2 - chei / 2 + px - speed) > 0) {
				py += speed;
				Dire = 0;
				g_MainData.py = py;
				HitEvent();
			}
		}
		else if (g_MainData.key[KEY_INPUT_RIGHT] >= 1) {
			Dire = 2;
			for (int i = 0; i < npc.size(); i++)
				if (npc[i].walkFlag == -1 || (npc[i].walkFlag != -1 && haveFlag[npc[i].walkFlag] == FALSE))
					if (npc[i].x * 32 - (640 / 2 - 32 / 2) < px + chei + speed && px + chei < npc[i].x * 32 + chei - (640 / 2 - 32 / 2) &&
						npc[i].y * 32 - (480 / 2 - 32 / 2) < py + chei && npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py)
						return;
			if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py + chei - 1) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py) / chei) != -1) {
				px += speed;
				g_MainData.px = px;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei + speed) / chei, (480 / 2 - chei / 2 + py) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py + chei - 1) / chei) == 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py) / chei) != -1) {
				py -= speed;
				Dire = 3;
				g_MainData.py = py;
				HitEvent();
			}
			else if (fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py) / chei) == 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py + chei - 1) / chei) != 2049 &&
				fmfMap->GetValue(fmfMap->GetLayerCount() - 1, (640 / 2 - chei / 2 + px + chei - 1 + speed) / chei, (480 / 2 - chei / 2 + py) / chei) != -1) {
				py += speed;
				Dire = 0;
				g_MainData.py = py;
				HitEvent();
			}
		}
		else if(g_MainData.key[KEY_INPUT_RETURN] == 1)
			isTalk();
		else if (g_MainData.key[KEY_INPUT_R] >= 1) {
			LoadEvent();
			LoadNPC();
		}
		else if(g_MainData.key[KEY_INPUT_C] >= 1)
			clsDx();
	}
	DWORD x = 5, y = 5;
}

void Player::Draw(){
	if (end == TRUE)
		Ending();
	if(chang == TRUE)
		DrawExtendGraph(640 / 2 - 32 / 2, 480 / 2 - 32 / 2, 640 / 2 - 32 / 2 + 32, 480 / 2 - 32 / 2 + 32, PlayerGr2[Dire * 3 + Anime], TRUE);
	else
		DrawExtendGraph(640/2 - 32/2, 480/2 - 32/2, 640 / 2 - 32 / 2 + 31, 480 / 2 - 32 / 2 + 31, PlayerGr[Dire*3 + Anime], TRUE);
	for (int i = 0; i < npc.size(); i++)
		if(npc[i].walkFlag == -1)
			DrawExtendGraph(npc[i].x * 32 - px, npc[i].y * 32 - py, npc[i].x * 32 - px + 31, npc[i].y * 32 - py +31, NPCgr[i][0 + Anime], TRUE);
		else if(haveFlag[npc[i].walkFlag] == FALSE)
			DrawExtendGraph(npc[i].x * 32 - px, npc[i].y * 32 - py, npc[i].x * 32 - px + 31, npc[i].y * 32 - py + 31, NPCgr[i][0 + Anime], TRUE);

	if (NowisTalk == TRUE) {
		if (TalkOn == FALSE) {
			WarpSkip = 0;
			for (int i = 0; i < 1024; i++)
				serifBuf[i] = '\0';
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 1024; j++)
					buf[i][j] = '\0';
			if (TalkMode == 0)
				strcpy_s(serifBuf, npc[TalkMan].serif);
			if (TalkMode == 1)
				strcpy_s(serifBuf, npc[TalkMan].seletserif[0]);
			if (TalkMode == 2)
				strcpy_s(serifBuf, npc[TalkMan].seletserif[1]);
			for (int i = 0; i < npc[TalkMan].needFlag.size(); i++)
				if (TalkMode == 0 && haveFlag[npc[TalkMan].needFlag[i]] == TRUE) {
					strcpy_s(serifBuf, npc[TalkMan].changSerif);
				}
				else if (TalkMode == 0 && haveFlag[npc[TalkMan].needFlag[i]] == FALSE) {
					strcpy_s(serifBuf, npc[TalkMan].serif);
					i = npc[TalkMan].needFlag.size();
				}
			TalkOn = TRUE;
		}
		if (selectStart == TRUE) {
			DrawBox(0, 480 - mojisize * 4 - 2, 640, 480, Black, TRUE);
			DrawBox(0, 480 - mojisize * 4, 640, 480, Black1, TRUE);
			DrawBox(0, 480 - mojisize * 5 - 2, 150 + 2, 480 - mojisize * 4 + 2, Black, TRUE);
			DrawBox(0, 480 - mojisize * 5, 150, 480 - mojisize * 4, Black1, TRUE);
			DrawFormatString(0, 480 - mojisize * 5 + 2, Black, "%s", npc[TalkMan].name);
			for (int j = 0; j < Ncount; j++)
				DrawFormatString(0, 480 - mojisize * (4 - j) + 7, Black, "%s", buf[j]);
			if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.key[KEY_INPUT_UP] >= 10 || g_MainData.pad[INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] >= 5 ||
				g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.key[KEY_INPUT_DOWN] >= 10 || g_MainData.pad[INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] >= 5)
				if (cursor == 0)
					cursor = 1;
				else
					cursor = 0;
			if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
				selectStart = FALSE;
				TalkOn = FALSE;
				TalkMode = cursor + 1;
				nowMoji = 0;
				Ncount = 0;
				return;
			}
			menu->DrawBox1(550, 480 - mojisize * 7, 1, YesNo, cursor);
		}
		for (int i = nowMoji; i <= 1024; i++) {
			if (serifBuf[i] == 'B') {
				DrawBox(0, 480 - mojisize * 4 - 2, 640, 480, Black, TRUE);
				DrawBox(0, 480 - mojisize * 4, 640, 480, Black1, TRUE);
				DrawBox(0, 480 - mojisize * 5 - 2, 150 + 2, 480 - mojisize * 4 + 2, Black, TRUE);
				DrawBox(0, 480 - mojisize * 5, 150, 480 - mojisize * 4, Black1, TRUE);
				DrawFormatString(0, 480 - mojisize * 5 + 2, Black, "%s", npc[TalkMan].name);
				for (int j = 0; j < Ncount; j++)
					DrawFormatString(0, 480 - mojisize * (4 - j) + 7, Black, "%s", buf[j]);
				WaitKey();
				nowMoji = i + 1;
				return;
			}
			else if (serifBuf[i] == 'N') {
				buf[Ncount][i] = '\0';
				DrawBox(0, 480 - mojisize * 4 - 2, 640, 480, Black, TRUE);
				DrawBox(0, 480 - mojisize * 4, 640, 480, Black1, TRUE);
				DrawBox(0, 480 - mojisize * 5 - 2, 150 + 2, 480 - mojisize * 4 + 2, Black, TRUE);
				DrawBox(0, 480 - mojisize * 5, 150, 480 - mojisize * 4, Black1, TRUE);
				DrawFormatString(0, 480 - mojisize * 5 + 2, Black, "%s", npc[TalkMan].name);
				for (int j = 0; j < Ncount; j++)
					DrawFormatString(0, 480 - mojisize * (4 - j) + 7, Black, "%s", buf[j]);
				if (Ncount < 3) {
					Ncount++;
				}
				else
					for (int j = 0; j < 3; j++) {
						strcpy_s(buf[j], buf[j + 1]);
					}
				nowMoji = i + 1;
				return;
			}
			else if (serifBuf[i] == 'F') {
				haveFlag[npc[TalkMan].giveFlag] = TRUE;
			}
			//else if (serifBuf[i] == 'W') {
			//	for (int i = 0; i < warp.size() - 1; i++)
			//		if (warp[i].npcNumber == TalkMan)
			//			if (WarpCount[g_MainData.mapID - 1][TalkMan] > WarpCount)
			//				//わからん
			//}
			else if (serifBuf[i] == 'S') {
				selectStart = TRUE;
				return;
			}
			else if (serifBuf[i] == 'E') {
				Ncount = 0;
				nowMoji = 0;
				TalkMode = 0;
				NowisTalk = FALSE;
				TalkOn = FALSE;
				return;
			}
			else {
				buf[Ncount][i - nowMoji] = serifBuf[i];
				i++;
				buf[Ncount][i - nowMoji] = serifBuf[i];
				buf[Ncount][i - nowMoji + 1] = '\0';
			}
		}
	}
}

int Player::getpx() {
	return px;
}

int Player::getpy() {
	return py;
}

void Player::LoadEvent() {
	if (oneLoad == FALSE) {
		fstream fs1;

		fs1.open("data/map/maplist.txt", ios::in);
		if (!fs1)return;

		char IDbuf[256];
		char fmfbuf[256];
		char pngbuf[256];
		char namebuf[256];
		mapList.clear();

		while (!fs1.eof()) {
			fs1.getline(IDbuf, 256);
			fs1.getline(fmfbuf, 256);
			fs1.getline(pngbuf, 256);
			fs1.getline(namebuf, 256);
			MapList add = { atoi(IDbuf), fmfbuf, pngbuf, namebuf };
			mapList.push_back(add);
		}

		fs1.close();
		oneLoad = TRUE;
	}

	char fname[50];
	sprintf_s(fname, "data/event/%s.txt", mapList[g_MainData.mapID-1].name.c_str());
	fstream fs;
	fs.open(fname, ios::in);
	if (!fs) {
		return;
	}

	char xbuf[256];
	char ybuf[256];
	char mapIDbuf[256];
	char xxbuf[256];
	char yybuf[256];
	event.clear();
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

void Player::HitEvent() {
	int size = event.size();
	int i = 0;
	if (Dire == 3) {
		while (i < size - 1) {
			if (event[i].y * 32 + 32 > py + 480 / 2 - 32 / 2 && event[i].y * 32 < py + 480 / 2 - 32 / 2)
				if (event[i].x * 32 < px + 32 + 640 / 2 - 32 / 2 && event[i].x * 32 + 32 > px + 640 / 2 - 32 / 2) {
					g_MainData.mapID = event[i].mapid;
					py = (event[i].yy+1) * 32 - 480 / 2 - 32 / 2 -16;
					px = (event[i].xx+1) * 32 - 640 / 2 - 32 / 2;
				}
			i++;
		}
	}
	if (Dire == 0) {
		while (i < size - 1) {
			if (event[i].y * 32 + 32 > py + 32 + 480 / 2 - 32 / 2 && event[i].y * 32 < py + 32 + 480 / 2 - 32 / 2)
				if (event[i].x * 32 < px + 32 + 640 / 2 - 32 / 2 && event[i].x * 32 + 32 > px + 640 / 2 - 32 / 2) {
					g_MainData.mapID = event[i].mapid;
					py = (event[i].yy + 1) * 32 - 480 / 2 - 32 / 2 +16;
					px = (event[i].xx + 1) * 32 - 640 / 2 - 32 / 2;
				}
			i++;
		}
	}
	if (Dire == 2) {
		while (i < size - 1) {
			if (event[i].y * 32 + 32 > py + 480 / 2 - 32 / 2 && event[i].y * 32 < py + 32 + 480 / 2 - 32 / 2)
				if (event[i].x * 32 < px + 32 + 640 / 2 - 32 / 2 && event[i].x * 32 + 32 > px +32 + 640 / 2 - 32 / 2) {
					g_MainData.mapID = event[i].mapid;
					py = (event[i].yy + 1) * 32 - 480 / 2 - 32 / 2;
					px = (event[i].xx + 1) * 32 - 640 / 2 - 32 / 2 +16;
				}
			i++;
		}
	}
	if (Dire == 1) {
		while (i < size - 1) {
			if (event[i].y * 32 + 32 > py + 480 / 2 - 32 / 2 && event[i].y * 32 < py + 32 + 480 / 2 - 32 / 2)
				if (event[i].x * 32 < px + 640 / 2 - 32 / 2 && event[i].x * 32 + 32 > px + 640 / 2 - 32 / 2) {
					g_MainData.mapID = event[i].mapid;
					py = (event[i].yy + 1) * 32 - 480 / 2 - 32 / 2;
					px = (event[i].xx + 1) * 32 - 640 / 2 - 32 / 2 -16;
				}
			i++;
		}
	}
}

void Player::LoadNPC() {
	npc.clear();
	char fname[50];
	int InputHandle = 0;
	sprintf_s(fname, "data/npc/%s.txt", mapList[g_MainData.mapID - 1].name.c_str());
	fstream fs;
	fs.open(fname, ios::in);
	if (!fs) {
		return;
	}
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

	for (int i = 0; i < npc.size(); i++) {
		char grname[100];
		int SizeX, SizeY, GrHandle;
		sprintf_s(grname, "data/character/%s", npc[i].gr.c_str());
		
		GrHandle = LoadGraph(grname);
		GetGraphSize(GrHandle, &SizeX, &SizeY);

		LoadDivGraph(grname, 3 * 4, 3, 4, SizeX / 3, SizeY / 4, NPCgr[i]);
	}

	sprintf_s(fname, "data/event/WarpList%s.txt", mapList[g_MainData.mapID - 1].name.c_str());
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

void Player::isTalk() {
	int chei = fmfMap->GetChipHeight() * 2;

	for (int i = 0; i < npc.size(); i++) {
		if (Dire == 3)
			if (npc[i].x * 32 - (640 / 2 - 32 / 2) < px + chei / 2 && px + chei / 2 < npc[i].x * 32 + chei - 1 - (640 / 2 - 32 / 2) &&
				npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py - speed - 1 && npc[i].y * 32 - (480 / 2 - 32 / 2) < py) {
				NowisTalk = TRUE;
				TalkMan = i;
			}
		if (Dire == 0)
			if (npc[i].x * 32 - (640 / 2 - 32 / 2) < px + chei - 1 && px < npc[i].x * 32 + chei - 1 - (640 / 2 - 32 / 2) &&
				npc[i].y * 32 - (480 / 2 - 32 / 2) < py + chei + speed + 1 && npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py + chei) {
				NowisTalk = TRUE;
				TalkMan = i;
			}
		if (Dire == 1)
			if (npc[i].x * 32 + chei - (640 / 2 - 32 / 2) > px - speed - 1 && px > npc[i].x * 32 - (640 / 2 - 32 / 2) &&
				npc[i].y * 32 - (480 / 2 - 32 / 2) < py + chei && npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py) {
				NowisTalk = TRUE;
				TalkMan = i;
			}
		if (Dire == 2)
			if (npc[i].x * 32 - (640 / 2 - 32 / 2) < px + chei + speed + 1 && px + chei < npc[i].x * 32 + chei - (640 / 2 - 32 / 2) &&
				npc[i].y * 32 - (480 / 2 - 32 / 2) < py + chei && npc[i].y * 32 + chei - (480 / 2 - 32 / 2) > py) {
				NowisTalk = TRUE;
				TalkMan = i;
			}
	}
}

void Player::InputSerif(char buf[1024]) {
	NowisTalk = TRUE;
	TalkOn = TRUE;
	for (int i = 0; i < 1024; i++)
		serifBuf[i] = '\0';
	strcpy_s(serifBuf, buf);
}

void Player::Ending() {
	//1左2右3上0下
	static int kodomo[3 * 4];
	static int kx = 640 / 2 - 32 / 2, ky = 480;
	static int kAnime = 1, kDire = 3;
	static int mano[3 * 4];
	static int mx = -32, my = -32;
	static int mAnime = 1, mDire = 3;
	static int GrHandle;

	DrawExtendGraph(kx, ky, kx + 31, ky + 31, kodomo[kDire * 3 + kAnime], TRUE);
	DrawExtendGraph(mx, my, mx + 31, my + 31, mano[mDire * 3 + mAnime], TRUE);
	if (FrameCount == 9) {	//9フレームごとに歩行
		if (kAnime == 1) {
			kAnime += AnimeDire;		//直立→右足前→直立→左足前→直立　を実装
			mAnime += AnimeDire;
		}
		else {
			kAnime = 1;
			mAnime = 1;
		}
	}
	switch (Endscene){
	case 1:
		int SizeX, SizeY;
		GrHandle = LoadGraph("data/character/子供.png");
		GetGraphSize(GrHandle, &SizeX, &SizeY);
		LoadDivGraph("data/character/子供.png", 3 * 4, 3, 4, SizeX / 3, SizeY / 4, kodomo);
		GrHandle = LoadGraph("data/character/魔の者.png");
		GetGraphSize(GrHandle, &SizeX, &SizeY);
		LoadDivGraph("data/character/魔の者.png", 3 * 4, 3, 4, SizeX / 3, SizeY / 4, mano);
		
		g_MainData.mapID = 14;
		Endscene++;
		break;
	case 2:
		LoadDivGraph("data/character/勇者2.png", 3 * 4, 3, 4, 20, 28, PlayerGr2);
		px = 504, py = 344;
		Dire = 3;
		mx = 640 / 2 - 32 / 2, my = 480 / 2 - 32 / 2 - 32 - 32 / 2;
		mDire = 0;
		NowisTalk = TRUE;
		TalkOn = TRUE;
		for (int i = 0; i < 1024; i++)
			serifBuf[i] = '\0';
		strcpy_s(serifBuf, "・・・終わったか。NBこの世界に、もう未練はない。NNBさぁ、勇者よ。NB務めを果たせ。NBE");		//ここセリフ
		strcpy_s(npc[TalkMan].name, "物の怪");
		Endscene++;
		break;
	case 3:
		if (NowisTalk == FALSE && haveFlag[3] == TRUE) {
			Dire = 0;
			Endscene++;
		}
		else if (NowisTalk == FALSE && haveFlag[3] == FALSE)
			Endscene = 11;
		break;
	case 4:
		ky--;
		if (ky < 480 / 2 + 480 / 4 && !(px > 504 + 32 + 32 / 2)) {
			Dire = 2;
			px++;
			kx--;
			mx--;
		}
		if (px > 504 + 32 + 32 / 2)
			Dire = 1;
		if(ky < 480/2+32/2+32/2)
			Endscene++;
		break;
	case 5:
		NowisTalk = TRUE;
		TalkOn = TRUE;
		for (int i = 0; i < 1024; i++)
			serifBuf[i] = '\0';
		strcpy_s(serifBuf, "あ、あの！NBこの前助けてくれてありがとう！・・・NBその時は驚いちゃって何も言えなくてごめんなさい。NBその・・・NB友達になってくれませんか！！NBE");		//ここセリフ
		strcpy_s(npc[TalkMan].name, "子供");
		Endscene++;
		break;
	case 6:
		if (NowisTalk == FALSE)
		Endscene++;
		break;
	case 7:
		NowisTalk = TRUE;
		TalkOn = TRUE;
		for (int i = 0; i < 1024; i++)
			serifBuf[i] = '\0';
		strcpy_s(serifBuf, "とぅるーえんど　「共存の足掛け」NBE");		//ここセリフ
		strcpy_s(npc[TalkMan].name, "　　");
		Endscene++;
		break;
	case 8:
		if (NowisTalk == FALSE)
		Endscene++;
		break;
	case 11:
		NowisTalk = TRUE;
		TalkOn = TRUE;
		for (int i = 0; i < 1024; i++)
			serifBuf[i] = '\0';
		strcpy_s(serifBuf, "ばっどえんど　「勇者の務め」NBE");		//ここセリフ
		strcpy_s(npc[TalkMan].name, "　　");
		Endscene++;
		break;
	case 12:
		if (NowisTalk == FALSE)
			Endscene++;
		break;
	default:
		EndFlag = 1;
		break;
	}
}