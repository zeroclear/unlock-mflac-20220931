
#include "resource.h"
#include <Windows.h>

DWORD QmflacDecrypt(BYTE* InData,DWORD Size,BYTE** OutDataPtr,DWORD* ErrorInfo);

void OnInitDialog(HWND hWndDlg)
{

}

void OnClose(HWND hWndDlg)
{

}

void OnPaint(HDC hDC)
{
	//Graphics g(hDC);
}

void OnCommand(HWND hWndDlg,int nCtlID,int nNotify)
{
	//nNotify==1,accelerator
	//nNotify==0,menu
	if (nNotify==BN_CLICKED && nCtlID==IDB_SAVE)
	{
		int Len=GetWindowTextLength(GetDlgItem(hWndDlg,IDE_PATH));
		if (Len!=0)
		{
			WCHAR* Name=new WCHAR[Len+1];
			GetDlgItemText(hWndDlg,IDE_PATH,Name,Len+1);
			HANDLE hFileR=CreateFile(Name,GENERIC_READ,0,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
			if (hFileR==INVALID_HANDLE_VALUE)
			{
				MessageBox(hWndDlg,L"无法打开文件",L"ok",MB_OK);
			}
			else
			{
				DWORD Size=GetFileSize(hFileR,NULL);
				BYTE* ReadBuf=new BYTE[Size];
				memset(ReadBuf,0,Size);
				ReadFile(hFileR,ReadBuf,Size,&Size,NULL);

				BYTE* OutData=NULL;
				DWORD ErrorInfo=0;
				Size=QmflacDecrypt(ReadBuf,Size,&OutData,&ErrorInfo);

				WCHAR LogBuf[MAX_PATH+20];
				if (Size==0)
				{
					wsprintf(LogBuf,L"解锁失败 %08X",ErrorInfo);
					MessageBox(hWndDlg,LogBuf,L"ok",MB_OK);
				}
				else
				{
					memcpy(Name+Len-5,L"flac\0",5*sizeof(WCHAR));
					HANDLE hFileW=CreateFile(Name,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
					WriteFile(hFileW,OutData,Size,&Size,NULL);
					CloseHandle(hFileW);
					delete[] OutData;

					wsprintf(LogBuf,L"解锁成功，保存为：\n%s",Name);
					MessageBox(hWndDlg,LogBuf,L"ok",MB_OK);
				}

				delete[] ReadBuf;
				CloseHandle(hFileR);
			}
			delete[] Name;
		}
	}
}

void OnTimer(HWND hWndDlg,int nTmrID)
{

}

void OnDropFile(HWND hWndDlg,HDROP hDrop)
{
	RECT Area;
	GetWindowRect(GetDlgItem(hWndDlg,IDS_DROP),&Area);
	POINT Pos;
	DragQueryPoint(hDrop,&Pos);
	if (PtInRect(&Area,Pos))
	{
		int Num=DragQueryFile(hDrop,0xFFFFFFFF,NULL,0);
		if (Num!=1)
		{
			MessageBox(hWndDlg,L"只支持每次1个文件",L"ok",MB_OK);
		}
		else
		{
			int Len=DragQueryFile(hDrop,0,NULL,0);
			WCHAR* Name=new WCHAR[Len+1];
			Len=DragQueryFile(hDrop,0,Name,Len+1);
			//C:\1.mflac
			if (Len<10 || memcmp(Name+Len-6,L".mflac",6*sizeof(WCHAR))!=0)
				MessageBox(hWndDlg,L"只支持.mflac文件",L"ok",MB_OK);
			else
				SetDlgItemText(hWndDlg,IDE_PATH,Name);
			delete[] Name;
		}
	}
	DragFinish(hDrop);
}

INT_PTR CALLBACK DialogProc(HWND hwndDlg,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		OnInitDialog(hwndDlg);
		return TRUE;
	case WM_CLOSE:
		OnClose(hwndDlg);
		EndDialog(hwndDlg,0);
		return TRUE;
	case WM_ERASEBKGND:
		return FALSE;	//nonzero if erased background
	case WM_PAINT:
		PAINTSTRUCT ps;
		BeginPaint(hwndDlg,&ps);
		OnPaint(ps.hdc);
		EndPaint(hwndDlg,&ps);
		return TRUE;
	case WM_COMMAND:
		OnCommand(hwndDlg,LOWORD(wParam),HIWORD(wParam));
		return TRUE;
	case WM_TIMER:
		OnTimer(hwndDlg,wParam);
		return TRUE;
	case WM_DROPFILES:
		OnDropFile(hwndDlg,(HDROP)wParam);
		return TRUE;
	}
	return FALSE;
}

void RunWinUI()
{
	DialogBoxParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG1),NULL,DialogProc,0);
}


