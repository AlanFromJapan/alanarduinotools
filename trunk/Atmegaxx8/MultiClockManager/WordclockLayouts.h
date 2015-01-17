/*
 * WordclockLayouts.h
 *
 * Created: 2014/11/23 13:16:22
 *  Author: Alan
 *
 *
 * README:
 * Display is chosen by #defining a USE_DISPLAY_XXX before the #include "WordClockLayouts.h" in your main.c.
 * Then, each display layout must define in a #ifdef USE_DISPLAY_XXX block the mapping of the following calls:
 * - MAP_DATE_TO_DISPLAY(p) where p is a Date*: do the "logic" part of decomposing time into something you will display
 *            i.e. If you use the standard 5x5 matrix you will update the in-memory representation of the time
 * - DRAW_DISPLAY(): how to map the in-memory matrix to your display
 * - SETUP_DISPLAY(): what to do at startup
 * - SET_EDIT_HOURS(): what to do visually when changing hours?
 * - SET_EDIT_MINUTES(): what to do visually when changing minutes?
 * - SET_EDIT_FINISH(): what to do visually when changing time is finished?
 *
 */ 


#ifndef WORDCLOCKLAYOUTS_H_
#define WORDCLOCKLAYOUTS_H_

#include "MCMLedMatrix.h"


#ifdef  USE_DISPLAY_BCD1
	#define MAP_DATE_TO_DISPLAY(p) MapTimeInLedMatrix_BCD1(p)
	#define DRAW_DISPLAY() drawLedMatrix()
	#define SETUP_DISPLAY() { setupLedMatrix(); resetLedMatrix(); }

	#define SET_EDIT_HOURS() 		;
	#define SET_EDIT_MINUTES() 		;
	#define SET_EDIT_FINISH() 		;
#endif
void MapTimeInLedMatrix_BCD1(Date* pD){
  resetLedMatrix();

  /*
   BCD : Binary coded decimal for the geeks among the geeks ... 
   I'm not in this group but making it is a nice POC.
   xx xx xx xx xx
   xx h3 xx xx m3
   xx h2 xx mm m2
   hh h1 xx mm m1 
   hh h0 xx mm m0
   */

  int vH = (*pD).hour;
  //high part of hour
  setCell(3, 0, vH >= 20);
  setCell(4, 0, vH >= 10 && vH < 20);
  //low part of hour
  vH = vH % 10;
  setCell(1, 1, (vH & 0x08) != 0);
  setCell(2, 1, (vH & 0x04) != 0);
  setCell(3, 1, (vH & 0x02) != 0);
  setCell(4, 1, (vH & 0x01) != 0);



  int vM = (*pD).minute / 10;
  //high part of minutes
  setCell(2, 3, (vM & 0x04) != 0);
  setCell(3, 3, (vM & 0x02) != 0);
  setCell(4, 3, (vM & 0x01) != 0);
  //low part of minutes
  vM = (*pD).minute % 10;
  setCell(1, 4, (vM & 0x08) != 0);
  setCell(2, 4, (vM & 0x04) != 0);
  setCell(3, 4, (vM & 0x02) != 0);
  setCell(4, 4, (vM & 0x01) != 0);
}



#endif /* WORDCLOCKLAYOUTS_H_ */