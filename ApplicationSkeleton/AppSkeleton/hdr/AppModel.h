#ifndef APPMODEL_H
#define APPMODEL_H

#include <QObject>
#include <QStringList>

#define MODEL AppModel::instance()

class AppModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString currentPath READ currentPath WRITE setCurrentPath NOTIFY currentPathChanged)
public:
    static AppModel *instance();
    QString currentPath() const;

    void setListImages();
    QStringList getListImages() const;

private:
    AppModel();
    AppModel(const AppModel& _other) = delete;
    void operator =(const AppModel& _other) = delete;

public slots:
    void setCurrentPath(QString currentPath);

signals:
    void currentPathChanged();

private:
    static AppModel* self;
    QStringList m_listImages;

    QString m_currentPath;
};

#endif // APPMODEL_H
