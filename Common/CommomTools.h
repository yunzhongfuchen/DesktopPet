#ifndef COMMONTOOLS
#define COMMONTOOLS

#include <Windows.h>

#define __QString(str) QString::fromLocal8Bit(str)

class CommomTools
{
public:
	// �ж�ϵͳ�Ƿ���64λ
	static BOOL Is64BitOS();

	// �жϽ����Ƿ���64λ
	static BOOL Is64BitPorcess(DWORD dwProcessID);
};

#endif // !COMMONTOOLS