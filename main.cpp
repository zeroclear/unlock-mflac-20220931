
#include <Windows.h>

//2022.10.01测试正常

BOOL SetComponent(HMODULE hDll);
void RunWinUI();

HMODULE LoadComponent()
{
	HMODULE hDll=LoadLibrary(L"CommonFunction.dll");
	if (hDll==NULL)
	{
		MessageBox(NULL,L"组件加载失败",L"error",MB_OK);
		return NULL;
	}
	if (!SetComponent(hDll))
	{
		FreeLibrary(hDll);
		MessageBox(NULL,L"组件加载失败",L"error",MB_OK);
		return NULL;
	}
	return hDll;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	HMODULE hDll=LoadComponent();
	if (hDll!=NULL)
	{
		RunWinUI();
		FreeLibrary(hDll);
	}
	return 0;
}