#!/usr/bin/env pythonw
#
# File automatically generated - vtk version : 6.1
# date : Sun Feb  8 13:02:47 2015

import vtk 


redColor   = (1.0, 0.0, 0.0) 
greenColor = (0.0, 1.0, 0.0) 
blueColor  = (0.0, 0.0, 1.0) 
blackColor = (0.0, 0.0, 0.0) 

file_vtk = vtk.vtkPolyDataReader() 
file_vtk.SetFileName("/Users/xavier/DEVELOPMENT/MESCHACH_WORK/DRIVERS_PYTHON/SolApproch1D.vtk") 
file_vtk.Update() 

xyplot = vtk.vtkXYPlotActor() 
xyplot.AddDataSetInput( file_vtk.GetOutput() ) 
xyplot.GetPositionCoordinate().SetValue(0.1, 0.1, 0.0)  
xyplot.GetPosition2Coordinate().SetValue(0.8, 0.7, 0.0)  
xyplot.SetXValuesToValue() 
#xyplot.LogxOn() 
#xyplot.LogyOn() 
#xyplot.SetXRange( 0.0, 1.3) 
#xyplot.SetYRange(-3.0, 2.0) 
xyplot.SetNumberOfXLabels(6) 
xyplot.SetNumberOfYLabels(6) 
#xyplot.SetTitle("Pressure") 
xyplot.SetXTitle("X") 
xyplot.SetYTitle("Y") 

xyplot.GetProperty().SetColor( blackColor ) 
xyplot.GetProperty().SetLineWidth( 1 ) 

xyplot.SetPlotColor( 0 , redColor) 
#xyplot.SetPlotColor( 1 , greenColor) 
 
#XAxis = xyplot.GetXAxisActor2D() 
#YAxis = xyplot.GetYAxisActor2D() 

# Create graphics stuff 

ren1 = vtk.vtkRenderer() 
ren1.SetBackground(1, 1, 1) 
ren1.SetViewport(0.0, 0.0, 1.0, 1.0) 
ren1.AddActor2D( xyplot ) 

renWin = vtk.vtkRenderWindow() 
renWin.SetSize( 500, 500) 
renWin.AddRenderer( ren1 ) 

iren = vtk.vtkRenderWindowInteractor()
iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
iren.SetRenderWindow(renWin)
iren.Initialize()
iren.Start()

