#ifndef MAINPLAYWIDGET_H
#define MAINPLAYWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QHBoxLayout>
#include <QMouseEvent>
#include "../lib/music_playwidget.h"
#include <QPropertyAnimation>
#include <QDebug>
#include "../lib/main_listWidget.h"
#include <QTimer>
#include <QMediaPlayer>
#include "PopupWidget/Popup_playlist.h"
#include "../lib/stackWidget/stack_localMusic.h"
#include "stackWidget/stack_findMusic.h"
#include "../Event/PSEventController.h"
#include "../Event/EventType_global.h"
#include "Widget_global.h"
#include "../Event/MusicSignals.h"

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
#include "Widget_global.h"

class music_play;

class mainPlay : public QWidget
{
    Q_OBJECT
public:
    explicit mainPlay(QWidget *parent = nullptr);
    ~mainPlay() {}

    QWidget *playWidget;
    friend class music_play;
    friend class MainWindow;
private:
    void PlayLayout(void);
    void SongInfoLayout(void);
    void PlayCtrlLayout(void);
    void SoundEffectLayout(void);
    void ConnectInit(void);
    void mediaPlayerInit(void);
    void ScanSongs(void);
    void getSingerImage(MediaObjectInfo info);
    void LoadStyleSheet(void);
    static void GetSingerImage_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg);

private:
    QHBoxLayout *play_hBoxLayout;
    //歌曲信息布局
    QWidget *songInfoWidget;
    QGridLayout *play_ginfoLayout;
    QPushButton *musicCoverButton;
    QPushButton *favorButton;
    QLabel *songNameLabel;
    QLabel *singerNameLabel;
    //歌曲控制布局
    QWidget *playCtrlWidget;
    QHBoxLayout *play_hLayout;
    QGridLayout *play_gctrlLayout;
    QPushButton *play_ctrlButton[6];
    QLabel *play_timeLabe[2];
    QSlider *play_slider;

    //音效控制开关
    QWidget *soundWidget;
    QHBoxLayout *sound_hBoxLayout;
    QPushButton *sound_PushButton[4];
    QSlider *sound_Volume;
    //音乐播放界面
    music_play *music_playWidget;
    //媒体信息存储
    QList <MediaObjectInfo> mediaObjectInfo;
    //音乐播放
    QMediaPlayer *musicPlayer;

    QMediaPlaylist *mediaPlaylist;
    //播放列表
    Pop_playlist *musicplaylist;

    //网络事件
    QNetworkAccessManager manager;
    QNetworkReply *SingerImageReply;



private slots:
    void btn_cover_clicked(void);
    void hide_animation_timeout(void);
    void btn_pre_clicked(void);
    void btn_play_clicked(void);
    void btn_next_clicked(void);

    //播放列表指定歌曲事件
	void PlayOneMusic(MediaObjectInfo info);

	void SetNextSheetSong(MediaObjectInfo info);
    void on_psEvent_playAppointSong(int index);
    //删除列表指定歌曲
    void on_psEvent_deleteAppointSong(int index);

    void playbuttontoggled_slot(bool state);
    //音乐信息更改
    void musicMetaDataChanged(void);
    //音乐播放列表按钮点击
    void btn_playlist_clicked(void);
    //媒体状态改变
    void mediaPlayerStateChanged(QMediaPlayer::State);
    //进度条点击释放
    void durationSliderReleased();
    //
    void musicPlayerDurationChanged(qint64 duration);
    //
    void mediaPlaylistCurrentIndexChanged(int index);
    //媒体播放位置改变
    void mediaPlayerPositionChanged(qint64);
    //播放列表改变
    void Rev_ListMsg_slot( QList <MediaObjectInfo> *info);
    //清空列表事件
    void on_psEvent_ClearList();
    //更新列表事件
    void on_psEvent_UpdateList(QList <MediaObjectInfo> list);
    //立即播放事件
    void on_psEvent_findmuicPlay(MediaObjectInfo info,QString url);
    //下一首播放事件
    void on_psEvent_findmusicNextPlay(MediaObjectInfo info,QString url);
    //获取歌手照片事件
    void reply_singimageFinish(QNetworkReply *reply);
protected:
    //大小变化
    void resizeEvent(QResizeEvent *e);
    //鼠标点击时间
    void mousePressEvent(QMouseEvent *event);
    //事件过滤
    bool eventFilter(QObject *watcher,QEvent *event);
};






#endif // MAINPLAYWIDGET_H
