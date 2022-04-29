
//#include "InpMtx.h"
#include "LinSol.h"

int main(void)
{
   FILE  *fp;

   int    SIZE=10;
   int    FILE_OUTPUT = 1;
   int    DO_PERMUTE  = 1;
   int    X_ERROR;

   int    i;
   double alpha = 1.0;
   int rc;

   InpMtx   *A_NSYM, *A_UPPER, *A_LOWER; /* store full matrix, the upper and lower symmetric matrix */
   DenseMtx *x;
   DenseMtx *y1, *y2, *y3;

   Bridge   *Wrapp1, *Wrapp2, *Wrapp3;

   if ( FILE_OUTPUT )
   {
      fp = fopen("test_spooles_dump.txt","w");
   }
   
   A_NSYM  = InpMtx_new();
   A_UPPER = InpMtx_new();
   A_LOWER = InpMtx_new();

   InpMtx_init(A_NSYM , INPMTX_BY_ROWS, SPOOLES_REAL, 0, 0);
   InpMtx_init(A_UPPER, INPMTX_BY_ROWS, SPOOLES_REAL, 0, 0);
   InpMtx_init(A_LOWER, INPMTX_BY_ROWS, SPOOLES_REAL, 0, 0);
   
   for (i=0; i<SIZE; i++)
   {
      InpMtx_inputRealEntry(A_NSYM ,i,i, 4.0); /* the diagonal */
      InpMtx_inputRealEntry(A_UPPER,i,i, 4.0); /* the diagonal */
      InpMtx_inputRealEntry(A_LOWER,i,i, 4.0); /* the diagonal */
   }
   for (i=0; i<SIZE-1; i++)
   {
      InpMtx_inputRealEntry(A_NSYM ,i,i+1, -1.0 ); /* the upper part */
      InpMtx_inputRealEntry(A_NSYM ,i+1,i, -1.0 ); /* the lower part */

      InpMtx_inputRealEntry(A_UPPER,i,i+1, -1.0 ); /* the upper part */
      
      InpMtx_inputRealEntry(A_LOWER,i+1,i, -1.0 ); /* the lower part */
   }

   /* ---------------------------------------------- */
   if ( FILE_OUTPUT )
   {
      fprintf(fp    ,"\n\n INIT A_NSYM    \n");
      //InpMtx_writeToFormattedFile(A_NSYM, fp);
      InpMtx_writeForHumanEye(A_NSYM, fp);

      fprintf(stdout    ,"\n\n INIT A_UPPER    \n");
      //InpMtx_writeToFormattedFile(A_UPPER, fp);
      InpMtx_writeForHumanEye(A_UPPER, fp);

      fprintf(stdout    ,"\n\n INIT A_LOWER    \n");
      //InpMtx_writeToFormattedFile(A_LOWER, fp);
      InpMtx_writeForHumanEye(A_LOWER, fp);
   }
   else
   {
      fprintf(stdout    ,"\n\n INIT A_NSYM    \n");
      //InpMtx_writeToFormattedFile(A_NSYM, stdout);
      InpMtx_writeForHumanEye(A_NSYM, stdout);

      fprintf(stdout    ,"\n\n INIT A_UPPER    \n");
      //InpMtx_writeToFormattedFile(A_UPPER, stdout);
      InpMtx_writeForHumanEye(A_UPPER, stdout);

      fprintf(stdout    ,"\n\n INIT A_LOWER    \n");
      //InpMtx_writeToFormattedFile(A_LOWER, stdout);
      InpMtx_writeForHumanEye(A_LOWER, stdout);
   }
   /* ---------------------------------------------- */

   
   x = DenseMtx_new();

   DenseMtx_init(x, SPOOLES_REAL, 0,0, SIZE,1 , 1,1);
   DenseMtx_zero(x);
   
   for (i=0; i<SIZE; i++)
   {
      DenseMtx_setRealEntry(x,i,0, (double)((i+1)*(i+1)) );
   }

   /* ---------------------------------------------- */
   if ( FILE_OUTPUT )
   {
      fprintf(fp    ,"\n\n INIT x    \n");
      //DenseMtx_writeToFormattedFile(x, fp);
      DenseMtx_writeForHumanEye(x, fp);
   }
   else
   {
      fprintf(stdout,"\n\n INIT x    \n");
      DenseMtx_writeForHumanEye(x, stdout);
   }
   /* ---------------------------------------------- */

   y1 = DenseMtx_new();
   y2 = DenseMtx_new();
   y3 = DenseMtx_new();

   DenseMtx_init(y1, SPOOLES_REAL, 0,0, SIZE,1 , 1,1);
   DenseMtx_init(y2, SPOOLES_REAL, 0,0, SIZE,1 , 1,1);
   DenseMtx_init(y3, SPOOLES_REAL, 0,0, SIZE,1 , 1,1);

   DenseMtx_zero(y1);
   DenseMtx_zero(y2);
   DenseMtx_zero(y3);

   InpMtx_nonsym_mmm(A_NSYM,y1, &alpha, x); // output is y1
   InpMtx_sym_mmm(A_UPPER,y2, &alpha, x); // output is y2
   InpMtx_sym_mmm(A_LOWER,y3, &alpha, x); // output is y3

   /* ---------------------------------------------- */
   if ( FILE_OUTPUT )
   {
      fprintf(fp    ,"\n\n EVAL y = A.x ---------\n");
      //DenseMtx_writeToFormattedFile(y, fp);
      DenseMtx_writeForHumanEye(y1, fp);
      DenseMtx_writeForHumanEye(y2, fp);
      DenseMtx_writeForHumanEye(y3, fp);
   }
   else
   {
      fprintf(stdout,"\n\n EVAL y = A.x ---------\n");
      DenseMtx_writeForHumanEye(y1, stdout);
      DenseMtx_writeForHumanEye(y2, stdout);
      DenseMtx_writeForHumanEye(y3, stdout);
   }
   /* ---------------------------------------------- */

   //DenseMtx_zero(x);

   fprintf(stdout, "\n\n INIT BRIDGE ----------------- \n");
   fprintf(fp    , "\n\n INIT BRIDGE ----------------- \n");

   Wrapp1 = Bridge_new();
   Wrapp2 = Bridge_new();
   Wrapp3 = Bridge_new();
   
   Bridge_setMatrixParams(Wrapp1, SIZE, SPOOLES_REAL, SPOOLES_NONSYMMETRIC);
   Bridge_setMatrixParams(Wrapp2, SIZE, SPOOLES_REAL, SPOOLES_SYMMETRIC);
   Bridge_setMatrixParams(Wrapp3, SIZE, SPOOLES_REAL, SPOOLES_SYMMETRIC);

   //----------------------------------------------------------------------- first try
// Bridge_setMessageInfo(Wrapp, msg1, msg2);
   rc = Bridge_setup(Wrapp1, A_NSYM);

   rc = Bridge_factor(Wrapp1, A_NSYM, DO_PERMUTE, &X_ERROR);

   rc = Bridge_solve(Wrapp1, DO_PERMUTE, x, y1);

   if ( FILE_OUTPUT )
   {
      fprintf(fp    , "\n\n SOLUTION (should be x) ----------------- \n");
      //DenseMtx_writeToFormattedFile(x, fp);
      DenseMtx_writeForHumanEye(x, fp);
      //DenseMtx_writeToFormattedFile(y, fp);
      DenseMtx_writeForHumanEye(y1, fp);
   }
   else
   {
      fprintf(stdout, "\n\n SOLUTION (should be x) ----------------- \n");
      DenseMtx_writeForHumanEye(x, stdout);
      /*DenseMtx_writeForHumanEye(y1, stdout);*/ /* y1 is changed ( permutated ) */
   }

   //----------------------------------------------------------------------- second try
   // Bridge_setMessageInfo(Wrapp, msg1, msg2);
   rc = Bridge_setup(Wrapp2, A_UPPER);

   rc = Bridge_factor(Wrapp2, A_UPPER, DO_PERMUTE, &X_ERROR);

   rc = Bridge_solve(Wrapp2, DO_PERMUTE, x, y2);

   if ( FILE_OUTPUT )
   {
      fprintf(fp    , "\n\n SOLUTION (should be x) ----------------- \n");
      //DenseMtx_writeToFormattedFile(x, fp);
      DenseMtx_writeForHumanEye(x, fp);
      //DenseMtx_writeToFormattedFile(y2, fp);
      DenseMtx_writeForHumanEye(y2, fp);
   }
   else
   {
      fprintf(stdout, "\n\n SOLUTION (should be x) ----------------- \n");
      DenseMtx_writeForHumanEye(x, stdout);
      /*DenseMtx_writeForHumanEye(y2, stdout);*/ /* y2 is changed ( permutated ) */
   }

   //----------------------------------------------------------------------- third try
   // Bridge_setMessageInfo(Wrapp, msg1, msg2);
   rc = Bridge_setup(Wrapp3, A_LOWER);

   rc = Bridge_factor(Wrapp3, A_LOWER, DO_PERMUTE, &X_ERROR);

   rc = Bridge_solve(Wrapp3, DO_PERMUTE, x, y3);

   if ( FILE_OUTPUT )
   {
      fprintf(fp    , "\n\n SOLUTION (should be x) ----------------- \n");
      //DenseMtx_writeToFormattedFile(x, fp);
      DenseMtx_writeForHumanEye(x, fp);
      //DenseMtx_writeToFormattedFile(y, fp);
      DenseMtx_writeForHumanEye(y3, fp);
   }
   else
   {
      fprintf(stdout, "\n\n SOLUTION (should be x) ----------------- \n");
      DenseMtx_writeForHumanEye(x, stdout);
      /*DenseMtx_writeForHumanEye(y3, stdout);*/ /* y3 is changed ( permutated ) */
   }

   
   fclose(fp);

   return 0;
}