#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QMutex>

#define BACKEND Backend::instance()

class Backend : public QObject
{
    Q_OBJECT
public:
    static Backend *instance();
    explicit Backend(QObject *parent = nullptr);
    Q_INVOKABLE QObject* getPixmapContainer() const;

signals:

private:
    static Backend* m_instance;
    static QMutex m_lock;
};

#endif // BACKEND_H
