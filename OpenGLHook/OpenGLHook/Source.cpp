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

bool toggle = false;



void *DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp - len);
}

void HookOpenGL()
{
	HMODULE openGLModule = GetModuleHandle("opengl32.dll");
	hookedglVertex3fv = (glVertex3fvType)DetourFunc((LPBYTE)GetProcAddress(openGLModule, "glVertex3fv"), (LPBYTE)&Hooked_glVertex3fv, 6);
	hookedglVertex3f = (glVertex3fType)DetourFunc((LPBYTE)GetProcAddress(openGLModule, "glVertex3f"), (LPBYTE)&Hooked_glVertex3f, 6);
	hookedglBegin = (glBeginType)DetourFunc((LPBYTE)GetProcAddress(openGLModule, "glBegin"), (LPBYTE)&Hooked_glBegin, 6);
	hookedglEnd = (glEndType)DetourFunc((LPBYTE)GetProcAddress(openGLModule, "glEnd"), (LPBYTE)&Hooked_glEnd, 6);
	hookedglClear = (glClearType)DetourFunc((LPBYTE)GetProcAddress(openGLModule, "glClear"), (LPBYTE)&Hooked_glClear, 7);
	hookedGlEnable = (glEnableType)DetourFunc((LPBYTE)GetProcAddress(openGLModule, "glEnable"), (LPBYTE)&Hooked_glEnable, 6);
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