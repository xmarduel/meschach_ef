
/** \file graphics_output.c
 *
 * Contains misc functions used in graphics
 *
 */

#include <time.h>

//#include "meschach_config.h"

#ifdef HAVE_SILO
#include "silo.h"
#endif

#include "MESCHACH_EF/INCLUDES/graphics_output.h" 

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

char* get_date()
{
   time_t clock    = time(NULL);

   return ctime(&clock);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void info_silo()
{
#ifdef HAVE_SILO
   /* Rappel des typedef dans SILO */
   printf("RAPPEL: typedef in SILO librairy: \n");
   printf("DB_FLOAT  = %d \n", DB_FLOAT );
   printf("DB_DOUBLE = %d \n", DB_DOUBLE);
#endif
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/


