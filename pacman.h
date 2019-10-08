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
/** @file pacman.h */
#ifndef PACMAN_H
#define PACMAN_H

#define PACMAN_COLOR 0xFFFFFF

/// The enum defines the status of a pacman
typedef enum {
    PACMAN_WAIT_COMMAND=0,
    PACMAN_RUNNING
} PACMAN_STATUS;

/// The enum defines the motion of a pacman
typedef enum {
    PACMAN_NO_UPDATE=0,
    PACMAN_HEADING_UP,
    PACMAN_HEADING_DOWN,
    PACMAN_HEADING_RIGHT,
    PACMAN_HEADING_LEFT
} PACMAN_MOTION;

/// This struct contains the status of a pacman
typedef struct {
    int grid_x;        ///< The position of pacman on the grid
    int grid_y;        ///< The position of pacman on the grid
    PACMAN_MOTION motion;
    PACMAN_STATUS status;  ///< See enum CITY_STATUS
} PLAYER;

/** Initialize a pacman with given position. The default mostion is PACMAN_HEADING_RIGHT
    and status is PACMAN_WAIT_COMMAND. 
    @param blk_x The horizontal position in the grid.
    @param blk_y The vertical position in the grid.
*/
void pacman_init(int grid_x, int grid_y);

/** Similar to init, but lives goes up and invTime goes down.
    @param blk_x The horizontal position in the grid.
    @param blk_y The vertical position in the grid.
*/
void pacman_lvlUp(int grid_x, int grid_y);

/** Similar to init, but lives goes down.
    @param blk_x The horizontal position in the grid.
    @param blk_y The vertical position in the grid.
*/
void pacman_die(int grid_x, int grid_y);

/** Data getter and setter by reference.
    @param x current x
    @param y current y
    @param s score
    @param l lives
    @param i invTime
*/
void pacman_gatherData(int *x, int *y, int *s, int *l, int *i);

/** Draw the pacman on the screen with different mouse direction base on current motion
*/
void pacman_draw(void);

// Clear the pacman in map
void pacman_clear(void);

/** Set the pacman's motion according to the input mostion
    @param motion The desired pacman motion.
*/
void pacman_set_action(PACMAN_MOTION motion);

// Update the pacman's position in map
void pacman_update_position(void);


#endif