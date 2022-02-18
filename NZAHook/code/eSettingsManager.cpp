#include "eSettingsManager.h"
#include "..\IniReader.h"
#include <Windows.h>
eSettingsManager* SettingsMgr = new eSettingsManager;

void eSettingsManager::Init()
{
	CIniReader ini("");
	CIniReader user("nzahook_user.ini");
	// user first
	iToggleFreezeRotation = user.ReadInteger("Settings", "iToggleCustomCamKey", 0);

	iHookMenuOpenKey = user.ReadInteger("Settings", "iHookMenuOpenKey", 0xFF);

	if (iHookMenuOpenKey == 0xFF)
		iHookMenuOpenKey = ini.ReadInteger("Settings", "iHookMenuOpenKey", VK_F1);


	iToggleFreezeRotation = user.ReadInteger("Settings", "iToggleFreezeRotation", VK_F2);
	iToggleFreeCamera = user.ReadInteger("Settings", "iToggleFreeCamera", VK_F3);

	iToggleSlowMoKey = user.ReadInteger("Settings", "iToggleSlowMoKey", 0xFF);
	if (iToggleSlowMoKey == 0xFF)
		iToggleSlowMoKey = VK_F5;

	iFreeCameraKeyXPlus = user.ReadInteger("Settings", "iFreeCameraKeyXPlus", 0xFF);
	if (iFreeCameraKeyXPlus == 0xFF)
		iFreeCameraKeyXPlus = ini.ReadInteger("Settings", "iFreeCameraKeyXPlus", 0);

	iFreeCameraKeyXMinus = user.ReadInteger("Settings", "iFreeCameraKeyXMinus", 0xFF);
	if (iFreeCameraKeyXMinus == 0xFF)
		iFreeCameraKeyXMinus = ini.ReadInteger("Settings", "iFreeCameraKeyXMinus", 0);


	iFreeCameraKeyYPlus = user.ReadInteger("Settings", "iFreeCameraKeyYPlus", 0xFF);
	if (iFreeCameraKeyYPlus == 0xFF)
		iFreeCameraKeyYPlus = ini.ReadInteger("Settings", "iFreeCameraKeyYPlus", 0);

	iFreeCameraKeyYMinus = user.ReadInteger("Settings", "iFreeCameraKeyYMinus", 0xFF);
	if (iFreeCameraKeyYMinus == 0xFF)
		iFreeCameraKeyYMinus = ini.ReadInteger("Settings", "iFreeCameraKeyYMinus", 0);

	iFreeCameraKeyZPlus = user.ReadInteger("Settings", "iFreeCameraKeyZPlus", 0xFF);
	if (iFreeCameraKeyZPlus == 0xFF)
		iFreeCameraKeyZPlus = ini.ReadInteger("Settings", "iFreeCameraKeyZPlus", 0);

	iFreeCameraKeyZMinus = user.ReadInteger("Settings", "iFreeCameraKeyZMinus", 0xFF);
	if (iFreeCameraKeyZMinus == 0xFF)
		iFreeCameraKeyZMinus = ini.ReadInteger("Settings", "iFreeCameraKeyZMinus", 0);

	fMenuScale = user.ReadFloat("MenuSettings", "fMenuScale", 1.0f);

	if (fMenuScale < 1.0f)
		fMenuScale = 1.0f;

	bEnableConsoleWindow = ini.ReadBoolean("Settings", "bEnableConsoleWindow", true);
}

void eSettingsManager::SaveSettings()
{
	CIniReader user("nzahook_user.ini");
	user.WriteFloat("MenuSettings", "fMenuScale", fMenuScale);

	user.WriteInteger("Settings", "iHookMenuOpenKey", iHookMenuOpenKey);
	user.WriteInteger("Settings", "iToggleSlowMoKey", iToggleSlowMoKey);
	user.WriteInteger("Settings", "iToggleFreezeRotation", iToggleFreezeRotation);
	user.WriteInteger("Settings", "iToggleFreeCamera", iToggleFreeCamera);
	user.WriteInteger("Settings", "iFreeCameraKeyXPlus", iFreeCameraKeyXPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyXMinus", iFreeCameraKeyXMinus);
	user.WriteInteger("Settings", "iFreeCameraKeyYPlus", iFreeCameraKeyYPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyYMinus", iFreeCameraKeyYMinus);
	user.WriteInteger("Settings", "iFreeCameraKeyZPlus", iFreeCameraKeyZPlus);
	user.WriteInteger("Settings", "iFreeCameraKeyZMinus", iFreeCameraKeyZMinus);


	CIniReader ini("");
	ini.WriteBoolean("Settings", "bEnableConsoleWindow", bEnableConsoleWindow);
}

void eSettingsManager::ResetKeys()
{
	iHookMenuOpenKey = VK_F1;
	iToggleFreeCamera = VK_F2;
	iToggleFreezeRotation = VK_F3;
	iToggleFreezeRotation = 255;
	iToggleSlowMoKey = VK_F5;
	iFreeCameraKeyXPlus = 104;
	iFreeCameraKeyXMinus = 98;
	iFreeCameraKeyYPlus = 102;
	iFreeCameraKeyYMinus = 100;
	iFreeCameraKeyZPlus = 103;
	iFreeCameraKeyZMinus = 97;
}
