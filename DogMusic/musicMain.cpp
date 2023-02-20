#include "MusicMain.h"

MusicMain* MusicMain::musicMain = nullptr;

MusicMain::MusicMain(QObject *parent)
	: QObject(parent)
{
	dbManager::destoryAllDBConn();
	connect(SignalsCore::Instance(), &SignalsCore::signal_musicBox, this, &MusicMain::StartMusic);
}

MusicMain::~MusicMain()
{}

MusicMain* MusicMain::Instance()
{
	if (!musicMain)
	{
		musicMain = new MusicMain();
	}
	return musicMain;
}

void MusicMain::StartMusic()
{
	if (!music)
	{
		music = new MainWindow;
	}
	music->show();
}