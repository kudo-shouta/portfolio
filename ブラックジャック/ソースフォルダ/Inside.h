#ifndef PLAYER_H
#define PLAYER_H


const int INIT = 0;
const int PLAYERTURN = 1;
const int DEALERTURN = 2;
const int RESULT = 3;




//extern status Player;
//extern status Dealer;
 struct status
{
	int Count; //手札が何枚あるか
	int Index[52]; //手札管理番号
	int Point[52]; //点数化した手札
	int Sum;//現在の点数
	int StandFlag; //スタンドしたら1に
};

//typedef int (*FUNCPTR)(status *st);
 
void Deck_Init();
void Init(status *st);
void DrawCard(status *st);
int GetKey();
void KeyUpdate();
//void Hit(*status, int);
int DealerJude(status *st);
void henkan(status *st);
//void Text();



#endif