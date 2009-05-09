#ifndef __CGAMEINFO_H__
#define __CGAMEINFO_H__
#include "global.h"

#include <vector>


/*
 * CGameInfo.h, part of VCMI engine
 *
 * Authors: listed in file AUTHORS in main folder
 *
 * License: GNU General Public License v2.0 or later
 * Full text of license available in license.txt file, in main folder
 *
 */

class CMapHandler;
class CArtHandler;
class CHeroHandler;
class CCreatureHandler;
class CAbilityHandler;
class CSpellHandler;
class CAmbarCendamo;
class CPreGameTextHandler;
class CBuildingHandler;
class CObjectHandler;
class CAudioHandler;
class CSemiLodHandler;
class CDefObjInfoHandler;
class CTownHandler;
class CLodHandler;
class CGeneralTextHandler;
class CConsoleHandler;
class CPathfinder;
class CCursorHandler;
class CScreenHandler;
class CGameState;
class CMapHandler;
class CGameInterface;
class CPreGame;
class CDefHandler;
/*
	CGameInfo class
	for allowing different functions for modifying game informations
*/
class CGameInfo
{
public:
	CGameState * state;
	CArtHandler * arth;
	CHeroHandler * heroh;
	CCreatureHandler * creh;
	CSpellHandler * spellh;
	CMapHandler * mh;
	CBuildingHandler * buildh;
	CObjectHandler * objh;
	CAudioHandler * audioh;
	CSemiLodHandler * sspriteh;
	CDefObjInfoHandler * dobjinfo;
	CTownHandler * townh;
	//CLodHandler * spriteh;
	//CLodHandler * bitmaph;
	CGeneralTextHandler * generaltexth;
	CConsoleHandler * consoleh;
	CCursorHandler * curh;
	CScreenHandler * screenh;

	CGameInfo();
	void setFromLib();
};


#endif // __CGAMEINFO_H__
