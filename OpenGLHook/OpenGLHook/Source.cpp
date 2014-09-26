#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <string>
#include <stdio.h>
#include <tlhelp32.h>
#include <iostream>

typedef void (APIENTRY *glBeginType)(GLenum);
typedef void (APIENTRY *glEndType)(void);
typedef void (APIENTRY *glClearType)(GLbitfield);
typedef void (APIENTRY *glVertex3fvType)(const GLfloat *vertex);
typedef void (APIENTRY *glVertex3fType)(GLfloat xCoord, GLfloat yCoord, GLfloat zCoord);
typedef void (APIENTRY *glEnableType)(GLenum);

glBeginType hookedglBegin = NULL;
glEndType hookedglEnd = NULL;
glClearType hookedglClear = NULL;
glVertex3fvType hookedglVertex3fv = NULL;
glVertex3fType hookedglVertex3f = NULL;
glEnableType hookedGlEnable = NULL;


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