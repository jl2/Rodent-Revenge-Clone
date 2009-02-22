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

#include <utility>
#include <iostream>

#include <cstdlib>
#include <ctime>

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

  catsPerLevel = 8;
  catsRemaining = catsPerLevel;
  updatesBetweenCats = 100;
  updatesTillNextCat = updatesBetweenCats;

  std::srand(std::time(0));
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
    setBlockAt(x+xoff, y+yoff, blockAt(x, y));
    setBlockAt(x, y, empty);
    return retVal+1;
  case cheese:
    points += 100;
    std::cout << "Score: " << points << "\n";
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
  Moves the block at (x,y) in the specified direction.
  Returns the number of blocks moved.
*/
cat_move_t Rodent::moveCat(unsigned &x, unsigned &y) {

  unsigned moves = 0;
  bool found_mouse = false;
  int xoff = 0;
  int yoff = 0;
  
  for (int i=-1;i<=1;++i) {
    for (int j=-1;j<=1;++j) {
      if (blockAt(x+i,y+j) == mouse) {
	found_mouse = true;
	xoff = i;
	yoff = j;
	setBlockAt(x+xoff, y+yoff, blockAt(x, y));
	setBlockAt(x, y, empty);
	x += xoff;
	y += yoff;
	return killed_mouse;
      }
      if (blockAt(x+i,y+j) == empty)
	moves++;
    }
  }
  if (moves == 0) return frozen;
  

  direction_t dir;
  do {
    dir = direction_t(std::rand()%last_dir);
    getOffset(dir, xoff, yoff);
  } while (blockAt(x+xoff,y+yoff) != empty);
  setBlockAt(x+xoff, y+yoff, blockAt(x, y));
  setBlockAt(x, y, empty);
  x += xoff;
  y += yoff;
  return cat_moved;
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
  return curBoard[y*23+x];
}

/*!
  Resets the reset_changes list, after a redraw, for example.
*/
void Rodent::resetChanged() {
  recent_changes.clear();
}

/*!
  Sets the block at (x,y) to type b.
  For consistency, this should be the only place where curBoard
  is modified.
*/
inline void Rodent::setBlockAt(unsigned x, unsigned y, rblock_t b) {
  curBoard[y*23+x] = b;
  recent_changes.push_back(std::make_pair(x,y));
}

/*!
  Generates a new board layout based on the cur_level variable.
  TODO: Generate an actual level, not a "test" level.
*/
void Rodent::genLevel() {
  std::cout << "Going to level " << cur_level << "\n";
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
//   for (unsigned img = empty; img<last_block; ++img) {
//     setBlockAt(1,img+1, rblock_t(img));
//   }

  curXPos = curYPos = board_size/2;
  
  setBlockAt(curXPos, curYPos, mouse);

  addCat();
}

/*!
  Performs an update of the game state a few times a second.
  This is primarily for updating cat movement/location, but also
  handles stuff like advancing the score.
*/
bool Rodent::update() {
  bool all_frozen = true;
  
  cat_list::iterator iter;
  for (iter = the_cats.begin();
       iter != the_cats.end();
       ++iter) {
    switch (moveCat(iter->first, iter->second)) {
    case frozen:
      setBlockAt(iter->first, iter->second, frozen_cat);
      break;
    case killed_mouse:
      std::cout << "Game over!\n";
      all_frozen = false;
      return false;
      break;
    case cat_moved:
      setBlockAt(iter->first, iter->second, cat);
      all_frozen = false;
      break;
    default:
      std::cout << "I dunno! lol\n";
    }
  }

  // All cats frozen, so convert them to cheese.
  if (all_frozen) {
    for (iter = the_cats.begin();
	 iter != the_cats.end();
	 ++iter) {
      setBlockAt(iter->first, iter->second, cheese);
    }
    catsRemaining -= the_cats.size();
    the_cats.clear();

    // If there are more cats for this level, add them
    // Otherwise the level is over, so generate the next level
    if (catsRemaining) {
      addCat();
      updatesTillNextCat = updatesBetweenCats;
    } else {
      catsRemaining = catsPerLevel;
      cur_level++;
      genLevel();
    }
  }
  // Player is too slow, add a penguin
  --updatesTillNextCat;
  if (updatesTillNextCat==0) {
    updatesTillNextCat = updatesBetweenCats;
    if (catsRemaining>the_cats.size()) {
      addCat();
    }
  }
  return true;
}

changed_list::iterator Rodent::changedBegin() {
  return recent_changes.begin();
}

changed_list::iterator Rodent::changedEnd() {
  return recent_changes.end();
}

void Rodent::addCat() {
  unsigned cpx;
  unsigned cpy;

  do {
    cpx = std::rand()%board_size;
    cpy = std::rand()%board_size;
  } while (blockAt(cpx, cpy) != empty);

  setBlockAt(cpx, cpy, cat);
  the_cats.push_back(std::make_pair(cpx, cpy));
}
