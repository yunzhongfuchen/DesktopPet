#ifndef SIGNALS
#define SIGNALS

#include <QWidget>

class SignalsCore  : public QWidget
{
	Q_OBJECT

public:
	static SignalsCore* Instance();

private:
	static SignalsCore* signalsCore;
public slots:
	void windowChange();

	//鼠标事件信号
	void mousePress();
	void mouseRelease(Qt::MouseButton btn);
	void mouseMove();
	void dragEnter();
	void dragLeave();
	void drop(QStringList pathlist);
signals:
	void signal_windowChange();

	void signal_mousePress();
	void signal_mouseRelease(Qt::MouseButton btn);
	void signal_mouseMove();
	void signal_dragEnter();
	void signal_dragLeave();
	void signal_drop(QStringList pathlist);
};


#endif // !SIGNALS