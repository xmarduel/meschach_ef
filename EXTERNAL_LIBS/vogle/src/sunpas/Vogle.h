{ 
	Vogle Pascal header file for Sun Pascal

	link with:

	pc progname.p -lvogle -lsuntool -lsunwindow -lpixrect -lm

}

const
{ Standard Colour Indices }
	BLACK		= 0;
	RED		= 1;
	GREEN		= 2;
	YELLOW		= 3;
	BLUE		= 4;
	MAGENTA		= 5;
	CYAN		= 6;
	WHITE		= 7;

{ Maximum number of vertices in a polygon }
	MAXVERTS	= 128;
{ Maximum length of a string type }
	MAXSTRINGLEN	= 255;
{ Maximum number of "curve" geometries (for curven) }
	MAXCURVEN	= 200;

type
{ The type of stings passed to various VOGLE routines... }
   string_t = varying [MAXSTRINGLEN] of char;

{ For getmatrix and loadmatrix etc, etc... }
   Matrix44_t = array[1..4] of array[1..4] of shortreal;
   Matrix43_t = array[1..4] of array[1..3] of shortreal;

{ For Poly and Poly2 .... }
   Coord2 = array[1..2] of shortreal;
   Coord3 = array[1..3] of shortreal;
   Coord4 = array[1..4] of shortreal;
   Poly2_array_t = array[1..MAXVERTS] of Coord2;
   Poly3_array_t = array[1..MAXVERTS] of Coord3;

{ For Curven....}
	GeomMat_t = array[1..MAXCURVEN, 1..3] of shortreal;

{ Device routines }
   procedure Vinit(device: string_t); external c;
   procedure Vexit; external c;
   procedure Voutput(path: string_t); external c;
   procedure VnewDev(device: string_t); external c;
   procedure VgetDev(var device: string_t); external c;
   procedure PushDev(device: string_t); external c;
   procedure PopDev; external c;
   function GetDepth: integer; external c;
   procedure VsetFlush(yesno: boolean); external c;
   procedure Vflush; external c;

{ Routines for Setting Up Windows }
   procedure PrefPosition(x, y: integer); external c;
   procedure PrefSize(width, height: integer); external c;

{ General routines }
   procedure Clear; external c;
   procedure Color(col: integer); external c;
   procedure MapColor(indx, red, green, blue: integer); external c;
   procedure Clipping(on: boolean); external c;
   function GetKey: integer; external c;
   function CheckKey: integer; external c;
   function GetString(bcol: integer; var string: string_t): integer; external c;
   function Locator(var xaddr, yaddr: real): integer; external c;
   function Slocator(var xaddr, yaddr: real): integer; external c;

{ Picking routines }
   procedure SetPickat(x, y: real); external c;
   procedure SetPicksize(x, y: real); external c;
   procedure Pickmode(col: integer); external c;
   function Picked: integer; external c;

{ Viewport routines }
   procedure ViewPort(left, right, bottom, top: real); external c;
   procedure PushViewPort; external c;
   procedure PopViewPort; external c;
   procedure CalcViewport; external c;
   procedure GetViewPort(var left, right, bottom, top: real); external c;

{ Getting the aspect details }
   function GetAspect: real; external c;
   procedure GetFactors(var w, h: real); external c;
   procedure GetDisplaySize(var w, h: real); external c;
   procedure ExpandViewport; external c;

{ Attribute stack routines }
   procedure PushAttributes; external c;
   procedure PopAttributes; external c;

{ Projection routines }
   procedure Ortho( left, right, bottom, top, near, far: real); external c;
   procedure Ortho2(left, right, bottom, top: real); external c;
   procedure Perspective(fov, aspect, near, far: real); external c;
   procedure Window(left, right, bottom, top, near, far: real); external c;

{ Matrix stack routines }
   procedure PushMatrix; external c;
   procedure PopMatrix; external c;

{ Viewpoint routines }
   procedure PolarView(distance, azimuth, incidence, twist: real); external c;
   procedure Up(x, y, z: real); external c;
   procedure LookAt(vx, vy, vz, px, py, pz, twist: real); external c;

{ Moving routines }
   procedure Move(x, y, z: real); external c;
   procedure Rmove(dx, dy, dz:real); external c;
   procedure Move2(x, y: real); external c;
   procedure Rmove2(dx, dy: real); external c;
   procedure Smove2(x, y: real); external c;
   procedure Rsmove2(dx, dy: real); external c;

{ Drawing routines }
   procedure SetDash(d: real); external c;
   procedure LineStyle(str: string_t); external c;
   procedure Draw(x, y, z: real); external c;
   procedure Rdraw(dx, dy, dz: real); external c;
   procedure Draw2(x, y: real); external c;
   procedure Rdraw2(dx, dy: real); external c;
   procedure Sdraw2(x, y: real); external c;
   procedure Rsdraw2(dx, dy: real); external c;

{ Arcs and circles }
   procedure CirclePrecision(nsegs: integer); external c;
   procedure Arc(x, y, radius, start_angle, end_angle: real); external c;
   procedure Sector(x, y, radius, start_angle, end_angle: real); external c;
   procedure Circle(x, y, radius: real); external c;

{ Curve routines }
   procedure CurveBasis(var basis: Matrix44_t); external c;
   procedure CurvePrecision(nsegs: integer); external c;
   procedure Rcurve(var geom: Matrix44_t); external c;
   procedure Curve(var geom: Matrix43_t); external c;
   procedure Curven(n: integer; var geom: GeomMat_t); external c;

{ Rectangles and general polygon routines }
   procedure Rect(x1, y1, x2, y2: real); external c;
   procedure PolyFill(on: boolean); external c;
   procedure PolyHatch(on: boolean); external c;
   procedure HatchAng(angle: real); external c;
   procedure HatchPitch(pitch: real); external c;
   procedure Poly2(n: integer; var points: Poly2_array_t); external c;
   procedure Poly(n: integer; var points: Poly3_array_t); external c;
   procedure MakePoly; external c;
   procedure ClosePoly; external c;
   procedure BackFace(onoff: boolean); external c;
   procedure BackFaceDir(clockwise: boolean); external c;

{ Text routines }
   procedure Font(fontname: string_t); external c;
   function NumChars: integer; external c;
   procedure TextSize(width, height: real); external c;
   procedure TextAng(angle: real); external c;
   procedure FixedWidth(on: boolean); external c;
   procedure CenterText(on: boolean); external c;
   procedure TextWeight(val: integer); external c;
   procedure TextSlant(val: real); external c;
   procedure TextJustify(val: integer); external c;
   procedure LeftJustify; external c;
   procedure RightJustify; external c;
   procedure TopJustify; external c;
   procedure BottomJustify; external c;
   procedure XcenterText; external c;
   procedure YcenterText; external c;
   procedure GetCharSize(c:char; var width, height: real); external c;
   procedure GetFontSize(var width, height: real); external c;
   procedure DrawChar(c: char); external c;
   procedure DrawStr(str: string_t); external c;
   procedure StrLength(str: string_t); external c;
   procedure BoxText(x, y, l, h: real; s: string_t); external c;
   procedure BoxFit(l, h: real; nchars: integer); external c;

{ Transformation routines }
   procedure Translate(x, y, z: real); external c;
   procedure Scale(x, y, z: real); external c;
   procedure Rotate(angle: real; axis: char); external c;

{ Patch routines }
   procedure PatchBasis(var tbasis, ubasis: Matrix44_t); external c;
   procedure PatchPrecision(tseg, useg: integer); external c;
   procedure PatchCurves(nt, nu: integer); external c;
   procedure Rpatch(var gx, gy, gz, gw: Matrix44_t); external c;
   procedure Patch(var gx, gy, gz: Matrix44_t); external c;

{ Point routines }
   procedure Point(x, y, z: real); external c;
   procedure Point2(x, y: real); external c;

{ Object routines }
   procedure MakeObj(n: integer); external c;
   procedure AppendObj(n: integer); external c;
   procedure CloseObj; external c;
   function GenObj: integer; external c;
   function GetOpenobj: integer; external c;
   procedure CallObj(n: integer); external c;
   function IsObj(n: integer): integer; external c;
   function DelObj(n: integer): integer; external c;
   procedure LoadObj(n: integer; filename: string_t); external c;
   procedure SaveObj(n: integer; filename: string_t); external c;

{ Double Buffering }
   function BackBuffer: integer; external c;
   procedure FrontBuffer; external c;
   procedure SwapBuffers; external real;

{ Position routines }
   procedure GetGp(var x, y, z: real); external c;
   procedure GetGpT(var x, y, z, w: real); external c;
   procedure GetGp2(var x, y: real); external c;
   procedure SgetGp2(var x, y: real); external c;
   procedure PushPos; external c;
   procedure PopPos; external c;
