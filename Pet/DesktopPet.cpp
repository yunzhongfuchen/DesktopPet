#include "DesktopPet.h"
#include <QDebug>



DesktopPet::DesktopPet(QWidget *parent)
    : QWidget(parent)
{
	static int petFlag = 0;
	static int petNum = 0;
	connect(SignalsCore::Instance(), &SignalsCore::signal_AddPet, this, [=] {
		TablePat* mypat2 = new TablePat(this, petFlag);		//新建一个空白宠物mypat
		PatAction* patAction2 = new PatAction(this, petFlag);		//新建一个行为组  
		patAction2->SetPat(mypat2);					//设置行为组的主体mypat 
		mypat2->show(); 
		++petFlag;	//宠物编号增加
		++petNum;	//宠物数量增加
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
