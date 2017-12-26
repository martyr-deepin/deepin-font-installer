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

#ifndef DFONTINFO_H
#define DFONTINFO_H

#include <QObject>

class DFontData
{
public:
    QString filePath;
    QString familyName;
    QString styleName;
    QString type;
    QString version;
    QString copyright;
    QString description;
};

class DFontInfo : public QObject
{
    Q_OBJECT

public:
    DFontInfo(QObject *parent = nullptr);
    ~DFontInfo();

    static QList<DFontData> families();
    static QString getFontType(const QString &filePath);
    static void getFontInfo(DFontData *data);
    static bool isFontInstalled(DFontData *data);
    int fontInstall(const QString &filePath);
    int fontsInstall(const QStringList &files);
    int fontRemove(DFontData *data);
};

#endif
