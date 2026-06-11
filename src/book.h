/*
  Xiphos, a UCI chess engine
  Copyright (C) 2018, 2019 Milos Tatarevic

  Xiphos is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Xiphos is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef BOOK_H
#define BOOK_H

#include "position.h"

#define BOOK_MAX_ENTRIES 1000000

typedef struct {
  uint64_t key;
  uint16_t move;
  uint16_t weight;
} book_entry_t;

typedef struct {
  book_entry_t *entries;
  int count;
  int loaded;
  char file_path[1024];
} book_t;

extern book_t book;

void book_init();
void book_load(const char *path);
uint64_t book_compute_key(position_t *pos);
int book_probe(position_t *pos, move_t *result);

#endif