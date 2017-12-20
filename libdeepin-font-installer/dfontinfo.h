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

#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <glib.h>

#include FT_FREETYPE_H
#include FT_TYPE1_TABLES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

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

    QString getFontType(const QString &suffix);
    void getFontInfo(DFontData *data);
    static QStringList families();
    static bool isFontInstalled(DFontData *data);
    static void fontInstall(const QStringList &files);

private:
    FT_Library m_library;
    FT_Face m_face;
};

#endif
