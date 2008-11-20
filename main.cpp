/*
  main.cpp
 
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

#include <QApplication>
#include "qrodent.h"

/*!
  Start the game running.
*/
int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  QRodent rg;
  rg.setWindowTitle(QObject::tr("QRodent"));
  rg.resize(320,320);
  rg.show();

  return app.exec();
}
