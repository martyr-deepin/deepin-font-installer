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

#include "homepage.h"
#include "dsvgrenderer.h"
#include "utils.h"
#include <QSvgWidget>

HomePage::HomePage(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_iconLabel(new QLabel),
      m_tipsLabel(new QLabel(tr("Drag font file here"))),
      m_splitLine(new QLabel),
      m_chooseBtn(new DLinkButton(tr("Select file")))
{
    QPixmap iconPixmap = DSvgRenderer::render(":/images/icon.svg", QSize(140, 140) * devicePixelRatioF());
    m_iconLabel->setFixedSize(140, 140);
    m_iconLabel->setPixmap(iconPixmap);

    m_splitLine->setPixmap(QPixmap(":/images/split_line.svg"));

    m_layout->addStretch();
    m_layout->addWidget(m_iconLabel, 0, Qt::AlignTop | Qt::AlignHCenter);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_tipsLabel, 0, Qt::AlignHCenter);
    m_layout->addSpacing(10);
    m_layout->addWidget(m_splitLine, 0, Qt::AlignHCenter);
    m_layout->addSpacing(20);
    m_layout->addWidget(m_chooseBtn, 0, Qt::AlignHCenter);
    m_layout->addStretch();

    connect(m_chooseBtn, &DLinkButton::clicked, this, &HomePage::onChooseBtnClicked);
}

HomePage::~HomePage()
{
}

void HomePage::onChooseBtnClicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(Utils::suffixList());

    if (dialog.exec() != QDialog::Accepted)
        return;

    emit fileSelected(dialog.selectedFiles());
}
