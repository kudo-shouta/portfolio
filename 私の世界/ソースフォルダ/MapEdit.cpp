#include "MapEdit.h"
#include <fstream>

MapEdit::MapEdit(MenuChanger* changer) : BaseMenu(changer) {
	//初期選択肢構築
	select.push_back("マップ構築");
	select.push_back("マップ移動");
}

MapEdit::~MapEdit() {
}

bool MapEdit::Initialize() {

	Load();

	//png.fmfファイル検索
	png = getFileName("data/map", "png");
	fmf = getFileName("data/map", "fmf");
	//プレビュー用のマップ表示クラス
	fmfMap = new CFmfMap();
	//マップ生成最後の選択肢
	finish.push_back("マップ生成");
	finish.push_back("戻る");
	//マップ移動選択肢
	for (int i = 0; i != mapList.size() - 1; i++)
		move.push_back(mapList[i].name);

	cursor = 0;
	Mode = -1;
	pngSelect = -1;
	fmfSelect = -1;
	isLoad = FALSE;
	isName = FALSE;

	return TRUE;
}

void MapEdit::Finalize() {
}

void MapEdit::Update() {
	int size;
	//表示する選択肢のモードに対し選択肢をセットする
	if (Mode == -1) size = select.size();
	if (Mode == 0) size = fmf.size();
	if (Mode == 1) size = png.size();
	if (Mode == 2) size = finish.size();
	if (Mode == 10) size = move.size();

	//上キー入力
	if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1) {
		if (cursor != 0)	//一番上でないならばカーソル上に移動
			cursor -= 1;
		else//一番上ならば下に回り込む
			cursor = size - 1;
		if (Mode == 1 || Mode == 10)			//カーソル移動に伴ってマップをロードし直す。未ロード判定にする
			isLoad = FALSE;
	}
	//下キー入力
	if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1) {
		
		if (cursor + 1 < size) {	//一番下でないならばカーソル下に移動
			cursor += 1;
		}
		else {//一番下ならば上に回り込む
			cursor = 0;
		}
		if (Mode == 1 || Mode == 10)
			isLoad = FALSE;		//未ロード判定にする
	}
	//ここまでカーソル移動処理

	//マップ生成か移動か
	if (Mode == -1) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			Mode = cursor * 10;		//生成[0*10=0]移動[1*10=10]
			cursor = 0;
		}
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			pMenuChanger->Changemenu("スタート");
	}

	//fmfファイルの選択
	else if (Mode == 0) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			Mode = 1;
			fmfSelect = cursor;
			cursor = 0;
		}
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1) {
			Mode = -1;
			cursor = 0;
		}
	}

	//マップチップ画像の選択
	else if (Mode == 1) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			pngSelect = cursor;
			Mode = 2;
			cursor = 0;
			
			//.fmf を消去したものをマップ名に
			mapName = fmf[fmfSelect];
			for (int i = 0; i < 4; i++)
				mapName.pop_back();
		}
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1) {
			Mode = 0;
			fmfSelect = -1;
			cursor = 0;
		}
	}

	//マップ生成の確定
	else if (Mode == 2) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			if (cursor == 0) {
				MapList add = { mapList.size() , fmf[fmfSelect], png[pngSelect], mapName.c_str() };
				mapList.push_back(add);
				Save();
				move.push_back(mapName.c_str());
				Mode = -1;
			}
			if (cursor == 1)
				g_MainData.key[KEY_INPUT_BACK] = 1;
		}
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1) {
			Mode = 1;
			cursor = pngSelect;
			pngSelect = -1;
			isLoad = FALSE;
		}
	}

	//マップ移動処理
	else if (Mode == 10) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			g_MainData.mapID = cursor + 1;
		}
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1) {
			Mode = -1;
			cursor = 0;
		}
	}

	
}

void MapEdit::Draw() {
	if (Mode == -1)	//生成移動選択肢
		DrawBox1(50, 50, 1, select, cursor);
	if (0 <= Mode && Mode < 10)	//マップ生成中
		if (fmfSelect == -1)	//fmf選択中
			DrawBox1(50, 50, 1, fmf, cursor);
		else                    //決定したfmfを裏に表示
			DrawBox1(50, 50, 1, fmf, fmfSelect);
	if (1 <= Mode && Mode < 10) {
		if (isLoad == FALSE) {	//カーソル移動のたびにマップをリロードしプレビュー表示
			fmfMap->Open(fmf[fmfSelect].c_str(), png[cursor].c_str());
			isLoad = TRUE;
		}
		fmfMap->Preview();
		if (pngSelect == -1)	//png選択中
			DrawBox1(100, 70, 1, png, cursor);
		else                    //決定したpngを裏に表示
			DrawBox1(100, 70, 1, png, pngSelect);
	}
	if (2 <= Mode &&  Mode < 10) {	//マップ生成決定？中
		DrawFormatString(200, 410, Black, "マップ名：%s", mapName.c_str());
		DrawBox1(230, 430, 1, finish, cursor);
	}
	if (Mode == 10) {	//マップ移動
		DrawBox1(50, 50, 1, move, cursor);
		if (isLoad == FALSE) {	//カーソル移動のたびにマップをリロードしプレビュー表示
			fmfMap->Open(mapList[cursor].fmf.c_str(), mapList[cursor].png.c_str());
			isLoad = TRUE;
		}
		fmfMap->Preview();
	}
}

void MapEdit::Load() {
	fstream fs;

	fs.open("data/map/maplist.txt", ios::in);
	if (!fs)return;

	char IDbuf[256];
	char fmfbuf[256];
	char pngbuf[256];
	char namebuf[256];
	mapList.clear();
	int i = 0;

	while (!fs.eof()) {
		fs.getline(IDbuf, 256);
		fs.getline(fmfbuf, 256);
		fs.getline(pngbuf, 256);
		fs.getline(namebuf, 256);
		MapList add = { atoi(IDbuf), fmfbuf, pngbuf, namebuf };
		mapList.push_back(add);
		i++;
	}

	/*FILE *fp;
	fopen_s(&fp, "maplist.dat", "rb");
	if (fp == NULL)
		return;
	int count;
	fread(&count, sizeof(count), 1, fp);
	mapList.resize(count);
	for (int i = 0; i < count; i++) {
		fread(&mapList[i].ID, sizeof(mapList[i].ID), 1, fp);
		fread(&mapList[i].fmf, (mapList[i].fmf.size()), 1, fp);
		fread(&mapList[i].png, (mapList[i].png.size()), 1, fp);
	}
	fclose(fp);
	printfDx("%d\n%s\n%s\n", mapList[0].ID, mapList[0].fmf.c_str(), mapList[0].png.c_str());*/
}


void MapEdit::Save() {
	fstream fs;

	fs.open("data/map/maplist.txt", ios::out | ios::app);
	if (!fs)return;
	fs << mapList[mapList.size() - 1].ID << endl;
	fs << mapList[mapList.size() - 1].fmf << endl;
	fs << mapList[mapList.size() - 1].png << endl;
	fs << mapList[mapList.size() - 1].name << endl;
	fs.close();
	printfDx("data/map/maplist.txt保存完了\n");
}