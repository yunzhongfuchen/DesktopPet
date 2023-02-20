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

void SignalsCore::mousePress()
{
	emit signal_mousePress();
}

void SignalsCore::mouseRelease(Qt::MouseButton btn)
{
	emit signal_mouseRelease(btn);
}

void SignalsCore::mouseMove()
{
	emit signal_mouseMove();
}

void SignalsCore::dragEnter()
{
	emit signal_dragEnter();
}

void SignalsCore::dragLeave()
{
	emit signal_dragLeave();
}

void SignalsCore::drop(QStringList pathlist)
{
	emit signal_drop(pathlist);
}

void SignalsCore::musicBox()
{
	emit signal_musicBox();
}
