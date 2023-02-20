#ifndef STACK_FINDMUSIC_H
#define STACK_FINDMUSIC_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTableWidget>
#include "../../Event/PSEventController.h"
#include "../../Event/EventType_global.h"
#include <QDebug>
#include <QHeaderView>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QMenu>
#include <QFile>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QEventLoop>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonParseError>
#include <QGraphicsDropShadowEffect>

#include "stack_localMusic.h"
#include "../../NetWork/NetWorkManager.h"
#include "../../ThirdParty/pagenavigator.h"
#include "../Widget_global.h"

struct SingInfo
{
    //歌曲名
    QString SongName;
    //歌曲ID
    QString SongId;
    //歌手
    QString Singer;
    //歌手照片
    QString SingerImage;
    //专辑名
    QString AlbumName;
    //播放时间
    QString playtime;
    //热度
    int hotnum;
    //来源
    int songSource;
};

class findmusic : public QWidget
{
    Q_OBJECT
public:
    explicit findmusic(QWidget *parent = nullptr);
    ~findmusic() {};


private:
    void LoadStyleSheet(void);
    void findmusic_LayoutInit(void);
    void TabWidget_Init(void);
    void SingSong_LayoutInit(void);
    void AddSingInfo(const QList <MediaObjectInfo> *info,int page);

    void ConnectInit(void);

    void GetSearchByKeywords(QString keyword,int page);
    static void GetKeywordsSearch_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg);

    void GetSongLinkBySongId(MediaObjectInfo info, QString Id, QString LinkState);
    static void GetSongId_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg);

    Q_INVOKABLE QPair<QString, QString> GetSingerInfo(QJsonArray array);

private:
    QVBoxLayout *findmusic_vLayout;
    QLabel *num_label;
    QTabWidget *findmusic_tab;

    QWidget *singSong;              //单曲界面
    QVBoxLayout *songsong_vLayout;  //单曲界面布局
    QTableWidget *singsong_table;   //单曲界面表单
    PageNavigator *singsong_page;   //单曲界面页面分页

    QWidget *singer;                //歌手
    QString keywords;

    //网络歌曲信息清单
    QList <MediaObjectInfo> songInfo;
    QString songUrl;
    QString songLyric;

    //网络部分
    QNetworkAccessManager *Manager;
    QNetworkReply *searchReply;
    QNetworkReply *getSongLinkReply;
    QNetworkReply *getSongLyricReply;

    QAction *viewComments;//查看评论
    QAction *play;//播放
    QAction *playnext;//下一首播放
    QAction *collecttolist;//收藏到歌单
    QAction *sharemusic;//分享
    QAction *coppy;//复制链接
    QAction *download;//下载
private slots:
    //导航栏搜索事件
    void on_psEvent_navigationSearch(QString msg);
    //右键菜单栏
    void table_customContextMenuRequest(const QPoint &pos);
    //播放槽函数
    void playActionclicked_slot(void);
    //播放下一首槽函数
    void playnextActionclicked_slot(void);
    //页码按钮点击槽函数
    void PageNavigatorClicked_slot(int page);
};



#endif // STACK_FINDMUSIC_H
