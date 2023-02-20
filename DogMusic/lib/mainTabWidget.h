#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H

#include <QObject>
#include <QTabWidget>
#include <QWidget>
#include "../ThirdParty/picturewidget.h"
#include <QVBoxLayout>
#include "../Event/PSEventController.h"
#include "../Event/EventType_global.h"

class mainTabWidget : public QWidget
{
    Q_OBJECT
public:
    explicit mainTabWidget(QWidget *parent = nullptr);
    ~mainTabWidget() {}

    void TabWidgetLayout(void);
    void ConnectInit(void);

    QTabWidget *list_tabWidget;

private:
    QWidget *personal;
    QWidget *SongSheet;
    QWidget *anchorStation;
    QWidget *Ranklist;
    QWidget *Singer;
    QWidget *LatestMusic;

    //个性推荐
    PictureWidget *pictureWidget;
    QVBoxLayout *PerLayout;

private:
    //个性推荐页面布局
    void PersonalLayout(void);
    //歌单页面布局
    void CustomizedLayout(void);
    //主播电台页面布局
    void AnchorStationLayout(void);
    //排行榜页面布局
    void RanklistLayout(void);
    //歌手页面布局
    void SingerLayout(void);
    //最新音乐页面布局
    void LatestMusicLayout(void);
private slots:

};







#endif // MAINTABWIDGET_H
