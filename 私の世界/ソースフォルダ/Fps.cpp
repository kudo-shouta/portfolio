#include "main.h"
#include "Fps.h"

//FPS�����ɂ���
void WaitFpsConstant(int fps) {
	static int beforeTime = 0;	//�O��̎���

	//�O�񂩂�̎��Ԍo��
	int elapsedTime = GetNowCount() - beforeTime;

	//FPS����]�������Ԃ��v�Z����
	int oneFrame = 1000 / fps;
	if ((oneFrame - elapsedTime) > 0) {
		//�]�肪����Έ�莞�ԑ҂�
		Sleep(oneFrame - elapsedTime);
	}
	//����̎��Ԃ̋L�^
	beforeTime = GetNowCount();
}

//FPS�̕\��
void DrawFps(int fps) {
	static int beforeTime = 0, aveTime = 0, frameTime[100], count = 0;

	//�t���[��������̎��Ԃ��L�^
	if (count == 0) {
		frameTime[count % fps] = 0;
	}
	else {
		frameTime[count % fps] = GetNowCount() - beforeTime;
	}
	beforeTime = GetNowCount();
	count++;

	//�Ō�̃t���[��
	if ((count % fps) == (fps - 1)) {
		//���σt���[�����Ԃ��v�Z����
		aveTime = 0;
		for (int i = 0; i < fps; i++) {
			aveTime += frameTime[i];
		}
		aveTime /= fps;
	}
	//���σt���[�������܂��Ă�����\������
	if (aveTime != 0) {
		DrawFormatString(0, 0, Black, "%.1fFPS", 1000.0 / (double)aveTime);
		DrawFormatString(0, 20, Black, "%dms", aveTime);
	}
}