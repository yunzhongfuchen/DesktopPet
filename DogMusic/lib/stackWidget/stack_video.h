#ifndef STACK_VIDEO_H
#define STACK_VIDEO_H

#include <QWidget>
#include <QLabel>
#include <QTabWidget>
#include <QVBoxLayout>
#include "../Widget_global.h"


class stack_video : public QTabWidget
{
    Q_OBJECT
public:
    explicit stack_video(QWidget *parent = nullptr);
    ~stack_video() {};

private:
    void Stack_LayoutInit(void);
    void Video_LayoutInit(void);
    void MV_LayoutInit(void);
    void StyleSheetInit(void);

private:
    QWidget *video_Widget;
    QWidget *mv_Widget;


    QLabel *testLabel;


};

#endif // STACK_VIDEO_H
