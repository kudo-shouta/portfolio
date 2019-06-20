#ifndef MENUBOX_H
#define MENUBOX_H

#include "main.h"
#include "BaseMenu.h"
#include "MenuChanger.h"

class MenuBox : public MenuChanger{
private:
	bool openFlag;  //�J���Ă��邩
	BaseMenu* menu;
public:
	MenuBox();
	~MenuBox();
	void Open();
	void Update();
	void Draw();
	bool isOpen();
	void Changemenu(const char* changer);
};

#endif // !MENU_H