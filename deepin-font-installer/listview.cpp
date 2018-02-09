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

#include "listview.h"

ListView::ListView(QWidget *parent)
    : DSimpleListView(parent)
{
    // enable frame and radius.
    setFrame(true);
    setClipRadius(8);

    // set row height.
    setRowHeight(56);

    // set fixed height.
    setFixedHeight(280);

    // init scrollbar attributes.
    scrollbarColor = "#101010";
    scrollbarNormalOpacity = 0.5;
    scrollbarHoverOpacity = 0.7;
    scrollbarPressOpacity = 0.8;
    scrollbarFrameNormalOpacity = 0;
    scrollbarFrameHoverOpacity = 0;
    scrollbarFramePressOpacity = 0;
}

ListView::~ListView()
{
}
