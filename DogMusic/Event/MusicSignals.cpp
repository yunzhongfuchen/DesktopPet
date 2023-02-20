#include "MusicSignals.h"


MusicSignals* MusicSignals::musicSignals = nullptr;

MusicSignals::MusicSignals(QWidget *parent)
	: QWidget(parent)
{}

MusicSignals* MusicSignals::Instance()
{
	if (!musicSignals)
	{
		musicSignals = new MusicSignals;
	}
	return musicSignals;
}

void MusicSignals::cleanPlaySheet()
{
	emit signal_cleanPlaySheet();
}

void MusicSignals::PlayAllSheetSong(QList <MediaObjectInfo> *info)
{
	emit signal_PlayAllSheetSong(info);
}

void MusicSignals::PlayOneSheetSong(MediaObjectInfo info)
{
	emit signal_PlayOneSheetSong(info);
}

void MusicSignals::SetNextSheetSong(MediaObjectInfo info)
{
	emit signal_SetNextSheetSong(info);
}

void MusicSignals::UpdatePlayList(QList <MediaObjectInfo> list)
{
	emit signal_UpdatePlayList(list);
}

void MusicSignals::ChangePage(int i)
{
	emit signal_ChangePage(i);
}

void MusicSignals::ShowTopSheet(QString sheetName)
{
	emit signal_ShowTopSheet(sheetName);
}