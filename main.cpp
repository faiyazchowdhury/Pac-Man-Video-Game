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
/** @file main.cpp */
// Include header files for platform
#include "mbed.h"
#include "wave_player.h"
#include "SDFileSystem.h"
#include "PinDetect.h"

// Include header files for pacman project
#include "globals.h"
#include "map_public.h"
#include "pacman.h"
#include "ghost.h"
#include "MMA8452.h"
#include "doubly_linked_list.h"

// Platform initialization
DigitalIn left_pb(p21);  // push bottem
DigitalIn right_pb(p22); // push bottem
DigitalIn up_pb(p23);    // push bottem
DigitalIn down_pb(p24);  // push bottem
uLCD_4DGL uLCD(p9,p10,p11); // LCD (serial tx, serial rx, reset pin;)
Serial pc(USBTX,USBRX);     // used by Accelerometer
MMA8452 acc(p28, p27, 100000); // Accelerometer
AnalogOut DACout(p18);      // speaker
wave_player waver(&DACout); // wav player
SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card and filesystem (mosi, miso, sck, cs)

DigitalOut myled1(LED1);
DigitalOut myled2(LED2);
DigitalOut myled3(LED3);
DigitalOut myled4(LED4);
// Extra Credit 2: LED for life and Death

// Example of the decleration of your implementation
void playSound(char * wav);


/** Main() is where you start your implementation
    @brief The hints of implementation are in the comments. <br>
    @brief You are expected to implement your code in main.cpp and pacman.cpp. But you could modify any code if you want to make the game work better.
*/
int main()
{   
    // Initialize the timer
    /// [Example of time control implementation]
        /// Here is a rough example to implement the timer control <br><br>
    int tick, pre_tick;
    srand (time(NULL));
    Timer timer;
    timer.start();
    tick = timer.read_ms();
    pre_tick = tick;
    
    // Initialize the buttons        
    left_pb.mode(PullUp);  // The variable left_pb will be zero when the pushbutton for moving the player left is pressed    
    right_pb.mode(PullUp); // The variable rightt_pb will be zero when the pushbutton for moving the player right is pressed        
    up_pb.mode(PullUp);    //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    down_pb.mode(PullUp);  //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    
    // My Variables:
    int orientation = 0;
    int time = 100;
    int lives = 2;
    int level = 1;
    int score1 = 0;
    int score2 = 0;
    int score3 = 0;
    int score4 = 0;
    // Menu
    while(1)
    {
        uLCD.cls();
        uLCD.locate(1,1);
        uLCD.printf("Choose your level Press Button!");
        uLCD.locate(4,5);
        uLCD.printf("Lvl 1 (Up)");
        uLCD.locate(4,7);
        uLCD.printf("Lvl 2 (Right)");
        uLCD.locate(4,9);
        uLCD.printf("Lvl 3 (Left)");
        uLCD.locate(4,11);
        uLCD.printf("History (Down)");
        while(up_pb && left_pb && right_pb) {
            if (!up_pb) {
                level = 1;
            } else if (!right_pb) {
                level = 2;
            } else if (!left_pb) {
                level = 3;
            } else if (!down_pb) {
                uLCD.cls();
                uLCD.locate(4,5);
                uLCD.printf("Score1: %d", score1);
                uLCD.locate(5,5);
                uLCD.printf("Score1: %d", score1);
                uLCD.locate(6,5);
                uLCD.printf("Score1: %d", score1);
                uLCD.locate(7,5);
                uLCD.printf("Score1: %d", score1);
                uLCD.locate(9,5);
                uLCD.printf("BACK (DOWN)");
                wait(1);
                while(!down_pb){
                    level = 0;
                }
            }
        }
        
        
        
        
        /// [Example of the game control implementation]
        /// Here is the example to initialize the game <br><br>
        uLCD.cls();
        map_init();
        pacman_init(8,9, level); // Center of the map
    
        //Your code here
        //Initiate & create & show the ghosts  
        ghost_init();
        ghost_create( 8,7 , 0xFF0000); //blinky
        ghost_create(9,7, 0xFFD5F1);  //pinky
        ghost_create(7,7, 0x00FFFF);  //inky
        ghost_create(8,6, 0xF49629);  //clyde
        ghost_show((DLinkedList*)(get_ghost_list));
        //[Demo of play sound file]
        playSound("/sd/wavfiles/START.wav");  //
        
        
    
        /// 1. Begin the game loop
        while(lives >= 0){
            tick = timer.read_ms(); // Read current time
            ghost_random_walk();

// Force Level Advance
            if ((!left_pb && (!right_pb || !up_pb || !down_pb)) || (!right_pb && (!left_pb || !up_pb || !down_pb))) {
                uLCD.cls();
                map_init();
                pacman_lvlUp(8,9);
                //ghost_init();
            } else if (!left_pb) { // Extra Credit 1. Changes orientation
                orientation++;
            } else if (!right_pb) {
                orientation--;
            } else if (!up_pb) {
                orientation = 0;
            } else if (!down_pb) {
                int count = 5;
                myled1 = 1;
                myled2 = 1;
                myled3 = 1;
                myled4 = 1;
                while(count != 0){
                    switch(count){
                    case 4:
                        myled4=0;
                        break;
                    case 3:
                        myled3=0;
                        break;
                    case 2:
                        myled2=0;
                        break;
                    case 1:
                        myled1=0;
                        break;
                    }
                wait(1);
                count--;
                }

            }


            /// 2. Implement the code to get user input and update the Pacman
            /// -[Hint] Implement the code to move Pacman. You could use either push-button or accelerometer. <br>
            /// The accelerometer's function readXYZGravity() might be useful. 
            double x = 0, y = 0, z = 0;
            if ((orientation%4) == 0) {
                acc.readXYZGravity(&x, &y, &z);
                uLCD.locate(0,1);
                uLCD.printf("Normal   :)");
            } else if ((orientation%4) == 1) {
                acc.readXYZGravity(&y, &x, &z); // Rotate
                y = -y;
                uLCD.locate(0,1);
                uLCD.printf("Left Rot!!");
            } else if ((orientation%4) == 2) {
                acc.readXYZGravity(&y, &x, &z); // Rotate
                x = -x;
                uLCD.locate(0,1);
                uLCD.printf("Right Rot!");
            } else if ((orientation%4) == 3) { // Upside Down
                acc.readXYZGravity(&x, &y, &z);
                x = -x;
                y = -y;
                uLCD.locate(0,1);
                uLCD.printf("Flipped !!");
            }


            if (abs(x) > 0.4 || abs(y) > 0.4) {
                if (abs(x) > abs(y)) {
                    if (x > 0) {
                        // GO DOWN
                        pacman_set_action(PACMAN_HEADING_DOWN);
                    } else {
                        // GO UP
                        pacman_set_action(PACMAN_HEADING_UP);
                    }
                } else {
                    if (y > 0) {
                        // Go RIGHT
                        pacman_set_action(PACMAN_HEADING_RIGHT);
                    } else {
                        // GO LEFT
                        pacman_set_action(PACMAN_HEADING_LEFT);
                    }
                }
            }

            if((tick-pre_tick)>500){ // Time step control
                 pre_tick = tick;
            
            /// 3. Update the Pacman on the screen
            /// -[Hint] You could update the position of Pacman here based on the input at step 2. <br>
            pacman_update_position();
            }
        
        /// 4. Implement the code to check the end of game.
        /// -[Hint] Check whether the ghost catch the Pacman. Make sure you could always detect that the ghost and Pacman meet on the screen.
        /// One tricky scenario is that: Pacman is at grid (3,3) and is heading to (3,4), while the ghost is at grid (3,4) and is heading to (3,3).
        /// Either at time t or t+1, you will see that the Pacman and the ghost are not on the same grid.
        /// However, the Pacman should be caught by ghost with this movement.
        /// -[Hint] Check whether Pacman win the game <br>
        //if (pacman.grid_x == 1000 && pacman.grid_y ==1000) { // DEATH
        //    uLCD.cls();
        //    map_init();
        //    pacman_die(8,9);
        //    ghost_init();
        //    // Extra Credit 4: Colorful death
        //}
        if (map_remaining_cookie() == 0) { // LVL UP
            uLCD.cls();
            map_init();
            pacman_lvlUp(8,9);
            orientation++;
            //ghost_init();
            playSound("/sd/wavfiles/pacman_beginning.wav");
        }
        }
    }
}



// Example of implementation of your functions
void playSound(char * wav)
{
    // open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");

    // play wav file
    waver.play(wave_file);

    // close wav file
    fclose(wave_file);
}

