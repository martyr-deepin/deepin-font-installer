/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "dfontmanager.h"
#include <QProcess>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static DFontManager *INSTANCE = 0;

DFontManager *DFontManager::instance()
{
    if (!INSTANCE) {
        INSTANCE = new DFontManager;
    }

    return INSTANCE;
}

DFontManager::DFontManager(QObject *parent)
    : QThread(parent)
{

}

DFontManager::~DFontManager()
{
}

void DFontManager::setType(Type type)
{
    m_type = type;
}

void DFontManager::setInstallFileList(const QStringList &list)
{
    if (!m_instFileList.isEmpty()) {
        m_instFileList.clear();
    }

    m_instFileList << list;
}

void DFontManager::setReInstallFile(const QString &reinstFile, const QString &sysFile)
{
    m_reinstFile = reinstFile;
    m_sysFile = sysFile;
}

void DFontManager::setUnInstallFile(const QString &filePath)
{
    m_uninstFile = filePath;
}

void DFontManager::run()
{
    switch (m_type) {
    case Install:
        handleInstall();
        break;
    case ReInstall:
        handleReInstall();
        break;
    case UnInstall:
        handleUnInstall();
        break;
    default:
        break;
    }
}

bool DFontManager::doCmd(const QString &program, const QStringList &arguments)
{
    QProcess *process = new QProcess;
    bool failed = false;

    if (m_type == Install) {
        connect(process, &QProcess::readyReadStandardOutput, this,
                [&] {
                    const QString output = process->readAllStandardOutput();

                    // single file installation.
                    if (m_instFileList.count() == 1) {
                        emit installChanged(output);
                    } else {
                        QJsonDocument document = QJsonDocument::fromJson(output.toUtf8());
                        QJsonObject object = document.object();
                        emit installing(object.value("FilePath").toString(),
                                        object.value("Percent").toDouble());
                    }
                });
    } else {
        connect(process, &QProcess::readyReadStandardOutput, this,
                [&] { emit output(process->readAllStandardOutput()); });
    }

    process->start(program, arguments);
    process->waitForFinished(-1);
    failed |= process->exitCode();

    if (!failed) {
        process->execute("fc-cache");
        process->waitForFinished(-1);
    }

    process->deleteLater();
    return !failed;
}

void DFontManager::handleInstall()
{
    if (doCmd("pkexec", QStringList() << "dfont-install" << m_instFileList)) {
        if (m_instFileList.count() == 1) {
            emit installFinished();
        }
    }
}

void DFontManager::handleUnInstall()
{
    if (doCmd("pkexec", QStringList() << "dfont-uninstall" << m_uninstFile)) {
        emit uninstallFinished();
    }
}

void DFontManager::handleReInstall()
{
    if (doCmd("pkexec", QStringList() << "dfont-reinstall" << m_reinstFile << m_sysFile)) {
        emit reinstallFinished();
    }
}
