
#ifndef PICTURE_H_

#define PICTURE_H_

//#include <color.h>


typedef struct
{
	uint16_t posX;
	uint16_t posY;
	uint16_t with;
	uint16_t height;
	RGB8_t [with] [height];
} Picture;


#endif /* PICTURE_H_ */
