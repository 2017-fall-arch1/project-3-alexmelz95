/** \file lcddraw.h
 *  \brief Adapted from RobG's EduKit
 */

#ifndef lcddraw_included
#define lcddraw_included

/** Draw single pixel at x,row
 *
 *  \param col Column to draw to
 *  \param row Row to draw to
 *  \param colorBGR Color of pixel in BGR
 */
void drawPixel(unsigned char col, unsigned char row, unsigned int colorBGR);

/** Fill rectangle
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void fillRectangle(unsigned char colMin, unsigned char rowMin, unsigned char width, unsigned char height, unsigned int colorBGR);

/** Clear screen (fill with color)
 *
 *  \param colorBGR The color to fill screen
 */
void clearScreen(unsigned int colorBGR);

/** Draw string at col,row
 *  Type:
 *  FONT_SM - small (5x8,) FONT_MD - medium (8x12,) FONT_LG - large (11x16)
 *  FONT_SM_BKG, FONT_MD_BKG, FONT_LG_BKG - as above, but with background color
 *  Adapted from RobG's EduKit
 *
 *  \param col Column to start drawing string
 *  \param row Row to start drawing string
 *  \param string The string
 *  \param fgColorBGR Foreground color in BGR
 *  \param bgColorBGR Background color in BGR
 */
void drawString5x7(unsigned char col, unsigned char row, char *string,
		   unsigned int fgColorBGR, unsigned int bgColorBGR);

/** 5x7 font - this function draws background pixels
 *  Adapted from RobG's EduKit
 */
void drawChar5x7(unsigned char col, unsigned char row, char c,
		 unsigned int fgColorBGR, unsigned int bgColorBGR);

/** Draw rectangle outline
 *
 *  \param colMin Column start
 *  \param rowMin Row start
 *  \param width Width of rectangle
 *  \param height Height of rectangle
 *  \param colorBGR Color of rectangle in BGR
 */
void drawRectOutline(unsigned char colMin, unsigned char rowMin, unsigned char width, unsigned char height,
		     unsigned int colorBGR);
#endif // included
