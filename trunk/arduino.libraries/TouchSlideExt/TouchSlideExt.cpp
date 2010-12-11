#include "TouchSlideExt.h"

//Writes a LIST of known images in memory
void ListKnownImages(){
	FLASH_FILE_ENTRY fileEntry;
	uint16_t ii;
	uint32_t lookupLoc;
	
	lookupLoc = (uint32_t)kLookupTablePage * (uint32_t)DATAFLASH_PAGESIZE;

	for (ii=0;ii < kLookupTableMaxEntries; ii++) {
		
		/* Read the file entry out of the flash lookup table */
		dataflash_read_block((unsigned char*)&fileEntry,
		lookupLoc,  
		sizeof(FLASH_FILE_ENTRY));

		/* Check for a file name match */
		if (fileEntry.fileName[0] != 0){
			int vH = ii*10;
			text(ii, (vH / SCREEN_HEIGHT) * 80, vH % SCREEN_HEIGHT);
			text((char*)&fileEntry.fileName[0], (vH / SCREEN_HEIGHT) * 80 +30, vH % SCREEN_HEIGHT);
		}
		/* Next entry */
		lookupLoc+= sizeof(FLASH_FILE_ENTRY);
		
	}
}


//AVI
void imageByIndex(short pIndex, int xLoc, int yLoc){
	FLASH_FILE_ENTRY fileEntry;
	uint16_t ii;
	uint32_t lookupLoc;
	
	lookupLoc = (uint32_t)kLookupTablePage * (uint32_t)DATAFLASH_PAGESIZE;
	lookupLoc+= sizeof(FLASH_FILE_ENTRY) * pIndex;

	// Read the file entry out of the flash lookup table 
	dataflash_read_block((unsigned char*)&fileEntry,
						 lookupLoc,  
						 sizeof(FLASH_FILE_ENTRY));

	// Check for a file name match 
	if (fileEntry.fileName[0] != 0){
		image((PImage)fileEntry.flashOffset, xLoc, yLoc);
	}
}

short mImageByIndex_NextIndex = 0;
void showNextImageByIndex(int pX, int pY){
	imageByIndex (mImageByIndex_NextIndex++, pX, pY);
	
	if (mImageByIndex_NextIndex >= kLookupTableMaxEntries){
		mImageByIndex_NextIndex = 0;
	}
}