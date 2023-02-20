#ifndef POPUP_PLAYLIST_H
#define POPUP_PLAYLIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QList>
#include <QTabWidget>
#include <QFrame>
#include <QPainter>
#include <QEvent>
#include <QGraphicsDropShadowEffect>
#include <qmath.h>
#include <QTimer>
#include <QApplication>

#include "../../lib/stackWidget/stack_localMusic.h"
#include "../../Event/PSEventController.h"
#include "../../Event/EventType_global.h"
#include "../../Database/dbManager.h"
#include "../Widget_global.h"
#include "../../Event/MusicSignals.h"


class Pop_playlist : public QWidget
{
    Q_OBJECT
public:
    explicit Pop_playlist(QWidget *parent = nullptr);
    ~Pop_playlist(){}

    friend class mainPlay;

private:
    void TabWidget_Init(void);
    void playlist_Init(void);
    void AddOneSong(MediaObjectInfo *info);
    void ConnectInit(void);
    void AddList(QList <MediaObjectInfo> *info);
    void UpdateList(void);
    void AddSong(MediaObjectInfo info);
    void addNextSong(MediaObjectInfo info,int position);
    void StyleSheetInit(void);
    void ReadListFromDB(void);
    void AddAllListtoDB(void);

private:
    QTabWidget *playTabWidget;

    QVBoxLayout *main_vLayout;
    //播放列表Widget
    QWidget *playWidget;
    QWidget *historyWidget;


    QVBoxLayout *playlist_vLayout;
    QPushButton *Playlist_Btn[4];
    QLabel *songnum_label;
    QTableWidget *playlist_table;

    //媒体信息存储
    QList <MediaObjectInfo> mediaObjectInfo;

    QAction *ViewComments;      //查看评论
    QAction *Play;              //播放
    QAction *Album;             //专辑
    QAction *Singer;            //歌手
    QAction *Comefrom;          //自来
    QAction *CollectToList;     //收藏到歌单
    QAction *Share;             //分享
    QAction *CoppLink;          //复制链接
    QAction *OpenFolder;        //打开文件所在目录
    QAction *DeleteFromList;    //从列表中删除
protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool eventFilter(QObject*watcher,QEvent *event);
    void focusOutEvent(QFocusEvent *event);
public slots:
    void Rev_ListMsg_slot( QList <MediaObjectInfo> *info);
    void btn_deleteclick_slot(void);
    void addtolist_slot(void);
    void tableClickedMenu_slot(const QPoint &pos);
    void PlayClicked_slot(void);
    void ListDeleteClicked_slot(void);
};


#endif // POPUP_PLAYLIST_H
