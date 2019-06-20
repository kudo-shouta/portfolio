#include "MapEdit.h"
#include <fstream>

MapEdit::MapEdit(MenuChanger* changer) : BaseMenu(changer) {
	//�����I�����\�z
	select.push_back("�}�b�v�\�z");
	select.push_back("�}�b�v�ړ�");
}

MapEdit::~MapEdit() {
}

bool MapEdit::Initialize() {

	Load();

	//png.fmf�t�@�C������
	png = getFileName("data/map", "png");
	fmf = getFileName("data/map", "fmf");
	//�v���r���[�p�̃}�b�v�\���N���X
	fmfMap = new CFmfMap();
	//�}�b�v�����Ō�̑I����
	finish.push_back("�}�b�v����");
	finish.push_back("�߂�");
	//�}�b�v�ړ��I����
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
	//�\������I�����̃��[�h�ɑ΂��I�������Z�b�g����
	if (Mode == -1) size = select.size();
	if (Mode == 0) size = fmf.size();
	if (Mode == 1) size = png.size();
	if (Mode == 2) size = finish.size();
	if (Mode == 10) size = move.size();

	//��L�[����
	if (g_MainData.key[KEY_INPUT_UP] == 1 || g_MainData.pad[INPUT_UP] == 1) {
		if (cursor != 0)	//��ԏ�łȂ��Ȃ�΃J�[�\����Ɉړ�
			cursor -= 1;
		else//��ԏ�Ȃ�Ή��ɉ�荞��
			cursor = size - 1;
		if (Mode == 1 || Mode == 10)			//�J�[�\���ړ��ɔ����ă}�b�v�����[�h�������B�����[�h����ɂ���
			isLoad = FALSE;
	}
	//���L�[����
	if (g_MainData.key[KEY_INPUT_DOWN] == 1 || g_MainData.pad[INPUT_DOWN] == 1) {
		
		if (cursor + 1 < size) {	//��ԉ��łȂ��Ȃ�΃J�[�\�����Ɉړ�
			cursor += 1;
		}
		else {//��ԉ��Ȃ�Ώ�ɉ�荞��
			cursor = 0;
		}
		if (Mode == 1 || Mode == 10)
			isLoad = FALSE;		//�����[�h����ɂ���
	}
	//�����܂ŃJ�[�\���ړ�����

	//�}�b�v�������ړ���
	if (Mode == -1) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			Mode = cursor * 10;		//����[0*10=0]�ړ�[1*10=10]
			cursor = 0;
		}
		if (g_MainData.key[KEY_INPUT_BACK] == 1 || g_MainData.pad[INPUT_A] == 1)
			pMenuChanger->Changemenu("�X�^�[�g");
	}

	//fmf�t�@�C���̑I��
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

	//�}�b�v�`�b�v�摜�̑I��
	else if (Mode == 1) {
		if (g_MainData.key[KEY_INPUT_RETURN] == 1 || g_MainData.pad[INPUT_B] == 1) {
			pngSelect = cursor;
			Mode = 2;
			cursor = 0;
			
			//.fmf �������������̂��}�b�v����
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

	//�}�b�v�����̊m��
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

	//�}�b�v�ړ�����
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
	if (Mode == -1)	//�����ړ��I����
		DrawBox1(50, 50, 1, select, cursor);
	if (0 <= Mode && Mode < 10)	//�}�b�v������
		if (fmfSelect == -1)	//fmf�I��
			DrawBox1(50, 50, 1, fmf, cursor);
		else                    //���肵��fmf�𗠂ɕ\��
			DrawBox1(50, 50, 1, fmf, fmfSelect);
	if (1 <= Mode && Mode < 10) {
		if (isLoad == FALSE) {	//�J�[�\���ړ��̂��тɃ}�b�v�������[�h���v���r���[�\��
			fmfMap->Open(fmf[fmfSelect].c_str(), png[cursor].c_str());
			isLoad = TRUE;
		}
		fmfMap->Preview();
		if (pngSelect == -1)	//png�I��
			DrawBox1(100, 70, 1, png, cursor);
		else                    //���肵��png�𗠂ɕ\��
			DrawBox1(100, 70, 1, png, pngSelect);
	}
	if (2 <= Mode &&  Mode < 10) {	//�}�b�v��������H��
		DrawFormatString(200, 410, Black, "�}�b�v���F%s", mapName.c_str());
		DrawBox1(230, 430, 1, finish, cursor);
	}
	if (Mode == 10) {	//�}�b�v�ړ�
		DrawBox1(50, 50, 1, move, cursor);
		if (isLoad == FALSE) {	//�J�[�\���ړ��̂��тɃ}�b�v�������[�h���v���r���[�\��
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
	printfDx("data/map/maplist.txt�ۑ�����\n");
}