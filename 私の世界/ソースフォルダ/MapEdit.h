#ifndef MAPEDIT_H
#define MAPEDIT_H

#include "main.h"
#include "BaseMenu.h"
#include "fmfmap.h"

class MapEdit : public BaseMenu {
private:
	//-1で初期　0=fmf　1=png　2=名前決定
	int Mode;
	int cursor;
	int pngSelect;
	int fmfSelect;
	std::vector<std::string> select;
	std::vector<std::string> png;
	std::vector<std::string> fmf;
	std::vector<std::string> finish;
	std::vector<std::string> move;
	std::string mapName;

	struct Map{
		std::string fmf;
		std::string png;
	};
	std::vector<Map> map;
	CFmfMap* fmfMap;

	bool isLoad;
	bool isName;
public:
	MapEdit(MenuChanger* changer);
	~MapEdit();
	bool Initialize();
	void Finalize();
	void Update();
	void Draw();
	void Load();
	void Save();
};

#endif //!MAPEDIT_H