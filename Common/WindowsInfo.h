#ifndef WINDOWSINFO
#define WINDOWSINFO

#include <QWidget>
#include <QVector>
#include <QAbstractNativeEventFilter>
#include <windows.h>
#include "CommomTools.h"
#include "Signals.h"

// 获取windows消息队列
class WindowsInfo  : public QWidget, public QAbstractNativeEventFilter
{
	Q_OBJECT

public:
	WindowsInfo(QWidget *parent = nullptr);
	~WindowsInfo(); 
	
	bool nativeEventFilter(const QByteArray & eventType, void * message, long * result);

	void SetOpen(bool open);
	void SetHWND(HWND hwnd);
private:
	bool open = false;
	HWND checkHwnd;
};

// 获取窗口信息
class GetWindowsHandle
{
public:
	GetWindowsHandle();

	// 获取所有带标题的窗口句柄
	QVector<HWND> GetHandle();

	// 获取窗口上下左右的位置
	QVector<int> GetWindowLocal(HWND hwnd);

	// 获取窗口的名称
	QString GetWindowTitle(HWND hwnd);
	
	// 回调函数，EnumWindows(MyEnumProc, (LPARAM)"")每获取到一个窗口便会调用一次该函数
	friend BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM param);

private:
	static QVector<HWND> handleVector; // 储存窗口句柄
	QVector<int> localInfo; //窗口上下左右位置
};


#endif // !WINDOWSINFO