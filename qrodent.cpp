/*
  qrodent.cpp
 
  Copyright (C) 2008 Jeremiah LaRocco

  This file is part of QRodent

  QRodent is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  QRodent is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with QRodent.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QPainter>

#include "qrodent.h"
#include <algorithm>

/*!
  Empty constructor just calls super class' constructor.
*/
QRodent::QRodent(QWidget *parent) : QWidget(parent) {
}

/*!
  Update the screen.
*/
void QRodent::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  
  size_t i, j;

  QRect rpr = event->rect();
  painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
  painter.fillRect(rpr, QColor(0x21,0xbf,0x46,0xff));
  
  unsigned imin = std::min(rpr.x()/bw, rg.width());
  unsigned imax = std::min((rpr.x()+rpr.width())/bw, rg.width());
  
  unsigned jmin = std::min(rpr.y()/bh, rg.height());
  unsigned jmax = std::min((rpr.y()+rpr.height())/bh, rg.height());
  
  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);

  rblock_t blk;
  if (tiles[empty].isNull()) {
    loadImages();
  }
  
  for (i=imin;i<imax;++i) {
    for (j=jmin;j<jmax;++j) {
      blk = rg.blockAt(i,j);
      if (blk && rg.blockChanged(i,j))
	painter.drawPixmap(i*bw,j*bh,tiles[blk]);
    }
  }
}

/*!
  Handle key presses.
  Only the direction keys matter
*/
void QRodent::keyReleaseEvent(QKeyEvent *event) {
  direction_t md;
  bool canMove = false;
  
  switch (event->key()) {
  case (Qt::Key_Up):       canMove = true; md = north; break;
  case (Qt::Key_Down):     canMove = true; md = south; break;
  case (Qt::Key_Right):    canMove = true; md = east;  break;
  case (Qt::Key_Left):     canMove = true; md = west;  break;
  case (Qt::Key_End):      canMove = true; md = sw;    break;
  case (Qt::Key_PageDown): canMove = true; md = se;    break;
  case (Qt::Key_Home):     canMove = true; md = nw;    break;
  case (Qt::Key_PageUp):   canMove = true; md = ne;    break;
  default:                 QWidget::keyReleaseEvent(event);
  }
  int num = 0;
  if (canMove) {
    unsigned int xp, yp;

    xp = rg.xpos();
    yp = rg.ypos();
    
    num = rg.move(md);

    if (num) {
      update(std::min((xp-num)*bw,xp*bw), std::min((yp-num)*bh,yp*bh), 3*num*bw, 3*num*bh);
    }
  }
  
}

/*!
  Resize the game window.
  Makes sure the window is an even multiple of the block size.
*/
void QRodent::resizeEvent(QResizeEvent *event) {
  bh = event->size().height()/rg.height();
  bw = event->size().width()/rg.width();
  
  if (tiles[empty].isNull()) {
    loadImages();
  }
  if (event->size().height() != (int)(bh * rg.height()) ||
      event->size().width() != (int)(bw*rg.width())) {
    resize(bw*rg.width(), bh*rg.height());
  } else {
    resizeImages();
  }
}

/*!
  Read images from disk.
*/
void QRodent::loadImages() {
  cached_tiles[empty].load(":/images/empty_block.svg");
  cached_tiles[movable].load(":/images/movable_block.svg");
  cached_tiles[hole].load(":/images/hole_block.svg");
  cached_tiles[yarn].load(":/images/yarn_block.svg");
  cached_tiles[cat].load(":/images/cat_block.svg");
  cached_tiles[mouse].load(":/images/mouse_block.svg");
  cached_tiles[solid].load(":/images/solid_block.svg");
  cached_tiles[trap].load(":/images/trap_block.svg");
  cached_tiles[cheese].load(":/images/cheese_block.svg");
  cached_tiles[frozen_cat].load(":/images/frozen_cat_block.svg");

  if ((bw==0) || (bh ==0)) {
    tiles[empty] = cached_tiles[empty];
    tiles[movable] = cached_tiles[movable];
    tiles[hole] = cached_tiles[hole];
    tiles[yarn] = cached_tiles[yarn];
    tiles[cat] = cached_tiles[cat];
    tiles[mouse] = cached_tiles[mouse];
    tiles[solid] = cached_tiles[solid];
    tiles[trap] = cached_tiles[trap];
    tiles[cheese] = cached_tiles[cheese];
    tiles[frozen_cat] = cached_tiles[frozen_cat];
  } else {
    resizeImages();
  }
}

/*!
  Update all tile images to the size (bw x bh)
*/
void QRodent::resizeImages() {
  for (unsigned img = empty; img<last_block; ++img) {
    tiles[img]=cached_tiles[img].scaled(bw,bh,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
  }
}
