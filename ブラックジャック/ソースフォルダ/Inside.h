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
	int Count; //��D���������邩
	int Index[52]; //��D�Ǘ��ԍ�
	int Point[52]; //�_����������D
	int Sum;//���݂̓_��
	int StandFlag; //�X�^���h������1��
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