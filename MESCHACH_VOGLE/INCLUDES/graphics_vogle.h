
#ifndef GRAPHICS_VOGLE_H
#define GRAPHICS_VOGLE_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_vogle.h
 *
 * Contains the enumerations for the VOGLE library
 * 
 * ----------------------------------------------------------------------- */
  
typedef int vogle_color;


typedef enum
{
   VOGLEe_COLOR_BLACK           =  0 , 
   VOGLEe_COLOR_RED             =  1 , /* default */
   VOGLEe_COLOR_GREEN           =  2 ,
   VOGLEe_COLOR_YELLOW          =  3 ,
   VOGLEe_COLOR_BLUE            =  4 ,
   VOGLEe_COLOR_MAGENTA         =  5 ,
   VOGLEe_COLOR_CYAN            =  6 ,
   VOGLEe_COLOR_WHITE           =  7 ,
   
   VOGLE_NB_COLORS              =  8

} VOGLE_COLOR;


typedef enum
{
   VOGLEe_LINE_STYLE_EMPTY              =  0 , /* no meaning */
   VOGLEe_LINE_STYLE_SOLID              =  1 ,
   VOGLEe_LINE_STYLE_LONG_DASHED        =  2 ,
   VOGLEe_LINE_STYLE_DASHED_DOT         =  3 ,
   VOGLEe_LINE_STYLE_DOTTED             =  4 ,
   VOGLEe_LINE_STYLE_DASHED_DOT_DOT_DOT =  5 ,

   VOGLE_NB_LINE_STYLES =  6

} VOGLE_LINE_STYLE;


typedef enum
{
   VOGLEe_FILL_STYLE_EMPTY         =  0 ,
   VOGLEe_FILL_STYLE_SOLID         =  1 ,
   VOGLEe_FILL_STYLE_OUTLINE       =  2 ,
   VOGLEe_FILL_STYLE_HATCHED       =  3 ,
   VOGLEe_FILL_STYLE_CROSS_HATCHED =  4 ,
   
   VOGLE_NB_FILL_STYLES =  5

} VOGLE_FILL_STYLE;


typedef enum
{
   VOGLEe_NO_MARKER            =  0 ,

   VOGLEe_MARKER_CIRCLE        =  1 ,
   VOGLEe_MARKER_CUBE          =  2 ,
   VOGLEe_MARKER_TRIANGLE      =  3 ,
   VOGLEe_MARKER_LOSANGE       =  4 ,
   VOGLEe_MARKER_CHRISMAS      =  5 ,
   VOGLEe_MARKER_CROSS         =  6 ,
   VOGLEe_MARKER_ADDITION      =  7 ,
   VOGLEe_MARKER_MULTIPICATION =  8 ,
   VOGLEe_MARKER_STAR          =  9 ,
   
   VOGLEe_NB_MARKERS           = 10

} VOGLE_MARKER;



typedef enum
{
   VOGLEe_NO_OUTPUT  = 0,
   VOGLEe_PPM  = 1,
   VOGLEe_GIF  = 2,
   VOGLEe_TIFF = 3,
   VOGLEe_XBM  = 4,
	VOGLEe_PNG  = 5,
   VOGLEe_PPS  = 6

} VOGLE_OUTPUT_FORMAT;


typedef enum
{
   VOGLEe_FONT_ASTROLOGY    ,
   VOGLEe_FONT_CURSIVE      ,
   VOGLEe_FONT_FUTURA_L     ,
   VOGLEe_FONT_FUTURA_M     ,
   VOGLEe_FONT_GOTHIC_ENG   ,
   VOGLEe_FONT_GOTHIC_GER   ,
   VOGLEe_FONT_GOTHIC_ITA   ,
   VOGLEe_FONT_GREEK        ,
   VOGLEe_FONT_JAPANESE     ,
   VOGLEe_FONT_MARKERS      ,
   VOGLEe_FONT_MATH_LOW     ,
   VOGLEe_FONT_MATH_UPP     ,
   VOGLEe_FONT_METEOROLOGY  ,
   VOGLEe_FONT_MUSIC        ,
   VOGLEe_FONT_CYRILLIC     ,
   VOGLEe_FONT_SCRIPT       ,
   VOGLEe_FONT_SYMBOLIC     ,
   VOGLEe_FONT_TIMES_G      ,
   VOGLEe_FONT_TIMES_IB     ,
   VOGLEe_FONT_TIMES_I      ,
   VOGLEe_FONT_TIMES_R      ,
   VOGLEe_FONT_TIMES_RB     ,

} VOGLE_FONT;


#define  VOGLEd_IDX_FIRST  (-1)   /**< indicates the range of indices where a vector must be plotted */
#define  VOGLEd_IDX_LAST   (-1)   /**< indicates the range of indices where a vector must be plotted */


#ifdef __cplusplus
}
#endif


#endif

/*! \enum VOGLE_COLOR
* \brief List all the colors used in the VOGLE library
*/

/*! \enum VOGLE_LINE_STYLE
* \brief List all the line-styles used in the VOGLE library
*/

/*! \enum VOGLE_FILL_STYLE
* \brief List all the fill-styles used in the VOGLE library
*/


/*! \enum VOGLE_MARKER
* \brief List all the type of markers used in the VOGLE library
*/


/*! \enum VOGLE_OUTPUT_FORMAT
* \brief List all the fonts used in the VOGLE library
*/


/*! \enum VOGLE_FONT
* \brief List all the output formats used in the VOGLE library
*/


