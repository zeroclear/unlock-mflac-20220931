
#include <Windows.h>
#pragma warning(disable:4102)	//未引用的标签

//utility
//////////////////////////////////////////////////////////////////////////
BYTE* Base64AllocEncodeBuf(int RawDataSize)
{
	int Size=(RawDataSize+2)/3*3*4;
	return new BYTE[Size];
}

BYTE* Base64AllocDecodeBuf(int Base64DataSize)
{
	int Size=Base64DataSize/4*3;
	return new BYTE[Size];
}

void Base64FreeBuf(BYTE* Base64Buf)
{
	if (Base64Buf!=NULL)
		delete [] Base64Buf;
}

int Base64Encode(BYTE* RawData,int RawSize,BYTE* OutBuf)
{
	const BYTE Base64Table[64]={
		'A','B','C','D','E','F','G','H',
		'I','J','K','L','M','N','O','P',
		'Q','R','S','T','U','V','W','X',
		'Y','Z','a','b','c','d','e','f',
		'g','h','i','j','k','l','m','n',
		'o','p','q','r','s','t','u','v',
		'w','x','y','z','0','1','2','3',
		'4','5','6','7','8','9','+','/'}; 

		BYTE* pDest=OutBuf;
		BYTE* pSrc=RawData;
		int nDestCur=0,nSrcCur=0;
		while (nSrcCur+3<=RawSize)
		{
			pDest[nDestCur]=Base64Table[(pSrc[nSrcCur]>>2)&63];
			pDest[nDestCur+1]=Base64Table[((pSrc[nSrcCur]<<4)|(pSrc[nSrcCur+1]>>4))&63];
			pDest[nDestCur+2]=Base64Table[((pSrc[nSrcCur+1]<<2)|(pSrc[nSrcCur+2]>>6))&63];
			pDest[nDestCur+3]=Base64Table[pSrc[nSrcCur+2]&63];
			nDestCur+=4;
			nSrcCur+=3;
		}
		if (RawSize%3==1)
		{
			pDest[nDestCur]=Base64Table[(pSrc[nSrcCur]>>2)&63];
			pDest[nDestCur+1]=Base64Table[(pSrc[nSrcCur]<<4)&63];
			pDest[nDestCur+2]='=';
			pDest[nDestCur+3]='=';
			nDestCur+=4;
		}
		else if (RawSize%3==2)
		{
			pDest[nDestCur]=Base64Table[(pSrc[nSrcCur]>>2)&63];
			pDest[nDestCur+1]=Base64Table[((pSrc[nSrcCur]<<4)|(pSrc[nSrcCur+1]>>4))&63];
			pDest[nDestCur+2]=Base64Table[(pSrc[nSrcCur+1]<<2)&63];
			pDest[nDestCur+3]='=';
			nDestCur+=4;
		}
		return nDestCur;
}

int Base64Decode(BYTE* Base64Data,int Base64Size,BYTE* OutBuf)
{
	const BYTE Base64TableRev[256]={
	//	0                     7  8                    15
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
	//	16                   23  24                   31
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
	//	32                   39  40       '+'         '/'
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,62,-1,-1,-1,63,
	//	'0'                  '7' '8'            '='   63
		52,53,54,55,56,57,58,59, 60,61,-1,-1,-1,-1,-1,-1,
	//	64 'A'               'G' 'H'                  'P'
		-1, 0, 1, 2, 3, 4, 5, 6,  7, 8, 9,10,11,12,13,14,
	//	'Q'                  'W' 'X'   'Z'            95
		15,16,17,18,19,20,21,22, 23,24,25,-1,-1,-1,-1,-1,
	//	96 'a'               'g' 'h'                  'p'
		-1,26,27,28,29,30,31,32, 33,34,35,36,37,38,39,40,
	//	'q'                  'w' 'x'   'z'            127
		41,42,43,44,45,46,47,48, 49,50,51,-1,-1,-1,-1,-1,
	//	128-255
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1,
		-1,-1,-1,-1,-1,-1,-1,-1, -1,-1,-1,-1,-1,-1,-1,-1};

		if (Base64Size%4!=0)
			return -1;

		BYTE* pDest=OutBuf;
		BYTE* pSrc=Base64Data;
		int nDestCur=0,nSrcCur=0;
		Base64Size-=4;

		while (nSrcCur<Base64Size)
		{
			if (Base64TableRev[pSrc[nSrcCur]]==0xFF ||
				Base64TableRev[pSrc[nSrcCur+1]]==0xFF ||
				Base64TableRev[pSrc[nSrcCur+2]]==0xFF ||
				Base64TableRev[pSrc[nSrcCur+3]]==0xFF)
				return -1;

			pDest[nDestCur]=(Base64TableRev[pSrc[nSrcCur]]<<2)|(Base64TableRev[pSrc[nSrcCur+1]]>>4);
			pDest[nDestCur+1]=(Base64TableRev[pSrc[nSrcCur+1]]<<4)|(Base64TableRev[pSrc[nSrcCur+2]]>>2);
			pDest[nDestCur+2]=(Base64TableRev[pSrc[nSrcCur+2]]<<6)|(Base64TableRev[pSrc[nSrcCur+3]]);
			nDestCur+=3;
			nSrcCur+=4;
		}

		if (Base64TableRev[pSrc[nSrcCur]]==0xFF || Base64TableRev[pSrc[nSrcCur+1]]==0xFF)
			return -1;
		if (pSrc[nSrcCur+2]=='=' && pSrc[nSrcCur+3]=='=')
		{
			pDest[nDestCur]=(Base64TableRev[pSrc[nSrcCur]]<<2)|(Base64TableRev[pSrc[nSrcCur+1]]>>4);
			nDestCur+=1;
		}
		else if (pSrc[nSrcCur+3]=='=')
		{
			pDest[nDestCur]=(Base64TableRev[pSrc[nSrcCur]]<<2)|(Base64TableRev[pSrc[nSrcCur+1]]>>4);
			pDest[nDestCur+1]=(Base64TableRev[pSrc[nSrcCur+1]]<<4)|(Base64TableRev[pSrc[nSrcCur+2]]>>2);
			nDestCur+=2;
		}
		else
		{
			if (Base64TableRev[pSrc[nSrcCur+2]]==0xFF || Base64TableRev[pSrc[nSrcCur+3]]==0xFF)
				return -1;
			pDest[nDestCur]=(Base64TableRev[pSrc[nSrcCur]]<<2)|(Base64TableRev[pSrc[nSrcCur+1]]>>4);
			pDest[nDestCur+1]=(Base64TableRev[pSrc[nSrcCur+1]]<<4)|(Base64TableRev[pSrc[nSrcCur+2]]>>2);
			pDest[nDestCur+2]=(Base64TableRev[pSrc[nSrcCur+2]]<<6)|(Base64TableRev[pSrc[nSrcCur+3]]);
			nDestCur+=3;
		}
		return nDestCur;
}


//platform
//////////////////////////////////////////////////////////////////////////
struct DecryptInfo_t
{
	DWORD* VTable;	//+00
	BYTE* Key;		//+04
	DWORD Size;		//+08
	DWORD Pad;		//+0C
	CRITICAL_SECTION Lock;	//+10
	DWORD Hash;		//+28
	BYTE* SBox;		//+2C
};

DWORD ObjVTable[6]={0x3760,0x3770,0x3860,0x3960,0x3AE0,0x36F0};
DWORD EntryOffset=0x3960;

DWORD BaseAddr=NULL;

BOOL SetComponent(HMODULE hDll)
{
	if (memcmp((BYTE*)hDll+EntryOffset,"\x55\x8B\xEC\x6A\xFF\x68\x68\x60",8)!=0)
		return FALSE;
	BaseAddr=(DWORD)hDll;
	return TRUE;
}

_declspec(naked)
	DWORD _stdcall DecryptEntry(DWORD OffsetLow,DWORD OffsetHigh,BYTE* EncData,DWORD DataSize,DecryptInfo_t* DecInfo,DWORD CallAddr)
{
	_asm
	{
		push ebp;
		mov ebp,esp;

		mov eax,dword ptr [ebp+0x1C];
		mov ecx,dword ptr [ebp+0x18];
		push dword ptr [ebp+0x14];
		push dword ptr [ebp+0x10];
		push dword ptr [ebp+0x0C];
		push dword ptr [ebp+0x08];
		call eax;	//stdcall

		mov esp,ebp;
		pop ebp;
		ret 0x18;
	}
}

DWORD CallDecrypt(BYTE* EncData,DWORD DataSize,BYTE* KeyData,DWORD KeySize,DWORD KeyHash,BYTE* SBox)
{
	DWORD VTable[6];
	for (int i=0;i<6;i++)
		VTable[i]=ObjVTable[i]+BaseAddr;
	DecryptInfo_t DecryptInfo;
	DecryptInfo.VTable=VTable;
	DecryptInfo.Key=KeyData;
	DecryptInfo.Size=KeySize;
	DecryptInfo.Pad=0;
	InitializeCriticalSection(&DecryptInfo.Lock);
	DecryptInfo.Hash=KeyHash;
	DecryptInfo.SBox=SBox;

	DWORD Result=DecryptEntry(0,0,EncData,DataSize,&DecryptInfo,EntryOffset+BaseAddr);

	DeleteCriticalSection(&DecryptInfo.Lock);
	return Result;
}


//cryptology
//////////////////////////////////////////////////////////////////////////
//bswap
DWORD ReverseDword(DWORD d)
{
	DWORD d1=(d>>0)&0xFF;
	DWORD d2=(d>>8)&0xFF;
	DWORD d3=(d>>16)&0xFF;
	DWORD d4=(d>>24)&0xFF;
	return (d1<<24)|(d2<<16)|(d3<<8)|(d4<<0);
}

void _fastcall UpdateIV(BYTE* InData,char* StrKey,BYTE* OutData)
{
	DWORD Key2[4];
	Key2[0]=ReverseDword(*(DWORD*)(StrKey+0));
	Key2[1]=ReverseDword(*(DWORD*)(StrKey+4));
	Key2[2]=ReverseDword(*(DWORD*)(StrKey+8));
	Key2[3]=ReverseDword(*(DWORD*)(StrKey+12));

	DWORD Base64Key1=ReverseDword(*(DWORD*)(InData+0));
	DWORD Base64Key2=ReverseDword(*(DWORD*)(InData+4));

	DWORD Magic=0xE3779B90;
	for (int i=0;i<16;i++)
	{
		DWORD Key1Result=((Base64Key1>>5)+Key2[3]) ^ ((Base64Key1<<4)+Key2[2]) ^ (Magic+Base64Key1);
		Base64Key2-=Key1Result;
		DWORD Key2Result=((Base64Key2>>5)+Key2[1]) ^ ((Base64Key2<<4)+Key2[0]) ^ (Magic+Base64Key2);
		Base64Key1-=Key2Result;
		Magic+=0x61C88647;
	}

	*(DWORD*)(OutData+0)=ReverseDword(Base64Key1);
	*(DWORD*)(OutData+4)=ReverseDword(Base64Key2);
}

//这个函数极有可能是TEA的oi_symmetry_decrypt2，不想再费脑细胞分析了
//看到中文注释吃了一惊，为什么叫TEA呢，难道是Tencent Encrypt Algorithm？
//https://github.com/TarsCloud/TarsCpp/blob/c6304ad1f9098a3d348bb2ef79df989060117b62/util/src/tc_tea.cpp
//其实我是后来发现https://gist.github.com/ix64/bcd72c151f21e1b050c9cc52d6ff27d5才知道的
//尽管他们分享的资料有所帮助，但是有一说一，把这样的灰色项目做成web版，恨不得别人都去部署甚至盈利，多少沾点NT
_declspec(naked)
	BOOL _fastcall QQMusicDecryptKey(BYTE* RawData,DWORD RawSize,BYTE* MixKey,BYTE* Buffer,DWORD* OutputSize)
{
	_asm
	{
A_565F4755: push ebp
A_565F4756: mov ebp,esp
A_565F4758: sub esp,0x34
A_565F475B: xor eax,eax;	//mov eax,dword ptr [0x565F900C] 栈检查相关
A_565F4760: nop;			//xor eax,ebp 栈检查相关
A_565F4762: mov dword ptr [ebp-0x04],eax;	//栈信息
A_565F4765: mov eax,dword ptr [ebp+0x0C]
A_565F4768: push ebx
A_565F4769: mov ebx,edx
A_565F476B: mov dword ptr [ebp-0x2C],eax
A_565F476E: push esi
A_565F476F: mov eax,ebx
A_565F4771: mov dword ptr [ebp-0x30],ebx
A_565F4774: mov esi,ecx
A_565F4776: mov ecx,dword ptr [ebp+0x08]
A_565F4779: mov dword ptr [ebp-0x34],ecx
A_565F477C: push edi
A_565F477D: mov edi,dword ptr [ebp+0x10]
A_565F4780: and eax,0x80000007
A_565F4785: jns A_565F478E
A_565F4787: dec eax
A_565F4788: or eax,0xFFFFFFF8
A_565F478B: add eax,0x01
A_565F478E: jne A_565F4978
A_565F4794: cmp ebx,0x10
A_565F4797: jl A_565F4978
A_565F479D: lea eax,dword ptr [ebp-0x0C]
A_565F47A0: mov edx,ecx
A_565F47A2: push eax
A_565F47A3: mov ecx,esi
A_565F47A5: call UpdateIV
A_565F47AA: nop;	//pop ecx 改为msvc版的fastcall栈平衡
A_565F47AB: movzx ecx,byte ptr [ebp-0x0C]
A_565F47AF: mov eax,ebx
A_565F47B1: and ecx,0x07
A_565F47B4: sub eax,ecx
A_565F47B6: sub eax,0x0A
A_565F47B9: cmp dword ptr [edi],eax
A_565F47BB: jl A_565F4978
A_565F47C1: test eax,eax
A_565F47C3: js A_565F4978
A_565F47C9: push 0x08
A_565F47CB: mov dword ptr [edi],eax
A_565F47CD: xor eax,eax
A_565F47CF: pop edx
A_565F47D0: nop;	//cmp eax,edx 不可能跳转
A_565F47D2: nop;	//jae A_565F4989
A_565F47D8: mov byte ptr [ebp+eax-0x14],0x00
A_565F47DD: inc eax
A_565F47DE: cmp eax,edx
A_565F47E0: jl A_565F47D2
A_565F47E2: lea eax,dword ptr [ebp-0x14]
A_565F47E5: mov dword ptr [ebp-0x20],esi
A_565F47E8: add esi,edx
A_565F47EA: mov dword ptr [ebp-0x24],eax
A_565F47ED: mov dword ptr [ebp-0x18],edx
A_565F47F0: lea ebx,dword ptr [ebp-0x0C]
A_565F47F3: xor eax,eax
A_565F47F5: mov edx,esi
A_565F47F7: inc eax
A_565F47F8: inc ecx
A_565F47F9: sub edx,ebx
A_565F47FB: mov dword ptr [ebp-0x1C],eax
A_565F47FE: mov dword ptr [ebp-0x28],edx
A_565F4801: mov ebx,dword ptr [ebp-0x18]
A_565F4804: cmp ecx,0x08
A_565F4807: jge A_565F4848
A_565F4809: inc ecx
A_565F480A: inc eax
A_565F480B: mov dword ptr [ebp-0x1C],eax
A_565F480E: mov edx,dword ptr [ebp-0x18]
A_565F4811: cmp eax,0x02
A_565F4814: jle A_565F4801
A_565F4816: mov edi,dword ptr [edi]
A_565F4818: test edi,edi
A_565F481A: je A_565F48FF
A_565F4820: mov ebx,esi
A_565F4822: lea eax,dword ptr [ebp-0x0C]
A_565F4825: sub ebx,eax
A_565F4827: cmp ecx,0x08
A_565F482A: jge A_565F48A4
A_565F482C: mov eax,dword ptr [ebp-0x24]
A_565F482F: mov edx,dword ptr [ebp-0x2C]
A_565F4832: mov al,byte ptr [ecx+eax]
A_565F4835: xor al,byte ptr [ebp+ecx-0x0C]
A_565F4839: inc dword ptr [ebp-0x2C]
A_565F483C: inc ecx
A_565F483D: mov byte ptr [edx],al
A_565F483F: dec edi
A_565F4840: mov edx,dword ptr [ebp-0x18]
A_565F4843: jmp A_565F48F7
A_565F4848: jne A_565F480E
A_565F484A: mov eax,dword ptr [ebp-0x20]
A_565F484D: mov edx,ebx
A_565F484F: xor ecx,ecx
A_565F4851: mov dword ptr [ebp-0x24],eax
A_565F4854: mov dword ptr [ebp-0x20],esi
A_565F4857: sub edx,esi
A_565F4859: mov eax,dword ptr [ebp-0x28]
A_565F485C: lea ebx,dword ptr [ebp-0x0C]
A_565F485F: add ebx,ecx
A_565F4861: add eax,ebx
A_565F4863: add eax,edx
A_565F4865: cmp eax,dword ptr [ebp-0x30]
A_565F4868: jge A_565F4978
A_565F486E: mov eax,dword ptr [ebp-0x28]
A_565F4871: push 0x08
A_565F4873: mov al,byte ptr [eax+ebx]
A_565F4876: xor byte ptr [ebx],al
A_565F4878: inc ecx
A_565F4879: pop ebx
A_565F487A: cmp ecx,ebx
A_565F487C: jl A_565F4859
A_565F487E: mov edx,dword ptr [ebp-0x34]
A_565F4881: lea eax,dword ptr [ebp-0x0C]
A_565F4884: push eax
A_565F4885: mov ecx,eax
A_565F4887: call UpdateIV
A_565F488C: mov edx,dword ptr [ebp-0x18]
A_565F488F: add esi,ebx
A_565F4891: add dword ptr [ebp-0x28],ebx
A_565F4894: add edx,ebx
A_565F4896: mov eax,dword ptr [ebp-0x1C]
A_565F4899: nop;	//pop ecx 改为msvc版的fastcall栈平衡
A_565F489A: mov dword ptr [ebp-0x18],edx
A_565F489D: xor ecx,ecx
A_565F489F: jmp A_565F4811
A_565F48A4: jne A_565F48F7
A_565F48A6: mov eax,dword ptr [ebp-0x20]
A_565F48A9: xor ecx,ecx
A_565F48AB: mov dword ptr [ebp-0x24],eax
A_565F48AE: sub edx,esi
A_565F48B0: mov dword ptr [ebp-0x20],esi
A_565F48B3: lea eax,dword ptr [ebp-0x0C]
A_565F48B6: add eax,ecx
A_565F48B8: add eax,ebx
A_565F48BA: mov dword ptr [ebp-0x1C],eax
A_565F48BD: add eax,edx
A_565F48BF: cmp eax,dword ptr [ebp-0x30]
A_565F48C2: jge A_565F4978
A_565F48C8: mov eax,dword ptr [ebp-0x1C]
A_565F48CB: mov al,byte ptr [eax]
A_565F48CD: xor byte ptr [ebp+ecx-0x0C],al
A_565F48D1: inc ecx
A_565F48D2: cmp ecx,0x08
A_565F48D5: jl A_565F48B3
A_565F48D7: mov edx,dword ptr [ebp-0x34]
A_565F48DA: lea eax,dword ptr [ebp-0x0C]
A_565F48DD: push eax
A_565F48DE: mov ecx,eax
A_565F48E0: call UpdateIV
A_565F48E5: mov edx,dword ptr [ebp-0x18]
A_565F48E8: add esi,0x08
A_565F48EB: add edx,0x08
A_565F48EE: add ebx,0x08
A_565F48F1: nop;	//pop ecx 改为msvc版的fastcall栈平衡
A_565F48F2: mov dword ptr [ebp-0x18],edx
A_565F48F5: xor ecx,ecx
A_565F48F7: test edi,edi
A_565F48F9: jne A_565F4827
A_565F48FF: xor edx,edx
A_565F4901: lea eax,dword ptr [ebp-0x0C]
A_565F4904: inc edx
A_565F4905: mov edi,esi
A_565F4907: mov dword ptr [ebp-0x1C],edx
A_565F490A: sub edi,eax
A_565F490C: cmp ecx,0x08
A_565F490F: jge A_565F4924
A_565F4911: mov eax,dword ptr [ebp-0x24]
A_565F4914: mov al,byte ptr [ecx+eax]
A_565F4917: cmp al,byte ptr [ebp+ecx-0x0C]
A_565F491B: jne A_565F4978
A_565F491D: inc ecx
A_565F491E: inc edx
A_565F491F: mov dword ptr [ebp-0x1C],edx
A_565F4922: jmp A_565F496E
A_565F4924: jne A_565F496E
A_565F4926: mov eax,dword ptr [ebp-0x20]
A_565F4929: xor ecx,ecx
A_565F492B: mov edx,dword ptr [ebp-0x18]
A_565F492E: mov dword ptr [ebp-0x24],eax
A_565F4931: sub edx,esi
A_565F4933: mov dword ptr [ebp-0x20],esi
A_565F4936: lea ebx,dword ptr [ebp-0x0C]
A_565F4939: add ebx,ecx
A_565F493B: lea eax,dword ptr [edi+ebx]
A_565F493E: add eax,edx
A_565F4940: cmp eax,dword ptr [ebp-0x30]
A_565F4943: jge A_565F4978
A_565F4945: mov al,byte ptr [edi+ebx]
A_565F4948: xor byte ptr [ebx],al
A_565F494A: inc ecx
A_565F494B: cmp ecx,0x08
A_565F494E: jl A_565F4936
A_565F4950: mov edx,dword ptr [ebp-0x34]
A_565F4953: lea eax,dword ptr [ebp-0x0C]
A_565F4956: push eax
A_565F4957: mov ecx,eax
A_565F4959: call UpdateIV
A_565F495E: add dword ptr [ebp-0x18],0x08
A_565F4962: add esi,0x08
A_565F4965: mov edx,dword ptr [ebp-0x1C]
A_565F4968: add edi,0x08
A_565F496B: nop;	//pop ecx 改为msvc版的fastcall栈平衡
A_565F496C: xor ecx,ecx
A_565F496E: cmp edx,0x07
A_565F4971: jle A_565F490C
A_565F4973: xor eax,eax
A_565F4975: inc eax
A_565F4976: jmp A_565F497A
A_565F4978: xor eax,eax
A_565F497A: mov ecx,dword ptr [ebp-0x04];	//栈信息
A_565F497D: pop edi
A_565F497E: pop esi
A_565F497F: nop;	//xor ecx,ebp 栈检查相关
A_565F4981: pop ebx
A_565F4982: nop;	//call CheckStack_ecx
A_565F4987: leave
A_565F4988: ret 0x0C;	//ret 改为msvc版的fastcall栈平衡
A_565F4989: nop;	//call ErrorReport
A_565F498E: nop;	//int3
	}
}

BYTE* MixKey1=(BYTE*)"386ZJY!@#*$%^&)(";
BYTE* MixKey2=(BYTE*)"**#!(#$%&^a1cZ,T";
//和libm_sse2_tan_precise有关，有一个8字节的浮点参数106
BYTE HalfMixKey[8]={0x69,0x56,0x46,0x38,0x2B,0x20,0x15,0x0B};

DWORD BuildMixKey3(BYTE* DecData,BYTE* HalfMixKey,BYTE* OutBuf)
{
	//理论上可以生成更多，实际只用到16字节
	for (int i=0;i<8;i++)
	{
		OutBuf[i*2]=HalfMixKey[i];
		OutBuf[i*2+1]=DecData[i];
	}
	return 16;
}

DWORD ShuffleSBox(BYTE* EncKey,DWORD KeySize,BYTE* BoxBuf)
{
	for (int i=0;i<KeySize;i++)
		BoxBuf[i]=i;

	int r=0;
	for (int i=0;i<KeySize;i++)
	{
		BYTE t=BoxBuf[i];
		int r2=i%KeySize;	//i总是小于KeySize，实际没有效果
							//推测上面的循环用了i%KeySize，下面顺手复制过来
		r=(EncKey[r2]+BoxBuf[i]+r)%KeySize;
		BoxBuf[i]=BoxBuf[r];
		BoxBuf[r]=t;
	}

	DWORD Hash=1;
	for (int i=0;i<KeySize;i++)
	{
		if (EncKey[i]!=0)
		{
			DWORD t=EncKey[i]*Hash;
			if (t==0)
				break;
			if (t<=Hash)
				break;
			Hash=t;
		}
	}
	return Hash;
}

DWORD QmflacDecryptInternal(BYTE* InData,DWORD Size,DWORD Type,BYTE** OutDataPtr)
{
	Size=Size-4-1;
	DWORD Key1Size=Type-1;
	BYTE* Key1Data=InData+Size-Key1Size;

	BYTE* Key2Data=Base64AllocDecodeBuf(Key1Size);
	DWORD Key2Size=Base64Decode(Key1Data,Key1Size,Key2Data);

	if (memcmp(Key2Data,"QQMusic EncV2,Key:",18)!=0)
	{
		Base64FreeBuf(Key2Data);
		return 0;
	}

	DWORD Key3Size=Key2Size*2;
	BYTE* Key3Data=new BYTE[Key3Size];
	BOOL DecOK=QQMusicDecryptKey(Key2Data+18,Key2Size-18,MixKey1,Key3Data,&Key3Size);
	if (DecOK==FALSE)
	{
		Base64FreeBuf(Key2Data);
		delete[] Key3Data;
		return 0;
	}

	DWORD Key4Size=Key3Size*2;
	BYTE* Key4Data=new BYTE[Key4Size];
	DecOK=QQMusicDecryptKey(Key3Data,Key3Size,MixKey2,Key4Data,&Key4Size);
	if (DecOK==FALSE)
	{
		Base64FreeBuf(Key2Data);
		delete[] Key3Data;
		delete[] Key4Data;
		return 0;
	}

	BYTE* Key5Data=Base64AllocDecodeBuf(Key4Size);
	DWORD Key5Size=Base64Decode(Key4Data,Key4Size,Key5Data);

	BYTE MixKey3[16];
	BuildMixKey3(Key5Data,HalfMixKey,MixKey3);

	DWORD Key6Size=Key5Size*2;
	BYTE* Key6Data=new BYTE[Key6Size];
	DecOK=QQMusicDecryptKey(Key5Data+8,Key5Size-8,MixKey3,Key6Data,&Key6Size);
	if (DecOK==FALSE)
	{
		Base64FreeBuf(Key2Data);
		delete[] Key3Data;
		delete[] Key4Data;
		Base64FreeBuf(Key5Data);
		delete[] Key6Data;
		return 0;
	}

	DWORD FinalSize=Key6Size+8;
	BYTE* FinalKey=new BYTE[FinalSize];
	memcpy(FinalKey,Key5Data,8);
	memcpy(FinalKey+8,Key6Data,Key6Size);

	BYTE* SBoxBuf=NULL;
	DWORD KeyHash=0;
	if (Type>300)
	{
		SBoxBuf=new BYTE[FinalSize];
		KeyHash=ShuffleSBox(FinalKey,FinalSize,SBoxBuf);
	}

	DWORD DecSize=Size-Key1Size;
	BYTE* DecBuf=new BYTE[DecSize];
	memcpy(DecBuf,InData,DecSize);
	DecSize=CallDecrypt(DecBuf,DecSize,FinalKey,FinalSize,KeyHash,SBoxBuf);

	Base64FreeBuf(Key2Data);
	delete[] Key3Data;
	delete[] Key4Data;
	Base64FreeBuf(Key5Data);
	delete[] Key6Data;
	delete[] FinalKey;
	if (SBoxBuf!=NULL)
		delete[] SBoxBuf;

	if (DecSize==0)
		delete[] DecBuf;
	else
		*OutDataPtr=DecBuf;
	return DecSize;
}


//interface
//////////////////////////////////////////////////////////////////////////
//成功时返回解密后的字节数，OutDataPtr指向解密后的数据
//失败时返回0，OutDataPtr为NULL
//如果因为加密类型无法识别而失败，ErrorInfo返回类型信息
//如果因为其他情况失败，ErrorInfo返回0xFFFFFFFF
//此函数负责为解密分配内存，若OutDataPtr返回了指针，调用者使用完毕后，应通过operator delete[]释放内存
DWORD QmflacDecrypt(BYTE* InData,DWORD Size,BYTE** OutDataPtr,DWORD* ErrorInfo)
{
	*OutDataPtr=NULL;
	if (Size<4)
	{
		if (ErrorInfo!=NULL)
			*ErrorInfo=0xFFFFFFFF;
		return 0;
	}

	DWORD Type=*(DWORD*)(InData+Size-4);
	if (Type!=0x3F1 && Type!=0x225)
	{
		if (ErrorInfo!=NULL)
			*ErrorInfo=Type;
		return 0;
	}
	else
	{
		//用unlock-music 1.11解锁的早期文件，会比现在多出几百字节，不知道为什么
		DWORD DecSize=QmflacDecryptInternal(InData,Size,Type,OutDataPtr);
		if (DecSize==0)
		{
			if (ErrorInfo!=NULL)
				*ErrorInfo=0xFFFFFFFF;
			return 0;
		}
		else
		{
			if (ErrorInfo!=NULL)
				*ErrorInfo=0;
			return DecSize;
		}
	}
	return 0;
}

//防止忘记
void QmflacFreeMem(BYTE* Ptr)
{
	if (Ptr!=NULL)
		delete[] Ptr;
}