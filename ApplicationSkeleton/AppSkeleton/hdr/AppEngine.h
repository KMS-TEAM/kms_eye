#ifndef APPENGINE_H
#define APPENGINE_H

#include <QQmlApplicationEngine>
#include <QQmlContext>

class AppEngine : public QQmlApplicationEngine
{
    Q_OBJECT
    QQmlContext *m_rootContext;
public:
    AppEngine();
    void initEngine();
    void startApplication();
public slots:
    void slotReceiveEvent(int event);
private:

};

#endif // APPENGINE_H
