#!/usr/bin/env python
#
# File automatically generated - vtk version 4.2
# date : Sun Jun 26 19:41:27 2022
#
import vtk


reader = vtk.vtkUnstructuredGridReader() 
reader.SetFileName( "./SolExact3D.vtk" )
reader.Update()

lut1 = vtk.vtkLookupTable() 
lut1.SetHueRange(0.667, 0.0) 
lut1.SetNumberOfTableValues(256) 
lut1.SetRampToSQRT() 
lut1.Build() 

lut2 = vtk.vtkLookupTable() 
lut2.SetHueRange(0.667, 0.0) 
lut2.SetNumberOfTableValues(256) 
lut2.SetRampToSQRT() 
lut2.Build() 

valrange = reader.GetOutput().GetPointData().GetScalars().GetRange() 

surfaceMapper = vtk.vtkDataSetMapper() 
surfaceMapper.SetInputData( reader.GetOutput() )
surfaceMapper.SetLookupTable( lut1 )
surfaceMapper.SetScalarRange( valrange[0], valrange[1] )
surfaceMapper.Update()

surfaceActor = vtk.vtkActor() 
surfaceActor.SetMapper( surfaceMapper )

contour = vtk.vtkContourFilter() 
contour.SetInputData( reader.GetOutput() ) 
contour.GenerateValues( 7 , valrange[0], valrange[1] )
contour.Update()

contourMapper = vtk.vtkPolyDataMapper() 
contourMapper.SetInputData( contour.GetOutput() ) 
contourMapper.SetLookupTable( lut2 ) 
contourMapper.SetScalarRange( valrange[0], valrange[1] )
contourMapper.Update()

contourActor = vtk.vtkActor() 
contourActor.SetMapper( contourMapper )


ren1 = vtk.vtkRenderer()
ren1.SetViewport(0.0, 0.0,  0.5, 1.0)
ren1.SetBackground( 1, 2, 1 ) 
ren1.AddActor( surfaceActor )
ren2 = vtk.vtkRenderer()
ren2.SetViewport(0.5, 0.0,  1.0, 1.0)
ren2.AddActor( contourActor )

renWin = vtk.vtkRenderWindow() 
renWin.SetSize( 800, 400 ) 
renWin.AddRenderer( ren1 )
renWin.AddRenderer( ren2 )

iren = vtk.vtkRenderWindowInteractor()
iren.SetRenderWindow(renWin)
iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
iren.Initialize()
iren.Start()
