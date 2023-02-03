#include "WindowsInfo.h"
#include <QDebug>

QVector<HWND> GetWindowsHandle::handleVector = QVector<HWND>();

WindowsInfo::WindowsInfo(QWidget *parent)
	: QWidget(parent)
{
	
}

WindowsInfo::~WindowsInfo()
{}

bool WindowsInfo::nativeEventFilter(const QByteArray & eventType, void * message, long * result)
{
	if (open)
	{
		/*if (eventType == "windows_generic_MSG" || eventType == "windows_dispatcher_MSG")
		{*/
			MSG * pMsg = reinterpret_cast<MSG *>(message);
			/*char title[MAXBYTE];
			GetWindowTextA(checkHwnd, title, MAXBYTE);
			qDebug() << QString::fromLocal8Bit(title);*/
			/*if (pMsg->hwnd == checkHwnd)
			{
				qDebug() << "pMsg->message:" << pMsg->message;
			}*/
			
				if (pMsg->message == WM_KEYDOWN)
				{

					//获取到系统鼠标移动，可以做像qq一样的忙碌检测
					qDebug() << "nativeEventFilter:" << pMsg->wParam;
					SignalsCore::Instance()->windowChange();
				}
			
		//}
	}

	return false;
}

void WindowsInfo::SetOpen(bool open)
{
	this->open = open;
}
void WindowsInfo::SetHWND(HWND hwnd)
{
	checkHwnd = hwnd;
}

BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM param)
{
	char title[MAXBYTE];
	if (IsWindow(hwnd) &&
		IsWindowEnabled(hwnd) &&
		IsWindowVisible(hwnd))
	{
		if (GetWindowTextA(hwnd, title, MAXBYTE) > 0) {
				//qDebug() << QString::fromLocal8Bit(title); // 打印出有标题的窗口
				GetWindowsHandle::handleVector.push_back(hwnd); 
		}
	}
	return TRUE;
}

GetWindowsHandle::GetWindowsHandle() 
{
	handleVector.clear();
}

QVector<HWND> GetWindowsHandle::GetHandle()
{
	handleVector.clear();
	EnumWindows(MyEnumProc, (LPARAM)""); 
	return handleVector;
}

QVector<int> GetWindowsHandle::GetWindowLocal(HWND hwnd)
{
	localInfo.clear();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	localInfo.push_back(rect.top);
	localInfo.push_back(rect.bottom);
	localInfo.push_back(rect.left);
	localInfo.push_back(rect.right);
	return localInfo;
}

QString GetWindowsHandle::GetWindowTitle(HWND hwnd)
{
	char title[MAXBYTE];
	if (IsWindow(hwnd) &&
		IsWindowEnabled(hwnd) &&
		IsWindowVisible(hwnd))
	{
		if (GetWindowTextA(hwnd, title, MAXBYTE) > 0) {
			return QString::fromLocal8Bit(title);
		}
	}
	return QString();
}