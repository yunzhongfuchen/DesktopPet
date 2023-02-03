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
	PatAction(QWidget *parent = nullptr);
	~PatAction();
	TablePat* patInstance;
	QLabel* movieLab;
	QPoint diff_;
	QMovie* movie = nullptr;
	//gif�����б�
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
	//gif�������Ӧ��·��

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
	int desktopWidth;//����Ŀ��
	int desktopHeight;//����ĸ߶�
	QString nowGif;//��¼�������ڲ��ŵ�gif
	Currentbehavior currentbehavior;
	QPropertyAnimation* pAnimation = nullptr;

	void SetPat(TablePat* Instance);

	// ����Windows��Ϣ����
	void SetWindowsInfoReceive(WindowsInfo* info);

	void RunAction(QString gifName, ActionType actionType, int spendTime = 0, QPair<int, int> begin = QPair<int, int>(0, 0), QPair<int, int> end = QPair<int, int>(0, 0), int action = 0);

	void RunAction(Currentbehavior behaviorType, int spendTime = 0, QString gifName = QString(), QPair<int, int> end = qMakePair(MININT, MININT), QPair<int, int> begin = qMakePair(MININT, MININT));
private:

	/// ���ó��ﵱǰ״̬
	void SetCurrentbehavior(Currentbehavior behavior);

	/// breaf ���gif·��
	/// 
	/// param dir gif�ļ���·��
	/// param actionMap ���·����map
	void InitActionMap(QString dir, QMap<QString, QString>& actionMap, QStringList& actionList);

	void ClearAction();

	/// breaf ���ŷ��ƶ�����
	/// 
	/// gifName gif����
	void OtherAction(QString gifName);

	/// breaf �������ƶ�����
	/// 
	/// gifName gif����
	/// spendTime ����ʱ��ϵ��
	/// begin ��ʼ����
	/// end ��������
	void ProduceAction(QString gifPath, int spendTime, QPair<int, int> end = qMakePair(MININT, MININT), QPair<int, int> begin = qMakePair(MININT, MININT));

	void randomEvent();

	bool FindSuitWindow(HWND &hwndOut);

	/// �ƶ������ڵĶ���
	void GoToWindowTop();

	/// ��������¼�
	void mousePress();
	void mouseRelease(Qt::MouseButton btn);
	void mouseMove();
	void dragEnter();
	void dragLeave();
	void drop(QStringList pathlist);
	WindowsInfo* windowsInfo = nullptr;

signals:
	void lineActionFinish(int action = 0);
};

#endif // !ACTION