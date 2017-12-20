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

DFontInfo::DFontInfo(QObject *parent)
    : QObject(parent)
{
    m_library = 0;
    m_face = 0;
}

DFontInfo::~DFontInfo()
{
}

QString DFontInfo::getFontType(const QString &suffix)
{
    if (suffix == "ttf" || suffix == "ttc") {
        return "TrueType";
    } else if (suffix == "otf") {
        return "OpenType";
    } else {
        return "Unknow";
    }
}

void DFontInfo::getFontInfo(DFontData *data)
{
    FT_Init_FreeType(&m_library);
    FT_New_Face(m_library, data->filePath.toLatin1().data(), 0, &m_face);

    // get the basic data.
    data->familyName = QString::fromLatin1(m_face->family_name);
    data->styleName = QString::fromLatin1(m_face->style_name);

    const QFileInfo fileInfo(data->filePath);
    data->type = getFontType(fileInfo.suffix());

    for (int i = 0; i < FT_Get_Sfnt_Name_Count(m_face); ++i) {
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
            }
        }

    // destroy object.
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);
}

QStringList DFontInfo::families()
{
    QStringList list;

    FcPattern *pattern = FcNameParse((FcChar8 *) ":");
    FcObjectSet *objectset = FcObjectSetBuild(FC_FILE, NULL);
    FcFontSet *fontset = FcFontList(NULL, pattern, objectset);
    FcInit();

    FT_Library library = 0;
    FT_Face face = 0;
    FT_Init_FreeType(&library);

    for (int i = 0; i < fontset->nfont; ++i) {
        FcChar8 *filePath;
        if (FcPatternGetString(fontset->fonts[i], FC_FILE, 0, &filePath) == FcResultMatch) {
            FT_New_Face(library, (char *)filePath, 0, &face);
            list << face->family_name;
        }

        // FcStrFree(filePath);
    }

    // destroy object.
    if (face)
        FT_Done_Face(face);
    if (library)
        FT_Done_FreeType(library);
    if (objectset)
        FcObjectSetDestroy(objectset);
    if (pattern)
        FcPatternDestroy(pattern);
    if (fontset)
        FcFontSetDestroy(fontset);

    return list;
}

bool DFontInfo::isFontInstalled(DFontData *data)
{
    const QStringList famList = families();

    for (const QString &name : famList) {
        if (data->familyName == name)
            return true;
    }

    return false;
}


void DFontInfo::fontInstall(const QStringList &files)
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
