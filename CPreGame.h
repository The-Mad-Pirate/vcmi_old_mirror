#ifndef CPREGAME_H
#define CPREGAME_H
#include "SDL.h"
#include "CSemiDefHandler.h"
#include "CSemiLodHandler.h"
#include "CPreGameTextHandler.h" 
#include "CMessage.h"
#include "map.h"
#include "CMusicHandler.h"
class CPreGame;
extern CPreGame * CPG;
enum Ebonus {brandom=-1,bartifact, bgold, bresource};
struct StartInfo
{
	struct PlayerSettings
	{
		int castle, hero, heroPortrait; //ID, if -1 then random, if -2 then none
		std::string heroName;
		Ebonus bonus; 
		Ecolor color; //from 0 - 
		int handicap;//0-no, 1-mild, 2-severe
		std::string name;
	};
	std::vector<PlayerSettings> playerInfos;
	int turnTime; //in minutes, 0=unlimited
};
class PreGameTab
{
public:
	bool showed;
	virtual void init()=0;
	virtual void show()=0;
	virtual void hide()=0;
	PreGameTab();
};
class RanSel : public PreGameTab
{
	Button<> horcpl[9], horcte[9], conpl[9], conte[8], water[4], monster[4], //last is random
			size[4], twoLevel, showRand;
	CGroup<> *Ghorcpl, *Ghorcte, *Gconpl, *Gconte, *Gwater, *Gmonster, *Gsize;
};
class Options : public PreGameTab
{
	bool inited;
	struct OptionSwitch:public HighButton
	{
		void hover(bool on=true){};
		void select(bool on=true){};
		OptionSwitch( SDL_Rect Pos, CDefHandler* Imgs)
			:HighButton(Pos,Imgs,false,7)
			{selectable=false;highlightable=false;}
		void press(bool down=true);
		bool left;
		int playerID;
		int serialID;
		int which; //-1=castle;0=hero;1=bonus
	};
	struct PlayerOptions
	{
		PlayerOptions(int serial, int player);
		Ecolor color;
		//SDL_Surface * bg;
		OptionSwitch left, right;
		int nr;
	};
public:
	Slider<> * turnLength;
	SDL_Surface * bg,
		* rHero, * rCastle, * nHero, * nCastle;
	std::vector<SDL_Surface*> bgs;
	CDefHandler //* castles, * heroes, * bonus,
		* left, * right;
	std::vector<PlayerOptions*> poptions;
	void show();
	void hide();
	void init();
	Options(){inited=showed=false;};
	~Options();
};
class MapSel : public PreGameTab
{
public:
	ESortBy sortBy;
	SDL_Surface * bg;
	int selected;
	CDefHandler * Dtypes, * Dvic; 
	CDefHandler *Dsizes, * Dloss;
	std::vector<Mapa*> scenList;
	std::vector<SDL_Surface*> scenImgs;
	int current;
	std::vector<CMapInfo> ourMaps;
	IntBut<> small, medium, large, xlarge, all;
	SetrButton<> nrplayer, mapsize, type, name, viccon, loscon;
	Slider<>  *slid, *descslid;
	int sizeFilter;
	int whichWL(int nr);
	int countWL();
	void show();
	void hide();
	void init();
	std::string gdiff(std::string ss);
	void printMaps(int from,int to=18, int at=0, bool abs=false);
	void select(int which);
	void moveByOne(bool up);
	void printSelectedInfo();
	MapSel();
	~MapSel();
};
class ScenSel
{
public:
	bool listShowed;
	//RanSel ransel;
	MapSel mapsel;
	SDL_Surface * background, *scenInf, *scenList, *randMap, *options ;
	Button<> bScens, bOptions, bRandom, bBegin, bBack;
	IntSelBut<>	bEasy, bNormal, bHard, bExpert, bImpossible;
	Button<> * pressed;
	CPoinGroup<> * difficulty;
	std::vector<Mapa> maps;
	int selectedDiff;
	void initRanSel();
	void showRanSel();
	void hideRanSel();
	void genScenList();
	~ScenSel(){delete difficulty;};
} ;
class CPreGame
{
public:	
	PreGameTab* currentTab;
	StartInfo ret;
	bool run;
	std::vector<Slider<> *> interested;
	CMusicHandler * mush;
	CSemiLodHandler * slh ;
	std::vector<HighButton *> btns;
	CPreGameTextHandler * preth ;
	SDL_Rect * currentMessage;	
	SDL_Surface * behindCurMes;
	CDefHandler *ok, *cancel;
	enum EState { //where are we?
		mainMenu, newGame, loadGame, ScenarioList
	} state;
	struct menuItems { 
		SDL_Surface * background, *bgAd;
		CDefHandler *newGame, *loadGame, *highScores,*credits, *quit;
		SDL_Rect lNewGame, lLoadGame, lHighScores, lCredits, lQuit;
		ttt fNewGame, fLoadGame, fHighScores, fCredits, fQuit;
		int highlighted;//0=none; 1=new game; 2=load game; 3=high score; 4=credits; 5=quit
	} * ourMainMenu, * ourNewMenu;
	ScenSel * ourScenSel;
	Options * ourOptions;
	std::string map; //selected map
	std::vector<CSemiLodHandler *> handledLods; 
	CPreGame(); //c-tor
	std::string buttonText(int which);
	menuItems * currentItems();
	void(CPreGame::*handleOther)(SDL_Event&);
	void scenHandleEv(SDL_Event& sEvent);
	void begin(){run=false;};
	void quitAskBox();
	void quit(){exit(0);};  
	void initScenSel(); 
	void showScenSel();  
	void showScenList(); 
	void initOptions();
	void showOptions();  
	void initNewMenu(); 
	void showNewMenu();  
	void showMainMenu();  
	StartInfo runLoop(); // runs mainloop of PreGame
	void initMainMenu(); //loads components for main menu
	void highlightButton(int which, int on); //highlights one from 5 main menu buttons
	void showCenBox (std::string data); //
	void showAskBox (std::string data, void(*f1)(),void(*f2)());
	void hideBox ();
	void printMapsFrom(int from);
	void setTurnLength(int on);
	void sortMaps();
};

#endif //CPREGAME_H
