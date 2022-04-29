
from sys   import *
from math  import *
import os

from vogle import *

USE_MOUSE = True

ESC =	27

TRANS		        = 0.06

xyz = [
   [ 0.000000,  0.000000,  1.000000],
   [ 0.809017, -0.587785,  0.500000],
   [ 0.809017,  0.587785,  0.500000],
   [-0.309017,  0.951057,  0.500000],
   [-1.000000,  0.000000,  0.500000],
   [-0.309017, -0.951057,  0.500000],
   [ 1.000000,  0.000000, -0.500000],
   [ 0.309017,  0.951057, -0.500000],
   [-0.809017,  0.587785, -0.500000],
   [-0.809017, -0.587785, -0.500000],
   [ 0.309017, -0.951057, -0.500000],
   [ 0.000000,  0.000000, -1.000000]
]

ncon = [
   [ 1,  2,  3],
   [ 1,  3,  4],
   [ 1,  4,  5],
   [ 1,  5,  6],
   [ 1,  6,  2],
   [ 2,  7,  3],
   [ 3,  8,  4],
   [ 4,  9,  5],
   [ 5, 10,  6],
   [ 6, 11,  2],
   [ 7,  8,  3],
   [ 8,  9,  4],
   [ 9, 10,  5],
   [10, 11,  6],
   [11,  7,  2],
   [ 7, 12,  8],
   [ 8, 12,  9],
   [ 9, 12, 10],
   [10, 12, 11],
   [11, 12,  7]
]

#

def drawshape():
    '''
    '''
    for i in range(20):
        
        color(i+1)
      
        polyfill(1)
        
        makepoly()
        move(xyz[ncon[i][0]-1][0], xyz[ncon[i][0]-1][1], xyz[ncon[i][0]-1][2])
        draw(xyz[ncon[i][1]-1][0], xyz[ncon[i][1]-1][1], xyz[ncon[i][1]-1][2])
        draw(xyz[ncon[i][2]-1][0], xyz[ncon[i][2]-1][1], xyz[ncon[i][2]-1][2])
        closepoly()

#
#

def main():
    '''
    '''
    DRAG = 0
    EVENT_X = 0
    EVENT_Y = 0

    XROT = 0.0
    YROT = 0.0

    XTRANS = 0.0
    YTRANS = 0.0
    ZTRANS = 0.0

    SCALE_FACTOR = 1.0

    SIZE = 300
   
    prefposition(50, 50)
    prefsize(SIZE, SIZE)

    w1 = winopen("X11", "Licosa")
    nplanes = getdepth()
    backbuffer()

    window(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0)
    lookat(0.0, 0.0, 2.1, 0.0, 0.0, 0.0, 0.0)

    textsize(0.15, 0.3)

    backface(1)

    #
    # Green color ramp...
    #
    for i in range(1,21):
        mapcolor(i, 20, 20 + i * 10 , 20)
   
    if backbuffer() < 0:
        vexit()
        print "licosa: device doesn't support double buffering.\n"
        exit(0)


    while (1):
   
        if USE_MOUSE:
            
            psx  = new_doublep()
            psy  = new_doublep()
            
            but = slocator(psx, psy)

            sx = doublep_value(psx)
            sy = doublep_value(psy)
            
            if (but==1):
                rotate(90.0 * sx, 'x')
            if (but==2):
                rotate(190.0 * sx, 'y')
            if (but==4):
                rotate(190.0 * sx, 'z')
            
            pushmatrix()
            rotate(90 * sx, 'y')
            rotate(90 * sy, 'z')
   
            color(BLACK)
            clear()
            drawshape()
            popmatrix()
            swapbuffers()

            key = checkkey()
            
            if key in [ord('q'), ESC] :	# Stop the program 
                vexit()
                exit(0)
            if key == ord('e'):	# Enlarge
                scale(1.1, 1.1, 1.1)
            if key == ord('r'):	# Reduce
                scale(0.9, 0.9, 0.9)
            if key == ord('w'):	# WireFrame
                polyfill(0)
            if key == ord('s'):	# Solid
                polyfill(1)
            if key == ord('g'):
                # gif
                print "make licosa.gif"
                w_gif = winopen("gif", "licosa.gif")
                
                for i in range(1,21):
                    mapcolor(i, 20, 20 + i * 10 , 20)
                pushmatrix()

                rotate(90 * sx, 'y')
                rotate(90 * sy, 'z')

                scale(SCALE_FACTOR,SCALE_FACTOR,SCALE_FACTOR)
                translate(XTRANS,-YTRANS,ZTRANS)

                color(BLACK)
                clear()
      
                drawshape()
      
                # if (nplanes == 1):
                #     drawshape(0)
      
                popmatrix()
            
                swapbuffers()
                winclose(w_gif)
                winset(w1)
           
        else:

            pushmatrix()

            rotate( -XROT , 'x')
            rotate(  YROT , 'y')

            scale(SCALE_FACTOR,SCALE_FACTOR,SCALE_FACTOR)
            translate(XTRANS,-YTRANS,ZTRANS)

            color(BLACK)
            clear()
      
            drawshape()
      
            # if (nplanes == 1):
            #     drawshape(0)
      
            popmatrix()
            
            swapbuffers()


            vev = new_Vevent()
            
            w = vgetevent(vev, 1)

            #if vev.type == VMOTION:
            if Vevent_type_get(vev) == VMOTION:
      
                if DRAG == 1 :
                
                    if Vevent_data_get(vev) == 1:
                    
                        DELTA_X = Vevent_x_get(vev) - EVENT_X
                        DELTA_Y = Vevent_y_get(vev) - EVENT_Y

                        print " In VMOTION , (DELTA_X,DELTA_Y) = (%d,%d) )\n" %(DELTA_X,DELTA_Y)

                        EVENT_X = Vevent_x_get(vev)
                        EVENT_Y = Vevent_y_get(vev)

                        print " In VMOTION , (EVENT_X,EVENT_Y) = (%d,%d) )\n"  %(EVENT_X,EVENT_Y)

                        rx = 2 * DELTA_X / (float)(SIZE)
                        ry = 2 * DELTA_Y / (float)(SIZE)

                        print " In VMOTION , (rx,ry) = (%6.3lf , %6.3lf) \n"  %(rx,ry)

                        XROT  = fmod( XROT + 180*ry , 360 )
                        YROT  = fmod( YROT + 180*rx , 360 )

                        print " In VMOTION , (XROT,YROT) = (%6.3lf , %6.3lf) \n"  %(XROT,YROT)

                    if Vevent_data_get(vev) == 2:
                   
                        DELTA_X = Vevent_x_get(vev) - EVENT_X
                        DELTA_Y = Vevent_y_get(vev) - EVENT_Y

                        print " In VMOTION , (DELTA_X,DELTA_Y) = (%d,%d) )\n"  %(DELTA_X,DELTA_Y)

                        EVENT_X = Vevent_x_get(vev)
                        EVENT_Y = Vevent_y_get(vev)

                        print " In VMOTION , (EVENT_X,EVENT_Y) = (%d,%d) )\n"  %(EVENT_X,EVENT_Y)

                        rx = 2000 * DELTA_X / (float)(SIZE)
                        ry = 2000 * DELTA_Y / (float)(SIZE)

                        print " In VMOTION , (rx,ry) = (%6.3lf , %6.3lf) \n" %(rx,ry)

                        XTRANS  -= rx
                        YTRANS  -= ry
                        ZTRANS  = 0.0
      
            if Vevent_type_get(vev) == VRESIZE:
      
                winset(w)
                calcviewport()
      
            if Vevent_type_get(vev) == VBUTTONPRESS:
         
                DRAG = 1
                #
                # print " In VBUTTONPRESS , (x.y) = (%d , %d)\n" %(vev.x,vev.y)
                #
                EVENT_X = Vevent_x_get(vev)
                EVENT_Y = Vevent_y_get(vev)
      
            if Vevent_type_get(vev) == VBUTTONRELEASE:
       
                DRAG = 0
      
      
            if Vevent_type_get(vev) == VKEYPRESS:
      
                if Vevent_data_get(vev) == ord('s'):
                    polyfill(1)
                if Vevent_data_get(vev) == ord('w'):
                    polyfill(0)
                if Vevent_data_get(vev) in [ ESC, ord('q')]:
                   vexit()
   
    vexit()

    return(0)

#
#
if __name__ == '__main__':
    main()
#
