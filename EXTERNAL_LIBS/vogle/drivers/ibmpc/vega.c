#include	"vogle.h"

/*
 * Vogle driver for cga/ega/vga
 *
 * Note, vga can't do double buffering by page swapping...`
 */

typedef struct {
	unsigned long	localid;
	int		old_mode;
	int		curcol;
} Vegadev;

static Vegadev	*vega = NULL;

#define C_PIX_ASPECT    2.4
#define E_PIX_ASPECT    1.3
#define	V_PIX_ASPECT	1.0

/*
 * Functions
 */
static int vega_draw(int x, int y);
static int vega_pnt(int x, int y);
static int pc_getkey(void);
static int pc_checkkey(void);
static int pc_locator(int *wx, int *wy);
static int vega_clear(void);
static int vega_color(int ind);
static int vega_mapcolor(int i, int r, int g, int b);
static int vega_font(const char *fontfile);
static int vega_char(char c);
static int vega_string(const char *s);
static int pc_fill(int n, int x[], int y[]);
static int vega_backbuf(void *p, int old, int bw, int bh);
static int vega_swapbuf(void);
static int vega_frontbuf(void);
static void vega_sync(void);
static int vega_setlw(int w);
static int vega_setls(int lss);
static void * vega_winopen(const char *dev, const char *title, int id);
static int vega_winset(void *devwin);
static int vega_winclose(void *devwin);
static int vega_windel(void *devwin);
static unsigned long vega_getevent(Vevent *vev);

extern void	vega_setpal(void),
extern int	setmode(int);
extern void	pc_locinit(int xs, int ys);
extern void	zsetup(void);

void _vega_devcpy(char *dev);


void *
vga_winopen(const char *dev, const char *title, init id)
{
	vega = (Vegadev *)vallocate(sizeof(Vegadev));
	memset(vega, 0, sizeof(Vegadev));

	vega->localid = vegalocalid++;
	vega->id = id;

	if (strcmp(dev, "ega") == 0) {
		vega->old_mode = setmode(16);
		vdevice.dev.sizeX = 349 * E_PIX_ASPECT;
		vdevice.dev.sizeY = 349;
		vdevice.dev.sizeSx = 640;
		vdevice.dev.sizeSy = 349;
	} else if (strcmp(dev, "vga") == 0) {
		vega->old_mode = setmode(18);
		vdevice.dev.sizeX = 479 * V_PIX_ASPECT;
		vdevice.dev.sizeY = 479;
		vdevice.dev.sizeSx = 640;
		vdevice.dev.sizeSy = 479;
	}

	vdevice.dev.depth = 4;

	_buffer_segment = 0xA000;
	_buffer_offset = 0;
	pc_locinit(vdevice.sizeSx, vdevice.sizeSy);
	zsetup();
	vega_setpal();

	return ((void *)vega);
}

int
vega_winset(void *p)
{
	vega = (Vegadev *)p;

	return(vega->id);
}

int
vega_winclode(void *p)
{
	Vegadev *v = (Vegadev *)p;

	/* Nothing */

	return(v->id);


/*
 * vega_windel
 *
 *	Sets the display back to text mode.
 */
int
vega_windel(void *p)
{
	Vegadev *v = (Vegadev *)p;

	(void)setmode(v->old_mode);

	free(v);

	return (1);
}

int
vega_draw(int x, int y)
{
        egaline(vdevice.cpVx, vdevice.dev.sizeSy - vdevice.cpVy, x, vdevice.dev.sizeSy - y, vega->curcol);
        vdevice.cpVx = x;
        vdevice.cpVy = y;
}


static DevEntry vgadev = {
	"vga",
	"large",
	"small",
	4, 0, 0, 0, 0,
	noop,
	vega_char,
	pc_checkkey,
	vega_clear,
	vega_color,
	vega_draw,
	vga_exit,
	pc_fill,
	vega_font,
	noop,
	pc_getkey,
	vga_init,
	pc_locator,
	noop,
	noop,
	vega_string,
	noop,
	noop
};

/*
 * _vga_devcpy
 *
 *	copy the pc device into vdevice.dev.
 */
_vga_devcpy()
{
	vdevice.dev = vgadev;
}

