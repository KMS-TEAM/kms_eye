#ifndef CONSTANTS_DEF_H
#define CONSTANTS_DEF_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QColor>
#include <QMutex>
#include <QCoreApplication>

#ifndef MACRO_DEFINE
#define MACRO_DEFINE

#define DEF_VAR(type, name, value) Q_PROPERTY(type name READ name NOTIFY dataUpdate) \
    public: type name() { return value; }

#define DEF_CONST(type, name, value) Q_PROPERTY(type name READ name CONSTANT) \
    public: type name() const { return value; }

#endif

#define DEFS Constants_Def::instance()
#define DELETE_DEFS Constants_Def::DestroyInstance()

#ifndef BUILD_DIR
#define BUILD_DIR QCoreApplication::applicationDirPath()
#endif

class Constants_Def : public QObject
{
    Q_OBJECT

    static Constants_Def* m_instance;
    static QMutex m_lock;

signals:
    void dataUpdated();

public:
    static Constants_Def* instance()
    {
        m_lock.lock();
        if(nullptr == m_instance){
            m_instance = new Constants_Def();
        }
        m_lock.unlock();
        return m_instance;
    }

    static void DestroyInstance()
    {
        m_lock.lock();
        if (nullptr != m_instance){
            delete m_instance;
        }
        m_instance = nullptr;
        m_lock.unlock();
    }

private:
    explicit Constants_Def(QObject* parent = nullptr) : QObject { parent } {}
    ~Constants_Def() {}
    Constants_Def(const Constants_Def&) = delete;
    bool operator= (const Constants_Def&) = delete;
};


#endif // CONSTANTS_DEF_H
