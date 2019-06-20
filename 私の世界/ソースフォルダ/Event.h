#pragma once

#include "main.h"
#include "BaseMenu.h"
#include "fmfmap.h"

class Event :public BaseMenu {
private:
	CFmfMap *fmf;
	int width, height;
	int px, py;

	typedef struct {
		int x;
		int y;
	}t_cursor;
	t_cursor cursor;
	t_cursor connectcursor;
	

	typedef struct{
		int x;
		int y;
		int mapid;
		int xx;
		int yy;
	}t_event;
	std::vector<t_event> event;
	std::vector<string> selectmap;
	std::vector<string> passege;
	int MenuFlag;
	int movecursor;
	int fin;
	int isLoad;
public:
	Event(MenuChanger *changer);
	~Event();
	bool Initialize();
	void Finalize();
	void Update();
	void Draw();
	void Load(const char *name);
	void Save();
	void SelectEvent();
	void MapMove();
};