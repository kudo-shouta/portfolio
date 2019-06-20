#include "DxLib.h"
#include <stdio.h>
#include "Inside.h"
#include "Outside.h"

static int Card[53];
int pcnt=-1;
int dcnt=-1;
int Sound;
int win,winflag;
int lose,loseflag;
int even,evenflag;
int fin;

//�J�[�h�O���t�B�b�N�A�T�E���h�̗p��
void Card_Init()
{
	char GraphName[50];

	for(int i=0; i<=52; i++)
	{
		sprintf_s(GraphName,"IMAGE/%d.png",i);
		Card[i] = LoadGraph(GraphName);
	}

	Sound = LoadSoundMem("MUSIC/hiku.mp3");

	winflag=0,loseflag=0,evenflag=0,fin=0;
}

//�����J�[�h��\�������邾���A���炵�B�f�B�[���[�͂Q���ڈȍ~�͕����ĕ\���A���U���g�̎��\��
void ViewCard(int Num, int Count, int jude, int Mode)
{
	int X = 10;
	int PY = 320;
	int DY = 10;

	if(jude==0)
		DrawExtendGraph(X + Count*50, PY, X+101 + Count*50, PY+151, Card[Num], TRUE);
	else if(jude==1)
	{
		DrawExtendGraph(X + Count*50, DY, X+101 + Count*50, DY+151, Card[0], TRUE);
		if((Mode == 3 && pcnt == -1 && dcnt == -1) || Count == 0)
			DrawExtendGraph(X + Count*50, DY, X+101 + Count*50, DY+151, Card[Num], TRUE);
	}
}

void MoveCard(int Num, int Count, int jude, int cnt, int Mode)
{
	if(cnt == 0){
		PlaySoundMem(Sound, DX_PLAYTYPE_BACK);
	}
	int X = 10;
	int PY = 320;
	int DY = 10;
	int SX = 600;
	int SY = 10;

	int dx = X -SX;
	int pdy = PY - SY;
	int ddy = DY - SY;

	int MoveX = (dx)/30;
	int pMoveY = (pdy)/30;
	int dMoveY = (ddy)/30;

	if(jude==0)
		DrawExtendGraph(SX + MoveX*cnt + Count*50, pMoveY*cnt, SX + MoveX*cnt+101 + Count*50, pMoveY*cnt+151, Card[Num], TRUE);
	else if(jude==1){
		DrawExtendGraph(SX + MoveX*cnt + Count*50, dMoveY*cnt, SX + MoveX*cnt+101 + Count*50, dMoveY*cnt+151, Card[0], TRUE);
		if(Mode == 3 && pcnt == -1 && dcnt == -1)
			DrawExtendGraph(SX + MoveX*cnt + Count*50, dMoveY*cnt, SX + MoveX*cnt+101 + Count*50, dMoveY*cnt+151, Card[Num], TRUE);
	}
	//printfDx("dx=%d,dy=%d ,mx=%d, my=%d\n", dx,pdy,MoveX, pMoveY);
	//printfDx("%dx%d %dx%d\n", MoveX*1, pMoveY*1, MoveX*cnt+101, pMoveY*cnt+151);
}

//void AnimeStart()
//{
//	cnt=0;
//}

void GraphDraw(int Mode, status *p, status *d)
{
	for(int i=0; i<p->Count-1; i++)
		ViewCard(p->Index[i], i, 0, Mode);
	for(int j=0; j<d->Count-1; j++)
		ViewCard(d->Index[j], j, 1, Mode);

	int GrHandle = 0;
	//ScreenFlip();
	//GetDrawScreenGraph(0, 0, 640, 480, GrHandle);
	if(pcnt >= 0){
		//ClearDrawScreen();
		MoveCard(p->Index[p->Count-1], p->Count-1, 0, pcnt, Mode);
		pcnt++;
		//DrawGraph(0, 0, GrHandle, FALSE);
		//ScreenFlip();
		if(pcnt >= 31){
			pcnt=-1;
		}
	} else {
		ViewCard(p->Index[p->Count-1], p->Count-1, 0, Mode);
	}
	if(dcnt >= 0 && pcnt == -1){
		MoveCard(d->Index[d->Count-1], d->Count-1, 1, dcnt, Mode);
		dcnt++;
		if(dcnt >= 31){
			dcnt=-1;
		}
	} else if(dcnt == -1){
		ViewCard(d->Index[d->Count-1], d->Count-1, 1, Mode);
	}

	Text(Mode, p, d);

	/*if(p->Sum>21)
		DrawString(10, 380, "�o�[�X�g���܂���", GetColor(255, 255, 255));

	if(Mode == 3)
		DrawString(10, 400, "�Q�[���I��", GetColor(255, 255, 255));*/
}

void Text(int Mode, status *p, status *d)
{
	int PSelectX = 10;//�v���C���[�I�������W
	int PSelectY = 280;
	int PointX = 170;//���_X���W
	int PPointY = 280;//�v���C���[���_Y���W
	int DPointY = 170;//�f�B�[���[����
	int BJX = 170;//�u���b�N�W���b�N�\��X���W
	int PBJY = 295;//�v���C���[�u���b�N�W���b�N�\��Y���W
	int DBJY = 185;//�f�B�[���[����
	char *BJ = "�u���b�N�W���b�N�I";
	char *NBJ = "�i�`�������u���b�N�W���b�N�I�I";
	int Black = GetColor(0, 0, 0);
	int recX = 10;
	int recY = 225;

	DrawFormatString(recX, recY, Black, "%d�� %d�� %d��",win,lose,even);
	if(Mode != 3)
		DrawString(PSelectX, PSelectY, "�q�b�g�Ȃ�Enter\n�X�^���h�Ȃ�Space", Black);
	else{
		DrawFormatString(PointX, DPointY, Black, "�����Ă�%d�_�ł�",d->Sum);
		if(d->Sum == 21)
			if(d->Count == 2)
				DrawFormatString(BJX, DBJY, Black, NBJ);
			else
				DrawFormatString(BJX, DBJY, Black, BJ);
	}

	DrawFormatString(PointX, PPointY, Black, "���Ȃ���%d�_�ł�",p->Sum);
	if(p->Sum == 21)
		if(p->Count == 2)
			DrawFormatString(BJX, PBJY, Black,NBJ);
		else
			DrawFormatString(BJX, PBJY, Black, BJ);
}

void EndText(status p, status d)
{
	int X = 170;
	int Y = 225;
	char *PBast = "���Ȃ����o�[�X�g���܂���";
	char *DBast = "�����Ă��o�[�X�g���Ă܂�";
	char *Win = "���Ȃ��̏����ł�";
	char *Lose = "���Ȃ��̕����ł�";
	char *Even = "���������ł�";
	int Black = GetColor(0, 0, 0);

	if(p.Sum > 21){
		DrawString(X, Y, PBast, Black);
		DrawString(X, Y+15, Lose, Black);
		lose++;
		return ;
	}
	else if(d.Sum > 21){
		DrawString(X, Y, DBast, Black);
		DrawString(X, Y+15, Win, Black);
		win++;
		return ;
	}
	if(p.Sum == 21)
		if(d.Sum == 21)
			if(p.Count == 2 && d.Count != 2){
				DrawString(X, Y+15, Win, Black);
				win++;
				return ;
			}
			else if(p.Count != 2 && d.Count == 2){
				DrawString(X, Y+15, Lose, Black);
				lose++;
				return ;
			}
			else{
				DrawString(X, Y+15, Even, Black);
				even++;
				return ;
			}
		else{
			DrawString(X, Y+15, Win, Black);
			win++;
			return ;
		}
	else if(d.Sum == 21){
		DrawString(X, Y, Lose, Black);
		lose++;
		return ;
	}
	if(p.Sum > d.Sum){
		DrawString(X, Y, Win, Black);
		win++;
		return ;
	}
	else if(p.Sum < d.Sum){
		DrawString(X, Y, Lose, Black);
		lose++;
		return ;
	}
	else{
		DrawString(X, Y, Even, Black);
		even++;
		return ;
	}

	if(fin==0){
		if(winflag==1){
			win++;
		}
		else if(loseflag==1){
			lose++;
		}
		else if(evenflag==1){
			even++;
		}
		fin=1;
	}


	/*if(Mode == 3)
		DrawString(10, 400, "�Q�[���I��", GetColor(255, 255, 255));*/
}

int ContinueText()
{
	int Black = GetColor(0, 0, 0);
	char *text = "�Q�[���𑱂��܂���";
	char *next = "Yes�|��Enter�L�[";
	char *end = "No�|��Space�L�[";
	int X = 400;
	int Y = 225;

	DrawString(X, Y, text, Black);
	DrawString(X, Y+15, next, Black);
	DrawString(X, Y+30, end, Black);

	

	return GetKey();
}