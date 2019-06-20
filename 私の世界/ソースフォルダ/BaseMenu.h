#ifndef BASEMENU_H
#define BASEMENU_H

#include "MenuChanger.h"

class BaseMenu {
private:
	int cursor;
protected:
	MenuChanger* pMenuChanger;
public:
	BaseMenu(MenuChanger* changer);
	virtual ~BaseMenu() = 0;
	virtual bool Initialize() = 0;
	virtual void Finalize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	void DrawBox1(int x, int y, int width, std::vector<std::string> select, int cursor);
};

#endif //!BASEMENU_H