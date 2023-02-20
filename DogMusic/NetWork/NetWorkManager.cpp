#include "NetWorkManager.h"

//QNetworkAccessManager NetWorkManager::NetWorkAccess;

NetWorkManager::NetWorkManager(QString url, QString content, ReceiveFun receiver, void* pCusData, int timeout, QString msg)
{
    m_url = url;
    m_content = content;
    m_receiver = receiver;
    m_pCusdata = pCusData;
    m_timeout = timeout;
    m_msg = msg;

    m_pNetworkAccessManager = new QNetworkAccessManager();
    //m_pNetworkAccessManager = &NetWorkManager::NetWorkAccess;
    m_pTimer = new QTimer();

    connect( m_pNetworkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(OnReplyFinished(QNetworkReply*)) );
    connect( m_pTimer, SIGNAL(timeout()), this, SLOT(OnTimer()));
}
NetWorkManager::~NetWorkManager()
{
    if (m_pNetworkAccessManager)
    {
        delete m_pNetworkAccessManager;
        m_pNetworkAccessManager = nullptr;
    }

    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }
}

void NetWorkManager::get( const QString& url, ReceiveFun receiver, void* data, QString msg /*= ""*/, int timeout /*= LimitTimeOut*/)
{
    NetWorkManager* p = new NetWorkManager( url, "", receiver, data, timeout, msg );
    p->excuteGet();
}

void NetWorkManager::post( const QString& url, const QString& content, ReceiveFun receiver, void* data, QString msg /*= ""*/, int timeout /*= LimitTimeOut*/)
{
    NetWorkManager* p = new NetWorkManager( url, content ,receiver, data, timeout, msg );
    p->excutePost();
}

void NetWorkManager::OnReplyFinished( QNetworkReply* reply )
{
    m_pTimer->stop();
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        if (m_receiver)
        {
            m_receiver(E_NetOK, bytes, m_pCusdata, m_msg);
        }
    }
    else
    {
        if (m_receiver)
        {
            m_receiver(E_NetReplyError, QByteArray(), m_pCusdata, m_msg);
        }
        //QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //QString strReplyInfo = QString::number(statusCodeV.toInt()) + "," + QString::number(reply->error()) + "," + reply->errorString();
        //g_WriteLog.WriteLog(strReplyInfo);
    }
    reply->deleteLater();
    this->deleteLater();
}

void NetWorkManager::OnTimer()
{
    m_pTimer->stop();
    if (m_receiver)
    {
        m_receiver( E_NetTimeOut, QByteArray(), m_pCusdata, m_msg); //超时
    }
    this->deleteLater();
}

void NetWorkManager::excuteGet()
{
    QNetworkRequest network_request;
    //network_request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=UTF-8"));
    //Token应该放在请求体中，提前与服务器端约定
    //QByteArray qbaUserToken(g_strToken.toStdString().c_str());
    //network_request.setRawHeader("Authorization", qbaUserToken);
    network_request.setUrl( QUrl(m_url) );
    m_pNetworkAccessManager->get(network_request);
    m_pTimer->start(m_timeout);
}

void NetWorkManager::excutePost()
{
    QNetworkRequest network_request;
    //network_request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json;charset=UTF-8"));
    //Token应该放在请求体中，提前与服务器端约定
    //QByteArray qbaUserToken(g_strToken.toStdString().c_str());
    //network_request.setRawHeader("Authorization", qbaUserToken);
    network_request.setUrl( QUrl(m_url) );
    QByteArray post_data;
    post_data.append(m_content);
    m_pNetworkAccessManager->post(network_request, post_data);
    m_pTimer->start(m_timeout);
}







