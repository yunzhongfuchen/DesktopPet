#ifndef NET_WORKMANAGER_H
#define NET_WORKMANAGER_H

#include <QObject>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QTimer>

#define LIMITTIMEOUT 5000 //请求超时事件(毫秒)

typedef void (*ReceiveFun)(int errCode, const QByteArray& bytes, void* pCusData,const QString msg);

enum E_NetworkReply
{
    E_NetOK,
    E_NetTimeOut,//超时
    E_NetReplyError,
};

class NetWorkManager : public QObject
{
    Q_OBJECT
public:
    NetWorkManager(QString url, QString content, ReceiveFun receiver, void* pCusData, int timeout, QString msg);
    ~NetWorkManager();

public:
    static void get(const QString& url, ReceiveFun receiver, void* data, QString key = "", int timeout = LIMITTIMEOUT);
    static void post(const QString& url, const QString& content, ReceiveFun receiver, void* data, QString key = "", int timeout = LIMITTIMEOUT);

    static QNetworkAccessManager NetWorkAccess;
private:
    void excuteGet();
    void excutePost();

private slots:
    void OnReplyFinished(QNetworkReply* reply);
    void OnTimer();

private:
    QNetworkAccessManager* m_pNetworkAccessManager;
    QTimer*		m_pTimer;

    QString		m_url;
    QString		m_content;
    ReceiveFun  m_receiver;
    void*		m_pCusdata;
    int			m_timeout;
    QString     m_msg;
};





#endif

