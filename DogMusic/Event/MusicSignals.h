#ifndef MUSICSIGNALS_H
#define MUSICSIGNALS_H

#include <QWidget>
#include <QList>
#include "../lib/stackWidget/stack_sheetMusic.h"
#include "../lib/stackWidget/stack_localMusic.h"

class MusicSignals : public QWidget
{
	Q_OBJECT

public:
	static MusicSignals* Instance();

	void cleanPlaySheet();
	void PlayAllSheetSong(QList <MediaObjectInfo> *info);
	void PlayOneSheetSong( MediaObjectInfo info);
	void SetNextSheetSong(MediaObjectInfo info);
	//	刷新播放列表
	void UpdatePlayList(QList <MediaObjectInfo> list);
	//	切换界面
	void ChangePage(int i);
	//	显示排行列表
	void ShowTopSheet(QString sheetName);
private:
	static MusicSignals* musicSignals;
	MusicSignals(QWidget *parent = nullptr);
signals:

	void signal_cleanPlaySheet();
	void signal_PlayAllSheetSong(QList <MediaObjectInfo> *info);
	void signal_PlayOneSheetSong(MediaObjectInfo info);
	void signal_SetNextSheetSong(MediaObjectInfo info);
	void signal_UpdatePlayList(QList <MediaObjectInfo> list);
	void signal_ChangePage(int i);
	void signal_ShowTopSheet(QString sheetName);
};

#endif // !MUSICSIGNALS_H