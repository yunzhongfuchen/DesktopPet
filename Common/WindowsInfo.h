#ifndef WINDOWSINFO
#define WINDOWSINFO

#include <QWidget>
#include <QVector>
#include <QAbstractNativeEventFilter>
#include <windows.h>
#include "CommomTools.h"
#include "Signals.h"

// ��ȡwindows��Ϣ����
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

// ��ȡ������Ϣ
class GetWindowsHandle
{
public:
	GetWindowsHandle();

	// ��ȡ���д�����Ĵ��ھ��
	QVector<HWND> GetHandle();

	// ��ȡ�����������ҵ�λ��
	QVector<int> GetWindowLocal(HWND hwnd);

	// ��ȡ���ڵ�����
	QString GetWindowTitle(HWND hwnd);
	
	// �ص�������EnumWindows(MyEnumProc, (LPARAM)"")ÿ��ȡ��һ�����ڱ�����һ�θú���
	friend BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM param);

private:
	static QVector<HWND> handleVector; // ���洰�ھ��
	QVector<int> localInfo; //������������λ��
};


#endif // !WINDOWSINFO