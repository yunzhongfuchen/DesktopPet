#ifndef MUSIC_PLAYWIDGET_H
#define MUSIC_PLAYWIDGET_H


#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include "../lib/mainwindow.h"
#include <QScrollArea>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QImage>
#include <QPushButton>
#include <QTextBrowser>
#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QRegularExpression>
#include <QRegExp>
#include <QMap>
#include <QTimer>
#include <QTextCharFormat>
#include <QFontComboBox>
#include <QTextEdit>
#include <QTextBlock>
#include <QPainter>
#include <QRgb>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "../lib/mainPlayWidget.h"
#include <QCoreApplication>
#include <QMediaObject>
#include <QFileInfoList>
#include <QScrollBar>
#include <QGridLayout>
#include <QMatrix>
#include <QScrollBar>
#include "stackWidget/stack_localMusic.h"

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

#include "../NetWork/NetWorkManager.h"

#define SINGLESTEP  (34)

class music_play : public QWidget
{
    Q_OBJECT
public:
    friend class MainWindow;
    friend class mainPlay;
    explicit music_play(QWidget *parent = nullptr);
    ~music_play() {};

private:
    void music_playLayout(void);
    void Lefd_cdWidget(void);
    void Right_lyricWidget(void);
    void Load_lyrics(MediaObjectInfo info,QString path);
    void GetSongLyricBySongId(QString Id);
    static void GetLyric_Reply(int errCode, const QByteArray& bytes, void* pCusData, QString msg);

    void AnalysicLyric(const QString *lyricString);

    void ConnectInit(void);
    void BlockInit(void);
    void MsgLayout(void);


private:
    QHBoxLayout *music_playlayout;

    QScrollArea *music_ScrollArea;
    QHBoxLayout *music_hScrollLayout;

    QVBoxLayout *cd_layout;
    QVBoxLayout *lyric_layout;
    QLabel * cd_label;
    QPushButton *music_button[4];
    //歌词显示
    QTextBrowser *lyric_display;
    QTextEdit *lyric_Edit;

    //歌词存储
    QMap<int,QString> lrcMap;       //保存歌词

    QMediaPlayer *music_player;
    int size;
    //歌曲信息
    QVBoxLayout *message_vLayout;
    QHBoxLayout *message_hLayout;
    QLabel *msgLabel[4];
    QPushButton *msgButton[3];
    //播放动画定时器
    QTimer *animation_Timer;
    //QByteArray pixdata;
    QPixmap pixdata;

    int cd_rotate;          //cd旋转角度
    int bofanggan_rotate;   //播放杆旋转角度
    int LyricPosition;      //歌词位置

    //网络部分
    QNetworkAccessManager Manager;
    QNetworkReply *getSongLyricReply;
    QString songLyric;
private slots:
    void LyricUpdate_TimerOut(void);
    void Animation_TimerOut(void);
    void on_psEvent_findmusicLyric(QString lyric);
protected:
    bool eventFilter(QObject *watched,QEvent *event);
};







#endif // MUSIC_PLAYWIDGET_H
