#include "TablePat.h"
#include <QDirIterator>
#include <QMimeData>

TablePat* TablePat::tablePat = nullptr;

TablePat* TablePat::Instance(QWidget* parent)
{
    if (!tablePat)
    {
        tablePat = new TablePat(parent);
    }
    return tablePat;
}

TablePat::TablePat(QWidget *parent)
    : QWidget(parent)
{
    //ui.setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//隐藏标题栏
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool);//置顶
    this->setAttribute(Qt::WA_TranslucentBackground);//隐藏
	this->setAcceptDrops(true);
    movieLab = new QLabel(this);
    //根据桌面长度来设置画面宠物大小
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    int desktopWidth = pDesktopWidget->availableGeometry().width();//桌面的宽度
    int desktopHeight = pDesktopWidget->availableGeometry().height();//桌面的高度
    this->resize(desktopWidth / 20, desktopWidth / 20);
    movieLab->resize(desktopWidth /20, desktopWidth /20);
    //初始处于底部的黄金分割位置
    this->move((desktopWidth)*0.681, desktopHeight - this->height());
}

TablePat::~TablePat()
{

}

void TablePat::SetCurrentbehavior(Currentbehavior behavior)
{
	currentbehavior = behavior;
}
void TablePat::mousePressEvent(QMouseEvent* event)
{
	//左键点击记录位置
	SignalsCore::Instance()->mousePress();
	if (event->button() == Qt::LeftButton)
	{
		diff_ = event->globalPos() - this->pos();
	}
	//中键
	else if (event->button() == Qt::MidButton)
	{
		qApp->exit(0);
	}
	else if (event->button() == Qt::RightButton)
	{
		
	}
}

void TablePat::mouseReleaseEvent(QMouseEvent* event)
{
	SignalsCore::Instance()->mouseRelease(event->button());
}

void TablePat::mouseMoveEvent(QMouseEvent* event)
{
	//左键移动
	if (event->buttons() & Qt::LeftButton)
	{
		this->move(event->globalPos() - this->diff_);
		SignalsCore::Instance()->mouseMove();
	}
}

void TablePat::dragEnterEvent(QDragEnterEvent *event)
{
	bool canEat = (currentbehavior == CB_NONE);
	canEat |= (currentbehavior == CB_RANDOM);
	canEat &= event->mimeData()->hasUrls();
	if (canEat)
	{
		QFileInfo file(event->mimeData()->urls().at(0).toLocalFile());
 		if (file.exists())
		{
			event->setAccepted(true);
			SignalsCore::Instance()->dragEnter();
		}
	}
}

void TablePat::dragLeaveEvent(QDragLeaveEvent *event)
{
	SignalsCore::Instance()->dragLeave();
}

void TablePat::dropEvent(QDropEvent *event) 
{
	QList<QUrl> urls = event->mimeData()->urls();
	QStringList pathlist = QStringList();
	foreach(QUrl url, urls)
	{
		QString filepath = url.toLocalFile();
		pathlist.append(filepath);
	}
	SignalsCore::Instance()->drop(pathlist);
}