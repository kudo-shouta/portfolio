#include "main.h"
#include "MenuBox.h"
#include "Start.h"
#include "MapEdit.h"
#include "Event.h"
#include "NPC.h"

#include <cstdlib>

MenuBox::MenuBox(){
	openFlag = FALSE;
}

MenuBox::~MenuBox(){
}

void MenuBox::Open(){
	if(openFlag == TRUE)
		delete menu;
	
	openFlag = TRUE;

	menu = (BaseMenu*) new Start(this);

	menu->Initialize();
}

void MenuBox::Update(){
	if(openFlag == TRUE)
		menu->Update();
}

void MenuBox::Draw(){
	if(openFlag == TRUE)

		menu->Draw();
}

bool MenuBox::isOpen(){
	return openFlag;
}

void MenuBox::Changemenu(const char* changer) {
	menu->Finalize();
	//delete menu;

	if (strcmp(changer, "とじる") == 0)
		openFlag = FALSE;
	
	if (strcmp(changer, "マップ") == 0) 
		menu = (BaseMenu*) new MapEdit(this);

	if (strcmp(changer, "スタート") == 0)
		menu = (BaseMenu*)  new Start(this);

	if (strcmp(changer, "移動設置") == 0)
		menu = (BaseMenu*) new Event(this);

	if (strcmp(changer, "ＮＰＣ") == 0)
		menu = (BaseMenu*) new NPC(this);

	menu->Initialize();
}