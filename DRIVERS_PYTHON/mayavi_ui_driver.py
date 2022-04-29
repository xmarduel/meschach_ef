#! /usr/bin/env pythonw
#
import sys
import os 
import Tkinter
#
#
#
#
root = Tkinter.Tk()
root.geometry("300x600")
#
# all variables
#
file                     = Tkinter.StringVar()
#
hasAxes                  = Tkinter.IntVar()
hasBandedSurfaceMap      = Tkinter.IntVar()
hasContourGridPlane      = Tkinter.IntVar()
hasCustomGridPlane       = Tkinter.IntVar()
hasGlyph                 = Tkinter.IntVar()
hasGridPlane             = Tkinter.IntVar()
hasHedgeHog              = Tkinter.IntVar()
hasIsoSurface            = Tkinter.IntVar()
hasLabels                = Tkinter.IntVar()
hasLocator               = Tkinter.IntVar()
hasOutline               = Tkinter.IntVar()
hasPolyData              = Tkinter.IntVar()
hasScalarCutPlane        = Tkinter.IntVar()
hasStreamlines           = Tkinter.IntVar()
hasStructuredGridOutline = Tkinter.IntVar()
hasSurfaceMap            = Tkinter.IntVar()
hasTensorGlyphs          = Tkinter.IntVar()
hasText                  = Tkinter.IntVar()
hasVectorCutPlane        = Tkinter.IntVar()
hasVelocityVector        = Tkinter.IntVar()
hasVolume                = Tkinter.IntVar()
hasWarpVectorCutPlan     = Tkinter.IntVar()
#
#
file.set("Stokes3D.vtk")
hasAxes.set(1)
hasBandedSurfaceMap.set(0)
hasContourGridPlane.set(0)
hasCustomGridPlane.set(0)
hasGlyph.set(0)
hasGridPlane.set(0)
hasHedgeHog.set(0)
hasIsoSurface.set(0)
hasLabels.set(0)
hasLocator.set(0)
hasOutline.set(1)
hasPolyData.set(0)
hasScalarCutPlane.set(0)
hasStreamlines.set(1)
hasStructuredGridOutline.set(0)
hasSurfaceMap.set(0)
hasTensorGlyphs.set(0)
hasText.set(0)
hasVectorCutPlane.set(0)
hasVelocityVector.set(0)
hasVolume.set(0)
hasWarpVectorCutPlan.set(0)
#
#
#
Tkinter.Label(text="Enter File").grid(sticky="e", row=0, column=1)
Tkinter.Entry( width=25, relief='sunken', textvariable = file).grid(sticky="w", row=0, column=0)
#
Tkinter.Checkbutton(text="Axes"                   , variable = hasAxes                  ).grid(sticky="w", row=1)
Tkinter.Checkbutton(text="BandedSurfaceMap"       , variable = hasBandedSurfaceMap      ).grid(sticky="w", row=2)
Tkinter.Checkbutton(text="ContourGridPlane"       , variable = hasContourGridPlane      ).grid(sticky="w", row=3)
Tkinter.Checkbutton(text="CustomGridPlane"        , variable = hasCustomGridPlane       ).grid(sticky="w", row=4)
Tkinter.Checkbutton(text="Glyph"                  , variable = hasGlyph                 ).grid(sticky="w", row=5)
Tkinter.Checkbutton(text="GridPlane"              , variable = hasGridPlane             ).grid(sticky="w", row=6)
Tkinter.Checkbutton(text="HedgeHog"               , variable = hasHedgeHog              ).grid(sticky="w", row=7)
Tkinter.Checkbutton(text="IsoSurface"             , variable = hasIsoSurface            ).grid(sticky="w", row=8)
Tkinter.Checkbutton(text="Labels"                 , variable = hasLabels                ).grid(sticky="w", row=9)
Tkinter.Checkbutton(text="Locator"                , variable = hasLocator               ).grid(sticky="w", row=10)
Tkinter.Checkbutton(text="Outline"                , variable = hasOutline               ).grid(sticky="w", row=11)
Tkinter.Checkbutton(text="PolyData"               , variable = hasPolyData              ).grid(sticky="w", row=12)
Tkinter.Checkbutton(text="ScalarCutPlane"         , variable = hasScalarCutPlane        ).grid(sticky="w", row=13)
Tkinter.Checkbutton(text="Streamlines"            , variable = hasStreamlines           ).grid(sticky="w", row=14)
Tkinter.Checkbutton(text="StructuredGridOutline"  , variable = hasStructuredGridOutline ).grid(sticky="w", row=15)
Tkinter.Checkbutton(text="SurfaceMap"             , variable = hasSurfaceMap            ).grid(sticky="w", row=16)
Tkinter.Checkbutton(text="TensorGlyphs"           , variable = hasTensorGlyphs          ).grid(sticky="w", row=17)
Tkinter.Checkbutton(text="Text"                   , variable = hasText                  ).grid(sticky="w", row=18)
Tkinter.Checkbutton(text="VectorCutPlane"         , variable = hasVectorCutPlane        ).grid(sticky="w", row=19)
Tkinter.Checkbutton(text="VelocityVector"         , variable = hasVelocityVector        ).grid(sticky="w", row=20)
Tkinter.Checkbutton(text="Volume"                 , variable = hasVolume                ).grid(sticky="w", row=21)
Tkinter.Checkbutton(text="WarpVectorCutPlan"      , variable = hasWarpVectorCutPlan     ).grid(sticky="w", row=22)
#
#
def mayavi_call():
    #
    file_name = file.get()
    system_call = "mayavi -d " + file_name
    #
    if ( hasAxes.get() == 1 ):
        system_call += " -m Axes"
    if ( hasBandedSurfaceMap.get() == 1 ):
        system_call += " -m BandedSurfaceMap"
    if ( hasContourGridPlane.get() == 1 ):
        system_call += " -m ContourGridPlane"
    if ( hasCustomGridPlane.get() == 1 ):
        system_call += " -m CustomGridPlane"
    if ( hasGlyph.get() == 1 ):
        system_call += " -m Glyph"
    if ( hasGridPlane.get() == 1 ):
        system_call += " -m GridPlane"
    if ( hasHedgeHog.get() == 1 ):
        system_call += " -m HedgeHog"
    if ( hasIsoSurface.get() == 1 ):
        system_call += " -m IsoSurface"
    if ( hasLabels.get() == 1 ):
        system_call += " -m Labels"
    if ( hasLocator.get() == 1 ):
        system_call += " -m Locator"
    if ( hasOutline.get() == 1 ):
        system_call += " -m Outline"
    if ( hasPolyData.get() == 1 ):
        system_call += " -m PolyData"
    if ( hasScalarCutPlane.get() == 1 ):
        system_call += " -m ScalarCutPlane"
    if ( hasStreamlines.get() == 1 ):
        system_call += " -m Streamlines"
    if ( hasStructuredGridOutline.get() == 1 ):
        system_call += " -m StructuredGridOutline"
    if ( hasSurfaceMap.get() == 1 ):
        system_call += " -m SurfaceMap"
    if ( hasTensorGlyphs.get() == 1 ):
        system_call += " -m TensorGlyphs"
    if ( hasText.get() == 1 ):
        system_call += " -m Text"
    if ( hasVectorCutPlane.get() == 1 ):
        system_call += " -m VectorCutPlane"
    if ( hasVelocityVector.get() == 1 ):
        system_call += " -m VelocityVector"
    if ( hasVolume.get() == 1 ):
        system_call += " -m Volume"
    if ( hasWarpVectorCutPlan.get() == 1 ):
        system_call += " -m WarpVectorCutPlan"
    #
    print system_call
    #
    os.system(system_call)
    #
#
#
#
Tkinter.Button(text="MayaVi", command=mayavi_call).grid(row=24, column=0, sticky="w")
Tkinter.Button(text="Quit"  , command=root.quit  ).grid(row=24, column=1)
#
#
Tkinter.mainloop()
#
