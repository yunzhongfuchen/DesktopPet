#ifndef STACK_SHEETMUSIC_H
#define STACK_SHEETMUSIC_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpacerItem>
#include <QTabWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QScrollArea>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QDialog>
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>
#include <QToolButton>
#include <QFileDialog>
#include <QVector>
#include <QScrollBar>
#include <QListWidget>
#include <QListWidgetItem>
#include <QBitmap>
#include <QPainter>
#include <QMouseEvent>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QMediaMetaData>
#include <QSqlRecord>
#include <QGraphicsDropShadowEffect>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "../../Database/dbManager.h"
#include "../Widget_global.h"
#include "../../NetWork/NetWorkManager.h"
#include "../../Event/MusicSignals.h"
#include "stack_localMusic.h"
//#include "lib/PopupWidget/Popup_playlist.h"


class sheetMusic : public QWidget
{
	Q_OBJECT
public:
	explicit sheetMusic(QWidget *parent = nullptr);

	~sheetMusic() {};

private:
	void localMusicLayout(void);
	void songInfoLayout(void);
	void ConnectInit(void);
	void LoadStyleSheet(void);
	void DataBaseInit(void);
	void GetplayPath(void);
	void SaveplayPath(void);

	void TabWidgetLoad(void);

	void GetSheet(QString sheetName);
	static void GetSheet_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg);
	void AddSingInfo();

	void PlayAll();
	static void GetAllSongId_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg);

	void playOneSong();  

private:
	QLabel *displayLabel[2];
	//选择目录按钮
	QPushButton *selectDir;
	QPushButton *matchMusic;
	QVBoxLayout *local_VBoxLayout;
	//播放全部按钮
	QPushButton *playAll;

	//最上行标签以及按钮布局
	QHBoxLayout *display_HBoxLayout;


	//歌曲信息Tab页面
	QTabWidget *musicInfo_Widget;
	QWidget *song;//歌曲
	//QWidget *singer;//歌手
	//QWidget *album;//专辑
	//QWidget *folder;//文件夹
	QTableWidget *music_Table;
	QScrollArea *music_ScrollArea;
	//存储对话框地址信息
	QList <QString> Dir_list;

	//媒体信息存储
	QList <MediaObjectInfo> mediaObjectInfo;

	//网络部分
	QNetworkAccessManager *Manager;
	QNetworkReply *searchReply;
	QNetworkReply *getSongLinkReply;
	QNetworkReply *getSongLyricReply;
signals:
	void addall_signal(QList <MediaObjectInfo> *);
private slots:
	void table_customContextMenuRequest(const QPoint &pos);
	//void btn_selectDirSlot(void);
	//void btn_addlistclicked_slot(void);
	//void btn_deleteclicked_slot(void);
};

class selectDia : public QDialog
{
	Q_OBJECT
public:
	explicit selectDia(QWidget *parent = nullptr);
	selectDia(QWidget *parent, QList <QString> list);
	~selectDia() {};

	friend class sheetMusic;
private:
	QLabel *headLabel;
	QLabel *describLabel;
	//关闭对话框按钮
	QPushButton *closeButton;

	QScrollArea *Scroll_listArea;

	QPushButton *confirm_Button;
	QPushButton *addfolder_Button;

	QVBoxLayout *dialog_vLayout;

	//目录列表
	QList <QString> Dir_List;
	//目录列表List
	QListWidget *listPath;

private:
	void select_LayoutInit(void);
	void LoadStyleSheet(void);
	void ConnectInit(void);

signals:
	void confirm_clicked();
	void addfolder_clicked();
private slots:
	//void confirmclicked_slot(void);
	//void addfolder_slot(void);
	//void btn_close_slot(void);
	//void PathlistClicked_slot(QListWidgetItem *item);
protected:
	//void showEvent(QShowEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
};












#endif // STACK_SHEETMUSIC_H
