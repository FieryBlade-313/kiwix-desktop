#include "settingsmanager.h"
#include "kiwix/tools/pathTools.h"
#include "kiwixapp.h"
#include <QDir>
#include <QFileDialog>

SettingsManager::SettingsManager(QObject *parent)
    : QObject(parent),
    m_settings("Kiwix", "Kiwix-desktop"),
    m_settingsViewDisplayed(false)
{
    initSettings();
}

SettingsManagerView* SettingsManager::getView()
{
    auto view = new SettingsManagerView();
    view->registerObject("settingsManager", this);
    view->setHtml();
    connect(view, &QObject::destroyed, this, [=]() { m_settingsViewDisplayed = false; });
    m_settingsViewDisplayed = true;
    return view;
}

void SettingsManager::setSettings(const QString &key, const QVariant &value)
{
    m_settings.setValue(key, value);
}

void SettingsManager::deleteSettings(const QString &key)
{
    m_settings.remove(key);
}

bool SettingsManager::settingsExists(const QString &key)
{
    return m_settings.contains(key);
}

QVariant SettingsManager::getSettings(const QString &key)
{
    return m_settings.value(key);
}

qreal SettingsManager::getZoomFactorByZimId(const QString &id)
{
    auto zoomFactor = m_zoomFactor;
    QString key = id + "/zoomFactor";
    if (settingsExists(key)) {
        zoomFactor = getSettings(key).toDouble();
    }
    return zoomFactor;
}

void SettingsManager::setKiwixServerPort(int port)
{
    m_kiwixServerPort = port;
    m_settings.setValue("localKiwixServer/port", port);
    emit(portChanged(port));
}

void SettingsManager::setZoomFactor(qreal zoomFactor)
{
    m_zoomFactor = zoomFactor;
    m_settings.setValue("view/zoomFactor", zoomFactor);
}

bool SettingsManager::setDownloadPath(QString downloadPath)
{
    QDir pathDir(downloadPath);
    if (!pathDir.exists()) {
        return false;
    }
    m_downloadPath = downloadPath;
    m_settings.setValue("download/path", downloadPath);
    emit(downloadPathChanged(m_downloadPath));
    return true;
}

void SettingsManager::resetDownloadPath()
{
    setDownloadPath(QString::fromStdString(getDataDirectory()));
}

void SettingsManager::browseDownloadPath()
{
    QString dir = QFileDialog::getExistingDirectory(KiwixApp::instance()->getMainWindow(),
                                                    tr("Browse Directory"),
                                                    QString(),
                                                    QFileDialog::ShowDirsOnly);
    setDownloadPath(dir);
}

void SettingsManager::initSettings()
{
    m_kiwixServerPort = m_settings.value("localKiwixServer/port", 8181).toInt();
    m_zoomFactor = m_settings.value("view/zoomFactor", 1).toDouble();
    m_downloadPath = m_settings.value("download/path", QString::fromStdString(getDataDirectory())).toString();
}