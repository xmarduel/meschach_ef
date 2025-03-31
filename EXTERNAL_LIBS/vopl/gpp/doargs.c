
#include "vopl.h"
#include "gpp.h"

graph	*gp;

int	ngraphs = 0,
	do_legend = 0,
	do_markers = 0,
	do_dash = 0,
	points_only = 0,
	uxscale = 0,
	uyscale = 0,
	uzscale = 0,
	maxpnts = MAXPNTS;

char	*xlabel,
	*ylabel,
	*zlabel,
	*title,
	fontname[100],			/* name of the font we use */
	device[20];			/* name of the device we use */

static	char	*myname;		/* Me */
static	int	fromenv = 0;

FILE	*infile;			/* current input file */

float	wholescale = 1.0;

/*
 * usage
 *
 *	Print a usage message
 */
void
usage(void)
{
	fprintf(stderr, "\nUsage: %s [-X] [-Y] [-x min max] [-y min max] [-S] [-P] [-G] [-l<n>]\n", myname);
	fprintf(stderr,"       [-s s0 sn] [-r<fact>] [-m<n>] [-g<n>] [-p<n>] [-L] [-n]\n");
	fprintf(stderr,"       [-D] [-Mx<n>] [-My<n>] [-a<maxsize>]\n");
	fprintf(stderr,"       [-d<device>] [-f<fontname>] [- or filename]\n\n");

#ifdef PC
	fprintf(stderr,"Press a key for more info....\n");
	getch();
#endif
	fprintf(stderr,"Where:\n");
	fprintf(stderr,"-X, -Y specifies logscaling of x and y axes\n");
	fprintf(stderr,"-x, -y specifies optional min and max  of x and y axes\n");
	fprintf(stderr,"-S uses a cardinal spline fit\n");
	fprintf(stderr,"-P uses a power equation fit\n");
	fprintf(stderr,"-G uses a saturated growth fit\n");
	fprintf(stderr,"-l uses a least squares fit of degree <n>\n");
	fprintf(stderr,"-s uses a 'clamped' spline fit with endslopes s0 and sn\n");
	fprintf(stderr,"-r reduces (enlarges) the plot by fact\n");
	fprintf(stderr,"-m places markers at every <n> data points\n");
	fprintf(stderr,"-g places a grid spaced every <n> data points over the plot\n");
	fprintf(stderr,"-p draws markers only at each <n>th point (ie. no lines)\n");
	fprintf(stderr,"-L draws a legend (if any are provided)\n");
	fprintf(stderr,"-n means don't draw any axis anotation\n");
	fprintf(stderr,"-D means Draw each graph with a different linestyle.\n");
	fprintf(stderr,"-Mx set number of minor tickmarks on X axis (def: 10).\n");
	fprintf(stderr,"-My set number of minor tickmarks on Y axis (def: 10).\n");
	fprintf(stderr,"-Tx set number of major tickmarks on X axis.\n");
	fprintf(stderr,"-Ty set number of major tickmarks on Y axis.\n");
	fprintf(stderr,"-a set max number of points per graph (def: %d).\n", MAXPNTS);
	fprintf(stderr,"-d uses VOGLE device <device>\n");
	fprintf(stderr,"-f uses VOGLE font <fontname>\n");
	fprintf(stderr,"- by itself uses standard input for input\n");
	fprintf(stderr,"filename by itself uses filename for input\n");
	
	exit(1);
}

/*
 * getone
 *
 *	Returns one floating point number form the argument list.
 */
float
getone(
	char	c,
	char	*arg)
{
	float	a;

	if (sscanf(arg, "%f", &a) != 1) {
		fprintf(stderr,"-%c option expects one number\n", c);
		usage();
	}
	return (a);
}

/*
 * gettwo
 *
 *	grabs two floating point numbers from the argument list
 */
static void
gettwo(
	char	**argv,
	char	c,
	float	*a,
	float	*b)
{	
	if (*(argv + 2) != (char *)NULL) {
		if (sscanf(*(++argv), "%f", a) != 1) {
			fprintf(stderr, "-%c option expects two numbers\n", c);
			usage();
		}
		if (sscanf(*(++argv), "%f", b) != 1) {
			fprintf(stderr, "-%c option expects two numbers\n", c);
			usage();
		}
	} else {
		fprintf(stderr, "-%c option expects two numbers\n", c);
		usage();
	}
}

/*
 * doargs
 *
 * 	Interpret command line args.
 */
void
doargs(
	int	argc,
	char	*argv[])
{
	float	a, b;
	char	c;
	int	i, gotinfile = 0;

	if (fromenv) {
		device[0] = '\0';
		fontname[0] = '\0';
	}

#ifdef PC
	if ((myname = rindex(argv[0],'\\')) == NULL)
#else
	if ((myname = rindex(argv[0],'/')) == NULL)
#endif
		myname = argv[0];
	else
		*myname++;

#ifdef PC
	*rindex(myname,'.') = '\0';
#endif

	while (argc > 1) 
		if (*(*++argv) == '-')
			switch(*(*argv+1)) {
			case 'x':
			case 'y':
			case 'z':
				c = *(*argv+1);
				gettwo(argv, c, &a, &b);
				argc -= 3;
				argv += 2;
				range(a, b, c);
				if (c == 'x')
					uxscale = 1;
				else if (c == 'y')
					uyscale = 1;
				else
					uzscale = 1;
				break;
			case 'X': 
				scaling(1, 'x');
				argc--;
				break;
			case 'Y': 
				scaling(1, 'y');
				argc--;
				break;
			case 's':
				fit(CUBIC_SPLINE);
				gettwo(argv, 's', &a, &b);
				endslopes(a, b);
				argc -= 3;
				argv += 2;
				break;
			case 'r':
				if (*(*argv + 2)) {
					a = getone('r', *argv + 2);
					argc--;
				} else {
					a = getone('r', *(++argv));
					argc -= 2;
				}
				/*graphscale(a);*/
				wholescale = a;
				break;
			case 'l':
				fit(LEAST_SQUARE);
				if (*(*argv + 2) != 0)
					degree(atoi(*argv + 2));
				argc--;
				break;
			case 'S':
				fit(CUBIC_SPLINE);
				argc--;
				break;
			case 'P':
				fit(POWER_EQN);
				argc--;
				break;
			case 'G':
				fit(SGR_FIT);
				argc--;
				break;
			case 'm':
				if ((do_markers = atoi(*argv + 2)) <= 0)
					do_markers = 1;

				argc--;
				break;
			case 'p':
				if ((i = atoi(*argv + 2)) <= 0)
					i = 1;

				do_markers = i;
				fit(NO_LINES);
				argc--;
				break;
			case 'a':
				if ((maxpnts = atoi(*argv + 2)) <= 0)
					maxpnts = MAXPNTS;

				argc--;
				break;
			case 'n':
				annotate("", 'x');
				annotate("", 'y');
				annotate("", 'z');
				argc--;
				break;
			case 'D':
				do_dash = 1;
				argc--;
				break;
			case 'M':
				if ((i = atoi(*argv + 3)) < 0)
					i = 0;

				minorticks(i, *(*argv + 2));

				argc--;
				break;
			case 'T':
				if ((i = atoi(*argv + 3)) < 0)
					i = 0;

				tickmarks(i, *(*argv + 2));

				argc--;
				break;
			case 'v':
				/* Print the version of gpp */
				printf("This is version %s of gpp\n", VERSION);
				exit(0);
			case 'g':
				if ((i = atoi(*argv + 2)) <= 0)
					i = 1;

				gridspacing(i);
				argc--;
				break;
			case 'L':
				do_legend = 1;
				argc--;
				break;
			case 'f' :
				if (*(*argv+2)) {
					strcpy(fontname, *argv + 2);
					argc--;
				} else {
					strcpy(fontname, *(++argv));
					argc -= 2;
				}
				break;
			case 'd':
				if (*(*argv + 2)) {
					strcpy(device,*argv + 2);
					argc--;
				} else {
					strcpy(device,*(++argv));
					argc -= 2;
				}
				break;
			case 0:			/* single - read stdin */
				gotinfile = 1;
				infile = stdin;
				argc--;
				break;
			default:
				fprintf(stderr, "Unknown flag: %s\n",*argv);
				usage();
				exit(-1);
			}
		else {   	/* the input file */
			if ((infile = fopen(*argv,"r")) == NULL) {
				fprintf(stderr, "Can't open: %s\n",*argv);
				usage();
				exit(-1);
			} else {
				gotinfile = 1;
				argc--;
			}
		}

	if (!fromenv && !gotinfile) 
		usage();

	if (!device[0]) {
		char	*d, *getenv();
		if (!(d = getenv("GPPDEV")))
			strcpy(device, DEFAULTDEV);
		else
			strcpy(device, d);
	}

}

/*
 * Parse the environment variable GPPENV for other common
 * command line args.
 */
void
doenv(void)
{

	char	*p, *e;
	char	*av[30], *getenv();	/* should be enuf */
	int	ac;

	if ((e = getenv("GPPENV")) == NULL)
		return;

	ac = 1;
	av[0] = (char *)malloc(4);
	strcpy(av[0], "gpp");

	while (*e) {
		for (p = e; *p && *p != ' '; p++)
			;

		av[ac] = (char *)malloc(p - e + 1);
		strncpy(av[ac], e, p - e + 1);
		av[ac][p - e] = 0;
		ac++;

		e = p;
		if (*p)
			e++;
	}

	fromenv = 1;
	doargs(ac, av);
	fromenv = 0;

	while(ac >= 0)
		free(av[ac--]);
}
