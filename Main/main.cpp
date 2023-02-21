#include <QtWidgets/QApplication>
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
	a.setQuitOnLastWindowClosed(false);
	QFileInfo fileinfo("first.txt");
	QFile file(fileinfo.absoluteFilePath());
	if (file.exists())
	{
		FirstOpen * firstOpen = new FirstOpen();
		firstOpen->exec();
		QFile::remove(fileinfo.absoluteFilePath());
	}

	WindowsInfo* windowsinfo = new WindowsInfo();
	a.installNativeEventFilter(windowsinfo);
	//patAction->SetWindowsInfoReceive(windowsinfo);	//windowsÏûÏ¢¼àÌı
	DesktopPet desktopPet;
	//desktopPet.show();
	return a.exec();
}
