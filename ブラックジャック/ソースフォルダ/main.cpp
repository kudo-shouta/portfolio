#include "DxLib.h"
#include "Outside.h"
#include "Inside.h"


//
//const int INIT = 0;
//#define int PLAYERTURN 1;
//#define int DEALERTURN 2;
//#define int RESULT 3;

struct status Player;
struct status Dealer;
extern int pcnt;
extern int dcnt;

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//AllocConsole();
	//FILE *fp;
	//freopen_s(&fp, "CONOUT$", "w", stdout);
	//freopen_s(&fp, "CONIN$", "r", stdin);

	SetWindowText("ブラックジャック");
	SetOutApplicationLogValidFlag(FALSE);
	if( ChangeWindowMode(TRUE) == -1) return -1;
	if( DxLib_Init() == -1) return -1;
	if( SetDrawScreen( DX_SCREEN_BACK) == -1) return -1;
	if( SetBackgroundColor( 100, 255, 0) == -1) return -1;

	int PStand=0,DStand=0;
	int EndFlag = 0;
	int Mode = INIT;
	int Result = 0;

	while( ProcessMessage() == 0 && EndFlag != 1)
	{	
		clsDx();
		ClearDrawScreen();

		//printfDx("mode=%d\n", Mode);
		//printfDx("pstand=%d\n", Player.StandFlag);
		//printfDx("dstand=%d\n", Dealer.StandFlag);

		switch(Mode)
		{
		case INIT:
			Card_Init();
			Deck_Init();
			Init(&Player);//初期手札の決定
			Init(&Dealer);

			Mode = PLAYERTURN;
			break;
		case PLAYERTURN:
			if (Player.StandFlag == 0) {
				Result = GetKey();
				if(Result == -1)
					break;
				else if(Result == 0){
					DrawCard(&Player);
					pcnt=0;
				}
				else if(Result == 1)
					Player.StandFlag = 1;
			}
			if(Player.Sum > 21) Dealer.StandFlag = 1;
			if(Player.StandFlag == 1 && Dealer.StandFlag == 1)
			{
				Mode = RESULT;
				break;
			}

			Mode = DEALERTURN;
			break;
		case DEALERTURN:
			if ( Dealer.StandFlag == 0){
			Result = DealerJude(&Dealer);
			if(Result == -1)
				break;
			else if(Result == 0){
				DrawCard(&Dealer);
				dcnt=0;
			}
			else if(Result == 1)
				Dealer.StandFlag = 1;
			}

			if(Player.StandFlag == 1 && Dealer.StandFlag == 1)
			{
				Mode = RESULT;
				break;
			}

			Mode = PLAYERTURN;
			break;
		case RESULT:
			if(pcnt==-1 && dcnt==-1){
				EndText(Player, Dealer);
				int res = ContinueText();
				EndFlag = res;
				if(res == 0){
					Mode = INIT;
				}
			}
		}

		
		GraphDraw(Mode, &Player, &Dealer);
		ScreenFlip();
		


		//PStand = GetKey();
		//DStand = DealerJude();
		//Hit(&Player, PStand);
		//GiveCard();
		//Text();
		//Stand = End();
		//if(Stand == 1) break;
	}
	WaitKey();

	DxLib_End();

	FreeConsole();

	return 0;
}