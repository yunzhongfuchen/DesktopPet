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

	//����¼��ź�
	void mousePress(int flag);
	void mouseRelease(Qt::MouseButton btn, int flag);
	void mouseMove(int flag);
	void dragEnter(int flag);
	void dragLeave(int flag);
	void drop(QStringList pathlist, int flag);

	void musicBox();
	void AddPet();
	void ReducePet();
	//	�Ƿ����windows��Ϣ
	void SetWindowInfoReceive(bool isOpen);
	//	��������
	void EndPet();
signals:
	void signal_windowChange();

	void signal_mousePress(int flag);
	void signal_mouseRelease(Qt::MouseButton btn, int flag);
	void signal_mouseMove(int flag);
	void signal_dragEnter(int flag);
	void signal_dragLeave(int flag);
	void signal_drop(QStringList pathlist, int flag);

	void signal_musicBox();
	void signal_AddPet();
	void signal_ReducePet(); 
	void signal_SetWindowInfoReceive(bool isOpen);
	void signal_EndPet();
};


#endif // !SIGNALS