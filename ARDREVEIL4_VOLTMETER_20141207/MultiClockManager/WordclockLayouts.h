/*
 * WordclockLayouts.h
 *
 * Created: 2014/11/23 13:16:22
 *  Author: Alan
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
   BCD : Binary coded decimal for the geeks among hte geeks ... 
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