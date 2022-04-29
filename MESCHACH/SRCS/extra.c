#include "sparse.h"

#define	MINROWLEN	10

/* sprow_copy2 -- copies r1 into r_out
   -- cannot be done in-situ
   -- type must be SPMAT or SPROW depending on
      whether r_out is a row of a SPMAT structure
      or a SPROW variable
   -- returns r_out */
SPROW	*sprow_copy2(SPROW *r1, SPROW *r_out, int type)
{
   int	idx1, idx_out, len1, len_out;
   row_elt	*elt1, *elt_out;
   
   if ( ! r1 )
     error(E_NULL,"sprow_copy2");
   if ( ! r_out )
     r_out = sprow_get(MINROWLEN);
   if ( r1 == r_out )
     error(E_INSITU,"sprow_copy2");
   
   /* Initialise */
   len1 = r1->len;  len_out = r_out->maxlen;
   idx1 = idx_out = 0;
   elt1 = r1->elt; elt_out = r_out->elt;
   
   while ( idx1 < len1 )
   {
      while ( idx_out >= len_out )
      {   /* r_out is too small */
	 r_out->len = idx_out;
	 r_out = sprow_xpd(r_out,0,type);
	 len_out = r_out->maxlen;
	 elt_out = &(r_out->elt[idx_out]);
      }
      elt_out->col = elt1->col;
      elt_out->val = elt1->val;
      elt1++;	idx1++;
      elt_out++;	idx_out++;
   }
   r_out->len = idx_out;
   
   return r_out;
}


/* sprow_get_val -- gets the j-th column entry of the sparse row r */
extern  double  sprow_get_val(SPROW *r, int j)
{
   int  idx;
   
   if ( ! r )
     error(E_NULL,"sprow_get_val");
   
   idx = sprow_idx(r,j);
   if ( idx >= 0 ) return (r->elt[idx].val);

   return 0.0;
 }




 

