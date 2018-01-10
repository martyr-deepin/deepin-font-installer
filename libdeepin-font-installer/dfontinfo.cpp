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

#include "dfontinfo.h"
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

#include <fontconfig/fontconfig.h>
#include <freetype/ftfntfmt.h>
#include <ft2build.h>
#include <glib.h>

#include FT_FREETYPE_H
#include FT_TYPE1_TABLES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

DFontInfo::DFontInfo(QObject *parent)
    : QObject(parent)
{
}

DFontInfo::~DFontInfo()
{
}

QList<DFontData> DFontInfo::families() const
{
    FT_Face face = 0;
    FT_Library library = 0;
    QList<DFontData> list;
    const QStringList fontList = getAllFontPath();

    FT_Init_FreeType(&library);

    for (auto path : fontList) {
        FT_New_Face(library, path.toLatin1().data(), 0, &face);

        DFontData data;
        data.filePath = path;
        data.familyName = face->family_name;
        data.styleName = face->style_name;
        list << data;

        FT_Done_Face(face);
    }

    FT_Done_FreeType(library);

    return list;
}

QStringList DFontInfo::getAllFontPath() const
{
    QStringList pathList;

    FcPattern *pattern = FcPatternCreate();
    FcObjectSet *objset = FcObjectSetBuild(FC_FILE, NULL);
    FcFontSet *fontset = FcFontList(0, pattern, objset);

    for (int i = 0; i < fontset->nfont; ++i) {
        FcChar8 *str;

        if (FcPatternGetString(fontset->fonts[i], FC_FILE, 0, &str) == FcResultMatch) {
            pathList << (char *)str;
        }
    }

    FcObjectSetDestroy(objset);
    FcPatternDestroy(pattern);
    FcFontSetDestroy(fontset);

    return pathList;
}

QString DFontInfo::getFontType(const QString &filePath)
{
    const QFileInfo fileInfo(filePath);
    const QString suffix = fileInfo.suffix().toLower();

    if (suffix == "ttf" || suffix == "ttc") {
        return "TrueType";
    } else if (suffix == "otf") {
        return "OpenType";
    } else {
        return "Unknown";
    }
}

void DFontInfo::getFontInfo(DFontData *data)
{
    FT_Library m_library = 0;
    FT_Face m_face = 0;

    FT_Init_FreeType(&m_library);
    FT_New_Face(m_library, data->filePath.toLatin1().data(), 0, &m_face);

    // get the basic data.
    data->familyName = QString::fromLatin1(m_face->family_name);
    data->styleName = QString::fromLatin1(m_face->style_name);
    data->type = getFontType(data->filePath);
    data->isInstalled = isFontInstalled(data);

    if (FT_IS_SFNT(m_face)) {
        const int count = FT_Get_Sfnt_Name_Count(m_face);

        for (int i = 0; i < count; ++i) {
            FT_SfntName sname;

            if (FT_Get_Sfnt_Name(m_face, i, &sname) != 0) {
                continue;
            }

            // only handle the unicode names for US langid.
            if (!(sname.platform_id == TT_PLATFORM_MICROSOFT &&
                  sname.encoding_id == TT_MS_ID_UNICODE_CS &&
                  sname.language_id == TT_MS_LANGID_ENGLISH_UNITED_STATES)) {
                continue;
            }

            switch (sname.name_id) {
            case TT_NAME_ID_COPYRIGHT:
                data->copyright = g_convert((char *)sname.string,
                                            sname.string_len,
                                            "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                break;

            case TT_NAME_ID_VERSION_STRING:
                data->version = g_convert((char *)sname.string,
                                          sname.string_len,
                                          "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                break;

            case TT_NAME_ID_DESCRIPTION:
                data->description = g_convert((char *)sname.string,
                                              sname.string_len,
                                              "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                break;
            default:
                break;
            }
        }
    }

    // destroy object.
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}

bool DFontInfo::isFontInstalled(DFontData *data)
{
    const QList<DFontData> list = families();
    QListIterator<DFontData> i(list);

    while (i.hasNext()) {
        auto item = i.next();
        if (item.familyName == data->familyName &&
            item.styleName == data->styleName) {
            return true;
        }
    }

    return false;
}

bool DFontInfo::fontInstall(const QString &filePath)
{
    QProcess *process = new QProcess;
    QString cmd = "pkexec cp -r " + filePath + " /usr/share/fonts";
    bool isInstall;

    process->start(cmd);
    process->waitForFinished();

    if (process->readAllStandardError().isEmpty()) {
        isInstall = true;
    } else {
        isInstall = false;
    }

    process->kill();
    process->close();

    return isInstall;
}

bool DFontInfo::fontsInstall(const QStringList &files)
{
    QString m_files;

    for (auto const file : files) {
        m_files.append(file + " ");
    }

    return fontInstall(m_files);
}

bool DFontInfo::fontRemove(DFontData *data)
{
    const QList<DFontData> famList = families();
    QProcess *process = new QProcess;
    QString filePath = nullptr;
    bool isRemove;

    for (const auto &famItem : famList) {
        if (data->familyName == famItem.familyName &&
            data->styleName == famItem.styleName) {
            filePath = famItem.filePath;
            break;
        }
    }

    process->start("pkexec rm -rf " + filePath);
    process->waitForFinished();

    if (process->readAllStandardError().isEmpty()) {
        isRemove = true;
    } else {
        isRemove = false;
    }

    process->kill();
    process->close();

    return isRemove;
}
