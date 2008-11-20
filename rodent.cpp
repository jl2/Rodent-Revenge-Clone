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


/*!
  Initialize a new game.
*/
Rodent::Rodent(unsigned level) : board_size(23),
				     cur_level(level),
				     curXPos(board_size/2),
				     curYPos(board_size/2),
				     points(0),
				     livesLeft(2) {
  genLevel();
}

/*!
  Jumpt to the specified level.
*/
void Rodent::newGame(unsigned level) {
  cur_level = level;
  genLevel();
}

/*!
  Converts a direction_t into x and y offsets for movement.
*/
inline void Rodent::getOffset(direction_t dir, int &xoff, int &yoff) {
  static int xoffsets[] = { 0,0,1,-1,-1, 1,-1,1};
  static int yoffsets[] = {-1,1,0, 0,-1,-1, 1,1};
  xoff = xoffsets[dir];
  yoff = yoffsets[dir];
}

/*!
  Moves the player in the specified direction.
  Returns the number of blocks moved.
*/
unsigned Rodent::move(direction_t dir) {
  int xoff = 0;
  int yoff = 0;
  unsigned rv = 0;
  
  getOffset(dir, xoff, yoff);
  
  if (canMove(curXPos, curYPos, dir)) {
    rv = doMove(curXPos, curYPos, dir);
    curXPos += xoff;
    curYPos += yoff;
  }
  return rv;
}

/*!
  Moves the block at (x,y) in the specified direction.
  Returns the number of blocks moved.
*/
unsigned Rodent::doMove(unsigned x, unsigned y, direction_t dir) {

  int xoff = 0;
  int yoff = 0;
  unsigned retVal = 0;
  getOffset(dir, xoff, yoff);

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

/*!
  Check whether the block at (x,y) can move in direction dir.
  Returns true if the block can move.
*/
bool Rodent::canMove(unsigned x, unsigned y, direction_t dir) {
  int xoff = 0;
  int yoff = 0;

  getOffset(dir, xoff, yoff);
  
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

/*!
  Returns the block type at (x,y)
  For consistency, this should be the only place where
  the curBoard array is accessed.
*/
rblock_t Rodent::blockAt(unsigned x, unsigned y) const {
  return curBoard[y][x];
}

/*!
  Returns whether or not the block at (x,y) has been modified since
  the last reset.
  This was added primarily to "optimize" drawing by avoiding updates to
  blocks that have not been changed recently.
*/
bool Rodent::blockChanged(unsigned x, unsigned y) const {
  return hasChanged[y][x];
}

/*!
  Resets the hasChanged matrix, after a redraw, for example.
*/
void Rodent::resetChanged() {
  for (size_t x = 0; x<board_size;++x) {
    for (size_t y = 0; y<board_size;++y) {
      hasChanged[y][x] = false;
    }
  }
}

/*!
  Sets the block at (x,y) to type b.
  For consistency, this should be the only place where curBoard
  is modified.
*/
inline void Rodent::setBlockAt(unsigned x, unsigned y, rblock_t b) {
  curBoard[y][x] = b;
  hasChanged[y][x] = true;
}

/*!
  Generates a new board layout based on the cur_level variable.
  TODO: Generate an actual level, not a "test" level.
*/
void Rodent::genLevel() {
  unsigned i = 0;
  unsigned j = 0;
  
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
    setBlockAt(1,img+1, rblock_t(img));
  }

  curXPos = curYPos = board_size/2;
  
  setBlockAt(curXPos, curYPos, mouse);
}

/*!
  Performs an update of the game state a few times a second.
  This is primarily for updating cat movement/location, but also
  handles stuff like advancing the score.
*/
bool Rodent::update() {
  return false;
}
