#include "TablePat.h"
#include <QDirIterator>
#include <QMimeData>
#include <QProcess>
#include <QAction>
#include <QMenu>


TablePat::TablePat(QWidget *parent, int flag)
    : QWidget(parent)
{
    //ui.setupUi(this);
	this->flag = flag;
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

void TablePat::SetCurrentbehavior(Currentbehavior behavior)
{
	currentbehavior = behavior;
}

void TablePat::mousePressEvent(QMouseEvent* event)
{
	//��������¼λ��
	SignalsCore::Instance()->mousePress(flag);
	if (event->button() == Qt::LeftButton)
	{
		diff_ = event->globalPos() - this->pos();
	}
	//�м�
	else if (event->button() == Qt::MidButton)
	{
		SignalsCore::Instance()->ReducePet();
		this->close();
	}
	else if (event->button() == Qt::RightButton)
	{
		
		QMenu *menu = new QMenu(this);
		menu->setMinimumWidth(150);
		menu->setAttribute(Qt::WA_DeleteOnClose);
		QAction* addPet = new QAction(__QString("����è"));
		QAction* music = new QAction(__QString("��������"));
		menu->addAction(addPet);
		menu->addAction(music);
		static QProcess* m_process = new QProcess();
		connect(music, &QAction::triggered, this, [=] {
			//��������
			//�Ƚ�music����exe���ٵ���exe����������ʱ����
			/*MusicMain::Instance();
			SignalsCore::Instance()->musicBox();*/
			QString cmdstr = QString("%1/../../Dll/DogMusic/cloudmusic.exe").arg(qApp->applicationDirPath());
			m_process->start(cmdstr);
		});
		connect(addPet, &QAction::triggered, SignalsCore::Instance(), &SignalsCore::AddPet);
		menu->exec(QCursor::pos());
	}
}

void TablePat::mouseReleaseEvent(QMouseEvent* event)
{
	SignalsCore::Instance()->mouseRelease(event->button(), flag);
}

void TablePat::mouseMoveEvent(QMouseEvent* event)
{
	//����ƶ�
	if (event->buttons() & Qt::LeftButton)
	{
		this->move(event->globalPos() - this->diff_);

		SignalsCore::Instance()->mouseMove(flag);
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
			SignalsCore::Instance()->dragEnter(flag);
		}
	}
}

void TablePat::dragLeaveEvent(QDragLeaveEvent *event)
{
	SignalsCore::Instance()->dragLeave(flag);
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
	SignalsCore::Instance()->drop(pathlist, flag);
}