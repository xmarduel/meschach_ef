/*

                           Copyright 1991 - 2000
                The Regents of the University of California.
                            All rights reserved.

This work was produced at the University of California, Lawrence
Livermore National Laboratory (UC LLNL) under contract no.  W-7405-ENG-48
(Contract 48) between the U.S. Department of Energy (DOE) and The Regents
of the University of California (University) for the operation of UC LLNL.
Copyright is reserved to the University for purposes of controlled
dissemination, commercialization through formal licensing, or other
disposition under terms of Contract 48; DOE policies, regulations and
orders; and U.S. statutes.  The rights of the Federal Government are
reserved under Contract 48 subject to the restrictions agreed upon by
DOE and University.

                                DISCLAIMER

This software was prepared as an account of work sponsored by an agency
of the United States Government. Neither the United States Government
nor the University of California nor any of their employees, makes any
warranty, express or implied, or assumes any liability or responsiblity
for the accuracy, completeness, or usefullness of any information,
apparatus, product, or process disclosed, or represents that its use
would not infringe privately owned rights. Reference herein to any
specific commercial products, process, or service by trade name, trademark,
manufacturer, or otherwise, does not necessarily constitute or imply its
endorsement, recommendation, or favoring by the United States Government
or the University of California. The views and opinions of authors
expressed herein do not necessarily state or reflect those of the United
States Government or the University of California, and shall not be used
for advertising or product endorsement purposes.

*/

#include <stdlib.h>     /* For exit()   */
#include <string.h>     /* For strcmp() */
#include "silo.h"

static void build_ucd(DBfile *dbfile, char *name);


/*-------------------------------------------------------------------------
 * Function:	main
 *
 * Purpose:	
 *
 * Return:	0
 *
 * Programmer:	
 *
 * Modifications:
 * 	Robb Matzke, 1999-04-09
 *	Added argument parsing to control the driver which is used.
 *
 *  Lisa J. Roberts, Fri Apr  7 10:40:49 PDT 2000
 *  Removed db and name, which were unused.
 *
 *-------------------------------------------------------------------------
 */
int
main(int argc, char *argv[])
{
    DBfile        *dbfile;
    int		i, driver = DB_PDB;
    char	*filename = "ucd.pdb";

    DBShowErrors(DB_ALL, NULL);

    for (i=1; i<argc; i++) {
	if (!strcmp(argv[i], "DB_PDB")) {
	    driver = DB_PDB;
	    filename = "ucd.pdb";
	} else if (!strcmp(argv[i], "DB_HDF5")) {
	    driver = DB_HDF5;
	    filename = "ucd.h5";
	} else if (!strcmp(argv[i], "DB_DMF")) {
	    driver = DB_DMF;
	    filename = "ucd.dmf";
	} else {
	    fprintf(stderr, "%s: ignored argument `%s'\n", argv[0], argv[i]);
	}
    }
    
    dbfile = DBCreate(filename, 0, DB_LOCAL, "ucd test file", driver);
    printf("Creating file: '%s'...\n", filename);
    build_ucd(dbfile, "ucdmesh");
    DBClose(dbfile);

    return 0;
}

/*----------------------------------------------------------------------
 *  Routine                                                   build_ucd
 *
 *  Purpose
 *
 *      Build ucd-mesh, ucd-var, facelist and zonelist, and return
 *      the mesh ID.
 *
 * Modifications
 *
 *     Lisa J. Roberts, Fri Apr  7 10:41:47 PDT 2000
 *     Removed dbid, dfnew, flid, zlid, meshid, varid, and matid, which
 *     were unused.
 *
 *--------------------------------------------------------------------*/
static void
build_ucd(DBfile *dbfile, char *name)
{

#define NZONES  1
#define NFACES  3
#define NNODES  6
#define NZSHAPES 1
#define NFSHAPES 1
#define LZNODELIST 6
#define LFNODELIST 9

   /*----------------------------------------------------------------------
       The test mesh looks like this:

        3.      |       0       5       4
                |
        2.      |       1       3       x
                |
        1.      |       2       x       x
                |
                |       x       x       x
        0.      ---------------------------

                        9.      10.      11.
     *---------------------------------------------------------------------*/

    int            i;

   int rc;
   DBoptlist *opt_list;
   void *pvalue;
   int value = DB_CURVILINEAR;
   
    /* OTHER model for material data */

    static float   x[NNODES] = {9., 9., 9., 10., 11., 10. };
    static float   y[NNODES] = {3., 2.,  1., 2.,  3.,  3. };

    static float   u[NNODES] = {4., 1., 0., 1., 4., 4.};
    static float   v[NNODES] = {0., .5, 1., 0., .5, 1.};

    static float   d[NZONES] = {0};

    static int     fnodelist[LFNODELIST] = {0, 1, 2, 2, 3, 4, 4, 5, 0 };
    static int     znodelist[LZNODELIST] = {0, 1, 2, 3, 4, 5 };

    int            fshapesize, fshapecnt, zshapesize, zshapecnt;
    int            idatatype;
    int            dims;


    float         *coords[3], *vars[2];
    char          *coordnames[3], *varnames[2];

    pvalue = &value;

   fshapesize = 3;
    fshapecnt = NFACES;
    zshapesize = 6;
    zshapecnt = NZONES;

    idatatype = DB_FLOAT;

    coords[0] = x;
    coords[1] = y;
    coordnames[0] = "X";
    coordnames[1] = "Y";
    vars[0] = d;
    varnames[0] = "d";

    opt_list = DBMakeOptlist(1);
    rc = DBAddOption(opt_list, DBOPT_FACETYPE, pvalue);
    
    (void)DBPutFacelist(dbfile, "fl", NFACES, 2, fnodelist, LFNODELIST, 0,
                        NULL, &fshapesize, &fshapecnt, NFSHAPES,
                        NULL, NULL, 0);

    (void)DBPutZonelist(dbfile, "zl", NZONES, 2, znodelist, LZNODELIST, 0,
                        &zshapesize, &zshapecnt, NZSHAPES);

    (void)DBPutUcdmesh(dbfile, name, 2, coordnames, coords,
                       NNODES, NZONES, "zl", NULL, DB_FLOAT, opt_list);


    vars[0] = u;
    varnames[0] = "u";

    (void)DBPutUcdvar(dbfile, varnames[0], name, 1, varnames, vars, NNODES,
                      NULL, 0, idatatype, DB_NODECENT, opt_list);

    vars[0] = v;
    varnames[0] = "v";

    (void)DBPutUcdvar(dbfile, varnames[0], name, 1, varnames, vars, NNODES,
                      NULL, 0, idatatype, DB_NODECENT, opt_list);

    vars[0] = u;
    varnames[0] = "ucomp";
    vars[1] = v;
    varnames[1] = "vcomp";

    (void)DBPutUcdvar(dbfile, "velocity", name, 2, varnames, vars, NNODES,
                      NULL, 0, idatatype, DB_NODECENT, opt_list);

    dims = NZONES;


    DBFreeOptlist(opt_list);
}
