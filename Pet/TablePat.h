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
	CB_NONE,	//��
	CB_RANDOM,	//�����Ϊ
	CB_RANDOM_DOT,
	CB_RANDOM_LEFT,
	CB_RANDOM_RIGHT,
	CB_TOWINDOWSTOP,	//�򴰿ڶ����ƶ�
	CB_ONTOP,	//�ڴ��ڶ���
	CB_DOWN, //����
	CB_SUSPENSION,	//����
	CB_DRAG, //�϶�
	CB_EAT //�ļ�����
};

class TablePat : public QWidget
{
    Q_OBJECT

public:
    static TablePat* Instance(QWidget* parent = nullptr);
    
	QLabel* movieLab;

	/// ���ó��ﵱǰ��Ϊ
	void SetCurrentbehavior(Currentbehavior behavior);
	Currentbehavior currentbehavior;
private:
    //Ui::TablePatClass ui;
    TablePat(QWidget* parent = nullptr);
    ~TablePat();
    static TablePat* tablePat;
    QPoint diff_;

    //��д����¼�
	void mousePressEvent(QMouseEvent* event);

	void mouseReleaseEvent(QMouseEvent* event);

	void mouseMoveEvent(QMouseEvent* event);

	//��д�϶��¼�
	void dragEnterEvent(QDragEnterEvent *event);

	void dragLeaveEvent(QDragLeaveEvent *event);

	void dropEvent(QDropEvent *event);
};

#endif // !TABLEPAT