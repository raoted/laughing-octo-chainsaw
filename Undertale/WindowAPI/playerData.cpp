#include "stdafx.h"
#include "playerData.h"

HRESULT playerData::init()
{
	return S_OK;
}

void playerData::release()
{
	saveMapToolData();
}

void playerData::saveMapToolData()
{
	sprintf_s(c, "%d", _mapAmount);
	INIDATA->addData("¸Ê", "mapAmount", c);
	INIDATA->saveINI("Data/System/Mapdata");

	sprintf_s(c, "%d", _tileAmount);
	INIDATA->addData("Å¸ÀÏ", "tileAmount", c);
	INIDATA->saveINI("Data/System/Tiledata");
}

void playerData::savePlayerData()
{
}
