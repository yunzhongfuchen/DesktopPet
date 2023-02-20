#ifndef STACK_FOUNDMUSIC_H
#define STACK_FOUNDMUSIC_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QScrollArea>
#include <QPushButton>
#include <QFile>
#include <QSpacerItem>
#include <QGridLayout>
#include <QDebug>
#include <QLabel>
#include "../../ThirdParty/picturewidget.h"
#include "../Widget_global.h"
#include "../../Event/MusicSignals.h"

class foundmusic : public QWidget
{
    Q_OBJECT
public:
    explicit foundmusic(QWidget *parent = nullptr);
    ~foundmusic() {};

    QTabWidget *list_tabWidget;

private:
    QScrollArea *personal_scrollWidget;
    QVBoxLayout *personal_vLayout;
    QWidget *RecommendWidget;

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
    void TabWidgetLayout(void);
    //个性推荐页面布局
    void PersonalLayout(void);
    void Recommendation(void);
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

};



#endif // STACK_FOUNDMUSIC_H
