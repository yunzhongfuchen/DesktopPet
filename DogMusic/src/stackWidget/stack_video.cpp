#include "../../lib/stackWidget/stack_video.h"
#include <QFile>


stack_video::stack_video(QWidget *parent) :
    QTabWidget(parent)
{
    this->setMinimumSize(800,530);

    Stack_LayoutInit();

    StyleSheetInit();
}

void stack_video::StyleSheetInit(void)
{
    QFile file(":/qss/stackqss/stack_video.qss");

    file.open(QIODevice::ReadOnly);
    if(file.isOpen())
    {
        QString style = this->styleSheet();
        style += QLatin1String(file.readAll());
        this->setStyleSheet(style);
        file.close();
    }
}

void stack_video::Stack_LayoutInit(void)
{
    Video_LayoutInit();
    MV_LayoutInit();

    QFont font = this->font();
    font.setPointSize(17);

    this->setFont(font);
    this->addTab(video_Widget,"��Ƶ");
    this->addTab(mv_Widget,"MV");

}

void stack_video::Video_LayoutInit(void)
{
    video_Widget = new QWidget();
    QVBoxLayout *video_vLayout = new QVBoxLayout();
    video_Widget->setLayout(video_vLayout);

    testLabel = new QLabel();
    testLabel->setText(__QString("��Ƶ������ʱû��д"));

    video_vLayout->addWidget(testLabel);


}

void stack_video::MV_LayoutInit(void)
{
    mv_Widget = new QWidget();

}
