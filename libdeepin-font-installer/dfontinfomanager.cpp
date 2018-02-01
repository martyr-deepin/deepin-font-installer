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

#include "dfontinfomanager.h"
#include <QFileInfo>
#include <QProcess>
#include <QDebug>
#include <QDir>

#include <fontconfig/fontconfig.h>
#include <ft2build.h>
#include <glib.h>

#include FT_FREETYPE_H
#include FT_TYPE1_TABLES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H

static QList<DFontInfo> dataList;
static DFontInfoManager *m_instance = 0;

QString dirSyntax(const QString &d)
{
    if(!d.isEmpty()) {
        QString ds(d);
        ds.replace("//", "/");

        int slashPos = ds.lastIndexOf('/');
        if (slashPos != (((int)ds.length()) - 1))
            ds.append('/');

        return ds;
    }

    return d;
}

DFontInfoManager *DFontInfoManager::instance()
{
    if (!m_instance) {
        m_instance = new DFontInfoManager;
    }

    return m_instance;
}

DFontInfoManager::DFontInfoManager(QObject *parent)
    : QObject(parent)
{
    refreshList();
}

DFontInfoManager::~DFontInfoManager()
{
}

void DFontInfoManager::refreshList()
{
    FT_Face face = 0;
    FT_Library library = 0;

    FT_Init_FreeType(&library);

    if (!dataList.isEmpty()) {
        dataList.clear();
    }

    for (auto path : getAllFontPath()) {
        FT_New_Face(library, path.toUtf8().constData(), 0, &face);

        DFontInfo data;
        data.filePath = path;
        data.familyName = face->family_name;
        data.styleName = face->style_name;

        if (FT_IS_SFNT(face)) {
            const int count = FT_Get_Sfnt_Name_Count(face);

            for (int i = 0; i < count; ++i) {
                FT_SfntName sname;

                if (FT_Get_Sfnt_Name(face, i, &sname) != 0) {
                    continue;
                }

                // only handle the unicode names for US langid.
                if (!(sname.platform_id == TT_PLATFORM_MICROSOFT &&
                      sname.encoding_id == TT_MS_ID_UNICODE_CS &&
                      sname.language_id == TT_MS_LANGID_ENGLISH_UNITED_STATES)) {
                    continue;
                }

                switch (sname.name_id) {
                case TT_NAME_ID_VERSION_STRING:
                    data.version = g_convert((char *)sname.string,
                                             sname.string_len,
                                              "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                    data.version = data.version.remove("Version").simplified();
                    break;
                }
            }
        }

        dataList << data;
        FT_Done_Face(face);
    }

    FT_Done_FreeType(library);
}

QStringList DFontInfoManager::getAllFontPath() const
{
    QStringList pathList;
    FcStrList *strList = FcConfigGetFontDirs(FcInitLoadConfigAndFonts());
    FcChar8 *fcDir;

    while ((fcDir = FcStrListNext(strList)) != NULL) {
        const QString pathStr = dirSyntax((const char *) fcDir);
        const QDir dir(pathStr);
        const QFileInfoList infoList = dir.entryInfoList(QDir::Files);

        for (const QFileInfo &info : infoList) {
            const QString filePath = info.absoluteFilePath();
            const QString suffix = info.suffix().toLower();

            if (suffix == "ttf" || suffix == "ttc" || suffix == "otf") {
                pathList.append(filePath);
            }
        }
    }

    FcStrListDone(strList);

    return pathList;
}

QString DFontInfoManager::getInstalledFontPath(DFontInfo *info)
{
    const QList<DFontInfo> famList = dataList;
    QString filePath = nullptr;

    for (const auto &famItem : famList) {
        if (info->familyName == famItem.familyName &&
            info->styleName == famItem.styleName) {
            filePath = famItem.filePath;
            break;
        }
    }

    return filePath;
}

QString DFontInfoManager::getFontType(const QString &filePath)
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

DFontInfo *DFontInfoManager::getFontInfo(const QString &filePath)
{
    DFontInfo *fontInfo = new DFontInfo;
    FT_Library m_library = 0;
    FT_Face m_face = 0;

    FT_Init_FreeType(&m_library);
    FT_New_Face(m_library, filePath.toUtf8().constData(), 0, &m_face);

    // get the basic data.
    fontInfo->filePath = filePath;
    fontInfo->familyName = QString::fromLatin1(m_face->family_name);
    fontInfo->styleName = QString::fromLatin1(m_face->style_name);
    fontInfo->type = getFontType(filePath);

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
                fontInfo->copyright = g_convert((char *)sname.string,
                                                sname.string_len,
                                                "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                fontInfo->copyright = fontInfo->copyright.simplified();
                break;

            case TT_NAME_ID_VERSION_STRING:
                fontInfo->version = g_convert((char *)sname.string,
                                              sname.string_len,
                                              "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                fontInfo->version = fontInfo->version.remove("Version").simplified();
                break;

            case TT_NAME_ID_DESCRIPTION:
                fontInfo->description = g_convert((char *)sname.string,
                                                  sname.string_len,
                                                  "UTF-8", "UTF-16BE", NULL, NULL, NULL);
                fontInfo->description = fontInfo->description.simplified();
                break;
            default:
                break;
            }
        }
    }

    fontInfo->isInstalled = isFontInstalled(fontInfo);

    // destroy object.
    FT_Done_Face(m_face);
    FT_Done_FreeType(m_library);

    return fontInfo;
}

bool DFontInfoManager::isFontInstalled(DFontInfo *data)
{
    const QList<DFontInfo> list = dataList;

    for (int i = 0; i < list.count(); ++i) {
        const DFontInfo item = list.at(i);

        if (item.familyName == data->familyName &&
            item.styleName == data->styleName) {
            data->sysVersion = item.version;
            return true;
        }
    }

    return false;
}
