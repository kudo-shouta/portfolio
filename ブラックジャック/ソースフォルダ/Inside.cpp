#include "DxLib.h"
#include "Outside.h"
#include "Inside.h"

//�v���C���[�A�f�B�[���[�̃X�e�[�^�X
// struct status
//{
//	int Count; //��D���������邩
//	int Index[52]; //��D�Ǘ��ԍ�
//	int Point[52]; //�_����������D
//}

//�R�D���
struct Deck
{
	int Index[52];
	int Max;
};

//struct status Player;
//struct status Dealer;

struct Deck Card;

static char NowKey[256];


//static int Card_Index[52];//�R�D�̗p��
//static int Player_Count,Player_Index[52], Player_Point[52];//�v���C���[�̎�D�����A��D�A�_���̗p��
//static int Dealer_Count,Dealer_Index[52],Dealer_Point[52];//�f�B�[���[�̎�D�����A��D�A�_���̗p��
//static int MaxCard=52;//�R�D�̍ő喇��

void Deck_Init() //�v���C���[�̏���J�[�h�̌���A�R�D�̗p��
{
	for(int j=0; j<=52; j++)//�R�D�̏�����
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

//�R�D����J�[�h���ꖇ�����āA�ő吔���炷
void DrawCard(status *st)
{
	int Num = GetRand(Card.Max-1);//1~52(�ő吔)�̃����_���Ȑ��A0�͎g��Ȃ�
	printf("�������J�[�h��num=%d mark=%d\n",Num%13+1, Num/13+1);
	Num++;
	Card.Index[Num] = Card.Index[Card.Max];
	Card.Max--;

	//AnimeStart();

	st->Index[st->Count] = Num;
	st->Count++;
	henkan(st);
}

//�J�[�h�z�z
//void GiveCard()
//{
//	int Jude=0;//0�Ńv���C���[�A1�Ńf�B�[���[
//	int Count=0;//�z�z�J�[�h���J�E���g���A�\������ʒu�����炵�Ă���
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

//�v���C���[�ւ̑I�����@0�Ȃ���� 1�Ȃ�����Ȃ�
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
			NowKey[i] = -1; //������Ė�����-1�ɂȂ�
		else NowKey[i]++; //��������0,1,2,3�E�E�E�ƂȂ�
	}
}

//�J�[�h���ꖇ�ǉ����邾���AInside_Init�Ƃقړ���
//void Hit(status *st, int Jude)
//{
//	if(Jude == 0)
//	{
//		st->Index[st->Count] = DrawCard();
//		st->Count++;
//	}
//}

//�f�B�[���[�̃J�[�h�ǉ�����
int DealerJude(status *st)
{
	if(st->Sum<=16)
		return 0;
	else return 1;
}

//�J�[�h���Ǘ����Ă���1~52�̐�����_���ɕϊ�����
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
				st->Point[i] = 1; ////////////�S���P�ɂ�������Ă�

		Sum = 0;
		for(int i=0; i<st->Count; i++)
				Sum += st->Point[i];
	}

	
	if(Sum>=21)
		st->StandFlag = 1;
	st->Sum = Sum;
}