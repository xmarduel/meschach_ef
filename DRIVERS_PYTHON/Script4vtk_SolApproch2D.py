#!/usr/bin/env python
#
# File automatically generated - vtk version : 4.2
# date : Sun Jun 19 12:31:55 2022
#
import vtk


reader = vtk.vtkUnstructuredGridReader()
reader.SetFileName("./SolApproch2D.vtk")
reader.Update()

lut = vtk.vtkLookupTable() 
lut.SetHueRange(0.667, 0.0) 
lut.SetNumberOfTableValues(256) 
lut.SetRampToSQRT() 
lut.Build() 

valrange      = reader.GetOutput().GetPointData().GetScalars().GetRange()

geofilter = vtk.vtkGeometryFilter()
geofilter.SetInputData(reader.GetOutput())
geofilter.Update()

polydatanormals = vtk.vtkPolyDataNormals()
polydatanormals.SetInputData(geofilter.GetOutput())
polydatanormals.Update()

surfaceMapper = vtk.vtkDataSetMapper()
surfaceMapper.SetInputData(polydatanormals.GetOutput())
#surfaceMapper.SetInputData(reader.GetOutput()) # -> jagged effect 
surfaceMapper.SetLookupTable( lut ) 
surfaceMapper.SetScalarRange( valrange[0], valrange[1]) 
surfaceMapper.Update()

surfaceActor = vtk.vtkActor()
surfaceActor.SetMapper(surfaceMapper)


ren = vtk.vtkRenderer()
ren.SetBackground(131, 22, 21)
ren.AddActor(surfaceActor)

renWin = vtk.vtkRenderWindow()
renWin.AddRenderer(ren)
renWin.SetSize( 500, 500)

iren = vtk.vtkRenderWindowInteractor()
iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())
iren.SetRenderWindow(renWin)
iren.Initialize()
iren.Start()
