#ifndef COMMONTOOLS
#define COMMONTOOLS

#include <Windows.h>

class CommomTools
{
public:
	// 判断系统是否是64位
	static BOOL Is64BitOS();

	// 判断进程是否是64位
	static BOOL Is64BitPorcess(DWORD dwProcessID);
};

#endif // !COMMONTOOLS