#! /usr/bin/env gnuplot
# 
# File automatically generated  - changes will not be kept - 
# date : Sun Jun 19 21:34:21 2022
# 
#  
set parametric 
#set size square 
set size ratio -1 
#set noxtics   
#set noytics   
#set view 60,340,1  
set xlabel " X axis " 
set ylabel " Y axis " 
set xtics 0 ,0.200000, 1  
set ytics 0 ,0.200000, 1  
splot 'SolApproch2D.dat' title 'Solution App' with lines
pause -1 
#  
