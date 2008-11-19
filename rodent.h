/*
  rodent.h
 
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

#ifndef RODENT_H
#define RODENT_H

#include <vector>
enum rblock_t {empty, movable, hole, yarn, cat, mouse, solid, trap, cheese, frozen_cat, last_block};
  
enum direction_t {north, south, east, west,
		  nw, ne, sw, se};

class Rodent {
 public:
  Rodent(unsigned level = 1);

  void gotoLevel(unsigned level);
  unsigned level() { return cur_level; }
  unsigned width() { return board_size; }
  unsigned height() { return board_size; }
  size_t xpos() { return curXPos; }
  size_t ypos() { return curYPos; }

  unsigned move(direction_t dir);
  bool canMove(unsigned x, unsigned y, direction_t dir);
  unsigned doMove(unsigned x, unsigned y, direction_t dir);
  void resetChanged();
  bool blockChanged(size_t x, size_t y) const;
  rblock_t blockAt(size_t x, size_t y) const;
  
 private:
  void setBlockAt(size_t x, size_t y, rblock_t b);
  void genLevel();
  
  const unsigned board_size;
  rblock_t curBoard[23][23];
  bool hasChanged[23][23];
  unsigned cur_level;
  size_t curXPos;
  size_t curYPos;
  unsigned points;
  unsigned livesLeft;
};

#endif
