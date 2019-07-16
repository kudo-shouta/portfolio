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

//カードグラフィック、サウンドの用意
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

//ただカードを表示させるだけ、ずらしつつ。ディーラーは２枚目以降は伏せて表示、リザルトの時表に
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
		DrawString(10, 380, "バーストしました", GetColor(255, 255, 255));

	if(Mode == 3)
		DrawString(10, 400, "ゲーム終了", GetColor(255, 255, 255));*/
}

void Text(int Mode, status *p, status *d)
{
	int PSelectX = 10;//プレイヤー選択肢座標
	int PSelectY = 280;
	int PointX = 170;//得点X座標
	int PPointY = 280;//プレイヤー得点Y座標
	int DPointY = 170;//ディーラー同上
	int BJX = 170;//ブラックジャック表示X座標
	int PBJY = 295;//プレイヤーブラックジャック表示Y座標
	int DBJY = 185;//ディーラー同上
	char *BJ = "ブラックジャック！";
	char *NBJ = "ナチュラルブラックジャック！！";
	int Black = GetColor(0, 0, 0);
	int recX = 10;
	int recY = 225;

	DrawFormatString(recX, recY, Black, "%d勝 %d負 %d分",win,lose,even);
	if(Mode != 3)
		DrawString(PSelectX, PSelectY, "ヒットならEnter\nスタンドならSpace", Black);
	else{
		DrawFormatString(PointX, DPointY, Black, "あいては%d点です",d->Sum);
		if(d->Sum == 21)
			if(d->Count == 2)
				DrawFormatString(BJX, DBJY, Black, NBJ);
			else
				DrawFormatString(BJX, DBJY, Black, BJ);
	}

	DrawFormatString(PointX, PPointY, Black, "あなたは%d点です",p->Sum);
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
	char *PBast = "あなたがバーストしました";
	char *DBast = "あいてがバーストしてます";
	char *Win = "あなたの勝ちです";
	char *Lose = "あなたの負けです";
	char *Even = "引き分けです";
	int Black = GetColor(0, 0, 0);

	if(p.Sum > 21){
		DrawString(X, Y, PBast, Black);
		DrawString(X, Y+15, Lose, Black);
		loseflag = 1;
	}
	else if(d.Sum > 21){
		DrawString(X, Y, DBast, Black);
		DrawString(X, Y+15, Win, Black);
		winflag = 1;
	}
	else if(p.Sum == 21)
		if(d.Sum == 21)
			if(p.Count == 2 && d.Count != 2){
				DrawString(X, Y+15, Win, Black);
				winflag = 1;
			}
			else if(p.Count != 2 && d.Count == 2){
				DrawString(X, Y+15, Lose, Black);
				loseflag = 1;
			}
			else{
				DrawString(X, Y+15, Even, Black);
				evenflag = 1;
			}
		else{
			DrawString(X, Y+15, Win, Black);
			winflag = 1;
		}
	else if(d.Sum == 21){
		DrawString(X, Y, Lose, Black);
		loseflag = 1;
	}
	else if(p.Sum > d.Sum){
		DrawString(X, Y, Win, Black);
		winflag = 1;
	}
	else if(p.Sum < d.Sum){
		DrawString(X, Y, Lose, Black);
		loseflag = 1;
	}
	else{
		DrawString(X, Y, Even, Black);
		evenflag = 1;
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
		DrawString(10, 400, "ゲーム終了", GetColor(255, 255, 255));*/
}

int ContinueText()
{
	int Black = GetColor(0, 0, 0);
	char *text = "ゲームを続けますか";
	char *next = "Yes－＞Enterキー";
	char *end = "No－＞Spaceキー";
	int X = 400;
	int Y = 225;

	DrawString(X, Y, text, Black);
	DrawString(X, Y+15, next, Black);
	DrawString(X, Y+30, end, Black);

	

	return GetKey();
}