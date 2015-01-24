/*
    fonts.c
    Simple example of how to use multiple fonts on the GB
    Michael Hope, 1999.
*/

#include <stdio.h>
//color constants are in drawing.h
#include <gb/drawing.h>
#include <gb/gb.h>

void main(void){
	UBYTE i, x, y, dx, dy;
	
    printf("Press start!"); 
	waitpad(J_START);
	plot (10, 10, WHITE);
	plot (20, 10, LTGREY);
	plot (30, 10, DKGREY);
	plot (40, 10, BLACK);
	
	x=50;
	y=50;
	dx=1;
	dy=0;
	
	while (1) {
		i = joypad();
		if(i & J_UP){
			dx=0;
			dy=-1;
		}
		if(i & J_DOWN){
			dx=0;
			dy=1;
		}
		if(i & J_LEFT){
			dx=-1;
			dy=0;
		}
		if(i & J_RIGHT){
			dx=1;
			dy=0;
		}
		
		x += dx;
		y += dy;
		
		plot(x,y,BLACK);
		
		delay(50);
	}
}

    
    
