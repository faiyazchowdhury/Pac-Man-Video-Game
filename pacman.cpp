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
 
#include "mbed.h"
#include "globals.h"
#include "map_public.h"
#include "pacman.h"

PLAYER pacman;
static int score;
static int lives;
static int invTime;

void pacman_init(int grid_x, int grid_y, int level){
    pacman.motion = PACMAN_HEADING_RIGHT;
    pacman.status = PACMAN_WAIT_COMMAND;
    pacman.grid_x = grid_x;
    pacman.grid_y = grid_y;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw();
    score = 0;
    lives = 3;
    invTime = 7 - level;
    uLCD.printf("score:%d, lives:%d",score,lives);
}

void pacman_gatherData(int *x, int *y, int *s, int *l, int *i){
    *x = pacman.grid_x;
    *y = pacman.grid_y;
    *s = score;
    *l = lives;
    *i = invTime;
}

int pacman_getLives(){
    return lives;
}

int pacman_getScore(){
    return score;
}

void pacman_die(int grid_x, int grid_y){
    pacman.motion = PACMAN_HEADING_RIGHT;
    pacman.status = PACMAN_WAIT_COMMAND;
    pacman.grid_x = grid_x;
    pacman.grid_y = grid_y;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw();
    lives--;
    uLCD.printf("score:%d, lives:%d",score,lives);
}

void pacman_lvlUp(int grid_x, int grid_y){
    pacman.motion = PACMAN_HEADING_RIGHT;
    pacman.status = PACMAN_WAIT_COMMAND;
    pacman.grid_x = grid_x;
    pacman.grid_y = grid_y;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw();
    lives++;
    invTime--;
    uLCD.printf("score:%d, lives:%d",score,lives);
}

void pacman_draw(void){
    int x = pacman.grid_x;
    int y = pacman.grid_y;
    GRID grid_info = map_get_grid_status(x,y);
    int screen_x = grid_info.x + GRID_RADIUS;
    int screen_y = grid_info.y + GRID_RADIUS;
    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS, PACMAN_COLOR);
    //draw the mouth of Pacman
    switch(pacman.motion){
        case PACMAN_HEADING_RIGHT: 
            uLCD.filled_rectangle(screen_x,screen_y-1,screen_x+GRID_RADIUS,screen_y+1, BACKGROUND_COLOR);
            break;
        case PACMAN_HEADING_LEFT: 
            uLCD.filled_rectangle(screen_x-GRID_RADIUS,screen_y-1,screen_x,screen_y+1, BACKGROUND_COLOR);
            break;
        case PACMAN_HEADING_UP: 
            uLCD.filled_rectangle(screen_x-1,screen_y-GRID_RADIUS,screen_x+1,screen_y, BACKGROUND_COLOR);
            break;
        case PACMAN_HEADING_DOWN: 
            uLCD.filled_rectangle(screen_x-1,screen_y,screen_x+1,screen_y+GRID_RADIUS, BACKGROUND_COLOR);
            break;
        default: //head right
            uLCD.filled_rectangle(screen_x,screen_y-1,screen_x+GRID_RADIUS,screen_y+1, BACKGROUND_COLOR);
            break;
    }
}

void pacman_clear(void){
    int x = pacman.grid_x;
    int y = pacman.grid_y;
    GRID grid_info = map_get_grid_status(x,y);
    uLCD.filled_rectangle(grid_info.x, grid_info.y, grid_info.x+GRID_SIZE-1, grid_info.y+GRID_SIZE-1, BACKGROUND_COLOR);
}

void pacman_set_action(PACMAN_MOTION motion){
    if(motion == PACMAN_NO_UPDATE)
        return;
        
    pacman.motion = motion;
    pacman.status = PACMAN_RUNNING;
}

void pacman_update_position(void){
    GRID next_grid_info;
    int x = pacman.grid_x;
    int y = pacman.grid_y;
    if(pacman.status==PACMAN_RUNNING){
        switch(pacman.motion){
            case PACMAN_HEADING_UP:
                if(y==0)
                    y = NUM_GRID_Y-1;
                else
                    y--;
                break;
            case PACMAN_HEADING_DOWN:
                if(y==(NUM_GRID_Y-1))
                    y = 0;
                else
                    y++;
                break;
            case PACMAN_HEADING_RIGHT:
                if(x==NUM_GRID_X-1)
                    x = 0;
                else
                    x++;
                break;
            case PACMAN_HEADING_LEFT:
                if(x==0)
                    x = NUM_GRID_X-1;
                else
                    x--;
                break;
        }
        next_grid_info = map_get_grid_status(x,y);
        if(next_grid_info.status==GRID_WALL){
            pacman.status=PACMAN_WAIT_COMMAND;
        }
        else{
            //clear the picture of previous
            pacman_clear();
            //update pacman position in map
            pacman.grid_x = x;
            pacman.grid_y = y;
            pacman_draw();
            if(next_grid_info.status>=GRID_COOKIE){
                map_eat_cookie(x,y);
                score++;
                if(next_grid_info.status==GRID_SUPER_COOKIE) //one super cookie worth 5 points // ADD CODE HERE
                    score+=4;
                uLCD.locate(0,0);
                uLCD.printf("score:%d, lives:%d",score,lives);
            }
        }
    }
        
}
