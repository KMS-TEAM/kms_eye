#include "QMLHandler.h"
#include "AppConstant.h"
#include <QDebug>

QMLHandler *QMLHandler::self = nullptr;

QMLHandler *QMLHandler::instance()
{
    if (nullptr == self)
    {
        self = new QMLHandler();
    }
    return self;
}

void QMLHandler::qmlSendEvent(int event)
{
    CONSOLE << "Event " << event << " sent";
    emit notifyQMLEvent(event);
}

void QMLHandler::qmlMessage(QString msg)
{
    CONSOLE << msg;
}

QMLHandler::QMLHandler()
{
    CONSOLE << "Init instance";
}
