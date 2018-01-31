/*
 * Copyright (C) 2017 ~ 2017 Deepin Technology Co., Ltd.
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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <iostream>
#include "dfontinfomanager.h"

inline QString dataToMd5Hex(const QByteArray &data)
{
    return QString(QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex());
}

inline void checkDirectory()
{
    const QString path("/usr/share/fonts/deepin-font-install");
    if (!QDir(path).exists()) {
        QDir dir(path);
        dir.mkpath(".");
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCommandLineParser parser;
    parser.process(app);
    checkDirectory();

    DFontInfoManager *fontInfoManager = DFontInfoManager::instance();
    const QString sysDir = "/usr/share/fonts/deepin-font-install";
    const QStringList fileList = parser.positionalArguments();

    for (const QString file : fileList) {
        DFontInfo *fontInfo = fontInfoManager->getFontInfo(file);
        QProcess *process = new QProcess;
        QString target = "";
        const bool isInstalled = fontInfo->isInstalled;

        if (isInstalled) {
            const QString sysPath = fontInfoManager->getInstalledFontPath(fontInfo);
            process->start("cp", QStringList() << "-f" << file << sysPath);
            process->waitForFinished(-1);
        } else {
            const QFileInfo info(file);
            target = QString("%1/%2/%3").arg(sysDir, dataToMd5Hex(info.fileName().toUtf8()), info.fileName());
            const QString targetDir = QString("%1/%2").arg(sysDir, dataToMd5Hex(info.fileName().toUtf8()));
            QDir dir(targetDir);
            dir.mkpath(".");
            QFile::copy(file, target);
        }

        process->start("fc-cache");
        process->waitForFinished(-1);
        process->deleteLater();

        const int currentIndex = fileList.indexOf(file);
        const int count = fileList.count() - 1;

        if (fileList.count() == 1) {
            std::cout << target.toUtf8().data() << std::endl;
        } else {
            const QString output = QString("%1:%2").arg(file, QString::number(currentIndex / float(count) * 100));
            std::cout << output.toUtf8().data() << std::endl;
        }
    }

    return 0;
}
