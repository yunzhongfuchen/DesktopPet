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
	//	����ֻ������һ��exe
	QSharedMemory shared_memory;
	shared_memory.setKey(QString("123"));//���ù̶������ڴ�ε�keyֵ
	if (!shared_memory.create(1)) //����1byte�Ĺ����ڴ��
	{
		return -1;
	}
	//	���������أ��Ӵ��ڹرղ���رճ���
	a.setQuitOnLastWindowClosed(false);
	//	��һ�ο�����ִ�н̳�
	QFileInfo fileinfo("first.txt");
	QFile file(fileinfo.absoluteFilePath());
	qDebug() << fileinfo.absoluteFilePath();
	if (file.exists())
	{
		FirstOpen * firstOpen = new FirstOpen();
		firstOpen->exec();
		QFile::remove(fileinfo.absoluteFilePath());
	}

	//windows��Ϣ����
	WindowsInfo* windowsinfo = new WindowsInfo();
	a.installNativeEventFilter(windowsinfo);
	DesktopPet desktopPet;
	return a.exec();
}
