#ifndef COMMONTOOLS
#define COMMONTOOLS

#include <Windows.h>

class CommomTools
{
public:
	// �ж�ϵͳ�Ƿ���64λ
	static BOOL Is64BitOS();

	// �жϽ����Ƿ���64λ
	static BOOL Is64BitPorcess(DWORD dwProcessID);
};

#endif // !COMMONTOOLS