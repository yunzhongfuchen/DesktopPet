#include <QtWidgets/QApplication>
#include <QFileInfo>
#include <QThread>
#include <QDebug>
#include <QFile>
#include "../Pet/TablePat.h"
#include "../Common/WindowsInfo.h"
#include "../Pet/Action.h"
#include "FirstOpen.h"
#include "../DogMusic/musicMain.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QFileInfo fileinfo("first.txt");
	QFile file(fileinfo.absoluteFilePath());
	if (file.exists())
	{
		FirstOpen * firstOpen = new FirstOpen();
		firstOpen->exec();
		QFile::remove(fileinfo.absoluteFilePath());
	}
	TablePat* mypat = TablePat::Instance();		//�½�һ���հ׳���mypat
	PatAction* patAction = new PatAction();		//�½�һ����Ϊ��  
	patAction->SetPat(mypat);					//������Ϊ�������mypat
	mypat->show();
	WindowsInfo* windowsinfo = new WindowsInfo();
	a.installNativeEventFilter(windowsinfo);
	patAction->SetWindowsInfoReceive(windowsinfo);	//windows��Ϣ����
	MusicMain::Instance();
	return a.exec();
}
