#ifndef FIRSTOPEN
#define FIRSTOPEN

#include <QDialog>
#include <QCloseEvent>
#include <QLabel>
#include <QPushButton>
#include "../Common/CommomTools.h"
class FirstOpen  : public QDialog
{
	Q_OBJECT

public:
	FirstOpen(QWidget *parent = nullptr);
	~FirstOpen();

private:
	QLabel* m_text = nullptr;
	QPushButton* m_next = nullptr;
	QLabel* m_movieLab = nullptr;
	QMovie *movie = nullptr;
	QLabel* m_text2 = nullptr;
	void closeEvent(QCloseEvent* event);
	void BeginCourse();
};

#endif // !FIRSTOPEN