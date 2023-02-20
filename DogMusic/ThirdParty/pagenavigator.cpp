#include "pagenavigator.h"

#include <QButtonGroup>

#include <QIntValidator>
//��Ҫ��������������setProperty����ʹ������
#include <QPaintEvent>
#include <QDebug>
#include <QtCore/qmath.h>

PageNavigator::PageNavigator(QWidget *parent) : QWidget(parent)
{
    mPageCount = 1;
    QString qss =QString("")
            + QString("QPushButton{border: none; background: #f5f5f5;border-radius: 4px;}")
            + QString("QPushButton:hover{background: #e7e7e7;}")
            + QString("QPushButton[currentPage=\"true\"]{background: #409EFF;color:#FFF}");
    this->setStyleSheet(qss);

    this->setFixedHeight(30);
    mBtnGroup=new QButtonGroup(this);
    mPageRoot= new QHBoxLayout(this);
    mPageRoot->setSpacing(8);
    mPageRoot->setMargin(0);

    //������һҳ��ť
    mPrevBtn=setPageButtonStyle();
    mPrevBtn->setIcon(QIcon(":/images/navigation/btn_back.png"));

    mStartBtn=setPageButtonStyle();
    mBtnGroup->addButton(mStartBtn);


    //����ҳ���б�
    mPagersList=new QList<QPushButton *>();

    //������һҳ��ť
    mNextBtn=setPageButtonStyle();
    mNextBtn->setIcon(QIcon(":/images/navigation/btn_front.png"));


    //����ǰ������ҳ����
    mGoToLabel=new QLabel(this);
    mGoToLabel->setText(QString::fromLocal8Bit("ǰ��"));

    mPageLineEdit=new QLineEdit(this);
    mPageLineEdit->installEventFilter(this);

    mPageLineEdit->setFixedSize(50,30);
    mPageLineEdit->setAlignment(Qt::AlignHCenter);

    mPageLineEdit->setValidator(new QIntValidator(1, 10000, this));
    mPageLineEdit->setStyleSheet("QLineEdit{border-radius: 4px;border: 1px solid #dcdfe6;}");

    mPageLabel=new QLabel(this);
    mPageLabel->setText(QString::fromLocal8Bit("ҳ"));
}

PageNavigator::~PageNavigator()
{
    delete mPagersList;
}

//total:���ݵ�������  pagesize:ÿҳ��ʾ����������,currentPage:��ǰ�ڼ�ҳ
void PageNavigator::initialize()
{
    mPagerCount=1;  //�����ܸ���
    mPageCount = 1; //��ҳ������
    mStartBtn->setText(QString("%1").arg(1));

    mPageRoot->addStretch();
    mPageRoot->addWidget(mPrevBtn);
    mPageRoot->addWidget(mStartBtn);

    for(int i=1;i<10;i++){

        QPushButton *tempBtn=setPageButtonStyle();
        mBtnGroup->addButton(tempBtn);
        tempBtn->setText(QString("%1").arg(i+1));
        tempBtn->hide();
        mPageRoot->addWidget(tempBtn);
    }

    mPageRoot->addWidget(mNextBtn);
    mPageRoot->addWidget(mGoToLabel);
    mPageRoot->addWidget(mPageLineEdit);
    mPageRoot->addWidget(mPageLabel);

    mPageRoot->addStretch();

    setCurrentPage(1,false);
}

QPushButton* PageNavigator::setPageButtonStyle(){

    QPushButton *btn=new QPushButton(this);
    btn->setFixedSize(30,30);
    btn->setCursor(Qt::PointingHandCursor);
    btn->installEventFilter(this);
    return btn;
}

void PageNavigator::setCurrentPage(int page,int sendFlag){

    if (page < 1) {
        page = 1;
    }
    if (page > mPageCount) {
        page = mPageCount;
    }

    mCurrentPage = page;

    mPageLineEdit->setText(QString("%1").arg(page));

    for(int i=0;i<mBtnGroup->buttons().count();i++){

        if(mBtnGroup->buttons().at(i)->text().toInt()==page){

            mBtnGroup->buttons().at(i)->setProperty("currentPage","true");

        }else{

            mBtnGroup->buttons().at(i)->setProperty("currentPage","false");
        }

        mBtnGroup->buttons().at(i)->setStyleSheet("/**/");
    }

    mPrevBtn->setCursor(Qt::PointingHandCursor);
    mNextBtn->setCursor(Qt::PointingHandCursor);

    mPrevBtn->setCheckable(true);
    mNextBtn->setCheckable(true);

    if(page==1){
        mPrevBtn->setCheckable(false);
        mPrevBtn->setCursor(Qt::ForbiddenCursor);
    }

    if(page==mPageCount){
        mNextBtn->setCheckable(false);
        mNextBtn->setCursor(Qt::ForbiddenCursor);
    }

    //�л���ҳʱ���Ƿ����ź�
    if (sendFlag == true)
    {
        emit currentPageChanged(page);
    }
}

/**
 * @brief ������ҳ��
 * @param pageCount
 */
void PageNavigator::setPageCount(int pageCount){

    int count = pageCount > 10 ? 10 : pageCount;
    count = count <= 0 ? 1 : count;

    mPageCount=count;

    for(int i=0;i<9;i++){
        if (i < count-1)
        {
            getButtons().at(i)->show();
        }
        else
        {
            getButtons().at(i)->hide();
        }
    }

    setCurrentPage(1,false);
}

int PageNavigator::getCurrentPageCount(void)
{
    return mPageCount;
}

/**
 * @brief ɸѡ���˵�һҳ�����һҳ�İ�ť��
 * @return
 */
QList<QAbstractButton *> PageNavigator::getButtons(){

    QList<QAbstractButton *> *buttons=new QList<QAbstractButton *>();

    for(int i=0;i<mBtnGroup->buttons().count();i++){

        //���仯�İ�ťɸѡ����
        if(mBtnGroup->buttons().at(i)!=mStartBtn){

            buttons->append(mBtnGroup->buttons().at(i));
        }
    }

    return *buttons;
}

/**
 * @brief ����ҳ��
 * @return
 */
QList<int> PageNavigator::updatePages(){

    int pagerCount=mPagerCount;
    int halfPagerCount=(pagerCount-1)/2;

    int currentPage=mCurrentPage;
    int pageCount=mPageCount;

    bool showPrevMore = false;
    bool showNextMore = false;

    if (pageCount > pagerCount) {
        if (currentPage > pagerCount - halfPagerCount) {
            showPrevMore = true;
        }

        if (currentPage < pageCount - halfPagerCount) {
            showNextMore = true;
        }
    }

    QList<int> array;

    if(showPrevMore&&!showNextMore){
        int startPage=pageCount - (pagerCount - 2);
        for(int i=startPage;i<pageCount;i++){
            array.append(i);
        }
    }else if(!showPrevMore && showNextMore){
        for (int i = 2; i < pagerCount; i++) {
            array.append(i);
        }
    }else if (showPrevMore && showNextMore) {
        int offset = qFloor(pagerCount / 2) - 1;
        for (int i = currentPage - offset ; i <= currentPage + offset; i++) {
            array.append(i);
        }
    } else {
        for (int i = 2; i < pageCount; i++) {
            array.append(i);
        }
    }

    mShowPrevMore = showPrevMore;
    mShowNextMore = showNextMore;

    return array;
}

bool PageNavigator::eventFilter(QObject *watched, QEvent *e){

    if (e->type() == QEvent::MouseButtonRelease){

        int newPage =-1;
        int pageCount = mPageCount;
        int currentPage = mCurrentPage;
        int pagerCountOffset = mPagerCount - 2;

        //��һҳ��ť�����
        if(watched==mPrevBtn&&mPrevBtn->isCheckable()){

            newPage = mCurrentPage - 1;
            setCurrentPage(newPage);
        }

        //��һҳ��ť�����
        if(watched==mNextBtn&&mNextBtn->isCheckable()){

            newPage = mCurrentPage + 1;
            setCurrentPage(newPage);
        }

        for (int i = 0; i < mBtnGroup->buttons().count(); ++i)
        {
            if (watched == mBtnGroup->buttons().at(i))
            {
                newPage = mBtnGroup->buttons().at(i)->text().toInt();
                setCurrentPage(newPage);
                break;
            }
        }

    }

    //��תҳ�û��س��¼�
    if (watched == mPageLineEdit && e->type() == QEvent::KeyRelease)
    {
        QKeyEvent *ke = static_cast<QKeyEvent *>(e);
        if (ke->key() == Qt::Key_Enter || ke->key() == Qt::Key_Return)
        {
            setCurrentPage(mPageLineEdit->text().toInt());
            return true;
        }
    }


    return QWidget::eventFilter(watched, e);
}



