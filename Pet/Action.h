#ifndef ACTION
#define ACTION

#include <QPropertyAnimation>
#include <QDesktopWidget>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QFileInfo>
#include <QDir>
#include <QMovie>
#include <QDebug>
#include <QTimer>
#include <QMimeData>
#include <QDirIterator>

#include "../Common/WindowsInfo.h"
#include "../Common/Signals.h"
#include "TablePat.h"

class TablePat;
class PatAction  : public QWidget
{
	Q_OBJECT

public:
	PatAction(QWidget *parent = nullptr, int flag = 0);
	~PatAction();
	TablePat* patInstance;
	QLabel* movieLab;
	QPoint diff_;
	QMovie* movie = nullptr;
	//gif名称列表
	QStringList randomDotList;
	QStringList randomMoveLeftList;
	QStringList randomMoveRightList;
	QStringList eatList;
	QStringList dragList;
	QStringList suspensionList;
	QStringList downList;
	QStringList onTopList;
	QStringList toWindowsTopList;
	QStringList noneList;
	//gif名称与对应的路径

	QMap<QString, QString> randomDotMap;
	QMap<QString, QString> randomMoveLeftMap;
	QMap<QString, QString> randomMoveRightMap;
	QMap<QString, QString> eatMap;
	QMap<QString, QString> dragMap;
	QMap<QString, QString> suspensionMap;
	QMap<QString, QString> downMap;
	QMap<QString, QString> onTopMap;
	QMap<QString, QString> toWindowsTopMap;
	QMap<QString, QString> noneMap;
	int desktopWidth;//桌面的宽度
	int desktopHeight;//桌面的高度
	QString nowGif;//记录现在正在播放的gif
	Currentbehavior currentbehavior;
	QPropertyAnimation* pAnimation = nullptr;

	void SetPat(TablePat* Instance);

	// 设置Windows消息监听
	//void SetWindowsInfoReceive(WindowsInfo* info);

	void RunAction(QString gifName, ActionType actionType, int spendTime = 0, QPair<int, int> begin = QPair<int, int>(0, 0), QPair<int, int> end = QPair<int, int>(0, 0), int action = 0);

	void RunAction(Currentbehavior behaviorType, int spendTime = 0, QString gifName = QString(), QPair<int, int> end = qMakePair(MININT, MININT), QPair<int, int> begin = qMakePair(MININT, MININT));
private:

	/// 设置宠物当前状态
	void SetCurrentbehavior(Currentbehavior behavior);

	/// breaf 获得gif路径
	/// 
	/// param dir gif文件夹路径
	/// param actionMap 存放路径的map
	void InitActionMap(QString dir, QMap<QString, QString>& actionMap, QStringList& actionList);

	void ClearAction();

	/// breaf 播放其他动画
	/// 
	/// gifName gif名称
	void OtherAction(QString gifName);

	/// breaf 播放动画
	/// 
	/// gifName gif名称
	/// spendTime 运行时间系数
	/// begin 起始坐标
	/// end 结束坐标
	void ProduceAction(QString gifPath, int spendTime, QPair<int, int> end = qMakePair(MININT, MININT), QPair<int, int> begin = qMakePair(MININT, MININT));

	void randomEvent();

	bool FindSuitWindow(HWND &hwndOut);

	/// 移动到窗口的顶部
	void GoToWindowTop();

	/// 处理鼠标事件
	void mousePress(int flag);
	void mouseRelease(Qt::MouseButton btn, int flag);
	void mouseMove(int flag);
	void dragEnter(int flag);
	void dragLeave(int flag);
	void drop(QStringList pathlist, int flag);
	int flag;	//行为编号
	//WindowsInfo* windowsInfo = nullptr;

signals:
	void lineActionFinish(int action = 0);
};

#endif // !ACTION