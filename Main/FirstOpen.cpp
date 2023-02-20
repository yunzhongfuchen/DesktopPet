#include "FirstOpen.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QMovie>
#include <QTimer>
#include <QDebug>
FirstOpen::FirstOpen(QWidget *parent)
	: QDialog(parent)
{
	this->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
	this->setWindowTitle(__QString("���ֽ̳�"));
	QVBoxLayout * mainLay = new QVBoxLayout(this);
	m_text = new QLabel(this);
	QFont font;
	font.setPointSize(15);
	m_text->setFont(font);
	m_text->setText(__QString("��������ҪһЩ����������"));
	m_next = new QPushButton(__QString("��һ��"), this);
	m_next->setVisible(false);
	QHBoxLayout* tip = new QHBoxLayout();
	tip->addWidget(m_text);
	tip->addWidget(m_next, 0, Qt::AlignRight);
	
	QPushButton* yes1 = new QPushButton(__QString("��"), this);
	QPushButton* yes2 = new QPushButton(__QString("�õ�"), this);
	QPushButton* yes3 = new QPushButton(__QString("��Ը��"), this);
	QHBoxLayout* yesLay = new QHBoxLayout();
	yesLay->addWidget(yes1);
	yesLay->addWidget(yes2);
	yesLay->addWidget(yes3);

	m_text2 = new QLabel(this);
	m_text2->setFont(font);
	m_movieLab = new QLabel(this);
	m_movieLab->setFixedSize(566, 282);
	m_movieLab->setVisible(false);
	mainLay->addSpacing(15);
	mainLay->addLayout(tip);
	mainLay->addWidget(m_text2);
	mainLay->addSpacing(15);
	mainLay->addWidget(m_movieLab);
	mainLay->addSpacing(15);
	mainLay->addLayout(yesLay);
	
	mainLay->addStretch();
	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	connect(yes1, &QPushButton::clicked, this, [=]()
	{
		yes1->setVisible(false);
		yes2->setVisible(false);
		yes3->setVisible(false);
		m_movieLab->setVisible(true);
		BeginCourse();
	});
	connect(yes2, &QPushButton::clicked, yes1, &QPushButton::click);
	connect(yes3, &QPushButton::clicked, yes1, &QPushButton::click);
	connect(m_next, &QPushButton::clicked, this, &FirstOpen::BeginCourse);
}

FirstOpen::~FirstOpen()
{}

void FirstOpen::BeginCourse()
{
	static int flag = 0;
	m_next->setVisible(false);
	if (movie != nullptr)
	{
		delete movie;
		movie = nullptr;
	}
	QString movPath;
	QString info;
	int time = 1;
	QFile file("first.txt");
	switch (flag)
	{
	case 0:
		info = __QString("������������·������˶�");
		movPath = ":/TablePatOther/gif/other/teachone.gif";
		time = 3;
		break;
	case 1:
		info = __QString("��Ȼ���е�ʱ��Ҳϲ�����ڸߴ�����ܺ���������");
		movPath = ":/TablePatOther/gif/other/teachtwo.gif";
		time = 5;
		break;
	case 2:
		info = __QString("����㲻���������ܣ�Ҳ���԰������ڱ���");
		movPath = ":/TablePatOther/gif/other/teachthree.gif";
		time = 4;
		break;
	case 3:
		info = __QString("���в���Ҫ���ļ�����Ҳ���԰���Ե�");
		movPath = ":/TablePatOther/gif/other/teachfore.gif";
		time = 17;
		break;
	case 4:
		if (file.exists())
		{
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			info = file.readLine().toStdString().c_str();
			m_text2->setText(file.readLine().toStdString().c_str());
			file.close();
		}
		else
		{
			info = __QString("��������ʱ��㣬��Ȼ��Ҫ����˵");
			m_text2->setText(__QString("ף����Զ�ഺ�������������ÿһ�죡"));
		}
		movPath = ":/TablePatOther/gif/other/newyear.gif";
		m_next->setText(__QString("���"));
		time = 5;
		break;
	default:
		this->reject();
		return;
	}

	m_text->setText(info);
	movie = new QMovie(movPath);
	movie->setScaledSize(m_movieLab->size());
	movie->start();
	m_movieLab->setMovie(movie);
	
	flag++;
	QTimer::singleShot(time * 1000, this, [=]() {
		qDebug() << flag;
		if (flag == 4)
		{
			m_text2->setText(__QString("��ܰ��ʾ���벻Ҫ������ʳ"));
			QPalette pe;
			pe.setColor(QPalette::WindowText, Qt::green);
			m_text2->setPalette(pe);
		}
		m_next->setVisible(true);
	});
	
}
void FirstOpen::closeEvent(QCloseEvent* event)
{
	QMessageBox::question(this, __QString("��ʾ"), __QString("���Ҫ�˳�����������"), __QString("��"), __QString("��"));
	event->ignore();
}