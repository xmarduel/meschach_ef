
from math  import *
from vogle import *


RADIUS = 10.0
SPHERE =	1

#
#  makesphere
# 
# 	make a sphere object
# 
def makesphere():
    '''
    '''
    makeobj(SPHERE)

    #
    # create the latitudinal rings
    #
    for i in range(0, 180, 20):
        pushmatrix()
        rotate(i, 'y')
        circle(0.0, 0.0, RADIUS)
        popmatrix()
		
    #
    # create the longitudinal rings
    #
    pushmatrix()
    rotate(90.0, 'x')
    for a in range(-90.0, 90.0, 20.0):
        r = RADIUS * cos(a * PI / 180.0)
        z = RADIUS * sin(a * PI / 180.0)
        pushmatrix()
        translate(0.0, 0.0, -z)
        circle(0.0, 0.0, r)
        popmatrix();

    popmatrix()

    closeobj()


#
# a demonstration of objects
#
def main():
    '''
    '''
    vinit("X11")

    vsetflush(1)

    #
    # set up our viewing transformation
    #
    perspective(90.0, 1.0, 0.001, 500.0)
    lookat(13.0, 13.0, 8.0, 0.0, 0.0, 0.0, 0.0)

    color(BLACK)
    clear()

    #
    # Call a routine to make the sphere object
    #
    makesphere()

    #
    # Now draw the sphere object scaled down. We use the pushmatrix
    # and the popmatrix to preserve the transformation matrix so
    # that only this sphere is drawn scaled.
    #
    color(CYAN)

    pushmatrix()
    scale(0.5, 0.5, 0.5)
    callobj(SPHERE)
    popmatrix()

    #
    # now we draw the same sphere translated, with a different
    # scale and color.
    #
    color(WHITE)

    pushmatrix()
    translate(0.0, -1.4 * RADIUS, 1.4 * RADIUS)
    scale(0.3, 0.3, 0.3)
    callobj(SPHERE)
    popmatrix()

    #
    # and maybe a few more times....
    #
    color(RED)

    pushmatrix()
    translate(0.0, RADIUS, 0.7 * RADIUS)
    scale(0.2, 0.2, 0.2)
    callobj(SPHERE)
    popmatrix()


    color(GREEN)

    pushmatrix()
    translate(0.0, 1.5 * RADIUS, -RADIUS)
    scale(0.15, 0.15, 0.15)
    callobj(SPHERE)
    popmatrix()


    color(YELLOW)

    pushmatrix()
    translate(0.0, -RADIUS, -RADIUS)
    scale(0.12, 0.12, 0.12)
    callobj(SPHERE)
    popmatrix()

    color(BLUE)

    pushmatrix()
    translate(0.0, -2.0*RADIUS, -RADIUS)
    scale(0.3, 0.3, 0.3)
    callobj(SPHERE)
    popmatrix()


    font("times.rb")
    ortho2(0.0, 1.0, 0.0, 1.0)
    centertext(1)
    textsize(0.08, 0.15)
    move2(0.8, 0.5)
    textang(-90.0)
    drawstr("I'm very ordinary!")

    getkey()

    vexit()
#
#
if __name__ == '__main__':
    main()
#
