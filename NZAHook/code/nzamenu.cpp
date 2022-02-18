#include "nzamenu.h"
#include <Windows.h>
#include <iostream>
#include "..\imgui\imgui.h"
#include <windows.h>
#include "eSettingsManager.h"
#include "helper/eKeyboardMan.h"
#include "..\eDirectX11Hook.h"
#include "..\utils\MemoryMgr.h"

using namespace Memory::VP;


NZAMenu* TheMenu = new NZAMenu();

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}


void NZAMenu::Initialize()
{
}

void NZAMenu::Draw()
{
	ImGui::GetIO().MouseDrawCursor = true;
	ImGui::Begin(GetNZAHookVersion(), &m_bIsActive, ImGuiWindowFlags_MenuBar);


	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Settings"))
		{
			m_bSubmenuActive[SUBMENU_SETTINGS] = true;
			ImGui::EndMenu();
		}
	}
	ImGui::EndMenuBar();

	if (ImGui::BeginTabBar("##tabs"))
	{
		if (ImGui::BeginTabItem("Camera"))
		{
			DrawCameraTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Speed"))
		{
			DrawSpeedTab();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::End();

	if (m_bSubmenuActive[SUBMENU_SETTINGS])
		DrawSettings();
}

void NZAMenu::Process()
{
	UpdateControls();
	*(bool*)(_addr(0xBA66DC)) = m_bFreeCam;
	*(bool*)(_addr(0xBA66DD)) = m_bFreezeCam;

	if (m_bSlowMotion)
		*(float*)(_addr(0xAADFA0)) = m_fSlowMotionSpeed;



}

void NZAMenu::UpdateControls()
{
	
}


void NZAMenu::DrawSpeedTab()
{
	ImGui::Text("Gamespeed Control");
	ImGui::InputFloat("Speed", &m_fSlowMotionSpeed, 0.1f);

	ImGui::Checkbox("Enable", &m_bSlowMotion);
}

void NZAMenu::DrawCameraTab()
{
	ImGui::Checkbox("Free Camera", &m_bFreeCam);
	ImGui::Checkbox("Freeze Camera", &m_bFreezeCam);


	CVector camPos = *(CVector*)(_addr(0xAB2DE8));
	ImGui::InputFloat3("X | Y | Z", &camPos.X);


}

void NZAMenu::DrawSettings()
{
	ImGui::SetNextWindowPos({ ImGui::GetIO().DisplaySize.x / 2.0f, ImGui::GetIO().DisplaySize.y / 2.0f }, ImGuiCond_Once, { 0.5f, 0.5f });
	ImGui::SetNextWindowSize({ 54 * ImGui::GetFontSize(), 54 * ImGui::GetFontSize() }, ImGuiCond_Once);
	ImGui::Begin("Settings", &m_bSubmenuActive[SUBMENU_SETTINGS]);

	static int settingID = 0;
	static const char* settingNames[] = {
		"Menu",
		"Keys"
	};

	enum eSettings {
		MENU,
		KEYS,
	};

	ImGui::BeginChild("##settings", { 12 * ImGui::GetFontSize(), 0 }, true);

	for (int n = 0; n < IM_ARRAYSIZE(settingNames); n++)
	{
		bool is_selected = (settingID == n);
		if (ImGui::Selectable(settingNames[n], is_selected))
			settingID = n;
		if (is_selected)
			ImGui::SetItemDefaultFocus();
	}

	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginChild("##content", { 0, -ImGui::GetFrameHeightWithSpacing() });

	switch (settingID)
	{
	case MENU:
		ImGui::TextWrapped("All user settings are saved to nzahook_user.ini.");
		ImGui::Text("Menu Scale");
		ImGui::InputFloat("", &SettingsMgr->fMenuScale);
		break;
	case KEYS:
		if (m_bPressingKey)
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.3f, 1.f), "Press a key!");

		if (ImGui::Button("Reset Keys", { -FLT_MIN, 0 }))
			SettingsMgr->ResetKeys();
		ImGui::Separator();
		ImGui::LabelText("", "Core");
		ImGui::Separator();
		KeyBind(&SettingsMgr->iHookMenuOpenKey, "Open/Close Menu", "menu");
		KeyBind(&SettingsMgr->iToggleSlowMoKey, "Toggle Gamespeed/Slow Motion", "slomo");
		ImGui::Separator();
		ImGui::LabelText("", "Camera");
		ImGui::Separator();

		KeyBind(&SettingsMgr->iToggleFreeCamera, "Toggle Free Camera", "freecam");
		KeyBind(&SettingsMgr->iToggleFreezeRotation, "Toggle Freeze Rotation", "freerot");
		/*
		KeyBind(&SettingsMgr->iFreeCameraKeyXMinus, "X-", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyXPlus, "X+", "x_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyXMinus, "X-", "x_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYPlus, "Y+", "y_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyYMinus, "Y-", "y_minus");
		KeyBind(&SettingsMgr->iFreeCameraKeyZPlus, "Z+", "z_plus");
		KeyBind(&SettingsMgr->iFreeCameraKeyZMinus, "Z-", "z_minus");
		*/
		ImGui::Separator();

		if (m_bPressingKey)
		{
			eVKKeyCode result = eKeyboardMan::GetLastKey();

			if (result >= VK_BACKSPACE && result < VK_KEY_NONE)
			{
				*m_pCurrentVarToChange = result;
				m_bPressingKey = false;
			}

		}
		break;
	default:
		break;
	}

	if (ImGui::Button("Save", { -FLT_MIN, 0 }))
	{
		eDirectX11Hook::ms_bShouldReloadFonts = true;
		SettingsMgr->SaveSettings();
	}

	ImGui::EndChild();

	ImGui::End();
}

void NZAMenu::DrawKeyBind(char* name, int* var)
{
	ImGui::SameLine();

	static char butName[256] = {};
	sprintf(butName, "%s##key%s", eKeyboardMan::KeyToString(*var), name);
	if (ImGui::Button(butName))
	{
		m_bPressingKey = true;
		m_pCurrentVarToChange = var;
	}
}

void NZAMenu::KeyBind(int* var, char* bindName, char* name)
{
	ImGui::LabelText("", bindName);
	DrawKeyBind(name, var);
}

#ifdef _DEBUG
void NZAMenu::DrawDebug()
{
	ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.95f);
	ImGui::SetNextWindowPos(ImVec2(10, 5));
	ImGui::Begin("devtext", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::PopStyleVar(1);
	ImGui::Text("NZAHook %s Debug (%.2f FPS)", NZAHOOK_VERSION, ImGui::GetIO().Framerate);
	ImGui::End();
}
#endif

bool NZAMenu::GetActiveState()
{
	return m_bIsActive;
}

char * GetNZAHookVersion()
{
	static char buffer[512] = {};
	sprintf(buffer, "NZAHook by ermaccer (%s)", NZAHOOK_VERSION);
	return buffer;
}
