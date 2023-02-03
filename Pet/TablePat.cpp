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
    this->setWindowFlags(Qt::FramelessWindowHint);//���ر�����
    this->setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint | Qt::Tool);//�ö�
    this->setAttribute(Qt::WA_TranslucentBackground);//����
	this->setAcceptDrops(true);
    movieLab = new QLabel(this);
    //�������泤�������û�������С
    QDesktopWidget* pDesktopWidget = QApplication::desktop();
    int desktopWidth = pDesktopWidget->availableGeometry().width();//����Ŀ��
    int desktopHeight = pDesktopWidget->availableGeometry().height();//����ĸ߶�
    this->resize(desktopWidth / 20, desktopWidth / 20);
    movieLab->resize(desktopWidth /20, desktopWidth /20);
    //��ʼ���ڵײ��Ļƽ�ָ�λ��
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
	//��������¼λ��
	SignalsCore::Instance()->mousePress();
	if (event->button() == Qt::LeftButton)
	{
		diff_ = event->globalPos() - this->pos();
	}
	//�м�
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
	//����ƶ�
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