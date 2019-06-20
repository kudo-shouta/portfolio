#ifndef MENUCHANGER_H
#define MENUCHANGER_H

#include "main.h"

class MenuChanger {
public:
	virtual ~MenuChanger();
	virtual void Changemenu(const char* OpenMenu);
};

#endif //!MENUCHANGER_H