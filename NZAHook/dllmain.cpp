// dllmain.cpp : Defines the entry point for the DLL application.
#include "eDirectX11Hook.h"
#include "utils/MemoryMgr.h"
#include "code/eSettingsManager.h"
#include "code/nzamenu.h"
#include "code/nza.h"

using namespace Memory::VP;


bool ValidateGameVersion() 
{
	char* gameName = (char*)_addr(0x932E90);

	if (strncmp(gameName, "SniperEliteV2", strlen("SniperEliteV2")) == 0)
		return true;
	else
	{
		MessageBoxA(0, "Invalid game version!\nNZAHook only supports latest Steam executable.\n\n"
			"If you still cannot run the plugin and made sure that the game is updated, NZAHook needs to be updated.", 0, MB_ICONINFORMATION);
		return false;
	}
}

void ImGuiInputWatcher()
{
	while (true)
	{
		if (eDirectX11Hook::ms_bInit)
		{
			ImGuiIO& io = ImGui::GetIO();
			POINT mPos;
			GetCursorPos(&mPos);
			ScreenToClient(eDirectX11Hook::ms_hWindow, &mPos);
			io.MousePos.x = mPos.x;
			io.MousePos.y = mPos.y;
			io.MouseDown[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
			io.MouseDown[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
			io.MouseDown[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;
		}
		Sleep(1);
	}
}

void CreateDX11Hook()
{
	((void(__cdecl*)())_addr(0x7EC580))();
	CreateThread(nullptr, 0, DirectXHookThread, nullptr, 0, nullptr);
}

void Init()
{
	if (SettingsMgr->bEnableConsoleWindow)
	{
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
	}
	printf("NZAHook::OnInitializeHook() | Begin!\n");

	TheMenu->Initialize();
	InjectHook(_addr(0x5C5473), CreateDX11Hook);
	//InjectHook(_addr(0x5E5495), &Camera::SetPosition); 
	//InjectHook(_addr(0x7B2147), &Camera::SetRotation);
}


BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		if (ValidateGameVersion())
		{
			SettingsMgr->Init();
			Init();
			eDirectX11Hook::Init();
			DisableThreadLibraryCalls(hMod);
			CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(ImGuiInputWatcher), nullptr, 0, nullptr);
		}
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}