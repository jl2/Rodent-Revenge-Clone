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

#include <utility>
#include <list>

enum rblock_t {empty, movable, hole, yarn, cat, mouse, solid, trap, cheese, frozen_cat, last_block};

enum direction_t {north, south, east, west,
		  nw, ne, sw, se, last_dir};
enum game_state_t {not_started, game_over};
enum cat_move_t {cat_moved, killed_mouse, frozen};
  
typedef std::list< std::pair<unsigned, unsigned> > cat_list;

typedef std::list< std::pair<unsigned, unsigned> > changed_list;

class Rodent {
 public:
  Rodent(unsigned level = 1);

  // Change to specified level
  void newGame(unsigned level = 1);

  // Accessor methods
  unsigned level() { return cur_level; }
  unsigned width() { return board_size; }
  unsigned height() { return board_size; }
  unsigned score() { return points; }
  unsigned lives() { return livesLeft; }
  unsigned xpos() { return curXPos; }
  unsigned ypos() { return curYPos; }

  // Move the mouse in direction dir
  unsigned move(direction_t dir);

  // Reset hasChanged array
  void resetChanged();

  // Whether block at (x,y) has been modified since last reset
  bool blockChanged(unsigned x, unsigned y) const;
  
  // Returns information about curBoard
  rblock_t blockAt(unsigned x, unsigned y) const;

  // Updates game state a few times per second
  bool update();

  changed_list::iterator changedBegin();
  changed_list::iterator changedEnd();
  
 private:
  // Returns validity of moving block at (x,y) in direction dir
  bool canMove(unsigned x, unsigned y, direction_t dir);
  
  // Moves the block at (x,y) in direction dir
  unsigned doMove(unsigned x, unsigned y, direction_t dir);

  // Modifies the curBoard array
  void setBlockAt(unsigned x, unsigned y, rblock_t b);
  cat_move_t moveCat(unsigned &x, unsigned &y);
  
  // Generate a level
  void genLevel();
  void addCat();
  
  // Converts a direction into an x and y offset for movement
  void getOffset(direction_t dir, int &xoff, int &yoff);

  // Will always be 23
  const unsigned board_size;

  // Current board layout
  rblock_t curBoard[23*23];

  // Current level
  unsigned cur_level;

  // Current location of the "mouse"
  unsigned curXPos;
  unsigned curYPos;

  // Score
  unsigned points;

  // Remaining lives
  unsigned livesLeft;

  // List of (x,y) positions of the cats.
  cat_list the_cats;

  changed_list recent_changes;

  int catsPerLevel;

  int catsRemaining;

  int updatesBetweenCats;
  int updatesTillNextCat;
};

#endif
