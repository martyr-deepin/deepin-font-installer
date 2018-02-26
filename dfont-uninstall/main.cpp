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
#include <QFile>
#include <QFileInfo>
#include <QProcess>
#include <QDir>
#include <QDebug>
#include <QThread>
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCommandLineParser parser;
    parser.process(app);

    const QStringList fileList = parser.positionalArguments();
    std::cout << 0 << std::endl;

    for (const QString file : fileList) {
        QFile openFile(file);

        if (openFile.remove()) {
            // remove empty directroy.
            const QString dirPath = QFileInfo(file).absolutePath();
            const QDir dir(dirPath);
            if (dir.entryList(QDir::Files).count() == 0) {
                dir.rmdir(dirPath);
            }

            QProcess process;
            process.start("fc-cache");
            process.waitForFinished();
        }
    }

    QThread::msleep(300);
    std::cout << 1 << std::endl;

    return 0;
}
