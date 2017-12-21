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
#include "dsvgrenderer.h"
#include <QFileInfo>

DWIDGET_USE_NAMESPACE

SingleFilePage::SingleFilePage(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_iconLabel(new QLabel),
      m_nameLabel(new QLabel),
      m_styleLabel(new QLabel),
      m_typeLabel(new QLabel),
      m_versionLabel(new QLabel),
      m_copyrightLabel(new QLabel),
      m_descriptionLabel(new QLabel),
      m_statusLabel(new QLabel),
      m_installBtn(new QPushButton(tr("Install"))),
      m_removeBtn(new QPushButton(tr("Remove"))),
      m_reinstallBtn(new QPushButton(tr("Reinstall")))
{
    m_descriptionLabel->setWordWrap(true);
    m_statusLabel->hide();

    // top icon, set pixmap to icon label.
    QPixmap iconPixmap = DSvgRenderer::render(":/images/icon.svg",QSize(65, 65) * devicePixelRatioF());
    m_iconLabel->setFixedSize(65, 65);
    m_iconLabel->setPixmap(iconPixmap);

    // info layout
    QHBoxLayout *styleLayout = new QHBoxLayout;
    QLabel *styleLabel = new QLabel(tr("Style: "));
    styleLabel->setStyleSheet("QLabel { color: #444444; }");
    styleLayout->addSpacing(20);
    styleLayout->addWidget(styleLabel);
    styleLayout->addWidget(m_styleLabel);
    styleLayout->addStretch();
    styleLayout->addSpacing(20);

    QHBoxLayout *typeLayout = new QHBoxLayout;
    QLabel *typeLabel = new QLabel(tr("Type: "));
    typeLabel->setStyleSheet("QLabel { color: #444444; }");
    typeLayout->addSpacing(20);
    typeLayout->addWidget(typeLabel);
    typeLayout->addWidget(m_typeLabel);
    typeLayout->addStretch();
    typeLayout->addSpacing(20);

    QHBoxLayout *versionLayout = new QHBoxLayout;
    QLabel *versionLabel = new QLabel(tr("Version: "));
    versionLabel->setStyleSheet("QLabel { color: #444444; }");
    versionLayout->addSpacing(20);
    versionLayout->addWidget(versionLabel, 0, Qt::AlignTop);
    versionLayout->addWidget(m_versionLabel, 0, Qt::AlignTop);
    versionLayout->addStretch();
    versionLayout->addSpacing(20);

    QHBoxLayout *copyrightLayout = new QHBoxLayout;
    QLabel *copyrightLabel = new QLabel(tr("Copyright: "));
    copyrightLabel->setStyleSheet("QLabel { color: #444444; }");
    copyrightLayout->addSpacing(20);
    copyrightLayout->addWidget(copyrightLabel, 0, Qt::AlignTop);
    copyrightLayout->addWidget(m_copyrightLabel, 0, Qt::AlignTop);
    copyrightLayout->addStretch();
    copyrightLayout->addSpacing(20);

    QHBoxLayout *descLayout = new QHBoxLayout;
    QLabel *descLabel = new QLabel(tr("Description: "));
    descLabel->setStyleSheet("QLabel { color: #444444; }");
    descLayout->addSpacing(20);
    descLayout->addWidget(descLabel, 0, Qt::AlignTop);
    descLayout->addWidget(m_descriptionLabel, 0, Qt::AlignTop);
    descLayout->addStretch();
    descLayout->addSpacing(20);

    // bottom
    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(m_installBtn);
    bottomLayout->addWidget(m_removeBtn);
    bottomLayout->addWidget(m_reinstallBtn);
    bottomLayout->addStretch();

    m_installBtn->setFixedSize(120, 36);
    m_installBtn->setObjectName("BlueButton");
    m_installBtn->hide();

    m_removeBtn->setFixedSize(120, 36);
    m_removeBtn->setObjectName("GrayButton");
    m_removeBtn->hide();

    m_reinstallBtn->setFixedSize(120, 36);
    m_reinstallBtn->setObjectName("GrayButton");
    m_reinstallBtn->hide();

    // add widgets to main layout.
    m_layout->addWidget(m_iconLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
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

    connect(m_installBtn, &QPushButton::clicked, this, &SingleFilePage::installBtnClicked);
}

SingleFilePage::~SingleFilePage()
{
}

#include <QDebug>

void SingleFilePage::updateInfo(DFontData *data)
{
    const QFontMetrics fm = m_versionLabel->fontMetrics();

    m_nameLabel->setText(data->familyName);
    m_styleLabel->setText(data->styleName);
    m_typeLabel->setText(data->type);
    m_versionLabel->setText(fm.elidedText(data->version, Qt::ElideRight,
                                          this->width() / 2));
    m_copyrightLabel->setText(fm.elidedText(data->copyright, Qt::ElideRight,
                                            this->width() / 2 + fm.width(tr("Copyright: "))));
    m_descriptionLabel->setText(fm.elidedText(data->description.remove(QRegExp("[\\n\\t\\r]")), Qt::ElideRight,
                                              this->width() * 1.2));


    qDebug() << data->description;

    // m_statusLabel->setText(tr("Same version installed"));
    // m_statusLabel->setStyleSheet("QLabel { color: #D70751; }");

    if (DFontInfo::isFontInstalled(data)) {
        m_installBtn->hide();
        m_removeBtn->show();
        m_reinstallBtn->show();
    } else {
        m_installBtn->show();
        m_removeBtn->hide();
        m_reinstallBtn->hide();
    }
}
