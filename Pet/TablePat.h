#ifndef TABLEPAT
#define TABLEPAT

#include <QtWidgets/QWidget>
//#include "ui_TablePat.h"
#include <QLabel>
#include <QMovie>
#include <QMouseEvent>
#include <QDesktopWidget>
#include <QPropertyAnimation>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QApplication>

#include "windows.h" 
#include "../Common/Signals.h"

enum ActionType
{
	NONE,
	LEFT,
	REIGH,
	DOWN,
	UP,
	EAT,
	OTHER
};

enum Currentbehavior
{
	CB_NONE,	//无
	CB_RANDOM,	//随机行为
	CB_RANDOM_DOT,
	CB_RANDOM_LEFT,
	CB_RANDOM_RIGHT,
	CB_TOWINDOWSTOP,	//向窗口顶端移动
	CB_ONTOP,	//在窗口顶端
	CB_DOWN, //下落
	CB_SUSPENSION,	//悬挂
	CB_DRAG, //拖动
	CB_EAT //文件进入
};

class TablePat : public QWidget
{
    Q_OBJECT

public:
    static TablePat* Instance(QWidget* parent = nullptr);
    
	QLabel* movieLab;

	/// 设置宠物当前行为
	void SetCurrentbehavior(Currentbehavior behavior);
	Currentbehavior currentbehavior;
private:
    //Ui::TablePatClass ui;
    TablePat(QWidget* parent = nullptr);
    ~TablePat();
    static TablePat* tablePat;
    QPoint diff_;

    //重写鼠标事件
	void mousePressEvent(QMouseEvent* event);

	void mouseReleaseEvent(QMouseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);

	//重写拖动事件
	void dragEnterEvent(QDragEnterEvent *event);

	void dragLeaveEvent(QDragLeaveEvent *event);

	void dropEvent(QDropEvent *event);
};

#endif // !TABLEPAT