#ifndef PLAYER_H
#define PLAYER_H

#include "main.h"
#include "fmfmap.h"
#include "BaseMenu.h"

class Player {
private:
	bool oneLoad;

	int PlayerGr[3 * 4];
	int PlayerGr2[3 * 4];
	int PlayerGr3[3 * 4];
	int Anime, AnimeDire;
	int Dire;
	int FrameCount;
	int px, py;
	int speed;
	CFmfMap* fmfMap;
	BaseMenu* menu;
	bool NowisTalk;
	int TalkMan;

	bool haveFlag[100];

	typedef struct {
		int x;
		int y;
		int mapid;
		int xx;
		int yy;
	}t_event;
	std::vector<t_event> event;

	typedef struct {
		int x;
		int y;
		char name[1024];
		char serif[1024];
		char changSerif[1024];
		char seletserif[2][1024];
		int walkFlag;
		int giveFlag;
		std::vector<int> needFlag;
		std::string gr;
		int npcNumber;
	}t_npc;
	std::vector<t_npc> npc;
	int NPCgr[50][3*4];

	typedef struct {
		int npcNumber;
		std::string mapName;
		int x;
		int y;
	}t_Warp;
	std::vector<t_Warp> warp;
	int WarpCount[50][50]; //[マップ番号][キャラクター番号]
	int WarpSkip;

	int nowmap;

	char buf[4][1024];
	int Ncount;
	int mojisize;
	int nowMoji;
	int TalkMode;
	bool TalkOn;
	bool selectStart;
	vector<string> YesNo;
	int cursor;
	char serifBuf[1024];

	bool chang;

	bool end;
	int Endscene;
public:
	Player(CFmfMap *fmf);
	bool Initialize();
	void Finalize();
	void Update(bool menuisOpen);
	void Draw();
	int getpx();
	int getpy();
	void LoadEvent();
	void HitEvent();
	void LoadNPC();
	void isTalk();
	void InputSerif(char buf[1024]);
	void Ending();
};

#endif // !PLAYER_H
