#ifndef PAGENAVIGATOR_H
#define PAGENAVIGATOR_H

#include <QEvent>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include <QHBoxLayout>
#include <QButtonGroup>

class PageNavigator : public QWidget
{
    Q_OBJECT
public:
    explicit PageNavigator(QWidget *parent = 0);
    ~PageNavigator();

public:
    void initialize();

    void setCurrentPage(int page, int sendFlag = true);

    QList<int> updatePages();
    void setPageCount(int pageCount);
    int getCurrentPageCount(void);
protected:
    virtual bool eventFilter(QObject *watched, QEvent *e);
signals:
    void currentPageChanged(int page);
private:
    QPushButton* setPageButtonStyle();


    QList<QAbstractButton *> getButtons();//获取除了第一个和最后一页的按钮
private :
    QButtonGroup *mBtnGroup;
    QPushButton *mPrevBtn;//上一页
    QPushButton *mNextBtn;//下一页

    QPushButton *mStartBtn;//第一页按钮

    QList<QPushButton *> *mPagersList;//两个省略号中间的按钮列表


    QLabel *mGoToLabel;
    QLabel *mPageLabel;
    QLineEdit *mPageLineEdit;
    QHBoxLayout *mPageRoot;

    int mCurrentPage;
    int mPagerCount;//页码按钮的数量
    int mPageCount;//总页数

    bool mShowPrevMore;//显示左边更多按钮
    bool mShowNextMore;//显示右边更多按钮


};

#endif // PAGENAVIGATOR_H
