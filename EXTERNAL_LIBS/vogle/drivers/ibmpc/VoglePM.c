/****************************************************************************

    VoglePM.C - A VOGLE driver for OS/2 Presentation Manager
    Version 1.0  (4/17/94)
    (C) Copyright Claudio Fahey, 1994

    e-mail: claudio@uclink.berkeley.edu

    Permission granted to use, distribute, and modify, provided that this
    notice remains intact.  If you distribute a modified version, you must
    identify your modifications as such.

****************************************************************************/

/****************************************************************************

    COMMENTS:

    This driver uses two threads.  The VOGLE application (the main() 
    function), is the primary thread.  When it calls PM_init(), a second 
    thread is started which deals with the PM window.  This allows the PM 
    window to always be responsive, regardless of the status of the primary 
    thread.  Unlike the MS-Windows driver, this driver won't lockup the system 
    message queue if the primary thread doesn't call any of the PM_xxx 
    functions for a long period of time.  Also, (and this is a very important 
    point) the primary thread will continue to run and draw on the screen when 
    the 2nd thread is showing the "About" window.  

    When the user closes the window using the System menu (the one in the 
    upper left), it suspends and eventually kills the primary thread right 
    where it is.  It doesn't wait until the primary thread calls a PM_xxx 
    function like the MS-Windows driver does.  If you're running a program 
    that shouldn't be killed at random points, then make sure you don't close 
    it through the System menu (or the Window List).

    This driver can be closed and restarted by the VOGLE application as many 
    times as needed, so printing to other drivers is supported.  However, the 
    sample programs that print and then restart this driver (such as licosa 
    and cube) don't work correctly in the new window.  This is because they 
    perform their initialization (call backbuffer, set colors, etc.)  only 
    once.  When the PM device is restarted, all these settings are lost.  I 
    consider this a bug in the example programs so unless someone really needs 
    this to work, I won't change this behavior.

    When the window is resized, the VOGLE device structure is updated with the
    new window size.  However, VOGLE still clips itself to the original size.  
    Unless someone wants me to fix this, I probably won't fix it anytime soon.  

    This driver uses a retained graphics segment to repaint itself on a 
    WM_PAINT message.  To optimize double buffered animation, drawing in the 
    back (Memory) buffer isn't retained.  Only drawing to the front (Screen) 
    buffer is retained.  This isn't a problem for most animations, such as 
    licosa and lcube, because the screen is constantly updated with 
    PM_swapbuf.  An exception to this is the teapot example program because it 
    only updates the screen after the user presses a key.  

    For details about how this driver works, just look at the code.  It is 
    heavily commented so you should be able to easily follow it (at least I 
    can :-) ).

****************************************************************************/

#define INCL_GPI
#define INCL_WIN
#define INCL_DOS
#include <os2.h>
#include <stdlib.h>         // for min()
#include <string.h>         // for strlen()
#include "vogle.h"


// Resource ID's
#define RID_FRAME       1
#define RID_ABOUTDLG    2

// Command ID's
#define ID_ABOUT        1L

#define ID_SCREEN_SEG   1L

#define MAX_POLY        50      // Largest number of lines in a filled polygon

// Vogle interface functions

int PM_init( void );
int PM_frontbuf( void );
int PM_backbuf( void );
int PM_swapbuf( void );
int PM_sync( void );
int PM_locator( int *x, int *y );
int PM_string( char *szString );
int PM_char( char c );
int PM_setlw( int width );
int PM_fill( int n, int *x, int *y );
int PM_draw( int x, int y );
int PM_color( int color );
int PM_clear( void );
int PM_font( char *font );
int PM_getkey( void );
int PM_checkkey( void );
int PM_exit( void );
int PM_verror( char *szString );
int _PM_devcpy( void );

// PM user interface functions
static MRESULT EXPENTRY ClientWinProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
static void PMMain( void );

// Global variables
static HAB hab;             // PM UI hab
static HAB habPrim;         // vogle app (primary thread) hab
static HMQ hmqPrim;         // vogle app (primary thread) message queue
static HWND hwndFrame, hwndClient;
static HDC hdcMemory;
static HPS hpsMemory;       // Presentation space handle for the back buffer
static HDC hdcScreen;
static HPS hpsScreen;       // Presentation space handle for the screen
static HPS hps;             // Current PS (hpsMemory or hpsScreen)
static POINTL aptlBitBlt[3] = { 0, 0, 0, 0, 0, 0 };
                            // Array used by GpiBitBlt
static int NextKey;         // Key "buffer" - 0 when buffer is empty
static BOOL flDontKillPrim; // TRUE when the 2nd thread shouldn't kill the
                            // primary thread when closing the PM window
static BOOL flSuspendPrim;  // TRUE when it's OK to suspend the primary thread
                            // or when the primary thread should suspend itself
                            // (see PM_verror)

// Semaphores
static HEV hevChar;         // posted when client user types a key
static HEV hevPMReady;      // posted when PM window is ready for vogle app
static HEV hevPMDone;       // posted when PM window has been completely closed
static HMTX hmtxPS;         // mutex semaphore for both PS's

// Macros for controlling access to the PS's from the two threads.
// The 2nd thread must use the PS's when it receives WM_PAINT or WM_SIZE.
#define REQUESTPS()     DosRequestMutexSem( hmtxPS, SEM_INDEFINITE_WAIT )
#define RELEASEPS()     DosReleaseMutexSem( hmtxPS )

// *** Functions ***

// Drawing functions

// PM_frontbuf and PM_backbuf don't need to request the PS because they
// only change hps which is only used by the primary thread

int PM_frontbuf( void )
    {
    hps = hpsScreen;
    return 0;
    }

int PM_backbuf( void )
    {
    hps = hpsMemory;
    return 0;
    }

// Copy Memory to Screen
int PM_swapbuf( void )
    {
    REQUESTPS();

    // The screen will be "redrawn" so restart the screen segment
    GpiDeleteSegment( hpsScreen, ID_SCREEN_SEG );
    GpiOpenSegment( hpsScreen, ID_SCREEN_SEG );

    // Do the actual copying to the screen
    GpiBitBlt( hpsScreen, hpsMemory, 3L, aptlBitBlt, ROP_SRCCOPY, BBO_IGNORE );

    RELEASEPS();
    return 0;
    }

int PM_clear( void )
    {
    // Set background color to foreground color
    REQUESTPS();
    GpiSetBackColor( hps, GpiQueryColor( hps ) );
    GpiErase( hps );
    if ( hps == hpsScreen )
        {
        // If the screen was cleared, we have to restart the graphics segment
        GpiDeleteSegment( hps, ID_SCREEN_SEG );
        GpiOpenSegment( hps, ID_SCREEN_SEG );
        }
    RELEASEPS();
    return 0;
    }

int PM_string( char *szString )
    {
    POINTL ptl;

    REQUESTPS();
    ptl.x = vdevice.cpVx;
    ptl.y = vdevice.cpVy;
    GpiMove( hps, & ptl);
    GpiCharString( hps, strlen(szString), szString );
    RELEASEPS();
    return 0;
    }

int PM_char( char c )
    {
    char szString[] = " ";

    szString[0] = c;
    return PM_string( szString );
    }

int PM_setlw( int width )
    {
    REQUESTPS();
    if ( width == 0 )   // thin
        GpiSetLineWidth( hps, LINEWIDTH_NORMAL );
    else
        GpiSetLineWidth( hps, LINEWIDTH_THICK );
    RELEASEPS();
    return 0;
    }

int PM_fill( int n, int *x, int *y )
    {
    POINTL aptl[MAX_POLY];
    POINTL ptl;
    POLYGON polygon;
    int i;

    polygon.ulPoints = n - 1;
    polygon.aPointl = aptl;

    ptl.x = x[0];
    ptl.y = y[0];

    for ( i = 0 ; i < polygon.ulPoints && i < MAX_POLY ; i++ )
        {
        aptl[i].x = x[i + 1];
        aptl[i].y = y[i + 1];
        }

    REQUESTPS();
    GpiMove( hps, &ptl );
    GpiPolygons( hps, 1L, &polygon, 0L, 0L );
    RELEASEPS();

    return 0;
    }

int PM_draw( int x, int y )
    {
    POINTL ptl;

    REQUESTPS();
    ptl.x = vdevice.cpVx;
    ptl.y = vdevice.cpVy;
    GpiMove( hps, & ptl);
    ptl.x = x;
    ptl.y = y;
    GpiLine( hps, &ptl );
    RELEASEPS();
    return 0;
    }

int PM_color( int color )
    {
    REQUESTPS();
    GpiSetColor( hps, (LONG) color );
    RELEASEPS();
    return 0;
    }

int PM_mapcolor( int i, int r, int g, int b )
    {
    LONG lColor[2];

    lColor[0] = i;
    lColor[1] = r*65536 + g*256 + b;

    REQUESTPS();
    // I assume the user would want to set the colors of both buffers
    GpiCreateLogColorTable( hpsMemory, LCOL_PURECOLOR, LCOLF_INDRGB, 0L, 2, lColor );
    GpiCreateLogColorTable( hpsScreen, LCOL_PURECOLOR, LCOLF_INDRGB, 0L, 2, lColor );
    RELEASEPS();
    return 0;
    }

int PM_font( char *font )
    {
    // Not implemented, yet
    return 1;
    }

// Other vogle interface functions (non-drawing functions)

int PM_sync( void )
    {
    return -1;
    }

int PM_locator( int *x, int *y )
    {
    POINTL ptl;
    int iButtons = 0;

    // Get mouse position and convert to client coord. */
    WinQueryPointerPos( HWND_DESKTOP, &ptl );
    WinMapWindowPoints( HWND_DESKTOP, hwndClient, (PPOINTL) &ptl, 1 );
    *x = ptl.x;
    *y = ptl.y;

    if ( WinGetKeyState( HWND_DESKTOP, VK_BUTTON1 ) & 0x8000 )
        iButtons |= 1;  // 001
    if ( WinGetKeyState( HWND_DESKTOP, VK_BUTTON2 ) & 0x8000 )
        iButtons |= 2;  // 010
    if ( WinGetKeyState( HWND_DESKTOP, VK_BUTTON3 ) & 0x8000 )
        iButtons |= 4;  // 100

    return iButtons;
    }

int PM_getkey( void )
    {
    int Key;
    ULONG cPost;

    // Wait for a semaphore.
    //    If user presses a key, then WM_CHAR will set NextKey to ascii value
    //    and then post the semaphore.

    DosWaitEventSem( hevChar, SEM_INDEFINITE_WAIT );
    // Reset the sem because we are clearing the buffer
    DosResetEventSem( hevChar, &cPost );
    Key = NextKey;
    NextKey = 0;                // Clear the "buffer"
    return Key;
    }

int PM_checkkey( void )
    {
    ULONG cPost;
    int Key;

    // Reset the sem because we are clearing the buffer
    DosResetEventSem( hevChar, &cPost );
    Key = NextKey;
    NextKey = 0;                // Clear the "buffer"
    return Key;
    }

int PM_verror( char *szString )
    {
    // Tell 2nd thread to not suspend this thread
    flSuspendPrim = FALSE;

    if ( ! hmqPrim || ! habPrim )
        {
        // The message queue hasn't been created yet so we'll have to create it
        // here before we give the user the error message.
        habPrim = WinInitialize( 0L );
        hmqPrim = WinCreateMsgQueue( habPrim, 0 );
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP,  szString, "A VOGLE error has occurred",
            0, MB_OK | MB_MOVEABLE | MB_ERROR );
        WinDestroyMsgQueue( hmqPrim );
        WinTerminate( habPrim );
        hmqPrim = 0L;
        habPrim = 0L;
        }
    else
        {
        // I can't make hwndClient the owner because if it's destroyed while in
        // WinMessageBox, the program will crash (and possibly take OS/2 down
        // with it).
        WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, szString, "A VOGLE error has occurred",
            0, MB_OK | MB_MOVEABLE | MB_ERROR );
        }

    if ( flSuspendPrim )
        // If primary thread wanted to suspend this thread, then we'll
        // suspend it now
        DosSuspendThread( 1 );
    else
        // Tell 2nd thread that it's ok to suspend this thread
        flSuspendPrim = TRUE;

    return 0;
    }

int PM_exit( void )
    {
    // Tell 2nd thread to not kill primary thread when exiting
    flDontKillPrim = TRUE;

    WinPostMsg( hwndClient, WM_QUIT, 0L, 0L );
    WinDestroyMsgQueue( hmqPrim );
    WinTerminate( habPrim );

    // Set pointers to null so PM_verror knows that a message queue doesn't exist
    hmqPrim = 0L;
    habPrim = 0L;

    DosWaitEventSem( hevPMDone, SEM_INDEFINITE_WAIT );
    DosCloseEventSem( hevPMDone );

    return 1;
    }

// This is the first function that is called by the vogle program.
// This may be called several times from the same program (after a PM_exit).
// Note: PM_verror may also be the first function.
int PM_init( void )
    {
    TID tid;

    // Initialize PM usage for this primary thread
    habPrim = WinInitialize( 0L );
    hmqPrim = WinCreateMsgQueue( habPrim, 0 );

    // Initialize the ready and done semaphore and set them
    DosCreateEventSem( 0L, &hevPMReady, 0L, FALSE );
    DosCreateEventSem( 0L, &hevPMDone, 0L, FALSE );

    // Tell 2nd thread to kill primary thread if 2nd thread exits
    flDontKillPrim = FALSE;

    // Tell 2nd thread that it's ok to suspend the primary thread
    flSuspendPrim = TRUE;

    // Start the PM user interface thread. It doesn't use any C library
    // functions so _beginthread doesn't need to be used.
    DosCreateThread( &tid, (PFNTHREAD) PMMain, 0, 0, 8192 );

    // Wait for the PM UI process to startup completely
    DosWaitEventSem( hevPMReady, SEM_INDEFINITE_WAIT );
    DosCloseEventSem( hevPMReady );

    return 1;
    }

// PM user interface functions

static void PMMain( void )
    {
    HMQ hmq;
    QMSG qmsg;
    ULONG flCreateFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER |
                          FCF_MINMAX | FCF_TASKLIST | FCF_ICON;
    static CHAR szClientClass[] = "Client Window";

    hab = WinInitialize( 0L );
    hmq = WinCreateMsgQueue( hab, 0 );

    WinRegisterClass( hab, szClientClass, ClientWinProc, CS_SIZEREDRAW, 0 );

    // Create the client; this will *send* WM_CREATE
    hwndFrame = WinCreateStdWindow( HWND_DESKTOP, WS_VISIBLE, &flCreateFlags,
            szClientClass, "Vogle PM", 0L, 0L, RID_FRAME, &hwndClient );

    // *** Now I'm ready for the vogle app ***
    // Post the ready semaphore to let PM_init continue
    DosPostEventSem( hevPMReady );

    while ( WinGetMsg( hab, &qmsg, 0L, 0, 0 ) )
        WinDispatchMsg( hab, &qmsg );

    // If the window was closed by this thread ...
    // Suspend the primary thread so it doesn't use semaphores that will be closed,
    // PS's that will be destroyed, or hwndClient which will be set to null.
    // We can't kill it now because that will kill the whole process.
    if ( ! flDontKillPrim )
        {
        if ( flSuspendPrim )
            DosSuspendThread( 1 );
        else
            // If the primary thread can't be suspended now (ie: if it's handling
            // the message queue), then tell it to suspend itself when it's done.
            // Currently, this will only happen if the program is closed when
            // PM_verror is running (showing the error message).
            flSuspendPrim = TRUE;
        }

    // At this point, the primary thread is no longer running (unless it is
    // displaying an error message, in which case it'll suspend itself when
    // it's done with the message).  It is either suspended or it is waiting
    // for the PMDone semaphore.

    // Let PM_verror know that the client window is no longer usable
    hwndClient = 0L;

    WinDestroyWindow( hwndFrame );
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    // If the window was closed by this thread then the whole process
    // should be killed.
    // If the window was closed by the vogle app, then I should post a
    // semaphore, telling PM_exit to return to the vogle app.
    if ( ! flDontKillPrim )
        DosExit( 1, 99 );   // Kill process and return error level 99
    else
        DosPostEventSem( hevPMDone );
    }

static MRESULT EXPENTRY ClientWinProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
    {
    static HBITMAP hbm = 0L;
    HPS hpsPaint;
    HBITMAP hbmOld;
    SIZEL sizl = {0,0};
    LONG lFormats[24];
    BITMAPINFO2 bmInfo;
    DEVOPENSTRUC dop = {0L, "DISPLAY", NULL, 0L, 0L, 0L, 0L, 0L, 0L};
    BOOL flRc;
    RECTL rcl;
    SWP swpDefault;
    int x, y, cx, cy, cxFrame, cyFrame, cxDefault, cyDefault;
    APIRET rc;
    LONG alDefaultColors[8] = { RGB_BLACK, RGB_RED, RGB_GREEN, RGB_YELLOW,
                                RGB_BLUE, RGB_PINK, RGB_CYAN, RGB_WHITE };
    HWND hwndSysMenu;
    MENUITEM MenuItem = { MIT_END, MIS_TEXT, 0L, ID_ABOUT, 0L, 1L };

    switch( msg )
        {
        case WM_CHAR:
            if ( ! (SHORT1FROMMP(mp1) & KC_KEYUP) && (SHORT1FROMMP(mp1) & KC_CHAR) )
                {
                NextKey = SHORT1FROMMP(mp2);    // Put key in our "buffer"
                DosPostEventSem( hevChar );     // Tell PM_getkey to continue
                }
            else if ( ! (SHORT1FROMMP(mp1) & KC_KEYUP) && (SHORT1FROMMP(mp1) & KC_VIRTUALKEY) )
                {
                if ( SHORT2FROMMP(mp2) == VK_ESC )
                    {
                    NextKey = 27;                   // Put key in our "buffer"
                    DosPostEventSem( hevChar );     // Tell PM_getkey to continue
                    }
                }
            break;

        case WM_SIZE:
            REQUESTPS();

            cx = SHORT1FROMMP(mp2);
            cy = SHORT2FROMMP(mp2);

            // Create and use a device compatible bitmap for double buffering

            GpiQueryDeviceBitmapFormats( hpsScreen, 24L, lFormats );
            bmInfo.cbFix = 16L;
            bmInfo.cPlanes = (USHORT) lFormats[0];
            bmInfo.cBitCount = (USHORT) lFormats[1];
            bmInfo.cx = cx;
            bmInfo.cy = cy;

            hbm = GpiCreateBitmap( hpsMemory, (PBITMAPINFOHEADER2) &bmInfo, 0L, NULL, NULL );
            hbmOld = GpiSetBitmap( hpsMemory, hbm );

            if ( hbmOld )
                GpiDeleteBitmap( hbmOld );

            // set bitblt region
            aptlBitBlt[1].x = cx;
            aptlBitBlt[1].y = cy;

            // Set vogle device parameters
            vdevice.sizeX = vdevice.sizeY = min(cx, cy);
            vdevice.sizeSx = cx;
            vdevice.sizeSy = cy;
            vdevice.depth = lFormats[0] * lFormats[1];

            RELEASEPS();
            break;

        case WM_PAINT:
            REQUESTPS();
            hpsPaint = WinBeginPaint( hwnd, hpsScreen, NULL );
            GpiErase( hpsScreen );
            GpiDrawSegment( hpsScreen, ID_SCREEN_SEG );
            WinEndPaint( hpsPaint );
            RELEASEPS();
            break;

        case WM_COMMAND:
            switch ( SHORT1FROMMP(mp1) )
                {
                case ID_ABOUT:
                    WinDlgBox( HWND_DESKTOP, hwndFrame, WinDefDlgProc, NULLHANDLE, RID_ABOUTDLG, NULL );
                    break;
                }
            break;

        case WM_CREATE:

            // Initialize Memory DC and PS
            hdcMemory = DevOpenDC( hab, OD_MEMORY, "*", 5L, (PDEVOPENDATA)&dop, NULLHANDLE );
            hpsMemory = GpiCreatePS( hab, hdcMemory, &sizl, GPIT_MICRO | GPIA_ASSOC | PU_PELS );

            // Initialize Screen DC and PS
            hdcScreen = WinOpenWindowDC( hwnd );
            hpsScreen = GpiCreatePS( hab, hdcScreen, &sizl, GPIT_NORMAL | GPIA_ASSOC | PU_PELS );
            GpiSetDrawingMode( hpsScreen, DM_DRAWANDRETAIN );
            GpiOpenSegment( hpsScreen, ID_SCREEN_SEG );

            // Initialize the color tables for the PS's
            GpiCreateLogColorTable( hpsMemory, LCOL_PURECOLOR, LCOLF_CONSECRGB,
                                    0L, 8, alDefaultColors );
            GpiCreateLogColorTable( hpsScreen, LCOL_PURECOLOR, LCOLF_CONSECRGB,
                                    0L, 8, alDefaultColors );

            // Set the default PS to the screen
            hps = hpsScreen;

            // Clear the keyboard "buffer"
            NextKey = 0;

            // Initialize the semaphores
            DosCreateEventSem( 0L, &hevChar, 0L, FALSE );   // initially set
            DosCreateMutexSem( 0L, &hmtxPS, 0L, FALSE );    // initially unowned

            // Get frame handle so we can add the "About" menu item
            // and set the frame's position and size
            hwndFrame = WinQueryWindow( hwnd, QW_PARENT );

            // *** Add "About" menu item to the system menu ***

            // Get window handle of for system menu
            hwndSysMenu = WinWindowFromID( hwndFrame, FID_SYSMENU );
            WinSendMsg( hwndSysMenu, MM_QUERYITEM, MPFROM2SHORT(SC_SYSMENU, TRUE), (MPARAM) &MenuItem );
            hwndSysMenu = MenuItem.hwndSubMenu;

            // Add separator
            MenuItem.iPosition = MIT_END;
            MenuItem.afStyle = MIS_SEPARATOR;
            MenuItem.afAttribute = 0;
            MenuItem.id = 0;
            MenuItem.hwndSubMenu = 0;
            MenuItem.hItem = 0;
            WinSendMsg( hwndSysMenu, MM_INSERTITEM, (MPARAM) &MenuItem, NULL );

            // Add "About" item
            MenuItem.afStyle = MIS_TEXT;
            MenuItem.id = ID_ABOUT;
            WinSendMsg( hwndSysMenu, MM_INSERTITEM, (MPARAM) &MenuItem,
                (MPARAM) "~About" );

            // *** Initialize the size and position of window ***

            // This is the prefered size from the user's program
            // x,y refer to the position of the frame window
            // cx,cy refer to the size of the client window
            getprefposandsize( &x, &y, &cx, &cy );
            
            // Get the size, position, and status recommended by OS/2
            WinQueryTaskSizePos( hab, 0, &swpDefault );
            
            // Calculate the client window size from the recommended frame size
            rcl.xLeft = rcl.yBottom = 0;
            rcl.xRight = swpDefault.cx;
            rcl.yTop = swpDefault.cy;
            WinCalcFrameRect( hwndFrame, &rcl, TRUE );
            cxDefault = rcl.xRight - rcl.xLeft;
            cyDefault = rcl.yTop - rcl.yBottom;
            
            // Vogle likes square windows for some reason so the default client
            // window size will be a square whose side length is the minimum of
            // the recommended lengths.
            cxDefault = cyDefault = min(cxDefault,cyDefault);
            
            // Now set x,y for the frame window and cx,cy for the client window if
            // they're not valid from getprefposandsize().
            if ( x < 0 )
                x = swpDefault.x;
            if ( y < 0 )
                y = swpDefault.y;
            if ( cx < 0 )
                cx = cxDefault;
            if ( cy < 0 )
                cy = cyDefault;
            
            // Now create a "dummy" rectangle - just the size is important.
            // Then we'll calculate the required size of the frame that will contain
            // the client window of size cx,cy.
            rcl.xLeft = rcl.yBottom = 0;
            rcl.xRight = cx;
            rcl.yTop = cy;
            WinCalcFrameRect( hwndFrame, &rcl, FALSE );
            cxFrame = rcl.xRight - rcl.xLeft;
            cyFrame = rcl.yTop - rcl.yBottom;
            
            // now resize (and maybe activate) the frame - this also resizes the client
            WinSetWindowPos( hwndFrame, HWND_TOP, x, y, cxFrame, cyFrame, swpDefault.fl );

            break;

        case WM_DESTROY:
            // Destroy everything created in WM_CREATE

            // NOTE: If this window is being closed from this thread, then the
            // semaphores, PS's, and bitmaps may be busy and may not be deleted,
            // closed, or destroyed.  (ie: hevChar would be
            // busy if the vogle app were waiting for a character when the user
            // closed the PM window.)  However, it doesn't matter because the
            // whole process will terminate.

            // Destroy the semaphores
            DosCloseEventSem( hevChar );
            DosCloseEventSem( hmtxPS );

            // Destroy the DC's and PS's
            GpiAssociate( hpsMemory, 0L );
            GpiDestroyPS( hpsMemory );
            GpiAssociate( hpsScreen, 0L );
            GpiDestroyPS( hpsScreen );

            // Destroy the bitmap if it exists
            if ( hbm )
                GpiDeleteBitmap( hbm );

            break;

        default:
            return WinDefWindowProc( hwnd, msg, mp1, mp2 );
        }
    return NULL;
    }

// Device entry for Vogle
static DevEntry PMDev =
    {
    "os2pm",
    "large",
    "small",
    PM_backbuf,
    PM_char,
    PM_checkkey,
    PM_clear,
    PM_color,
    PM_draw,
    PM_exit,
    PM_fill,
    PM_font,
    PM_frontbuf,
    PM_getkey,
    PM_init,
    PM_locator,
    PM_mapcolor,
    PM_setlw,
    PM_string,
    PM_swapbuf,
    PM_sync
    };

int _PM_devcpy()
    {
    vdevice.dev = PMDev;
    return 0;
    }

