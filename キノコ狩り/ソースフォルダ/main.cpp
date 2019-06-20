#include "DxLib.h"

#define TEXT_SIZE 18
char moji[3];//現在表示するテキスト
char nmoji[3];
int NowX;//表示テキストの座標
int x;//参照する座標
int tate,yoko;//表示位置
int Dan;//何段あげるか
int NCount,NFlag;//改行
int Timer;//文字表示タイミングタイマー
int End,Fin;
int WaitFlag,WaitTime;

void text(char []);
void haikei(int);
int sentaku2(char [][106]);
int sentaku4(char [][106]);
void op();

//////////////ここに本文と選択肢を書いていく////////////////
char ie[]=
{
	"～ある日の朝～BUN"
	"少女　「おはよう」BUN"
	"母　「おはよう」BUN"
	"母　「ねえ」BUN"
	"少女　「なに？」BUN"
	"母　「森に生えてる珍しいキノコが必要なんだけど…N　　　お母さんちょっと今手が離せなくて」BE"
};
char ie2[]=
{
	"母　「採ってきてくれる？」BE"
};
char ookido[]=
{
	"母　「採ってきたほうがいいと思うなー」BE"	
};
char ie3[]=
{
	"少女　「いいよ」BUN"
	"母　「ありがとう」BE"
};
char ie4[]=
{
	"母　「まず、キノコの生えてる場所だけど、N　　　森に入って最初の分かれ道をひだり、二番目の分かれ道をみぎ、N　　　三番目の分かれ道をひだりに行くと着くわ」BUUUN"
	"母　「次に採ってきてほしいキノコだけど、N　　　白いキノコ、ひし形のキノコ、かわいそうなキノコの三つよ」BUUN"
	"母　「毒キノコに触らないように気をつけて」BUN"
	"母　「それと、近頃は夜になるとオオカミが出るみたいだからN　　　早く帰ってきてね」BE"
};
char ie5[]=
{
	"母　「分かった？」BE"
};
char ie6[]=
{
	"少女　「分かった」BUN"
	"少女　「行ってきます」BUN"
	"母　「いってらっしゃい。気をつけてね」BUN"
	"………BE"
};
char michi[]=
{
	"少女　「張り切っていこう」BUN"
	"少女　「遅くなる前に戻らなきゃ」BUN"
	"…BUN"
	"…BUN"
	"…BUN"
	"分かれ道だBN"
	"どっちに行こうかBE"
};
char michi10[]=
{
	"ひだりに行こうBUN"
	"…BE"
};
char michi2[]=
{
	"まだまだ先は長いBUN"
	"少女　「がんばろう」BUN"
	"…BUN"
	"…BUN"
	"…BUN"
	"また分かれ道だBN"
	"どっちに行こうかBE"
};
char michi11[]=
{
	"みぎに行こうBUN"
	"…BE"
};
char michi3[]=
{
	"少女　（もう少しで着くかなぁ）BUN"
	"…BUN"
	"…BUN"
	"…BUN"
	"またまた分かれ道だBN"
	"どっちに行こうかBE"
};
char michi12[]=
{
	"ひだりに行こうBUN"
	"…BE"
};
char kinoko[]=
{
	"少女　「着いた！」BUN"
	"少女　（キノコはあるかな）BUN"
	"少女　「あった！」BUN"
	"少女　（う～ん…どれだっけ…）BE"
};
char kinoko2[]=
{
	"少女　「これかなぁ」BUN"
	"少女　（他にはあるかな）BUN"
	"少女　「あった」BUN"
	"少女　（どれだろう…）BE"
};
char kinoko3[]=
{
	"少女　「たぶんこれだ」BUN"
	"少女　（他にはないかな）BUN"
	"少女　「あった」BUN"
	"少女　（最後は…）BE"
};
char kiro[]=
{
	"少女　「これで全部かな」BUN"
	"少女　「帰ろう」BUN"
	"………BE"
};
char kiro2[]=
{
	"少女　（けっこう時間かかっちゃったな。N　　　　早く帰らないと）BUUN"
	"…BUN"
	"…BE"
};
char kiro3[]=
{
	"少女　「あっ！」BUN"
	"少女　（お母さんの好きなハーブだ）BUN"
	"少女　（お母さんは遅くなっちゃいけないって言ってたけど…N　　　　少しだけならいいよね）BE"
};
char kiro4[]=
{
	"少女　（…これ以上遅くなるとお母さんも心配する）BUN"
	"少女　（やっぱり帰ろう）BE"
};
char kiro5[]=
{
	"………BE"
};
char kiro6[]=
{
	"少女　（日が暮れてきた）BUN"
	"少女　（早く帰らなきゃ）BE"
};
char kiro7[]=
{
	"…BUN"
	"…BE"
};
char kiro8[]=
{
	"…BUN"
	"…BE"
};
char kitaku[]=
{
	"少女　（やっと着いた）BUN"
	"少女　「ただいまー」BUN"
	"母　「おかえりー」BUN"
	"母　「………うん。頼んだものは全部あるね」BUN"
	"母　「ありがとう。助かったよ」BUN"
	"………BUN"
	"☆Game clear☆BE"
};
char mayoi[]=
{
	"みぎに行こうBE"
};

char mayoi2[]=
{
	"ひだりに行こうBE"
};
char mayoi3[]=
{
	"…BE"
};
char mayoi4[]=
{
	"…BE"
};
char mayoi5[]=
{
	"…BUN"
	"道に迷ってしまった…BUN"
	"-Game over- BE"
};
char kyuusai[]=
{
	"＜途中からやり直しますか？＞BE"
};
char poison[]=
{
	"！？BE"
};
char poison2[]=
{
	"なんだか…急に意識が………BE"
};
char poison3[]=
{
	"-Game over- BE"
};
char wolf[]=
{
	"………BE"
};
char wolf2[]=
{
	"少女　「遅くなっちゃった」BUN"
	"…BUN"
	"ガサッBUN"
	"少女　「！？」BUN"
	"少女　「誰か……いるの……？」BUN"
	"？？？　「グルルルルル…」BE"
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
	{"1→いいよ"},
	{"2→めんどくさい"},
};
char sentakusi2[][106]=
{
	{"1→左"},
	{"2→右"},
};
char sentakusi3[][106]=
{
	{"1→赤いキノコ"},
	{"2→黄色いキノコ"},
	{"3→紫のキノコ"},
	{"4→白いキノコ"},
};
char sentakusi4[][106]=
{
	{"1→丸いキノコ"},
	{"2→ひし形のキノコ"},
	{"3→だえん型のキノコ"},
	{"4→星型のキノコ"},
};
char sentakusi5[][106]=
{
	{"1→たのしそうなキノコ"},
	{"2→かなしそうなキノコ"},
	{"3→かわいそうなキノコ"},
	{"4→うらめしそうなキノコ"},
};
char sentakusi6[][106]=
{
	{"1→採っていく"},
	{"2→無視して帰る"},
};
char sentakusi7[][106]=
{
	{"1→分かった"},
	{"2→もう一回言って"},
};
char sentakusi10[][106]=
{
	{"1→はい"},
	{"2→いいえ"},
};

////////////////////////////////////////////////////////////
int WINAPI WinMain( HINSTANCE hInstace, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow)
{	
	SetOutApplicationLogValidFlag(FALSE);
	SetWindowText("キノコ狩り");
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
	DrawString(200,200,"キノコ狩り",GetColor(205,133,63));
	SetFontSize(40);
	DrawString(60,405,"キーかクリックでスタート！",GetColor(112,128,144));
	WaitKey();
	SetFontSize(16);
	
}

void text(char sean[])
{
	if(End==0){
		char Moji=sean[NowX];
		switch(Moji)
		{
		case 'E'://終了
			End=1;
			NowX=0,tate=0,yoko=0,Dan=0;
			break;
		case 'U'://段上げ
			Dan++;
			NowX++;
			NFlag=1;
			break;
		case 'B'://ボタン待ち
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