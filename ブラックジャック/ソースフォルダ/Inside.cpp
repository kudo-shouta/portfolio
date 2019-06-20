#include "DxLib.h"
#include "Outside.h"
#include "Inside.h"

//プレイヤー、ディーラーのステータス
// struct status
//{
//	int Count; //手札が何枚あるか
//	int Index[52]; //手札管理番号
//	int Point[52]; //点数化した手札
//}

//山札情報
struct Deck
{
	int Index[52];
	int Max;
};

//struct status Player;
//struct status Dealer;

struct Deck Card;

static char NowKey[256];


//static int Card_Index[52];//山札の用意
//static int Player_Count,Player_Index[52], Player_Point[52];//プレイヤーの手札枚数、手札、点数の用意
//static int Dealer_Count,Dealer_Index[52],Dealer_Point[52];//ディーラーの手札枚数、手札、点数の用意
//static int MaxCard=52;//山札の最大枚数

void Deck_Init() //プレイヤーの初手カードの決定、山札の用意
{
	for(int j=0; j<=52; j++)//山札の初期化
		Card.Index[j] = j;
	Card.Max = 52;
	
	//GiveCard();
	//Text();
}

void Init(status *st)
{
	st->Count=0;

	int i=2;
	while(i)
	{
		DrawCard(st);
		i--;
	}
	st->StandFlag = 0;
	henkan(st);
}

//山札からカードを一枚引いて、最大数減らす
void DrawCard(status *st)
{
	int Num = GetRand(Card.Max-1);//1~52(最大数)のランダムな数、0は使わない
	printf("引いたカードはnum=%d mark=%d\n",Num%13+1, Num/13+1);
	Num++;
	Card.Index[Num] = Card.Index[Card.Max];
	Card.Max--;

	//AnimeStart();

	st->Index[st->Count] = Num;
	st->Count++;
	henkan(st);
}

//カード配布
//void GiveCard()
//{
//	int Jude=0;//0でプレイヤー、1でディーラー
//	int Count=0;//配布カードをカウントし、表示する位置をずらしていく
//	for(int i=0; i<Player.Count; i++)
//	{
//		ViewCard(Player.Index[i],Count,Jude);
//		Count++;
//	}
//
//	Jude=1;
//	Count=0;
//	for(int j=0; j<Dealer.Count; j++)
//	{
//		ViewCard(Dealer.Index[j], Count,Jude);
//		Count++;
//	}
//}

//プレイヤーへの選択肢　0なら引く 1なら引かない
int GetKey()
{
	KeyUpdate();
	if(NowKey[ KEY_INPUT_RETURN] == 0)
	{
		return 0;
	}
	else if(NowKey[ KEY_INPUT_SPACE] == 0 )
	{
		return 1;
	}
	else
	{
		return -1;
	}
	return -1;
}

void KeyUpdate()
{
	char Key[256];
	GetHitKeyStateAll(Key);
	for(int i=0; i<=256; i++)
	{
		if(Key[i] == 0)
			NowKey[i] = -1; //押されて無いと-1になり
		else NowKey[i]++; //押されると0,1,2,3・・・となる
	}
}

//カードを一枚追加するだけ、Inside_Initとほぼ同じ
//void Hit(status *st, int Jude)
//{
//	if(Jude == 0)
//	{
//		st->Index[st->Count] = DrawCard();
//		st->Count++;
//	}
//}

//ディーラーのカード追加判定
int DealerJude(status *st)
{
	if(st->Sum<=16)
		return 0;
	else return 1;
}

//カードを管理している1~52の数字を点数に変換する
void henkan(status *st)
{
	int Sum=0;
	for(int i=0; i<st->Count; i++)
	{
		st->Point[i] = st->Index[i]%13;
		if(st->Point[i]==0 || st->Point[i]==11 || st->Point[i]==12)
			st->Point[i] = 10;
		else if(st->Point[i] == 1)
			st->Point[i] = 11;
	}

	for(int i=0; i<st->Count; i++)
			Sum += st->Point[i];

	if(Sum>21)
	{
		for(int i=0; i<st->Count; i++)
			if(st->Point[i] == 11)
				st->Point[i] = 1; ////////////全部１にしちゃってる

		Sum = 0;
		for(int i=0; i<st->Count; i++)
				Sum += st->Point[i];
	}

	
	if(Sum>=21)
		st->StandFlag = 1;
	st->Sum = Sum;
}