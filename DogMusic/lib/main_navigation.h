#ifndef MAIN_NAVIGATION_H
#define MAIN_NAVIGATION_H


#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QAction>
#include <QMouseEvent>
#include <QFile>
#include "../Event/PSEventController.h"
#include "../Event/EventType_global.h"
#include "../Event/MusicSignals.h"
#include "Widget_global.h"

class main_navigation : public QWidget
{
    Q_OBJECT
public:
    explicit main_navigation(QWidget *parent = nullptr);
    ~main_navigation() {}

    QPushButton *main_Button[4];
private:
    void Navigation_Layout(void);
    void ConnectInit(void);
    void LoadStyleSheet(void);

private:
    QHBoxLayout *navigationLayout;
    QPushButton *NetEasyLogo;
    QLabel *navLabel[4];
    QPushButton *navButton[14];
    QLineEdit *searchLine;
    QSpacerItem *hSpacer;
    //搜索栏查找按钮
    QAction *searchAction;
protected:
    void mousePressEvent(QMouseEvent *event);
private slots:
    //搜索框槽函数
    void searchclicked_slot(void);
    void btn_close_clicked(void);
    void btn_min_clicked(void);
    void btn_max_clicked(void);
    void btn_logo_clicked(void);
};






#endif // MAIN_NAVIGATION_H
