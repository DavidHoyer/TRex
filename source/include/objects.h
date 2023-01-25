/**
  *  @file objects.h
  *  @ingroup include
  *  @date 30.12.2022
  *  @author Lukas Roth
  *  @brief Working with objects on the LCD
  */

#ifndef INCLUDE_OBJECTS_H_
#define INCLUDE_OBJECTS_H_

//*********************************************************************
//*** Includes     													***
//*********************************************************************

#include <leguan.h>

//*********************************************************************
//*** Defines     													***
//*********************************************************************

#define TRUE		1
#define FALSE		0
#define BGRA565_COLOR(r, g, b, a) 	{ ((uint8_t)(b)) >> 3, ((uint8_t)(g)) >> 2, ((uint8_t)(r)) >> 3, ((uint8_t)(a)) }

//*********************************************************************
//*** Structs														***
//*********************************************************************

/** @struct pixel
 * 	@brief This struct is used to creat a linked list of boarder pixel
 */
typedef struct pixel
{
	/** X Position of boarder pixel*/
	uint16_t		x;
	/** Y Position of boarder pixel*/
	uint16_t		y;
	/** pointer to next boarder pixel*/
	struct pixel    *next;
}pixel_t;



/** @struct bmp_t
 * 	@brief This struct is used to save all the important data of a game object
 */
typedef struct
{
	/** X Position of BMP*/
	uint16_t		x;
	/** Y Position of BMP*/
	uint16_t		y;
	/** Width of BMP*/
	uint16_t		w;
	/** Height of BMP*/
	uint16_t 		h;
	/** pointer to pixelData Array*/
	unsigned char	(*pixels)[4];
	/** visibility flag*/
	unsigned char	visible;
	/** selected flag*/
	unsigned char	selected;
	/** pointer to head of boarder pixel list*/
	pixel_t			*head;
}bmp_t;



/**
 * @brief Draw funktion for BMP on LCD without transparent pixels
 * @param bmp : Struct with all information about the bmp
 * @param x, y : coordinates where to draw the bmp
 */
void DrawBmp(bmp_t *bmp, const uint16_t x, const uint16_t y);

/**
 * @brief Draw funktion for BMP on LCD with transparent pixels
 * @param bmp : Struct with all information about the bmp
 * @param x,y : coordinates where to draw the bmp
 */
void DrawBmpWithout_A(bmp_t *bmp, const uint16_t x, const uint16_t y);

/**
 * @brief moves an object to a given position
 * @param bmp : Struct with all information about the bmp
 * @param x, y : coordinates where to move the bmp to
 */
void MoveBmp(bmp_t *bmp, const uint16_t x, const uint16_t y);

/**
 * @brief shifts bmp for a given amount of pixels
 * @param bmp : Struct with all information about the bmp
 * @param x, y : amounts of pixels to be moved
 */
void ShiftBmp(bmp_t *bmp, const uint16_t ix, const uint16_t iy);

/**
 * @brief Deletes bmp from LCD, means clear the are of the bmp
 * @param bmp : Struct with all information about the bmp
 */
void DeleteBmp(bmp_t *bmp);

/**
 * @brief clears given area on LCD
 * @param x1, y1 : start coordinates of area
 * @param x2, y2 : end coordinates of area
 */
void LcdClearArea(const uint16_t x1, const uint16_t y1, const uint16_t x2, const uint16_t y2);

/**
 * @brief creates a list of boarder pixels, using the moore-edgefinding algorithm
 * @param bmp : Struct with all information about the bmp
 * @return head of boarder pixel list
 */
pixel_t *GetBoarder (bmp_t bmp);

/**
 * @brief sorts the list of boarder pixel, so they go along the boarder in one direction
 * @param head : head of border pixel list
 * @return head of sorted border pixel list
 */
pixel_t *SortBoarder(pixel_t *head);

/**
 * @brief go's along the boarder and checks for edge pixels
 * @param head : head of sorted boarder pixel list
 * @return head of edge pixel list
 */
pixel_t *CreateEdges(pixel_t *head);

/**
 * @brief converts BGRA_8888 to BGRA_5658 color code
 * @param pixelData :  pixel array with color values
 * @param w, h : width and height of bmp
 */
void ConvertArray(unsigned char pixelData[][4], const uint16_t w, const uint16_t h);

/**
 * @brief creates gradient boarder on the horizontal axis
 * @param x, y :  x and y coordinates of rectangle
 * @param width, height : width and height of rectangle
 * @param clr1, clr 2: start and end color of gradient color
 */
void LCD_Rect_Gradient(uint16_t x, uint16_t y, uint16_t width, uint16_t height, LCD_Color_t clr1, LCD_Color_t clr2);

#endif /* INCLUDE_OBJECTS_H_ */
