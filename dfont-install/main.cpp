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

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCryptographicHash>
#include <QJsonObject>
#include <QJsonDocument>
#include <QThread>
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <iostream>
#include "dfontinfomanager.h"
#include <fontconfig/fontconfig.h>

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

inline bool FcAddAppFont(const char * filepath)
{
    FcInit();
    bool result = FcConfigAppFontAddFile(NULL, (FcChar8 *) filepath);
    return result;
}

inline bool FcAddAppFontDir(const char * dir)
{
    FcInit();
    bool result = FcConfigAppFontAddDir(NULL, (FcChar8 *) dir);
    return result;
}

inline bool FcLoadConfig(const char * filepath)
{
    FcInit();
    bool result = FcConfigParseAndLoad(FcConfigGetCurrent(), (FcChar8 *) filepath, FcFalse);
    return result;
}

inline bool FcCacheUpdate(void)
{
    FcInit();
    FcConfigDestroy(FcConfigGetCurrent());
    return !FcConfigUptoDate(NULL) && FcInitReinitialize();
}

inline bool FcEnableUserConfig(bool enable)
{
    FcInit();
    bool result = FcConfigEnableHome(enable);
    return result;
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
    QString target = "";
    QString targetDir = "";

    for (const QString file : fileList) {
        QProcess process;
        DFontInfo *fontInfo = fontInfoManager->getFontInfo(file);
        const bool isInstalled = fontInfo->isInstalled;

        if (isInstalled) {
            const QString sysPath = fontInfoManager->getInstalledFontPath(fontInfo);
            target = sysPath;
            process.start("cp", QStringList() << "-f" << file << sysPath);
            process.waitForFinished(-1);
        } else {
            const QFileInfo info(file);
            QString dirName = fontInfo->familyName;

            if (dirName.at(0) == '.') {
                dirName = dirName.remove(0, 1);
            }

            target = QString("%1/%2/%3").arg(sysDir, dirName, info.fileName());
            targetDir = QString("%1/%2").arg(sysDir, dirName);

            // const QDir fontDir(sysDir);
            // if (fontDir.entryList(QDir::Files).count() == 0) {
            //     targetDir = sysDir;
            //     target = QString("%1/%2").arg(sysDir, info.fileName());
            // }

            QDir dir(targetDir);
            dir.mkpath(".");
            QFile::copy(file, target);
        }

        // FcCacheUpdate();

        const int currentIndex = fileList.indexOf(file);
        const int count = fileList.count() - 1;

        if (fileList.count() == 1) {
            FcCacheUpdate();
            std::cout << target.toUtf8().data() << std::endl;
        } else {
            QJsonObject object;
            object.insert("FilePath", file);
            object.insert("Percent", currentIndex / double(count) * 100);

            QJsonDocument document;
            document.setObject(object);
            QByteArray array = document.toJson(QJsonDocument::Compact);

            std::cout << array.data() << std::endl;

            // output too fast will crash.
            QThread::msleep(10);
        }
    }

    return 0;
}
