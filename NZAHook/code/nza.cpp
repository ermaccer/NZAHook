#include "nza.h"
#include "nzamenu.h"
#include <Windows.h>

int GetMKXEntryPoint()
{
	static int addr = reinterpret_cast<int>(GetModuleHandle(nullptr));
	return addr;
}

int _addr(int addr)
{
	return GetMKXEntryPoint() - 0x400000 + addr;
}

void Camera::SetPosition(CVector* pos)
{
	if (!TheMenu->m_bCustomCameraPos)
	{
		TheMenu->camPos = *pos;
		((void(__thiscall*)(Camera*, CVector*))_addr(0x721530))(this, pos);
	}
	else
		((void(__thiscall*)(Camera*, CVector*))_addr(0x721530))(this, &TheMenu->camPos);

}

void Camera::SetRotation(CVector* rot)
{
	if (!TheMenu->m_bFreezeCam)
		((void(__thiscall*)(Camera*, CVector*))_addr(0x612650))(this, rot);
}
