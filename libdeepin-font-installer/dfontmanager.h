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

#ifndef DFONTMANAGER_H
#define DFONTMANAGER_H

#include <QThread>

class DFontManager : public QThread
{
    Q_OBJECT

public:
    enum Type {
        Install,
        ReInstall,
        UnInstall
    };

    static DFontManager *instance();
    DFontManager(QObject *parent = nullptr);
    ~DFontManager();

    void setType(Type type);
    void setInstallFileList(const QStringList &list);
    void setReInstallFile(const QString &reinstFile, const QString &sysFile);
    void setUnInstallFile(const QString &filePath);

signals:
    void output(const QString &output);
    void installing(const QString &filePath, const float &percent);
    void singleInstallFinished();
    void installFinished();
    void installChanged(const QString &instPath);
    void reinstallFinished();
    void uninstallFinished();

protected:
    void run();

private:
    bool doCmd(const QString &program, const QStringList &arguments);
    void handleInstall();
    void handleUnInstall();
    void handleReInstall();

private:
    QStringList m_instFileList;
    QString m_uninstFile;
    QString m_reinstFile;
    QString m_sysFile;
    Type m_type;
};

#endif
