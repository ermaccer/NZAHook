#pragma once
#include "nza.h"
#include <Windows.h>
// as usual, based on mh2 debug menu

enum eCustomCameras {
	CAMERA_3RDPERSON,
	CAMERA_3RDPERSON2,
	CAMERA_1STPERSON,
	CAMERA_1STPERSON_MID,
	CAMERA_MK11,
	CAMERA_HEAD_TRACKING,
	TOTAL_CUSTOM_CAMERAS
};

enum eMenuSubMenus {
	SUBMENU_SETTINGS,
	TOTAL_SUBMENUS
};


class NZAMenu {
public:
	bool	 m_bIsActive = false;
	bool	 m_bSubmenuActive[TOTAL_SUBMENUS] = {};
	bool     m_bPressingKey = false;
	bool	 m_bSlowMotion = false;
	bool	 m_bIsFocused = false;
	bool	 m_bCustomCameraPos = false;
	bool	 m_bFreezeCam = false;
	bool	 m_bFreeCam = false;
	
	int* m_pCurrentVarToChange = nullptr;

	float	 m_fSlowMotionSpeed = 0.5f;



	CVector  camPos;

	void Initialize();
	void Draw();
	void Process();
	void UpdateControls();

	void DrawSpeedTab();
	void DrawCameraTab();

	void DrawSettings();


	void DrawKeyBind(char* name, int* var);
	void KeyBind(int* var, char* bindName, char* name);

#ifdef _DEBUG
	static void DrawDebug();
#endif



	bool GetActiveState();
};


extern NZAMenu* TheMenu;
char* GetNZAHookVersion();