#ifndef MAIN_LISTWIDGET_H
#define MAIN_LISTWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QSpacerItem>
#include <QListWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "../lib/stackWidget/stack_foundMusic.h"
#include "../lib/stackWidget/stack_localMusic.h"
#include "../lib/stackWidget/stack_video.h"
#include "../lib/stackWidget/stack_findMusic.h"
#include "../lib/stackWidget/stack_sheetMusic.h"
#include "Widget_global.h"




void getWidthSize(void);
void getHeightSize(void);

class main_listwidget : public QWidget
{
    Q_OBJECT
public:
    explicit main_listwidget(QWidget *parent = nullptr);
    ~main_listwidget() {}

    QWidget *main_listWidget;
    friend void getWidthSize(void);
    friend void getHeightSize(void);
private:
    void listWidgeLayout();
    void setList1(void);
    void setList2(void);
    void setList3(void);
    void setStackWidget(void);
    void ReadListWidgetStyle(void);
    void ConnectInit(void);
private:
    QLabel *list_ScrollLabel[2];
    QListWidget *list_ScrollWidget[3];
    QPushButton *list_addButton;
    QWidget *listWidget;
    QScrollArea *mainScrollArea;
    QSpacerItem *list_hSpacer;
    //QTabWidget *list_tabWidget;
    QHBoxLayout *list_hLayout;
    QVBoxLayout *Scroll_vLayout;
    QStackedWidget *main_stackWidget;
    //发现音乐界面
    foundmusic * main_stackfoud;
    //本地音乐界面
	localMusic * main_stacklocal;
	//排行榜音乐界面
	sheetMusic * main_stacksheet;
    //视频界面
    stack_video * main_stackvideo;
    //音乐搜索界面
    findmusic *main_findmusic;
private slots:
    void on_list1Widget_itemClicked(QListWidgetItem *item);
    void on_list2Widget_itemClicked(QListWidgetItem *item);
	void on_list3Widget_itemClicked(QListWidgetItem *item);
    void on_psEvent_changeStackWidget(int index);
protected:
    //大小变化
    void resizeEvent(QResizeEvent *event);
};

#endif // MAIN_LISTWIDGET_H
