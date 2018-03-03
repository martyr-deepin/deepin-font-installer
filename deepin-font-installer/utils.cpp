/*
 * Copyright (C) 2017 ~ 2018 Deepin Technology Co., Ltd.
 *
 * Author:     rekols <rekols@foxmail.com>
 * Maintainer: rekols <rekols@foxmail.com>
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

#include "utils.h"

#include <QUrl>
#include <QDir>
#include <QFile>
#include <QDebug>
#include <QFontInfo>
#include <QMimeType>
#include <QApplication>
#include <QMimeDatabase>
#include <QStandardPaths>

Utils::Utils(QObject *parent)
    : QObject(parent)
{
}

Utils::~Utils()
{
}

QString Utils::getQssContent(const QString &filePath)
{
    QFile file(filePath);
    QString qss;

    if (file.open(QIODevice::ReadOnly)) {
        qss = file.readAll();
    }

    return qss;
}

QString Utils::getConfigPath()
{
    QDir dir(QDir(QStandardPaths::standardLocations(QStandardPaths::ConfigLocation).first())
             .filePath(qApp->organizationName()));

    return dir.filePath(qApp->applicationName());
}

bool Utils::isFontMimeType(const QString &filePath)
{
    const QMimeType mimeType = QMimeDatabase().mimeTypeForUrl(QUrl(filePath));
    const QString mimeName = mimeType.name();

    if (mimeName == "application/x-font-ttf" ||
        mimeName == "application/x-font-otf" ||
        mimeName == "application/x-font-type1") {
        return true;
    }

    return false;
}

bool Utils::isFont(const QString &filePath)
{
    QFileInfo file(filePath);

    if (file.exists()) {
        QStringList fontMimes;
        fontMimes << "application/x-font-ttf"
                  << "application/x-font-otf"
                  << "application/x-font-type1";
        return fontMimes.contains(QMimeDatabase().mimeTypeForFile(file).name());
    } else {
        return false;
    }
}

QString Utils::suffixList()
{
    return QString("Font Files (*.ttf *.ttc *.otf)");
}
