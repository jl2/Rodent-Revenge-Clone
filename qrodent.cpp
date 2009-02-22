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
#include <iostream>


/*!
  Empty constructor just calls super class' constructor.
*/
QRodent::QRodent(QWidget *parent) : QWidget(parent),
				    gameStarted(false),
				    gameSpeed(20),
				    timerId(startTimer(gameSpeed*50)),
				    redrawAll(true),
				    triedResize(0) {
  setAutoFillBackground(false);
  setAttribute(Qt::WA_OpaquePaintEvent);
  QPixmapCache::setCacheLimit(1024*40);
}
void QRodent::timerEvent(QTimerEvent *event) {
  if (event->timerId() == timerId) {
    if (gameStarted) {
      rg.update();
      repaint();
    }

  } else {
    QWidget::timerEvent(event);
  }
}
/*!
  Update the screen.
*/
void QRodent::paintEvent(QPaintEvent *) {
  QPainter painter(this);
  QPixmap tmp;
  
  if (redrawAll) {
    rblock_t blk;

    
    painter.fillRect(0, 0,
		     size().width(),
		     size().height(),
		     QColor(0x21,0xbf,0x46,0xff));

    for (unsigned i=0;i<rg.width();++i) {
      for (unsigned j = 0; j<rg.height();++j) {
	blk = rg.blockAt(i,j);
	if (!QPixmapCache::find(be_to_str[blk], tmp)) {
	  QPixmapCache::insert(be_to_str[blk], tiles[blk]);
	  QPixmapCache::find(be_to_str[blk], tmp);
	}

	painter.drawPixmap(i*bw,
			   j*bh,
			   tmp);
      }
    }
  } else {
    changed_list::iterator iter;
    rblock_t blk;
    for (iter = rg.changedBegin();
	 iter != rg.changedEnd();
	 ++iter) {

      painter.fillRect(iter->first*bw,
		       iter->second*bh,
		       bw,bh,
		       QColor(0x21,0xbf,0x46,0xff));
      blk = rg.blockAt(iter->first,iter->second);
      if (!QPixmapCache::find(be_to_str[blk], tmp)) {
	QPixmapCache::insert(be_to_str[blk], tiles[blk]);
	QPixmapCache::find(be_to_str[blk], tmp);
      }
      painter.drawPixmap(iter->first*bw,
			 iter->second*bh,
			 tmp);
    }
    rg.resetChanged();
  }
  redrawAll = false;
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
    gameStarted = true;
    unsigned int xp, yp;

    xp = rg.xpos();
    yp = rg.ypos();
    
    num = rg.move(md);

    if (num) {
      repaint();
    }
  }
  
}

/*!
  Resize the game window.
  Makes sure the window is an even multiple of the block size.
*/
void QRodent::resizeEvent(QResizeEvent *event) {
  bh =double(event->size().height())/double(rg.height());
  bw = double(event->size().width())/double(rg.width());
  
  if (cached_tiles[empty].isNull()) {
    loadImages();
  }
  resizeImages();
  triedResize=0;
  redrawAll = true;
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

  resizeImages();
}

/*!
  Update all tile images to the size (bw x bh)
*/
void QRodent::resizeImages() {
  for (unsigned img = empty; img<last_block; ++img) {
    QPixmapCache::remove(be_to_str[img]);
    tiles[img] = cached_tiles[img].scaled(bw,bh,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    QPixmapCache::insert(be_to_str[img], tiles[img]);
  }
}
