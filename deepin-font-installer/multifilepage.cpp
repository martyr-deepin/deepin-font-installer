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

#include "multifilepage.h"
#include "listdelegate.h"
#include <QDebug>
#include <QApplication>

MultiFilePage::MultiFilePage(ListModel *model, QWidget *parent)
    : QWidget(parent),
      m_fontInfo(new DFontInfo),
      m_layout(new QVBoxLayout(this)),
      m_listView(new ListView),
      m_installBtn(new QPushButton(tr("Install"))),
      m_closeBtn(new QPushButton(tr("OK"))),
      m_listDelegate(new ListDelegate)
{
    m_listView->setModel(model);
    m_listView->setItemDelegate(m_listDelegate);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addWidget(m_installBtn, 0, Qt::AlignHCenter);
    bottomLayout->addWidget(m_closeBtn, 0, Qt::AlignHCenter);

    m_installBtn->setObjectName("BlueButton");
    m_closeBtn->setObjectName("BlueButton");

    m_installBtn->hide();
    m_closeBtn->hide();

    m_layout->addWidget(m_listView);
    m_layout->addSpacing(15);
    m_layout->addLayout(bottomLayout);
    m_layout->addSpacing(10);

    connect(m_closeBtn, &QPushButton::clicked, this, &QApplication::quit);
}

MultiFilePage::~MultiFilePage()
{
}

void MultiFilePage::reset()
{
    m_listView->reset();
}
