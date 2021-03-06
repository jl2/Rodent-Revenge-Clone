/*
  qrodent.h
 
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

#ifndef QRODENT_H
#define QRODENT_H_

#include <QtGui>
#include <QWidget>

#include "rodent.h"

static const QString be_to_str[] = {QObject::tr("empty"),
			      QObject::tr("movable"),
			      QObject::tr("hole"),
			      QObject::tr("yarn"),
			      QObject::tr("cat"),
			      QObject::tr("mouse"),
			      QObject::tr("solid"),
			      QObject::tr("QObject::trap"),
			      QObject::tr("cheese"),
			      QObject::tr("frozen_cat"),
			      QObject::tr("last_block")}; 

class QRodent : public QWidget {
  Q_OBJECT;

 public:
  QRodent(QWidget *parent = 0);

 protected:
  void paintEvent(QPaintEvent *event);
  void keyReleaseEvent(QKeyEvent *event);
  void resizeEvent(QResizeEvent *event);
  void timerEvent(QTimerEvent *event);
  
 private:
  // Cache of unsized original tile images
  QPixmap cached_tiles[last_block];
  
  // The tile images currently being used
  QPixmap tiles[last_block];

  // Load images from disk  
  void loadImages();

  // Update tile image sizes
  void resizeImages();

  // The actual game instance
  Rodent rg;

  // Width and height of tiles
  double bw;
  double bh;
  bool gameStarted;

  unsigned gameSpeed;
  int timerId;
  bool redrawAll;
  int triedResize;
};

#endif
