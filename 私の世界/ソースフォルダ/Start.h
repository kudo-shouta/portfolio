#ifndef START_H
#define START_H

#include "BaseMenu.h"

class Start : public BaseMenu {
private:
	int cursor;		//�J�[�\���ʒu
	std::vector<std::string> select;
public:
	Start(MenuChanger* changer);
	~Start();
	bool Initialize();
	void Finalize();
	void Update();
	void Draw();
};

#endif //!START_H