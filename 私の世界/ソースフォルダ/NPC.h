#pragma once

#include "main.h"
#include "BaseMenu.h"
#include "fmfmap.h"

class NPC :public BaseMenu {
private:
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
	t_npc add;
	int addNumber;

	typedef struct {
		int npcNumber;
		std::string mapName;
		int x;
		int y;
	}t_Warp;
	std::vector<t_Warp> warp;
	t_Warp addWarp;
	int WarpAddNumber;
	int newWarp;
	std::vector<std::string> warpList;
	int warpSelect;
	int mode9;
	int sentaku;
	std::vector<std::string> sentakusi;
	
	typedef struct {
		char name[1024];
	}t_flag;
	std::vector<t_flag> flag;
	std::vector<std::string> Boxflag;
	t_flag addflag;

	int selectFlagCursor;

	typedef struct {
		int x;
		int y;
	}t_cursor;
	t_cursor cursor;
	int pngcursor;
	bool isLoadpng;
	int gr;

	bool isNew;
	bool newFlag;

	vector<std::string> NPCgr;

	int nowmap;
	int mode;
	CFmfMap *fmf;
	int width, height;
	int px, py;

	/*typedef struct {
		int x;
		int y;
		std::string name;
		int giveFlag;
		int needFlagNum;
		std::vector<int> needFlag;
		std::string self;
	}t_npc;*/

	int InputHandle;
	std::string InputBuf;
	bool isMakeHandle;

	std::vector<std::string> select;
	int selectCursor;
public:
	NPC(MenuChanger *changer);
	~NPC();
	bool Initialize();
	void Finalize();
	void Update();
	void Draw();
	void Save();
	void Load(const char *name);
	int Key();
};