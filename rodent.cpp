/*
  rodent.cpp
 
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

#include "rodent.h"

Rodent::Rodent(unsigned level) :board_size(23),
				     cur_level(level),
				     curXPos(board_size/2),
				     curYPos(board_size/2),
				     points(0),
				     livesLeft(2) {
  genLevel();
}

void Rodent::gotoLevel(unsigned level) {
  cur_level = level;
  genLevel();
}

unsigned Rodent::move(direction_t dir) {
  int xoff = 0;
  int yoff = 0;
  unsigned rv = 0;
  
  switch (dir) {
  case north: yoff = -1; break;
  case south: yoff =  1; break;
  case east:  xoff =  1; break;
  case west:  xoff = -1; break;
  case nw:    xoff = -1; yoff = -1; break;
  case se:    xoff =  1; yoff = 1; break;
  case ne:    xoff =  1; yoff = -1; break;
  case sw:    xoff = -1; yoff = 1; break;

  default:    xoff = yoff = 0;
  }
  
  if (canMove(curXPos, curYPos, dir)) {
    rv = doMove(curXPos, curYPos, dir);
    //    std::cout << "doMove() returned " << rv << "\n";
    curXPos += xoff;
    curYPos += yoff;
  }
  return rv;
}

unsigned Rodent::doMove(unsigned x, unsigned y, direction_t dir) {

  int xoff = 0;
  int yoff = 0;
  unsigned retVal = 0;
  
  switch (dir) {
  case north: yoff = -1; break;
  case south: yoff =  1; break;
  case east:  xoff =  1; break;
  case west:  xoff = -1; break;
  case nw:    xoff = -1; yoff = -1; break;
  case se:    xoff =  1; yoff = 1; break;
  case ne:    xoff =  1; yoff = -1; break;
  case sw:    xoff = -1; yoff = 1; break;
  default:    xoff = yoff = 0;
  }
//   std::cout << x << " " << y << " "
// 	    << xoff << " " << yoff << "\n";
  switch (blockAt(x+xoff,y+yoff)) {
  case movable:
      retVal += doMove(x+xoff, y+yoff, dir);
  case cheese:
  case empty:
  case mouse:
    setBlockAt(x+xoff, y+yoff, blockAt(x, y));
    setBlockAt(x, y, empty);
    return retVal+1;
  default:
    return retVal;
  }
}

bool Rodent::canMove(unsigned x, unsigned y, direction_t dir) {
  int xoff = 0;
  int yoff = 0;
  
  switch (dir) {
  case north: yoff = -1; break;
  case south: yoff =  1; break;
  case east:  xoff =  1; break;
  case west:  xoff = -1; break;
  case nw:    xoff = -1; yoff = -1; break;
  case se:    xoff =  1; yoff = 1; break;
  case ne:    xoff =  1; yoff = -1; break;
  case sw:    xoff = -1; yoff = 1; break;
  default:    xoff = yoff = 0;
  }
  
  switch (blockAt(x+xoff, y+yoff)) {
  case empty:
    return true;
  case movable:
    return canMove(x+xoff, y+yoff, dir);
  case cheese:
    return true;
  default:
    return false;
  }
  
}

rblock_t Rodent::blockAt(size_t x, size_t y) const {
  return curBoard[y][x];
}

bool Rodent::blockChanged(size_t x, size_t y) const {
  return hasChanged[y][x];
}

void Rodent::resetChanged() {
  for (size_t x = 0; x<board_size;++x) {
    for (size_t y = 0; y<board_size;++y) {
      hasChanged[y][x] = false;
    }
  }
}

inline void Rodent::setBlockAt(size_t x, size_t y, rblock_t b) {
  curBoard[y][x] = b;
  hasChanged[y][x] = true;
}

void Rodent::genLevel() {
  size_t i = 0;
  size_t j = 0;
  
  for (i=0;i<board_size;++i) {
    for (j=0;j<board_size;++j) {
      if (i==0) {
	setBlockAt(i,j,solid);
      } else if (j==0) {
	setBlockAt(i,j,solid);
      } else if (i==(board_size-1)) {
	setBlockAt(i,j,solid);
      } else if (j==(board_size-1)) {
	setBlockAt(i,j,solid);
      } else {
	setBlockAt(i,j,empty);
      }
    }
  }
  
  for (i=3;i<(board_size-3);++i) {
    for (j=3;j<(board_size-3);++j) {
      setBlockAt(i,j,movable);
    }
  }
  for (unsigned img = empty; img<last_block; ++img) {
    setBlockAt(1,img, rblock_t(img));
  }

  curXPos = curYPos = board_size/2;
  
  setBlockAt(curXPos, curYPos, mouse);
}
