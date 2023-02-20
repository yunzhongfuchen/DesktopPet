#ifndef PSEVENTCONTROLLER_H
#define PSEVENTCONTROLLER_H

#include <QObject>
#include <QReadWriteLock>
#include <QMap>
#include <QList>


#define METHOD_PREFIX "on_psEvent_"

class PSEventController : public QObject
{
    Q_OBJECT
public:
    static void unSubscribe(QObject* listener, const QByteArray& eventName);

    static bool subscribe(QObject* listener, const QByteArray& eventName);

    static bool publish(const QByteArray& eventName, Qt::ConnectionType connectionType,
        QGenericArgument val0 = QGenericArgument(), QGenericArgument val1 = QGenericArgument(),
        QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(),
        QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(),
        QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(),
        QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument());

    static inline bool publish(const QByteArray& eventName,
        QGenericArgument val0 = QGenericArgument(), QGenericArgument val1 = QGenericArgument(),
        QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(),
        QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(),
        QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(),
        QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument())
    {
        return publish(eventName, Qt::AutoConnection, val0, val1, val2, val3, val4, val5, val6, val7, val8, val9);
    }

    static QGenericReturnArgument publishDirect(const QByteArray& eventName,
        QGenericArgument val0 = QGenericArgument(), QGenericArgument val1 = QGenericArgument(),
        QGenericArgument val2 = QGenericArgument(), QGenericArgument val3 = QGenericArgument(),
        QGenericArgument val4 = QGenericArgument(), QGenericArgument val5 = QGenericArgument(),
        QGenericArgument val6 = QGenericArgument(), QGenericArgument val7 = QGenericArgument(),
        QGenericArgument val8 = QGenericArgument(), QGenericArgument val9 = QGenericArgument());


    static inline QString get_Errors()
    {
        return ps_LastError_;
    }

    static inline void clearEvents()
    {
        QWriteLocker locker(&ps_Lock_);
        psEvents_pool_.clear();
    }
    
    static inline QByteArray methodFormatting(const QByteArray& eventName) {
        return METHOD_PREFIX + eventName;
    }
private:
    static QMap<QByteArray, QList<QObject*>> psEvents_pool_;

    static QReadWriteLock ps_Lock_;

    static QString ps_LastError_;
};

#endif // PSEVENTCONTROLLER_H
