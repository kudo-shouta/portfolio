#include "DxLib.h"

#define TEXT_SIZE 18
char moji[3];//���ݕ\������e�L�X�g
char nmoji[3];
int NowX;//�\���e�L�X�g�̍��W
int x;//�Q�Ƃ�����W
int tate,yoko;//�\���ʒu
int Dan;//���i�����邩
int NCount,NFlag;//���s
int Timer;//�����\���^�C�~���O�^�C�}�[
int End,Fin;
int WaitFlag,WaitTime;

void text(char []);
void haikei(int);
int sentaku2(char [][106]);
int sentaku4(char [][106]);
void op();

//////////////�����ɖ{���ƑI�����������Ă���////////////////
char ie[]=
{
	"�`������̒��`BUN"
	"�����@�u���͂悤�vBUN"
	"��@�u���͂悤�vBUN"
	"��@�u�˂��vBUN"
	"�����@�u�ȂɁH�vBUN"
	"��@�u�X�ɐ����Ă钿�����L�m�R���K�v�Ȃ񂾂��ǁcN�@�@�@���ꂳ�񂿂���ƍ��肪�����Ȃ��āvBE"
};
char ie2[]=
{
	"��@�u�̂��Ă��Ă����H�vBE"
};
char ookido[]=
{
	"��@�u�̂��Ă����ق��������Ǝv���ȁ[�vBE"	
};
char ie3[]=
{
	"�����@�u������vBUN"
	"��@�u���肪�Ƃ��vBE"
};
char ie4[]=
{
	"��@�u�܂��A�L�m�R�̐����Ă�ꏊ�����ǁAN�@�@�@�X�ɓ����čŏ��̕����ꓹ���Ђ���A��Ԗڂ̕����ꓹ���݂��AN�@�@�@�O�Ԗڂ̕����ꓹ���Ђ���ɍs���ƒ�����vBUUUN"
	"��@�u���ɍ̂��Ă��Ăق����L�m�R�����ǁAN�@�@�@�����L�m�R�A�Ђ��`�̃L�m�R�A���킢�����ȃL�m�R�̎O��vBUUN"
	"��@�u�ŃL�m�R�ɐG��Ȃ��悤�ɋC�����āvBUN"
	"��@�u����ƁA�ߍ��͖�ɂȂ�ƃI�I�J�~���o��݂���������N�@�@�@�����A���Ă��ĂˁvBE"
};
char ie5[]=
{
	"��@�u���������H�vBE"
};
char ie6[]=
{
	"�����@�u���������vBUN"
	"�����@�u�s���Ă��܂��vBUN"
	"��@�u�����Ă�����Ⴂ�B�C�����ĂˁvBUN"
	"�c�c�cBE"
};
char michi[]=
{
	"�����@�u����؂��Ă������vBUN"
	"�����@�u�x���Ȃ�O�ɖ߂�Ȃ���vBUN"
	"�cBUN"
	"�cBUN"
	"�cBUN"
	"�����ꓹ��BN"
	"�ǂ����ɍs������BE"
};
char michi10[]=
{
	"�Ђ���ɍs����BUN"
	"�cBE"
};
char michi2[]=
{
	"�܂��܂���͒���BUN"
	"�����@�u����΂낤�vBUN"
	"�cBUN"
	"�cBUN"
	"�cBUN"
	"�܂������ꓹ��BN"
	"�ǂ����ɍs������BE"
};
char michi11[]=
{
	"�݂��ɍs����BUN"
	"�cBE"
};
char michi3[]=
{
	"�����@�i���������Œ������Ȃ��jBUN"
	"�cBUN"
	"�cBUN"
	"�cBUN"
	"�܂��܂������ꓹ��BN"
	"�ǂ����ɍs������BE"
};
char michi12[]=
{
	"�Ђ���ɍs����BUN"
	"�cBE"
};
char kinoko[]=
{
	"�����@�u�������I�vBUN"
	"�����@�i�L�m�R�͂��邩�ȁjBUN"
	"�����@�u�������I�vBUN"
	"�����@�i���`��c�ǂꂾ�����c�jBE"
};
char kinoko2[]=
{
	"�����@�u���ꂩ�Ȃ��vBUN"
	"�����@�i���ɂ͂��邩�ȁjBUN"
	"�����@�u�������vBUN"
	"�����@�i�ǂꂾ�낤�c�jBE"
};
char kinoko3[]=
{
	"�����@�u���Ԃ񂱂ꂾ�vBUN"
	"�����@�i���ɂ͂Ȃ����ȁjBUN"
	"�����@�u�������vBUN"
	"�����@�i�Ō�́c�jBE"
};
char kiro[]=
{
	"�����@�u����őS�����ȁvBUN"
	"�����@�u�A�낤�vBUN"
	"�c�c�cBE"
};
char kiro2[]=
{
	"�����@�i�����������Ԃ�������������ȁBN�@�@�@�@�����A��Ȃ��ƁjBUUN"
	"�cBUN"
	"�cBE"
};
char kiro3[]=
{
	"�����@�u�����I�vBUN"
	"�����@�i���ꂳ��̍D���ȃn�[�u���jBUN"
	"�����@�i���ꂳ��͒x���Ȃ����Ⴂ���Ȃ����Č����Ă����ǁcN�@�@�@�@���������Ȃ炢����ˁjBE"
};
char kiro4[]=
{
	"�����@�i�c����ȏ�x���Ȃ�Ƃ��ꂳ����S�z����jBUN"
	"�����@�i����ς�A�낤�jBE"
};
char kiro5[]=
{
	"�c�c�cBE"
};
char kiro6[]=
{
	"�����@�i�������Ă����jBUN"
	"�����@�i�����A��Ȃ���jBE"
};
char kiro7[]=
{
	"�cBUN"
	"�cBE"
};
char kiro8[]=
{
	"�cBUN"
	"�cBE"
};
char kitaku[]=
{
	"�����@�i����ƒ������jBUN"
	"�����@�u�������܁[�vBUN"
	"��@�u��������[�vBUN"
	"��@�u�c�c�c����B���񂾂��̂͑S������ˁvBUN"
	"��@�u���肪�Ƃ��B����������vBUN"
	"�c�c�cBUN"
	"��Game clear��BE"
};
char mayoi[]=
{
	"�݂��ɍs����BE"
};

char mayoi2[]=
{
	"�Ђ���ɍs����BE"
};
char mayoi3[]=
{
	"�cBE"
};
char mayoi4[]=
{
	"�cBE"
};
char mayoi5[]=
{
	"�cBUN"
	"���ɖ����Ă��܂����cBUN"
	"-Game over- BE"
};
char kyuusai[]=
{
	"���r�������蒼���܂����H��BE"
};
char poison[]=
{
	"�I�HBE"
};
char poison2[]=
{
	"�Ȃ񂾂��c�}�Ɉӎ����c�c�cBE"
};
char poison3[]=
{
	"-Game over- BE"
};
char wolf[]=
{
	"�c�c�cBE"
};
char wolf2[]=
{
	"�����@�u�x���Ȃ���������vBUN"
	"�cBUN"
	"�K�T�bBUN"
	"�����@�u�I�H�vBUN"
	"�����@�u�N���c�c����́c�c�H�vBUN"
	"�H�H�H�@�u�O�����������c�vBE"
};
char wolf3[]=
{
	"-Game over- BE"
};
char thanks[]=
{
	"Thank you for playing!! BE"
};
char sentakusi[][106]=
{
	{"1��������"},
	{"2���߂�ǂ�����"},
};
char sentakusi2[][106]=
{
	{"1����"},
	{"2���E"},
};
char sentakusi3[][106]=
{
	{"1���Ԃ��L�m�R"},
	{"2�����F���L�m�R"},
	{"3�����̃L�m�R"},
	{"4�������L�m�R"},
};
char sentakusi4[][106]=
{
	{"1���ۂ��L�m�R"},
	{"2���Ђ��`�̃L�m�R"},
	{"3��������^�̃L�m�R"},
	{"4�����^�̃L�m�R"},
};
char sentakusi5[][106]=
{
	{"1�����̂������ȃL�m�R"},
	{"2�����Ȃ������ȃL�m�R"},
	{"3�����킢�����ȃL�m�R"},
	{"4������߂������ȃL�m�R"},
};
char sentakusi6[][106]=
{
	{"1���̂��Ă���"},
	{"2���������ċA��"},
};
char sentakusi7[][106]=
{
	{"1����������"},
	{"2��������񌾂���"},
};
char sentakusi10[][106]=
{
	{"1���͂�"},
	{"2��������"},
};

////////////////////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstace, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow)
{	
	SetOutApplicationLogValidFlag(FALSE);
	SetWindowText("�L�m�R���");
	ChangeWindowMode(TRUE);
	SetDrawScreen( DX_SCREEN_BACK);
	SetGraphMode(640,480,16) ; 
	if(DxLib_Init() == -1) return -1;
	int sean=0;

	int house=LoadGraph("IMG/house.jpg");
	int housee=LoadGraph("IMG/house,evning.jpg");
	int forest=LoadGraph("IMG/forest,noon.jpg");
	int foreste=LoadGraph("IMG/forest,evning.jpg");
	int skye=LoadGraph("IMG/sky,evning.jpg");
	int skyn=LoadGraph("IMG/sky,night.jpg");
	int kusa=LoadGraph("IMG/kusa.jpg");
    int kusan=LoadGraph("IMG/kusa,naname.jpg");
	int nemoto=LoadGraph("IMG/nemoto.jpg");
	int rivern=LoadGraph("IMG/river,noon.jpg");
    int rivere=LoadGraph("IMG/river,evning.jpg");
	
	int Result=0;

	while(ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0 && ClearDrawScreen() == 0 && Fin == 0)
	{
		switch(sean)
		{
		case -1:
			Fin = 1;
			break;
		case 0:
			op();
			sean=1;
			break;
		case 1:
			text(ie);
			haikei(house);
			if(End == 1){sean = 2,End=0;}
			break;
		case 2:
			text(ie2);
			haikei(house);
			Result=sentaku2(sentakusi);
			if(Result==1)sean=3;
			if(Result==2)sean=20;
			break;
		case 3:
			text(ie3);
			haikei(house);
			if(End == 1){sean = 40,End=0;}
			break;
		case 40:
			text(ie4);
			haikei(house);
			if(End == 1){sean = 41,End=0;}
			break;
		case 41:
			text(ie5);
			haikei(house);
			Result=sentaku2(sentakusi7);
			if(Result==1)sean=42;
			if(Result==2)sean=40;
			break;
		case 42:
			text(ie6);
			haikei(house);
			if(End == 1){sean = 4,End=0;}
			break;
		case 4:
			text(michi);
			haikei(forest);
			Result=sentaku2(sentakusi2);
			if(Result==1){sean=43,Result=0;}
			if(Result==2)sean=21;
			break;
		case 43:
			text(michi10);
			haikei(forest);
			if(End == 1){sean = 5,End=0;}
			break;
		case 5:
			text(michi2);
			haikei(rivern);
			Result=sentaku2(sentakusi2);
			if(Result==1)sean=46;
			if(Result==2){sean=44,Result=0;}
			break;
		case 44:
			text(michi11);
			haikei(rivern);
			if(End == 1){sean = 6,End=0;}
			break;
		case 46:
			text(michi10);
			haikei(rivern);
			if(End == 1){sean = 23,End=0;}
			break;
		case 6:
			text(michi3);
			haikei(kusan);
			Result=sentaku2(sentakusi2);
			if(Result==1){sean=45,Result=0;}
			if(Result==2)sean=47;
			break;
		case 47:
			text(michi11);
			haikei(kusan);
			if(End == 1){sean = 23,End=0;}
			break;
		case 45:
			text(michi12);
			haikei(kusan);
			if(End == 1){sean = 7,End=0;}
			break;
		case 7:
			text(kinoko);
			haikei(kusa);
			Result=sentaku4(sentakusi3);
			if(Result==1)sean=26;
			if(Result==2)sean=26;
			if(Result==3)sean=26;
			if(Result==4){sean=8,Result=0;}
			break;
		case 8:
			text(kinoko2);
			haikei(kusa);
			Result=sentaku4(sentakusi4);
			if(Result==1)sean=26;
			if(Result==2){sean=9,Result=0;}
			if(Result==3)sean=26;
			if(Result==4)sean=26;
			break;
		case 9:
			text(kinoko3);
			haikei(kusa);
			Result=sentaku4(sentakusi5);
			if(Result==1)sean=26;
			if(Result==2)sean=26;
			if(Result==3){sean=10,Result=0;}
			if(Result==4)sean=26;
			break;
		case 10:
			text(kiro);
			haikei(kusa);
			if(End == 1){sean = 11,End=0;}
			break;
		case 11:
			text(kiro2);
			haikei(forest);
			if(End == 1){sean = 12,End=0;}
			break;
		case 12:
			text(kiro3);
			haikei(nemoto);
			Result=sentaku2(sentakusi6);
			if(Result==1)sean=32;
			if(Result==2){sean=13,Result=0;}
			break;
		case 13:
			text(kiro4);
			haikei(nemoto);
			if(End == 1){sean = 14,End=0;}
			break;
		case 14:
			text(kiro5);
			if(End == 1){sean = 15,End=0;}
			break;
		case 15:
			text(kiro6);
			haikei(skye);
			if(End == 1){sean = 16,End=0;}
			break;
		case 16:
			text(kiro7);
			haikei(rivere);
			if(End == 1){sean = 17,End=0;}
			break;
		case 17:
			text(kiro8);
			haikei(foreste);
			if(End == 1){sean = 18,End=0;}
			break;
		case 18:
			text(kitaku);
			haikei(housee);
			if(End == 1){sean = 53,End=0;}
			break;
		case 20:
			text(ookido);
			haikei(house);
			if(End == 1){sean = 2,End=0;}
			break;
		case 21:
			text(mayoi);
			haikei(forest);
			if(End == 1){sean=23,End=0;}
			break;
		case 22:
			text(mayoi2);
			haikei(forest);
			if(End == 1){sean=23,End=0;}
			break;
		case 23:
			text(mayoi3);
			haikei(foreste);
			if(End == 1){sean = 24,End=0;}
			break;
		case 24:
			text(mayoi4);
			haikei(skye);
			if(End == 1){sean = 25,End=0;}
			break;
		case 25:
			text(mayoi5);
			haikei(skyn);
			if(End == 1){sean = 50,End=0;}
			break;
		case 26:
			text(poison);
			haikei(kusa);
			if(End == 1){sean = 27,End=0;}
			break;
		case 27:
			text(poison2);
			if(End == 1){sean = 31,End=0;}
			break;
		case 31:
			text(poison3);
			if(End == 1){sean = 51,End=0;}
			break;
		case 32:
			text(wolf);
			if(End == 1){sean = 28,End=0;}
			break;
		case 28:
			text(wolf);
			haikei(skye);
			if(End == 1){sean = 29,End=0;}
			break;
		case 29:
			text(wolf2);
			haikei(skyn);
			if(End == 1){sean = 30,End=0;}
			break;
		case 30:
			text(wolf3);
			if(End == 1){sean = 52,End=0;}
			break;
		case 50:
			text(kyuusai);
			haikei(skyn);
			Result=sentaku2(sentakusi10);
			if(Result==1){sean=4,Result=0;}
			if(Result==2)Fin = 1;
			break;
		case 51:
			text(kyuusai);
			Result=sentaku2(sentakusi10);
			if(Result==1){sean=7,Result=0;}
			if(Result==2)Fin = 1;
			break;
		case 52:
			text(kyuusai);
			Result=sentaku2(sentakusi10);
			if(Result==1){sean=11,Result=0;}
			if(Result==2)Fin = 1;
			break;
		case 53:
			text(thanks);
			if(End == 1)Fin = 1;
			break;
		}

		DrawFormatString(10, 10, GetColor(255,255,255),"%d",sean);
		ScreenFlip();
		if(NFlag==1)NFlag=0;
		else WaitTimer(50);
		if(WaitFlag==1){
			WaitKey();
			WaitFlag=0;
		}
		if(WaitTime==1){
			WaitTimer(500);
			WaitTime=0;
		}
	}
	WaitKey();

	DxLib_End();
	return 0;
}

void op()
{
	int forestn=LoadGraph("IMG/forest,night.jpg");
	haikei(forestn);
	SetFontSize(50);
	DrawString(200,200,"�L�m�R���",GetColor(205,133,63));
	SetFontSize(40);
	DrawString(60,405,"�L�[���N���b�N�ŃX�^�[�g�I",GetColor(112,128,144));
	WaitKey();
	SetFontSize(16);
	
}

void text(char sean[])
{
	if(End==0){
		char Moji=sean[NowX];
		switch(Moji)
		{
		case 'E'://�I��
			End=1;
			NowX=0,tate=0,yoko=0,Dan=0;
			break;
		case 'U'://�i�グ
			Dan++;
			NowX++;
			NFlag=1;
			break;
		case 'B'://�{�^���҂�
			WaitFlag=1;
		default:
			tate=0;
			yoko=0;
			for(int i=0; i<=NowX; i+=2){
				for(int j=0; j<=3; j++)
					moji[j]='\0';
				moji[0]=sean[i];
				if(moji[0]=='N'){
					tate++;
					yoko=0;
					i--;
				}else if(moji[0]=='B'||moji[0]=='E'||moji[0]=='U'){
					i--;
				}else{
					moji[1]=sean[i+1];
					DrawString(yoko*TEXT_SIZE+10, 480-TEXT_SIZE*5-Dan*TEXT_SIZE+tate*TEXT_SIZE-12, moji, GetColor(255,255,255));
					yoko++;
					
				}
			}
			DrawBox(5, 480-20*5-5, 635,475, GetColor(255,255,255), FALSE);
			NowX++;
		}
	}
}


void haikei(int GraphHandle)
{
	DrawExtendGraph(0, 0, 680, 480-20*5-6+1, GraphHandle, FALSE);
}

int sentaku2(char Choice[][106])
{
	if(End==1){
		Dan=0;
		int res=0;
		for(int j=0; j<=1; j++){
			DrawFormatString(yoko*TEXT_SIZE+10, 480-TEXT_SIZE*(5-j)-Dan*TEXT_SIZE+tate*TEXT_SIZE-12, GetColor(255,255,255), Choice[j]);
		}
		DrawBox(5, 480-20*5-5, 635,475, GetColor(255,255,255), FALSE);
		if(CheckHitKey( KEY_INPUT_NUMPAD1)||CheckHitKey( KEY_INPUT_1)) res=1;
		if(CheckHitKey( KEY_INPUT_NUMPAD2)||CheckHitKey( KEY_INPUT_2)) res=2;
		if(res==1||res==2){
			DrawCircle(16, 485-20*(6-res)-2, 10, GetColor(255, 0, 0), FALSE, 1);
			WaitTime=1;
			End=0;
			return res;
		}
		else return 0;
	}
}

int sentaku4(char Choice[][106])
{
	if(End==1){
		Dan=0;
		int res=0;
		for(int j=0; j<=3; j++){
			DrawFormatString(yoko*TEXT_SIZE+10, 480-TEXT_SIZE*(5-j)-Dan*TEXT_SIZE+tate*TEXT_SIZE-12, GetColor(255,255,255), Choice[j]);
		}
		DrawBox(5, 480-20*5-5, 635,475, GetColor(255,255,255), FALSE);
		if(CheckHitKey( KEY_INPUT_NUMPAD1)||CheckHitKey( KEY_INPUT_1)) res=1;
		if(CheckHitKey( KEY_INPUT_NUMPAD2)||CheckHitKey( KEY_INPUT_2)) res=2;
		if(CheckHitKey( KEY_INPUT_NUMPAD3)||CheckHitKey( KEY_INPUT_3)) res=3;
		if(CheckHitKey( KEY_INPUT_NUMPAD4)||CheckHitKey( KEY_INPUT_4)) res=4;
		if(res==1||res==2||res==3||res==4){
			DrawCircle(16, 485-20*(6-res)-2, 10, GetColor(255, 0, 0), FALSE, 1);
			WaitTime=1;
			End=0;
			return res;
		}
		else return 0;
	}
}