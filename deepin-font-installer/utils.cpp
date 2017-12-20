/*
 * Copyright (C) 2011 ~ 2017 Deepin Technology Co., Ltd.
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

#include <QDebug>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <QFontInfo>
#include <QFontDatabase>

#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <glib.h>

#include FT_FREETYPE_H
#include FT_TYPE1_TABLES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

QString Utils::getQssContent(const QString &filePath)
{
    QFile file(filePath);
    QString qss;

    if (file.open(QIODevice::ReadOnly)) {
        qss = file.readAll();
    }

    return qss;
}

bool Utils::isFontSuffix(const QString &suffix)
{
    if (suffix == "ttf" || suffix == "ttc" || suffix == "otf") {
        return true;
    } else {
        return false;
    }
}

QString Utils::suffixList()
{
    return QString("Font Files (*.ttf *.ttc *.otf *.txt)");
}

QString Utils::getFontType(const QString &suffix)
{
    if (suffix == "ttf" || suffix == "ttc") {
        return "TrueType";
    } else if (suffix == "otf") {
        return "OpenType";
    } else {
        return "Unknow";
    }
}

void Utils::fontInstall(const QStringList &files)
{
    QProcess *process = new QProcess;
    QString cmd = "pkexec cp -r ";

    for (auto const file : files) {
        cmd.append(file + " ");
    }

    cmd.append("/usr/share/fonts/");

    process->start(cmd);
    process->waitForFinished();
    process->kill();
    process->close();
}
