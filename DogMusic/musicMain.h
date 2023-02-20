#pragma once

#include <QObject>
#include "Database/dbManager.h"
#include "lib/mainwindow.h"
#include "../Common/Signals.h"

class MusicMain : public QObject
{
	Q_OBJECT

public:
	MusicMain(QObject *parent = nullptr);
	~MusicMain();
	static MusicMain* Instance();
private:
	MainWindow* music = nullptr;
	static MusicMain* musicMain;
	void StartMusic();
};
