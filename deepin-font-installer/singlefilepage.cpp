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
#include <QSvgWidget>
#include <QFileInfo>
#include <QProcess>
#include <QDebug>

SingleFilePage::SingleFilePage(QWidget *parent)
    : QWidget(parent),
      m_infoManager(new DFontInfoManager),
      m_layout(new QVBoxLayout(this)),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_typeLabel(new QLabel),
      m_versionLabel(new QLabel),
      m_copyrightLabel(new QLabel),
      m_descriptionLabel(new QLabel),
      m_statusLabel(new QLabel),
      m_installBtn(new QPushButton(tr("Install"))),
      m_removeBtn(new QPushButton(tr("Remove"))),
      m_reinstallBtn(new QPushButton(tr("Reinstall"))),
      m_viewFileBtn(new QPushButton(tr("View font directory"))),
      m_closeBtn(new QPushButton(tr("OK")))
{
    m_nameLabel->setStyleSheet("QLabel { font-size: 18px; font-weight: 500; }");
    m_copyrightLabel->setWordWrap(true);
    m_descriptionLabel->setWordWrap(true);

    // top icon, set pixmap to icon label.
    QSvgWidget *iconWidget = new QSvgWidget(":/images/font-x-generic.svg");
    iconWidget->setFixedSize(70, 70);

    // info layout
    QHBoxLayout *styleLayout = new QHBoxLayout;
    QLabel *styleLabel = new QLabel(tr("Style: "));
    styleLabel->setStyleSheet("QLabel { color: #444444; }");
    styleLayout->addSpacing(50);
    styleLayout->addWidget(styleLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    styleLayout->addWidget(m_styleLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    styleLayout->addStretch();
    styleLayout->addSpacing(50);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    QLabel *typeLabel = new QLabel(tr("Type: "));
    typeLabel->setStyleSheet("QLabel { color: #444444; }");
    typeLayout->addSpacing(50);
    typeLayout->addWidget(typeLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    typeLayout->addWidget(m_typeLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    typeLayout->addStretch();
    typeLayout->addSpacing(50);

    QHBoxLayout *versionLayout = new QHBoxLayout;
    QLabel *versionLabel = new QLabel(tr("Version: "));
    versionLabel->setStyleSheet("QLabel { color: #444444; }");
    versionLayout->addSpacing(50);
    versionLayout->addWidget(versionLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    versionLayout->addWidget(m_versionLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    versionLayout->addStretch();
    versionLayout->addSpacing(50);

    QHBoxLayout *copyrightLayout = new QHBoxLayout;
    QLabel *copyrightLabel = new QLabel(tr("Copyright: "));
    copyrightLabel->setStyleSheet("QLabel { color: #444444; }");
    copyrightLayout->addSpacing(50);
    copyrightLayout->addWidget(copyrightLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    copyrightLayout->addWidget(m_copyrightLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    copyrightLayout->addStretch();
    copyrightLayout->addSpacing(50);

    QHBoxLayout *descLayout = new QHBoxLayout;
    QLabel *descLabel = new QLabel(tr("Description: "));
    descLabel->setStyleSheet("QLabel { color: #444444; }");
    descLayout->addSpacing(50);
    descLayout->addWidget(descLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    descLayout->addWidget(m_descriptionLabel, 0, Qt::AlignLeft | Qt::AlignTop);
    descLayout->addStretch();
    descLayout->addSpacing(50);

    // bottom
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_installBtn);
    bottomLayout->addWidget(m_removeBtn);
    bottomLayout->addWidget(m_reinstallBtn);
    bottomLayout->addWidget(m_viewFileBtn);
    bottomLayout->addWidget(m_closeBtn);
    bottomLayout->addStretch();
    bottomLayout->setSpacing(15);

    m_installBtn->setFixedSize(160, 36);
    m_installBtn->setObjectName("BlueButton");
    m_installBtn->hide();

    m_removeBtn->setFixedSize(160, 36);
    m_removeBtn->setObjectName("GrayButton");
    m_removeBtn->hide();

    m_reinstallBtn->setFixedSize(160, 36);
    m_reinstallBtn->setObjectName("GrayButton");
    m_reinstallBtn->hide();

    m_viewFileBtn->setFixedSize(160, 36);
    m_viewFileBtn->setObjectName("BlueButton");
    m_viewFileBtn->hide();

    m_closeBtn->setFixedSize(160, 36);
    m_closeBtn->setObjectName("BlueButton");
    m_closeBtn->hide();

    // add widgets to main layout.
    m_layout->addWidget(iconWidget, 0, Qt::AlignTop | Qt::AlignHCenter);
    m_layout->addSpacing(10);
    m_layout->addWidget(m_nameLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(10);
    m_layout->addLayout(styleLayout);
    m_layout->addLayout(typeLayout);
    m_layout->addLayout(versionLayout);
    m_layout->addLayout(copyrightLayout);
    m_layout->addLayout(descLayout);
    m_layout->addStretch();
    m_layout->addWidget(m_statusLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(5);
    m_layout->addLayout(bottomLayout);
    m_layout->addSpacing(20);
    m_layout->setMargin(0);

    connect(m_installBtn, &QPushButton::clicked, this, &SingleFilePage::handleInstall);
    connect(m_removeBtn, &QPushButton::clicked, this, &SingleFilePage::handleRemove);
    connect(m_reinstallBtn, &QPushButton::clicked, this, &SingleFilePage::handleReinstall);
    connect(m_viewFileBtn, &QPushButton::clicked, this, &SingleFilePage::viewFilePath);
    connect(m_closeBtn, &QPushButton::clicked, this, &QApplication::quit);
}

SingleFilePage::~SingleFilePage()
{
    delete m_data;
    delete m_infoManager;
}

void SingleFilePage::updateInfo(DFontInfo *data)
{
    const QFontMetrics fm = m_versionLabel->fontMetrics();
    m_data = data;

    // if the content is empty, it shows "Unknown".
    if (data->copyright.isEmpty()) {
        m_data->copyright = tr("Unknown");
    }

    if (data->description.isEmpty()) {
        m_data->description = tr("Unknown");
    }

    m_nameLabel->setText(m_data->familyName);
    m_styleLabel->setText(m_data->styleName);
    m_typeLabel->setText(data->type);
    m_versionLabel->setText(fm.elidedText((m_data->version.isEmpty() ? tr("Unknown") : data->version),
                                          Qt::ElideRight,
                                          this->width() / 2));

    m_copyrightLabel->setText(fm.elidedText(m_data->copyright.simplified(), Qt::ElideRight,
                                            this->width()));

    //description string in some font files has '\n' & '\t' & '\r'
    m_descriptionLabel->setText(fm.elidedText(m_data->description.simplified(),
                                              Qt::ElideRight,
                                              this->width() * 1.2));

    m_statusLabel->setText("");
    m_closeBtn->hide();
    m_removeBtn->hide();
    m_reinstallBtn->hide();
    m_viewFileBtn->hide();
    m_closeBtn->hide();

    if (data->isInstalled) {
        if (data->sysVersion != data->version) {
            m_statusLabel->setText(QString(tr("Other version installed: %1")).arg(data->sysVersion));
            m_statusLabel->setStyleSheet("QLabel { color: #ff5a5a; }");
        } else {
            m_statusLabel->setText(tr("Same version installed"));
            m_statusLabel->setStyleSheet("QLabel { color: #ff5a5a; }");
        }

        m_installBtn->hide();
        m_removeBtn->show();
        m_reinstallBtn->show();
        m_viewFileBtn->hide();
    } else {
        m_installBtn->show();
        m_removeBtn->hide();
        m_reinstallBtn->hide();
    }
}

void SingleFilePage::showInstalled()
{
    m_viewFileBtn->show();
    m_installBtn->hide();
    m_removeBtn->hide();
    m_reinstallBtn->hide();

    m_statusLabel->setText(tr("Installed successfully"));
    m_statusLabel->setStyleSheet("QLabel { color: #47790c; }");
}

void SingleFilePage::handleInstall()
{
    bool isInstall = m_infoManager->fontsInstall(QStringList() << m_data->filePath);

    if (isInstall) {
        showInstalled();
        m_data->isInstalled = true;
    }
}

void SingleFilePage::handleRemove()
{
    bool isRemove = m_infoManager->fontRemove(m_data);

    if (isRemove) {
        m_statusLabel->setText(tr("Removed successfully"));
        m_statusLabel->setStyleSheet("QLabel { color: #47790c; }");

        m_closeBtn->show();
        m_viewFileBtn->hide();
        m_installBtn->hide();
        m_removeBtn->hide();
        m_reinstallBtn->hide();
        m_data->isInstalled = false;
    }
}

void SingleFilePage::handleReinstall()
{
    bool isReinstall = m_infoManager->fontReinstall(m_data);

    if (isReinstall) {
        showInstalled();
    }
}

void SingleFilePage::viewFilePath()
{
}
