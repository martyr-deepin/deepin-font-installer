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

#include "singlefilepage.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QSvgWidget>
#include <QFileInfo>
#include <QUrlQuery>
#include <QProcess>
#include <QTimer>
#include <QUrl>
#include <QDir>

void setElidedText(QLabel *label, const QString &text)
{
    const QFontMetrics fm(label->font());
    const int textWidth = label->width() * 1.85;
    const QString clippedText = fm.elidedText(text, Qt::ElideRight, textWidth);
    label->setText(clippedText);
}

SingleFilePage::SingleFilePage(QWidget *parent)
    : QWidget(parent),
      m_infoManager(DFontInfoManager::instance()),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_typeLabel(new QLabel),
      m_versionLabel(new QLabel),
      m_copyrightLabel(new QLabel),
      m_descriptionLabel(new QLabel),
      m_tipsLabel(new QLabel()),
      m_installBtn(new QPushButton(tr("Install"))),
      m_uninstallBtn(new QPushButton(tr("Remove"))),
      m_reinstallBtn(new QPushButton(tr("Reinstall"))),
      m_viewFileBtn(new QPushButton(tr("View font directory"))),
      m_closeBtn(new QPushButton(tr("Done"))),
      m_progress(new Progress),
      m_propertyAnimation(new QPropertyAnimation(m_progress, "value", this)),
      m_bottomLayout(new QStackedLayout)
{
    QSvgWidget *iconWidget = new QSvgWidget(":/images/font-x-generic.svg");
    iconWidget->setFixedSize(70, 70);

    QHBoxLayout *styleLayout = new QHBoxLayout;
    QLabel *styleLabel = new QLabel(tr("Style: "));
    styleLayout->addWidget(styleLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    styleLayout->addSpacing(3);
    styleLayout->addWidget(m_styleLabel, Qt::AlignLeft);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    QLabel *typeLabel = new QLabel(tr("Type: "));
    typeLayout->addWidget(typeLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    typeLayout->addSpacing(3);
    typeLayout->addWidget(m_typeLabel, Qt::AlignLeft);

    QHBoxLayout *versionLayout = new QHBoxLayout;
    QLabel *versionLabel = new QLabel(tr("Version: "));
    versionLayout->addWidget(versionLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    versionLayout->addSpacing(3);
    versionLayout->addWidget(m_versionLabel, Qt::AlignLeft);

    QHBoxLayout *copyrightLayout = new QHBoxLayout;
    QLabel *copyrightLabel = new QLabel(tr("Copyright: "));
    copyrightLayout->addWidget(copyrightLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    copyrightLayout->addSpacing(3);
    copyrightLayout->addWidget(m_copyrightLabel, Qt::AlignLeft);

    QHBoxLayout *descLayout = new QHBoxLayout;
    QLabel *descLabel = new QLabel(tr("Description: "));
    descLayout->addWidget(descLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    descLayout->addSpacing(3);
    descLayout->addWidget(m_descriptionLabel, Qt::AlignLeft);

    QWidget *btnsWidget = new QWidget;
    QHBoxLayout *btnsLayout = new QHBoxLayout(btnsWidget);
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_installBtn);
    btnsLayout->addWidget(m_uninstallBtn);
    btnsLayout->addWidget(m_reinstallBtn);
    btnsLayout->addWidget(m_viewFileBtn);
    btnsLayout->addWidget(m_closeBtn);
    btnsLayout->addStretch();
    btnsLayout->setSpacing(20);
    btnsLayout->setMargin(0);

    QWidget *progressWidget = new QWidget;
    QVBoxLayout *progressLayout = new QVBoxLayout(progressWidget);
    progressLayout->addWidget(m_progress, 0, Qt::AlignCenter);
    progressWidget->setFixedHeight(36);

    m_bottomLayout->addWidget(btnsWidget);
    m_bottomLayout->addWidget(progressWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(iconWidget, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_nameLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(styleLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(typeLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(versionLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(copyrightLayout);
    mainLayout->addSpacing(6);
    mainLayout->addLayout(descLayout);
    mainLayout->addStretch();
    mainLayout->addSpacing(10);
    mainLayout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(m_bottomLayout);
    mainLayout->addSpacing(20);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(50, 0, 50, 0);

    // init property.
    styleLabel->setStyleSheet("QLabel { color: #444444; }");
    typeLabel->setStyleSheet("QLabel { color: #444444; }");
    versionLabel->setStyleSheet("QLabel { color: #444444; }");
    copyrightLabel->setStyleSheet("QLabel { color: #444444; }");
    descLabel->setStyleSheet("QLabel { color: #444444; }");

    m_nameLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: 500; }");
    m_versionLabel->setWordWrap(true);
    m_copyrightLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);
    m_tipsLabel->setText("");

    m_installBtn->setFixedSize(160, 36);
    m_installBtn->setObjectName("BlueButton");
    m_installBtn->setVisible(false);

    m_uninstallBtn->setFixedSize(160, 36);
    m_uninstallBtn->setObjectName("GrayButton");
    m_uninstallBtn->setVisible(false);

    m_reinstallBtn->setFixedSize(160, 36);
    m_reinstallBtn->setObjectName("GrayButton");
    m_reinstallBtn->setVisible(false);

    m_viewFileBtn->setFixedSize(160, 36);
    m_viewFileBtn->setObjectName("BlueButton");
    m_viewFileBtn->setVisible(false);

    m_closeBtn->setFixedSize(160, 36);
    m_closeBtn->setObjectName("BlueButton");
    m_closeBtn->setVisible(false);

    m_propertyAnimation->setDuration(500);
    m_propertyAnimation->setStartValue(0);
    m_propertyAnimation->setEndValue(100);

    // connect the signals to the slots function.
    connect(m_installBtn, &QPushButton::clicked, this, &SingleFilePage::handleInstall);
    connect(m_uninstallBtn, &QPushButton::clicked, this, &SingleFilePage::handleRemove);
    connect(m_reinstallBtn, &QPushButton::clicked, this, &SingleFilePage::handleReinstall);
    connect(m_viewFileBtn, &QPushButton::clicked, this, &SingleFilePage::viewFilePath);
    connect(m_closeBtn, &QPushButton::clicked, this, &QApplication::quit);
}

SingleFilePage::~SingleFilePage()
{
}

void SingleFilePage::updateInfo(DFontInfo *info)
{
    m_fontInfo = info;
    refreshPage();

    const QString copyright = m_fontInfo->copyright;
    const QString description = m_fontInfo->description;

    m_nameLabel->setText(m_fontInfo->familyName);
    m_styleLabel->setText(m_fontInfo->styleName);
    m_typeLabel->setText(m_fontInfo->type);
    m_versionLabel->setText(m_fontInfo->version);
    setElidedText(m_copyrightLabel, copyright);
    setElidedText(m_descriptionLabel, description);
}

void SingleFilePage::refreshPage()
{
    const bool isInstalled = m_fontInfo->isInstalled;
    const bool isSameVersion = m_fontInfo->sysVersion == m_fontInfo->version;

    if (isInstalled) {
        m_installBtn->setVisible(false);
        m_uninstallBtn->setVisible(true);
        m_reinstallBtn->setVisible(true);
        m_viewFileBtn->setVisible(false);
        m_closeBtn->setVisible(false);

        if (isSameVersion) {
            m_tipsLabel->setStyleSheet("QLabel { color: #ff5a5a; }");
            m_tipsLabel->setText(tr("Same version installed"));
        } else {
            m_tipsLabel->setStyleSheet("QLabel { color: #ff5a5a; }");
            m_tipsLabel->setText(QString(tr("Other version installed: %1")).arg(m_fontInfo->sysVersion));
        }
    } else {
        m_tipsLabel->setText("");
        m_installBtn->setVisible(true);
        m_uninstallBtn->setVisible(false);
        m_reinstallBtn->setVisible(false);
        m_viewFileBtn->setVisible(false);
        m_closeBtn->setVisible(false);
    }

    if (m_fontInfo->copyright.isEmpty()) {
        m_fontInfo->copyright = tr("Unknown");
    }
    if (m_fontInfo->description.isEmpty()) {
        m_fontInfo->description = tr("Unknown");
    }
    if (m_fontInfo->version.isEmpty()) {
        m_fontInfo->version = tr("Unknown");
    }
}

void SingleFilePage::progressBarStart()
{
    m_bottomLayout->setCurrentIndex(1);
    m_tipsLabel->setText("");
    m_progress->setValue(0);
    m_propertyAnimation->start();
}

void SingleFilePage::handleInstall()
{
    bool failed = m_infoManager->fontsInstall(QStringList() << m_fontInfo->filePath);

    if (!failed) {
        progressBarStart();

        QTimer::singleShot(m_propertyAnimation->duration(), this, [=] {
                m_fontInfo->isInstalled = true;
                m_filePath = "/usr/share/fonts/" + QFileInfo(m_fontInfo->filePath).fileName();

                m_tipsLabel->setStyleSheet("QLabel { color: #47790c; }");
                m_tipsLabel->setText(tr("Installed successfully"));
                m_installBtn->setVisible(false);
                m_uninstallBtn->setVisible(false);
                m_reinstallBtn->setVisible(false);
                m_viewFileBtn->setVisible(true);
                m_closeBtn->setVisible(false);
                m_progress->setValue(0);
                m_bottomLayout->setCurrentIndex(0);
        });
    }
}

void SingleFilePage::handleRemove()
{
    bool failed = m_infoManager->fontRemove(m_fontInfo);

    if (!failed) {
        progressBarStart();

        QTimer::singleShot(m_propertyAnimation->duration(), this, [=] {
                m_fontInfo->isInstalled = false;

                m_tipsLabel->setStyleSheet("QLabel { color: #47790c; }");
                m_tipsLabel->setText(tr("Removed successfully"));
                m_installBtn->setVisible(false);
                m_uninstallBtn->setVisible(false);
                m_reinstallBtn->setVisible(false);
                m_viewFileBtn->setVisible(false);
                m_closeBtn->setVisible(true);
                m_progress->setValue(0);
                m_bottomLayout->setCurrentIndex(0);
            });
    }
}

void SingleFilePage::handleReinstall()
{
    const QString reinstStr = m_infoManager->fontReinstall(m_fontInfo);

    if (!reinstStr.isEmpty()) {
        m_filePath = reinstStr;
        progressBarStart();

        QTimer::singleShot(m_propertyAnimation->duration(), this, [=] {
                m_tipsLabel->setStyleSheet("QLabel { color: #47790c; }");
                m_tipsLabel->setText(tr("Installed successfully"));
                m_installBtn->setVisible(false);
                m_uninstallBtn->setVisible(false);
                m_reinstallBtn->setVisible(false);
                m_viewFileBtn->setVisible(true);
                m_closeBtn->setVisible(false);
                m_progress->setValue(0);
                m_bottomLayout->setCurrentIndex(0);
        });
    }
}

void SingleFilePage::viewFilePath()
{
    QUrl url = QUrl::fromLocalFile(QFileInfo(m_filePath).dir().absolutePath());
    QUrlQuery query;
    query.addQueryItem("selectUrl", QUrl::fromLocalFile(m_filePath).toString());
    url.setQuery(query);

    QProcess::startDetached("dde-file-manager", QStringList(url.toString()));
}
