#include "Signals.h"

SignalsCore* SignalsCore::signalsCore = nullptr;

SignalsCore* SignalsCore::Instance()
{
	if (!signalsCore)
	{
		signalsCore = new SignalsCore;
	}
	return signalsCore;
}
void SignalsCore::windowChange()
{
	emit signal_windowChange();
}

void SignalsCore::mousePress(int flag)
{
	emit signal_mousePress(flag);
}

void SignalsCore::mouseRelease(Qt::MouseButton btn, int flag)
{
	emit signal_mouseRelease(btn, flag);
}

void SignalsCore::mouseMove(int flag)
{
	emit signal_mouseMove(flag);
}

void SignalsCore::dragEnter(int flag)
{
	emit signal_dragEnter(flag);
}

void SignalsCore::dragLeave(int flag)
{
	emit signal_dragLeave(flag);
}

void SignalsCore::drop(QStringList pathlist, int flag)
{
	emit signal_drop(pathlist, flag);
}

void SignalsCore::musicBox()
{
	emit signal_musicBox();
}

void SignalsCore::AddPet()
{
	emit signal_AddPet();
}

void SignalsCore::ReducePet()
{
	emit signal_ReducePet();
}

void SignalsCore::SetWindowInfoReceive(bool isOpen)
{
	emit signal_SetWindowInfoReceive(isOpen);
}

void SignalsCore::EndPet()
{
	emit signal_EndPet();
}