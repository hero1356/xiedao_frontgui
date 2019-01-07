#include "Http.h"
#include <QTextCodec>
#include <QCryptographicHash>

const int nHTTP_TIME = 10000; //10秒

Http::Http(QObject *parent) :
    QObject(parent)
{
    this->isreturn = false;
    m_pNetworkManager = new QNetworkAccessManager(this);
    m_pNetworkReply = NULL;
    m_pTimer = new QTimer(this);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));//超时信号
}

//发起请求
void Http::get(const QString &strUrl)
{
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    {
        QSslConfiguration sslConfig;
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfig.setProtocol(QSsl::TlsV1_1);
        netRequest.setSslConfiguration(sslConfig);
    }
//    QString strBody; //http body部分，可封装参数信息
//    QByteArray contentByteArray = strBody.toLatin1();//转成二进制
//    m_pNetworkReply = m_pNetworkManager->post(netRequest,contentByteArray);//发起post请求

    m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
    connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号
    m_pTimer->start(nHTTP_TIME);
}

//发起请求
void Http::post(const QString &strUrl, QString strBody)
{
    m_strUrl = strUrl;
    QNetworkRequest netRequest;
    netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    netRequest.setUrl(QUrl(strUrl)); //地址信息
    if(strUrl.toLower().startsWith("https"))//https请求，需ssl支持(下载openssl拷贝libeay32.dll和ssleay32.dll文件至Qt bin目录或程序运行目录)
    {
        QSslConfiguration sslConfig;
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
        sslConfig.setProtocol(QSsl::TlsV1_1);
        netRequest.setSslConfiguration(sslConfig);
    }
//    QString strBody; //http body部分，可封装参数信息
    QByteArray contentByteArray = strBody.toLocal8Bit();//转成二进制
    m_pNetworkReply = m_pNetworkManager->post(netRequest,contentByteArray);//发起post请求
//    m_pNetworkReply = m_pNetworkManager->get(netRequest); //发起get请求
    connect(m_pNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //请求完成信号
    m_pTimer->start(nHTTP_TIME);
}


//请求结束
void Http::slot_requestFinished()
{
    QMutexLocker locker(&isreturnMutex);
    if(isreturn){
        return;
    }
    m_pTimer->stop();//关闭定时器
    QByteArray resultContent = m_pNetworkReply->readAll();
    QTextCodec* pCodec = QTextCodec::codecForName("GBK");
    QString strResult = pCodec->toUnicode(resultContent);
    int nHttpCode = m_pNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    if(nHttpCode == 200)//成功
    {
        emit signal_requestFinished(true,strResult);//请求成功
    }
    else
    {
        emit signal_requestFinished(false,strResult);//请求失败
    }
    m_pNetworkReply->deleteLater();
    this->deleteLater(); //释放内存
}

//请求超时
void Http::slot_requestTimeout()
{
    QMutexLocker locker(&isreturnMutex);
    if(isreturn){
        return;
    }else{
        isreturn=true;
    }
    emit signal_requestFinished(false,"timeout");//请求失败
    m_pNetworkReply->deleteLater();
    this->deleteLater();//释放内存

}
