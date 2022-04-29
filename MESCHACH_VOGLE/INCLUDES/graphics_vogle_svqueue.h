#ifndef GRAPHICS_VOGLE_SVQUEUE__H
#define GRAPHICS_VOGLE_SVQUEUE__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_vogle_svqueue.h
 *
 * Contains the routines for plotting with VOGLE library
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH_ADDS/INCLUDES/queue.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* access the static stuff */
SVQueue * svqueue_vogle_get_static(void);
SVQueue * svqueue_vogle_set_static(SVQueue* queue);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

/*! \fn svqueue_vogle_get_static ( void );
* 
*
* Return the static queue which has been set with  svqueue_set_static
*/

/*! \fn svqueue_vogle_set_static ( SVQueue* queue  );
* 
* \param queue    : the queue we want to set has static
*
* set the static queue -- used for transient problems --
*/

