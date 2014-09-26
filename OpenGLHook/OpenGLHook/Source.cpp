#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include <stdio.h>
#include <tlhelp32.h>
#include <iostream>


void HookOpenGL()
{

}

DWORD WINAPI HookThread(LPVOID)
{
	HookOpenGL();

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE instance, DWORD reason, LPVOID lpReserved)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, HookThread, 0, 0, 0);
	}
	return TRUE;
}