#pragma once
#include <Windows.h>

// as usual, pluginmh/mugenhook

class eSettingsManager {
public:
	void Init();

	bool bEnableConsoleWindow;
	bool bEnable60FPSFrontend;

	bool bEnableGamepadSupport;

	// generic
	int iHookMenuOpenKey;
	int iToggleFreezeRotation;
	int iToggleFreeCamera;
	int iToggleSlowMoKey = VK_F5;
	// free camera keys
	int iFreeCameraKeyXPlus;
	int iFreeCameraKeyXMinus;
	int iFreeCameraKeyYPlus;
	int iFreeCameraKeyYMinus;
	int iFreeCameraKeyZPlus;
	int iFreeCameraKeyZMinus;

	float fMenuScale;

	void SaveSettings();
	void ResetKeys();

};

extern eSettingsManager* SettingsMgr;