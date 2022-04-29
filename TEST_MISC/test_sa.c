
#include <stdlib.h>


int toto_error()
{
    return 0; /* or whatever eg longjump .... */
}

/* toto definition */
typedef	struct	{
    int	dim;
} TOTO;


int main(int argc, char **argv)
{
    TOTO *m1 = malloc(sizeof(TOTO));
    TOTO *m2 = malloc(sizeof(TOTO));
    TOTO *m3 = malloc(sizeof(TOTO));
    
    TOTO *m4 = NULL;
    TOTO *m5 = malloc(sizeof(TOTO));
    
    if (m1 == NULL) 
    {
        if (!toto_error()) return 0;
    }
    
    if (m2 == NULL) 
    {
        ; /* nothing */
    }
    
    if (m3 == NULL) 
    {
        return 0;
    }
    
    m1->dim = 3; /* m1 is checked if NULL and there is a wrong warning : 
    the static analyser is unable to analyse how the programm handles m1 == NULL */
    
    /* Access to field 'dim' results in a dereference of a null pointer (loaded from variable 'm1') */
    
    m2->dim = 3; /* m2 is checked if NULL and there is a good warning because of no return stmt */
    m3->dim = 3; /* m3 is checked if NULL and there is no warning (ok) because of the return stmt */

    m4->dim = 3; /* m4 is checked if NULL and there is a good warning (ok) */
    m5->dim = 3; /* m5 is checked if NULL and there is no warning (ok) */

    return 0;
}
