#include <QtWidgets/QApplication>
#include <QSharedMemory>
#include <QFileInfo>
#include <QThread>
#include <QDebug>
#include <QFile>
#include "../Pet/TablePat.h"
#include "../Pet/DesktopPet.h"
#include "../Common/WindowsInfo.h"
#include "../Pet/Action.h"
#include "FirstOpen.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//	设置只能启动一个exe
	QSharedMemory shared_memory;
	shared_memory.setKey(QString("123"));//设置固定共享内存段的key值
	if (!shared_memory.create(1)) //创建1byte的共享内存段
	{
		return -1;
	}
	//	主窗口隐藏，子窗口关闭不会关闭程序
	a.setQuitOnLastWindowClosed(false);
	//	第一次卡开则执行教程
	QFileInfo fileinfo("first.txt");
	QFile file(fileinfo.absoluteFilePath());
	qDebug() << fileinfo.absoluteFilePath();
	if (file.exists())
	{
		FirstOpen * firstOpen = new FirstOpen();
		firstOpen->exec();
		QFile::remove(fileinfo.absoluteFilePath());
	}

	//windows消息监听
	WindowsInfo* windowsinfo = new WindowsInfo();
	a.installNativeEventFilter(windowsinfo);
	DesktopPet desktopPet;
	return a.exec();
}
