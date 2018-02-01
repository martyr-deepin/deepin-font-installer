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
#include <QFormLayout>
#include <QSvgWidget>
#include <QFileInfo>
#include <QUrlQuery>
#include <QProcess>
#include <QDebug>
#include <QTimer>
#include <QUrl>
#include <QDir>

void setElidedText(QLabel *label, const QString &text, const int &textWidth)
{
    const QFontMetrics fm(label->font());
    const QString clippedText = fm.elidedText(text, Qt::ElideRight, textWidth);
    label->setText(clippedText);
}

SingleFilePage::SingleFilePage(QWidget *parent)
    : QWidget(parent),
      m_fontInfoManager(DFontInfoManager::instance()),
      m_fontManager(DFontManager::instance()),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_typeLabel(new QLabel),
      m_versionLabel(new QLabel),
      m_copyrightLabel(new QLabel),
      m_descriptionLabel(new QLabel),
      m_tipsLabel(new QLabel()),
      m_installBtn(new DSuggestButton),
      m_uninstallBtn(new QPushButton(tr("Remove"))),
      m_reinstallBtn(new QPushButton(tr("Reinstall"))),
      m_viewFileBtn(new DSuggestButton),
      m_closeBtn(new QPushButton(tr("Done"))),
      m_progress(new Progress),
      m_propertyAnimation(new QPropertyAnimation(m_progress, "value", this)),
      m_bottomLayout(new QStackedLayout)
{
    QSvgWidget *iconWidget = new QSvgWidget(":/images/font-x-generic.svg");
    iconWidget->setFixedSize(70, 70);

    QLabel *styleLabel = new QLabel(tr("Style: "));
    QLabel *typeLabel = new QLabel(tr("Type: "));
    QLabel *versionLabel = new QLabel(tr("Version: "));
    QLabel *copyrightLabel = new QLabel(tr("Copyright: "));
    QLabel *descLabel = new QLabel(tr("Description: "));

    copyrightLabel->setAlignment(Qt::AlignTop);
    m_copyrightLabel->setAlignment(Qt::AlignTop);

    descLabel->setAlignment(Qt::AlignTop);
    m_descriptionLabel->setAlignment(Qt::AlignTop);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow(styleLabel, m_styleLabel);
    formLayout->addRow(typeLabel, m_typeLabel);
    formLayout->addRow(versionLabel, m_versionLabel);
    formLayout->addRow(copyrightLabel, m_copyrightLabel);
    formLayout->addRow(descLabel, m_descriptionLabel);
    formLayout->setLabelAlignment(Qt::AlignLeft);
    formLayout->setHorizontalSpacing(5);
    formLayout->setVerticalSpacing(7);

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
    mainLayout->addSpacing(15);
    mainLayout->addLayout(formLayout);
    mainLayout->addStretch();
    mainLayout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(m_bottomLayout);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    mainLayout->setContentsMargins(50, 0, 50, 20);

    // init property.
    styleLabel->setStyleSheet("QLabel { color: #444444; font-size: 15px; }");
    typeLabel->setStyleSheet("QLabel { color: #444444; font-size: 15px; }");
    versionLabel->setStyleSheet("QLabel { color: #444444; font-size: 15px; }");
    copyrightLabel->setStyleSheet("QLabel { color: #444444; font-size: 15px; }");
    descLabel->setStyleSheet("QLabel { color: #444444; font-size: 15px; }");

    m_nameLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: 510; }");
    m_styleLabel->setStyleSheet("QLabel { font-size: 15px; }");
    m_typeLabel->setStyleSheet("QLabel { font-size: 15px; }");
    m_versionLabel->setStyleSheet("QLabel { font-size: 15px; }");
    m_copyrightLabel->setStyleSheet("QLabel { font-size: 15px; }");
    m_descriptionLabel->setStyleSheet("QLabel { font-size: 15px; }");

    m_copyrightLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);
    m_tipsLabel->setText("");

    m_installBtn->setText(tr("Install"));
    m_installBtn->setFixedSize(160, 36);
    m_installBtn->setFocusPolicy(Qt::NoFocus);
    m_installBtn->setVisible(false);

    m_uninstallBtn->setFixedSize(160, 36);
    m_uninstallBtn->setFocusPolicy(Qt::NoFocus);
    m_uninstallBtn->setObjectName("GrayButton");
    m_uninstallBtn->setVisible(false);

    m_reinstallBtn->setFixedSize(160, 36);
    m_reinstallBtn->setFocusPolicy(Qt::NoFocus);
    m_reinstallBtn->setObjectName("GrayButton");
    m_reinstallBtn->setVisible(false);

    m_viewFileBtn->setText(tr("View font directory"));
    m_viewFileBtn->setFixedSize(160, 36);
    m_viewFileBtn->setFocusPolicy(Qt::NoFocus);
    m_viewFileBtn->setVisible(false);

    m_closeBtn->setFixedSize(160, 36);
    m_closeBtn->setFocusPolicy(Qt::NoFocus);
    m_closeBtn->setObjectName("BlueButton");
    m_closeBtn->setVisible(false);

    m_propertyAnimation->setDuration(500);
    m_propertyAnimation->setStartValue(0);
    m_propertyAnimation->setEndValue(100);

    // connect the signals to the slots function.
    connect(m_fontManager, &DFontManager::installFinished, this, &SingleFilePage::onInstallFinished);
    connect(m_fontManager, &DFontManager::uninstallFinished, this, &SingleFilePage::onUninstallFinished);
    connect(m_fontManager, &DFontManager::reinstallFinished, this, &SingleFilePage::onReinstallFinished);
    connect(m_fontManager, &DFontManager::installChanged, this, [=] (const QString &path) { m_filePath = path.trimmed(); });
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

    const QFontMetrics fm = m_versionLabel->fontMetrics();
    const int cpLineWidth = rect().width() - 100 - fm.width(tr("Copyright: "));
    const int descLineWidth = rect().width() - 100 - fm.width(tr("Description: "));

    m_nameLabel->setText(m_fontInfo->familyName);
    m_styleLabel->setText(m_fontInfo->styleName);
    m_typeLabel->setText(m_fontInfo->type);

    setElidedText(m_versionLabel, m_fontInfo->version, cpLineWidth / 1.1);
    setElidedText(m_copyrightLabel, m_fontInfo->copyright, cpLineWidth * 1.8);
    setElidedText(m_descriptionLabel, m_fontInfo->description, descLineWidth * 1.8);
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
    m_fontManager->setType(DFontManager::Install);
    m_fontManager->setInstallFileList(QStringList() << m_fontInfo->filePath);
    m_fontManager->start();
}

void SingleFilePage::handleRemove()
{
    m_fontInfoManager->refreshList();
    const QString target = m_fontInfoManager->getInstalledFontPath(m_fontInfo);

    m_fontManager->setType(DFontManager::UnInstall);
    m_fontManager->setUnInstallFile(target);
    m_fontManager->start();
}

void SingleFilePage::handleReinstall()
{
    const QString sysPath = m_fontInfoManager->getInstalledFontPath(m_fontInfo);
    m_filePath = sysPath;

    m_fontManager->setType(DFontManager::ReInstall);
    m_fontManager->setReInstallFile(m_fontInfo->filePath, sysPath);
    m_fontManager->start();
}

void SingleFilePage::onInstallFinished()
{
    progressBarStart();

    QTimer::singleShot(m_propertyAnimation->duration(), this, [=] {
        m_fontInfo->isInstalled = true;

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

void SingleFilePage::onUninstallFinished()
{
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

void SingleFilePage::onReinstallFinished()
{
    progressBarStart();

    QTimer::singleShot(m_propertyAnimation->duration(), this, [=] {
        m_fontInfo->isInstalled = true;
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

void SingleFilePage::viewFilePath()
{
    QUrl url = QUrl::fromLocalFile(QFileInfo(m_filePath).dir().absolutePath());
    QUrlQuery query;
    query.addQueryItem("selectUrl", QUrl::fromLocalFile(m_filePath).toString());
    url.setQuery(query);

    QProcess::startDetached("dde-file-manager", QStringList(url.toString()));}
