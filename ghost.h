/* Gatech ECE2035 2015 SPRING PAC MAN
 * Copyright (c) 2015 Gatech ECE2035
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/** @file ghost.h */
#ifndef GHOST_H
#define GHOST_H

#include "mbed.h"
//#include "uLCD_4DGL.h"
#include "ghost.h"
#include "map_public.h"
#include "globals.h"
#include "pacman.h"
#include "doubly_linked_list.h"

extern uLCD_4DGL uLCD;


/// The enum defines the motion of a ghost
typedef enum {
    GHOST_UP=0, ///< move up
    GHOST_DOWN, ///< move down
    GHOST_LEFT, ///< move left
    GHOST_RIGHT, ///< move right
    GHOST_DIED
} GHOST_MOTION;


/// This struct contains the status of a ghost
typedef struct
{
    unsigned int x;   ///< horizontal position in the grid
    unsigned int y;   ///< vertical position in the grid
    unsigned int color;   ///< color of the ghost
    GHOST_MOTION ghost_motion;  ///< the motion of the ghost  
} GHOST;

//Initialize an empety doublely linked list
void ghost_init(void);

/** Create a ghost with given position and color. Then add to the ghost doublely linked list
    @param blk_x The horizontal position in the grid.
    @param blk_y The vertical position in the grid.
    @param color Color of the ghost
*/
void ghost_create(unsigned int blk_x, unsigned int blk_y, unsigned int color);

/** Draw the ghost in the linked list on the screen
*/
void ghost_show(DLinkedList* list);

/** Move every ghost in the list randomly on the map for one step
*/
void ghost_random_walk(void);

/** Return the doubly linked list of ghost
*/
DLinkedList* get_ghost_list(void);


/** Extra Feature Function:
    Create a super ghost with given position and color. Then add to the super ghost doublely linked list
    A super ghost is itself a max-size-3 linked list. A super ghost is itself a linked list. The head always 
    walk randomly while the other two node keep track of the two lastest history position.
    @param blk_x The horizontal position in the grid.
    @param blk_y The vertical position in the grid.
    @param color Color of the ghost
*/
void super_ghost_create(unsigned int blk_x, unsigned int blk_y, unsigned int color);

/** Extra Feature Function:
    Move the super ghost in the list randomly on the map for one step
*/
void super_ghost_random_walk(void);

/** Extra Feature Function:
    Return the doubly linked list of super ghost
*/
DLinkedList* get_super_ghost_list(void);
#endif //GHOST_H
