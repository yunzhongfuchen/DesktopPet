#include "PSEventController.h"
#include <QWriteLocker>

QMap<QByteArray, QList<QObject*>> PSEventController::psEvents_pool_;
QReadWriteLock PSEventController::ps_Lock_;
QString PSEventController::ps_LastError_;


void PSEventController::unSubscribe(QObject* listener, const QByteArray& eventName)
{
    QWriteLocker locker(&ps_Lock_);
    int index = -1;
    if (psEvents_pool_.contains(eventName) &&
        (index = psEvents_pool_[eventName].indexOf(listener)) >= 0 && index < psEvents_pool_[eventName].count())
        psEvents_pool_[eventName].takeAt(index);
}

bool PSEventController::subscribe(QObject* listener, const QByteArray& eventName)
{
    QWriteLocker locker(&ps_Lock_);
    if (psEvents_pool_.contains(eventName)) {
        if (-1 != psEvents_pool_[eventName].indexOf(listener)) {
            ps_LastError_ = QString("This object is subscribed to this eventName");
            return false;
        }
        psEvents_pool_[eventName].push_back(listener);
        return true;
    } else {
        psEvents_pool_.insert(eventName, { listener });
        return true;
    }
}

bool PSEventController::publish(const QByteArray& eventName, Qt::ConnectionType connectionType,
    QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3,
    QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7,
    QGenericArgument val8, QGenericArgument val9)
{
    QReadLocker locker(&ps_Lock_);
    if (!psEvents_pool_.contains(eventName)) {
        ps_LastError_ = QString("No objects subscribe to this eventName");
        return false;
    }
    auto methodName = methodFormatting(eventName);
    QStringList errors;
    for (auto listener : psEvents_pool_[eventName]) {
        if (!listener)
            continue;
        auto ret = QMetaObject::invokeMethod(listener, methodName, connectionType,
            val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
        if (!ret)
            errors.append(QString("%1:%2").arg(listener->metaObject()->className()).arg(listener->objectName()));
    }
    if (errors.isEmpty())
        return true;
    ps_LastError_ = QString("%1 execution failed:[\n").arg(QString(eventName));
    for (auto& err : errors)
        ps_LastError_ += QString("%1;\n").arg(err);
    ps_LastError_ += "]\n";
    return false;
}


QGenericReturnArgument PSEventController::publishDirect(const QByteArray& eventName,
    QGenericArgument val0, QGenericArgument val1, QGenericArgument val2, QGenericArgument val3,
    QGenericArgument val4, QGenericArgument val5, QGenericArgument val6, QGenericArgument val7,
    QGenericArgument val8, QGenericArgument val9)
{
    QReadLocker locker(&ps_Lock_);
    if (!psEvents_pool_.contains(eventName)) {
        ps_LastError_ = QString("No objects subscribe to this eventName");
        return 0;
    }
    auto methodName = methodFormatting(eventName);

    QGenericReturnArgument result = 0;
    QStringList errors;
    for (auto listener : psEvents_pool_[eventName]) {
        if (!listener)
            continue;
        auto ret = QMetaObject::invokeMethod(listener, methodName, Qt::DirectConnection, result,
            val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
        if (!ret)
            errors.append(QString("%1:%2").arg(listener->metaObject()->className()).arg(listener->objectName()));
    }
    if (errors.isEmpty())
        return result;
    ps_LastError_ = QString("%1 execution failed:[\n").arg(QString(eventName));
    for (auto& err : errors)
        ps_LastError_ += QString("%1;\n").arg(err);
    ps_LastError_ += "]\n";
    return 0;

}


