#include "Action.h"
#include <QMessageBox>

PatAction::PatAction(QWidget *parent)
	: QWidget(parent)
{
	QDesktopWidget* pDesktopWidget = QApplication::desktop();
	desktopWidth = pDesktopWidget->availableGeometry().width();
	desktopHeight = pDesktopWidget->availableGeometry().height();

	InitActionMap("gif/random/random_dot", randomDotMap, randomDotList);
	InitActionMap("gif/random/random_move/left", randomMoveLeftMap, randomMoveLeftList);
	InitActionMap("gif/random/random_move/right", randomMoveRightMap, randomMoveRightList);
	InitActionMap("gif/eat", eatMap, eatList);
	InitActionMap("gif/drag", dragMap, dragList);
	InitActionMap("gif/suspension", suspensionMap, suspensionList);
	InitActionMap("gif/down", downMap, downList);
	InitActionMap("gif/ontop", onTopMap, onTopList);
	InitActionMap("gif/towindowstop", toWindowsTopMap, toWindowsTopList);
	InitActionMap("gif/none", noneMap, noneList);
	//connect(SignalsCore::Instance(), &SignalsCore::signal_mousePress, this, &PatAction::mousePress);
	connect(SignalsCore::Instance(), &SignalsCore::signal_mouseRelease, this, &PatAction::mouseRelease);
	connect(SignalsCore::Instance(), &SignalsCore::signal_mouseMove, this, &PatAction::mouseMove);
	connect(SignalsCore::Instance(), &SignalsCore::signal_dragEnter, this, &PatAction::dragEnter);
	connect(SignalsCore::Instance(), &SignalsCore::signal_dragLeave, this, &PatAction::dragLeave);
	connect(SignalsCore::Instance(), &SignalsCore::signal_drop, this, &PatAction::drop);
}

void PatAction::SetPat(TablePat* Instance)
{
	patInstance = TablePat::Instance();
	movieLab = patInstance->movieLab;
	SetCurrentbehavior(CB_NONE);
	RunAction(CB_NONE, 3, "hi.gif");
	connect(this, &PatAction::lineActionFinish, this, [=](int action)
	{
		switch (currentbehavior)
		{
		case CB_NONE:
			randomEvent();
			break;
		case CB_RANDOM:
			randomEvent();
			break;
		case CB_TOWINDOWSTOP:
		{
			SetCurrentbehavior(CB_ONTOP);
			RunAction(CB_ONTOP);
		}
		break;
		case CB_ONTOP:
			break;
		case CB_DOWN:
			SetCurrentbehavior(CB_NONE);
			randomEvent();
			break;
		case CB_SUSPENSION:
			break;
		case CB_DRAG:
			break;
		case CB_EAT:
			break;
		default:
			break;
		}

	});
}

void PatAction::SetWindowsInfoReceive(WindowsInfo* info)
{
	windowsInfo = info;
}

void PatAction::SetCurrentbehavior(Currentbehavior behavior)
{
	currentbehavior = behavior;
	patInstance->SetCurrentbehavior(behavior);
}

void PatAction::randomEvent()
{
	if (pAnimation != nullptr)
	{
		return;
	}
	switch (rand()%3)

	{
	case 0:///非移动行为
		RunAction(CB_RANDOM_DOT, rand() % 20 + 5);
		SetCurrentbehavior(CB_RANDOM);
		break;
	case 1://横向移动
	{
		int end = 0;
		if (patInstance->x() > (desktopWidth / 2))
		{
			end = rand() % (desktopWidth / 2) + patInstance->width();
			RunAction(CB_RANDOM_LEFT, 15, QString(), qMakePair(end, patInstance->y()));
		}
		else
		{
			end = rand() % (desktopWidth / 2) + (desktopWidth / 2) - patInstance->width();
			RunAction(CB_RANDOM_RIGHT, 15, QString(), qMakePair(end, patInstance->y()));
		}
		SetCurrentbehavior(CB_RANDOM);
		break;
	}
	case 2:
		GoToWindowTop();
		break;
	default:
		break;
	}
 	srand(time(NULL));
}

void PatAction::GoToWindowTop()
{
	HWND hwndWindow;
	if (FindSuitWindow(hwndWindow))
	{

		HWND patHwnd = (HWND)patInstance->winId();	//获取宠物的句柄
		// 获取目标窗口的位置
		GetWindowsHandle handle;
		QVector<int> local = handle.GetWindowLocal(hwndWindow);
		qDebug() << QCoreApplication::applicationDirPath();
		// 加载dll
		QFileInfo file("Dll1.dll");
		qDebug() << file.absoluteFilePath();
		QString str = ".//dll//Dll1.dll";
		std::string str1 = str.toStdString(); //QString转为String
		LPCSTR strdll = str1.c_str();
		HINSTANCE hDLL = LoadLibraryA(strdll);
		if (hDLL != NULL)
		{
			typedef bool(*P)(HWND);
			P p = (P)GetProcAddress(hDLL, "SetBackHwnd");
			DWORD ID;
			if (! p(patHwnd))
			{
				qDebug() << "hook backcall Hwnd set failed";
				QMessageBox::warning(this, "warning", "hook backcall Hwnd set failed");
			}
			else
			{
				HOOKPROC  hkprcSysMsg = (HOOKPROC)GetProcAddress(hDLL, "MouseProc");
				HINSTANCE temp;
				HHOOK hhook = SetWindowsHookEx(
					WH_CALLWNDPROC,
					hkprcSysMsg,
					hDLL,
					GetWindowThreadProcessId(hwndWindow, (LPDWORD)&ID));
				if (hhook)
				{
					qDebug() << "----" << GetWindowThreadProcessId(hwndWindow, (LPDWORD)&ID) << "    " << ID;
					SetCurrentbehavior(CB_TOWINDOWSTOP);
					RunAction(CB_TOWINDOWSTOP, 15, QString(), qMakePair(local[2] + rand() % (local[3] - local[2] - patInstance->width()), local[0] - patInstance->height()));
					windowsInfo->SetHWND(hwndWindow);
					windowsInfo->SetOpen(true);
					connect(SignalsCore::Instance(), &SignalsCore::signal_windowChange, this, [=]()
					{
						RunAction(CB_DOWN, 15, QString(), qMakePair(patInstance->x(), desktopHeight - patInstance->height()));
						SetCurrentbehavior(CB_DOWN);
						windowsInfo->SetOpen(false);
						UnhookWindowsHookEx(hhook);
					});
					return;
				}
				else
				{
					qDebug() << "dll insert failed";
					QMessageBox::warning(this, "warning", "dll insert failed");
				}
			}
		}
		else
		{
			qDebug() << "dll加载失败";
			QMessageBox::warning(this, "warning", "dll add failed");
		}
	}
	emit lineActionFinish();
}

void PatAction::InitActionMap(QString dir, QMap<QString, QString>& actionMap, QStringList& actionList)
{
	QDirIterator it(dir, QDirIterator::Subdirectories);
	while (it.hasNext()) {
		QString path = it.next();
		QString key = path.split("/")[path.split("/").size() - 1];
		if (key.endsWith(".gif"))
		{
			actionMap.insert(key, path);
			actionList << key;
			//qDebug() << key << path;
		}
	}
}

void PatAction::ClearAction()
{
	if (movie != nullptr)
	{
		delete movie;
		movie = nullptr;
	}
	if (pAnimation != nullptr)
	{
		pAnimation->stop();
		pAnimation = nullptr;
	}
}

void PatAction::RunAction(Currentbehavior behaviorType, int spendTime, QString gifName, QPair<int, int> end, QPair<int, int> begin)
{
	QString gifPath = QString();
	switch (behaviorType)
	{
	case CB_NONE:gifName == QString() ? gifPath = noneMap[noneList[rand() % noneList.size()]] : gifPath = noneMap[gifName];
		break;
	case CB_RANDOM_DOT:gifName == QString() ? gifPath = randomDotMap[randomDotList[rand() % randomDotList.size()]] : gifPath = randomDotMap[gifName];
		break;
	case CB_RANDOM_LEFT:gifName == QString() ? gifPath = randomMoveLeftMap[randomMoveLeftList[rand() % randomMoveLeftList.size()]] : gifPath = randomMoveLeftMap[gifName];
		break;
	case CB_RANDOM_RIGHT:gifName == QString() ? gifPath = randomMoveRightMap[randomMoveRightList[rand() % randomMoveRightList.size()]] : gifPath = randomMoveRightMap[gifName];
		break;
	case CB_TOWINDOWSTOP:gifName == QString() ? gifPath = toWindowsTopMap[toWindowsTopList[rand() % toWindowsTopList.size()]] : gifPath = toWindowsTopMap[gifName];
		break;
	case CB_ONTOP:gifName == QString() ? gifPath = onTopMap[onTopList[rand() % onTopList.size()]] : gifPath = onTopMap[gifName];
		break;
	case CB_DOWN:gifName == QString() ? gifPath = downMap[downList[rand() % downList.size()]] : gifPath = downMap[gifName];
		break;
	case CB_SUSPENSION:gifName == QString() ? gifPath = suspensionMap[suspensionList[rand() % suspensionList.size()]] : gifPath = suspensionMap[gifName];
		break;
	case CB_DRAG:gifName == QString() ? gifPath = dragMap[dragList[rand() % dragList.size()]] : gifPath = dragMap[gifName];
		break;
	case CB_EAT:gifName == QString() ? gifPath = eatMap[eatList[rand() % eatList.size()]] : gifPath = eatMap[gifName];
		break;
	default:
		break;
	}
	if (spendTime > 0)
	{
		ProduceAction(gifPath, spendTime, end, begin);
	}
	else
	{
		OtherAction(gifPath);
	}
}

void PatAction::OtherAction(QString gifPath)
{
	ClearAction();
	movie = new QMovie(gifPath);
	movie->setScaledSize(movieLab->size());
	movie->start();
	movieLab->setMovie(movie);
	nowGif = gifPath;
}

void PatAction::ProduceAction(QString gifPath, int spendTime, QPair<int, int> end, QPair<int, int> begin)
{
	ClearAction();
	
	int beginx = 0;
	int beginy = 0;
	int endx = 0;
	int endy = 0;

	if (qMakePair(MININT, MININT) == begin)
	{
		beginx = patInstance->x();
		beginy = patInstance->y();
	}
	else
	{
		beginx = begin.first;
		beginy = begin.second;
	}

	if (qMakePair(MININT, MININT) == end)
	{
		endx = patInstance->x();
		endy = patInstance->y();
	}
	else
	{
		endx = end.first;
		endy = end.second;
	}

	int needTime = 0;	//该动作花费时间
	if (begin != end)
	{
		needTime = sqrt((beginx - endx) * (beginx - endx) + (beginy - endy) * (beginy - endy));
		needTime *= spendTime;
	}
	else
	{
		needTime = spendTime * 1000;
	}
		pAnimation = new QPropertyAnimation(patInstance, "geometry");
		pAnimation->setDuration(needTime+1);		//设置运动时间(最低一毫秒)
		pAnimation->setStartValue(QRect(beginx, beginy, patInstance->width(), patInstance->height()));   //起始位置
		pAnimation->setEndValue(QRect(endx, endy, patInstance->width(), patInstance->height()));       //终点位置
		pAnimation->start(QAbstractAnimation::DeleteWhenStopped);	//该动作停止时自动删除，释放内存
		movie = new QMovie(gifPath);
		movie->setScaledSize(movieLab->size());
		movieLab->setMovie(movie);
		movie->start();
		nowGif = gifPath;
		connect(pAnimation, &QPropertyAnimation::finished, this, [=]()
		{
			pAnimation = nullptr;
			emit lineActionFinish();
		});
}

bool PatAction::FindSuitWindow(HWND &hwndOut)
{
	int tallMax = desktopHeight;
	QVector<HWND> suitWindows = QVector<HWND>();
	GetWindowsHandle handle;
	for each (auto hwnd in handle.GetHandle())
	{
		char title[MAXBYTE];
		GetWindowTextA(hwnd, title, MAXBYTE);
		QString titleStr = QString::fromLocal8Bit(title);

		if (hwnd != (HWND)patInstance->winId())
		{
			QVector<int> local = handle.GetWindowLocal(hwnd);

			if (local.size() == 4)
			{
				bool suit = local[0] > patInstance->height(); //里顶部的空间大于宠物的身高
				suit &= local[3] - local[2] > patInstance->width(); //窗口的高度大于宠物的高度
				suit &= (hwnd == GetForegroundWindow());	//判断该窗口是否有焦点

				if (suit)
				{
					DWORD ID;
					GetWindowThreadProcessId(hwnd, (LPDWORD)&ID);
					if (CommomTools::Is64BitPorcess(ID))
					{
						hwndOut = hwnd;
						qDebug() << "---" << titleStr;
						return true;
					}
				}
			}
		}
	}
	return false;
}

void PatAction::mouseRelease(Qt::MouseButton btn)
{
	if (patInstance->x() <= 0)
	{
		SetCurrentbehavior(CB_SUSPENSION);
		RunAction(CB_SUSPENSION, 1, "suspension_left.gif", qMakePair(0, patInstance->y()));
	}
	else if (patInstance->x() >= (desktopWidth - patInstance->width()))
	{
		SetCurrentbehavior(CB_SUSPENSION);
		RunAction(CB_SUSPENSION, 1, "suspension_right.gif", qMakePair(desktopWidth- patInstance->width(), patInstance->y()));
	}
	else if (patInstance->y() < patInstance->height())
	{
		SetCurrentbehavior(CB_SUSPENSION);
		RunAction(CB_SUSPENSION, 1, "suspension_top.gif", qMakePair(patInstance->x(), 0));
	}
	else if (btn == Qt::LeftButton)
	{
		RunAction(CB_DOWN, 15, QString(), qMakePair(patInstance->x(), desktopHeight - patInstance->height()));
		SetCurrentbehavior(CB_DOWN);
	}
}

void PatAction::mouseMove()
{
	//左键移动
	if (nowGif != dragMap["push.gif"])
	{
		RunAction(CB_DRAG);
		SetCurrentbehavior(CB_DRAG);
	}
}

void PatAction::dragEnter()
{
	SetCurrentbehavior(CB_EAT);
	RunAction(CB_EAT, 0, "happy.gif");
}

void PatAction::dragLeave()
{
	RunAction(CB_EAT, 3, "cry.gif");
	SetCurrentbehavior(CB_NONE);
}

void PatAction::drop(QStringList pathlist)
{
	static int eatNum = 0;
	bool allFile = true;
	bool allDir = true;
	//去掉重复路径
	pathlist = pathlist.toSet().toList();
	foreach(QString path, pathlist)
	{
		QFileInfo file(path);
		if (file.isDir())
		{
			QDir dir(file.absoluteFilePath());
			dir.removeRecursively();
			allFile = false;
		}
		else
		{
			QFile::remove(file.absoluteFilePath());
			allDir = false;
		}
		eatNum++;
	}
	if (eatNum < 4)
	{
		if (pathlist.size() == 1 && allFile)
		{
			RunAction(CB_EAT, 3, "milk.gif");
		}
		else if (pathlist.size() > 1 && allFile)
		{
			RunAction(CB_EAT, 3, "eatmelon.gif");
		}
		else if (pathlist.size() == 1 && allDir)
		{
			RunAction(CB_EAT, 3, "eatpizza.gif");
		}
		else if (pathlist.size() > 1 && allDir)
		{
			RunAction(CB_EAT, 3, "eatburger.gif");
		}
		else
		{
			RunAction(CB_EAT, 3, "eatchicken.gif");
		}
	}
	else
	{
		RunAction(CB_EAT, 12, "milktoyue.gif");
	}
	SetCurrentbehavior(CB_NONE);
	
	QTimer::singleShot(1000 * 10, this, [=]() {
		eatNum = 0;
	});
}

PatAction::~PatAction()
{}
