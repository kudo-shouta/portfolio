#include "main.h"
#include "Fps.h"

//FPSを一定にする
void WaitFpsConstant(int fps) {
	static int beforeTime = 0;	//前回の時間

	//前回からの時間経過
	int elapsedTime = GetNowCount() - beforeTime;

	//FPSから余った時間を計算する
	int oneFrame = 1000 / fps;
	if ((oneFrame - elapsedTime) > 0) {
		//余りがあれば一定時間待つ
		Sleep(oneFrame - elapsedTime);
	}
	//今回の時間の記録
	beforeTime = GetNowCount();
}

//FPSの表示
void DrawFps(int fps) {
	static int beforeTime = 0, aveTime = 0, frameTime[100], count = 0;

	//フレーム当たりの時間を記録
	if (count == 0) {
		frameTime[count % fps] = 0;
	}
	else {
		frameTime[count % fps] = GetNowCount() - beforeTime;
	}
	beforeTime = GetNowCount();
	count++;

	//最後のフレーム
	if ((count % fps) == (fps - 1)) {
		//平均フレーム時間を計算する
		aveTime = 0;
		for (int i = 0; i < fps; i++) {
			aveTime += frameTime[i];
		}
		aveTime /= fps;
	}
	//平均フレームが決まっていたら表示する
	if (aveTime != 0) {
		DrawFormatString(0, 0, Black, "%.1fFPS", 1000.0 / (double)aveTime);
		DrawFormatString(0, 20, Black, "%dms", aveTime);
	}
}