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

#include "ghost.h"

//================================================================//
// Private functions
void clean_blk(unsigned int blk_x, unsigned int blk_y)
{
    GRID grid_info = map_get_grid_status(blk_x,blk_y);
    uLCD.filled_rectangle(grid_info.x, grid_info.y, grid_info.x+GRID_SIZE-1, grid_info.y+GRID_SIZE-1, BACKGROUND_COLOR);
}

bool check_blk_occupied(unsigned int blk_x, unsigned int blk_y)
{
    if (map_get_grid_status(blk_x,blk_y).status==GRID_WALL)
        return true;
    else
        return false;
}

void draw_ghost(unsigned int blk_x, unsigned int blk_y, unsigned int ghost_color)
{
    GRID grid = map_get_grid_status(blk_x, blk_y);
    unsigned pos_x = grid.x + GRID_RADIUS;
    unsigned pos_y = grid.y + GRID_RADIUS;
    uLCD.filled_circle(pos_x,pos_y,GRID_RADIUS,ghost_color);
    uLCD.filled_rectangle(pos_x-GRID_RADIUS,pos_y,pos_x+GRID_RADIUS,pos_y+GRID_RADIUS,ghost_color);
    uLCD.filled_circle(pos_x+1,pos_y-1,1,BLACK);
    uLCD.filled_circle(pos_x-1,pos_y-1,1,BLACK);
}

void ghost_move(GHOST * g, unsigned int new_blk_x, unsigned int new_blk_y)
{
    // clean up ghost at old position
    clean_blk(g->x, g->y);
    // clean the block at new position
    clean_blk(new_blk_x, new_blk_y);
    // draw the ghost at new position
    draw_ghost(new_blk_x, new_blk_y, g->color);

    // recover map component
    map_draw_grid(g->x, g->y);

    g->x = new_blk_x;
    g->y = new_blk_y;
}

// move ghost up/down/left/right
// return false if failed. true if success
bool ghost_up(GHOST * g)
{
    if (check_blk_occupied(g->x, g->y-1)) return false;
    if (g->y==0)
        ghost_move(g, g->x, (NUM_GRID_Y-1));
    else
        ghost_move(g, g->x, g->y-1);
    return true;
}
bool ghost_down(GHOST * g)
{
    if (check_blk_occupied(g->x, g->y+1)) return false;

    if (g->y==(NUM_GRID_Y-1))
        ghost_move(g, g->x, 0);
    else
        ghost_move(g, g->x, g->y+1);
    return true;
}

bool ghost_left(GHOST * g)
{
    if (check_blk_occupied(g->x-1, g->y)) return false;
    if (g->x==0) 
        ghost_move(g, (NUM_GRID_X-1), g->y);
    else
        ghost_move(g, g->x-1, g->y);
    return true;
}
bool ghost_right(GHOST * g)
{
    if (check_blk_occupied(g->x+1, g->y)) return false;
    if (g->x==(NUM_GRID_X-1)) 
        ghost_move(g, 0, g->y);
    else
        ghost_move(g, g->x+1, g->y);
    return true;
}

void gen_random_direction(GHOST * g)
{
    bool blocked[4];
    blocked[0] = check_blk_occupied(g->x, g->y-1);    //up
    blocked[1] = check_blk_occupied(g->x, g->y+1);    //down
    blocked[2] = check_blk_occupied(g->x-1, g->y);    //left
    blocked[3] = check_blk_occupied(g->x+1, g->y);    //right
    
    unsigned int npath=0;
    unsigned int outcome=0;
    if (blocked[0]==false) npath++;
    if (blocked[1]==false) npath++;
    if (blocked[2]==false) npath++;
    if (blocked[3]==false) npath++;
    
    unsigned curr=(unsigned int) g->ghost_motion;
    unsigned reverse;
    if ((curr%2)==0)
        reverse = curr+1;
    else 
        reverse = curr-1;
    
    unsigned off=rand();
    for (int i=0;i<4;i++)
    {
        outcome=(off+i)%4;
        // skip the reverse path if possible
        if (npath>1 && reverse==outcome) continue;
        if (blocked[outcome]==false) break;
    }
    g->ghost_motion = (GHOST_MOTION)outcome;
    
    return;
}
//======================================================================================//
//All the function descriptions are in the ghost.h file

//Create a DLL for ghosts
DLinkedList* ghostDLL = NULL;


void ghost_init(void){
    ghostDLL = create_dlinkedlist();
    //Your code here
}

// Public functions
void ghost_create(unsigned int blk_x, unsigned int blk_y, unsigned int color)
{
    GHOST * ghost = (GHOST *) malloc(sizeOf(GHOST));
    ghost->x = blk_x;
    ghost->y = blk_y;
    ghost->color = color;
    insertTail(ghostDLL, ghost);
    //Your code here  
}

DLinkedList* get_ghost_list(void) {
    //Your code here
    return ghostDLL;
}

void ghost_show(DLinkedList* list)
{
    GHOST* newGhost = getHead(list);
    while(newGhost){
        clean_blk(newGhost->x, newGhost->y);
        map_draw_grid(newGhost->x, newGhost->y);
        draw_ghost(newGhost->x, newGhost->y, newGhost->color);
        newGhost = (GHOST*)getNext(ghostDLL);
    }
    //Your code here
    //Functions like map_draw_grid, clean_blk, draw_ghost may be useful
}

void ghost_random_walk(void)
{
    GHOST* newGhost = (GHOST*)getHead(ghostDLL);
    while(newGhost){ 
        if(newGhost->ghost_motion == GHOST_DIED){ 
            //recover map grid
            map_draw_grid(newGhost->x, newGhost->y);
            //delete from the linked list
            newGhost = (GHOST*)deleteForward(ghostDLL);    
        }else{
             gen_random_direction(newGhost);   
             switch (newGhost->ghost_motion) {
                case GHOST_UP:
                    ghost_up(newGhost);
                    break;
                case GHOST_DOWN:
                    ghost_down(newGhost);
                    break;
                case GHOST_LEFT:
                    ghost_left(newGhost);
                    break;
                case GHOST_RIGHT:
                    ghost_right(newGhost);
                break;
                default:
                     break;
            }                            
        }
        //advance the loop 
        newGhost = (GHOST*)getNext(ghostDLL);
    } 
}

//=======================================================================//

