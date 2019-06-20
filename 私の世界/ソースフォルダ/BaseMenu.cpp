#include "BaseMenu.h"
#include "main.h"

BaseMenu::BaseMenu(MenuChanger* changer) {
	pMenuChanger = changer;
}

BaseMenu::~BaseMenu() {
}

void BaseMenu::DrawBox1(int x, int y, int width, std::vector<std::string> select, int cursor){
	int size = select.size();
	int leng = 0;
	int curSize = 20;
	int textHeight = 20;
	for (int i = 0; i < size; i++)
		if (leng < GetDrawStringWidth(select[i].c_str(), strlen(select[i].c_str())))
			leng = GetDrawStringWidth(select[i].c_str(), strlen(select[i].c_str()));
	DrawBox(x, y, curSize + x + width * (leng + curSize), 10 + y + (size+width-1) / width * textHeight, Black1, TRUE);
	for (int i = 0; i < size; i++)
		DrawFormatString(curSize + x + i%width*(leng + curSize), 10 + y + i / width * textHeight, Black, "%s", select[i].c_str());
	DrawBox(x, y, curSize + x + width*(leng + curSize), 10 + y + (size+width-1) / width * textHeight, Black, FALSE);
	DrawCircle(curSize/2 + x + cursor%width*(leng + curSize), 20 + y + cursor/width * textHeight, 5, Black, TRUE);
}