#include "DesktopPet.h"
#include <QDebug>



DesktopPet::DesktopPet(QWidget *parent)
    : QWidget(parent)
{
	static int petFlag = 0;
	static int petNum = 0;
	connect(SignalsCore::Instance(), &SignalsCore::signal_AddPet, this, [=] {
		TablePat* mypat2 = new TablePat(this, petFlag);		//�½�һ���հ׳���mypat
		PatAction* patAction2 = new PatAction(this, petFlag);		//�½�һ����Ϊ��  
		patAction2->SetPat(mypat2);					//������Ϊ�������mypat 
		mypat2->show(); 
		++petFlag;	//����������
		++petNum;	//������������
	});
	connect(SignalsCore::Instance(), &SignalsCore::signal_ReducePet, this, [=] {
		--petNum;
		if (petNum <= 0)
		{
			qApp->exit(0);
		}
	});
	SignalsCore::Instance()->AddPet();
}

DesktopPet::~DesktopPet()
{}
