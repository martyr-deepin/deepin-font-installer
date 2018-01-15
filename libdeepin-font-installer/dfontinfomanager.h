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

class DFontInfo
{
public:
    QString filePath;
    QString familyName;
    QString styleName;
    QString type;
    QString version;
    QString copyright;
    QString description;
    bool isInstalled;
};

class DFontInfoManager : public QObject
{
    Q_OBJECT

public:
    DFontInfoManager(QObject *parent = nullptr);
    ~DFontInfoManager();

    void initFamilies();
    QList<DFontInfo> families(bool isRefresh = false);
    QStringList getAllFontPath() const;
    QString getFontType(const QString &filePath);
    void getFontInfo(DFontInfo *data);
    bool isFontInstalled(DFontInfo *data);
    bool fontsInstall(const QStringList &files);
    bool fontRemove(DFontInfo *data);
};

#endif
