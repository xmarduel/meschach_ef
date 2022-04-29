# ----------------------------------------------------- 
# Ceci est un programme GNUPLOT nomme '<name_file>.gnu' 
# ----------------------------------------------------- 
#  
set size ratio -1
#
set xlabel " X axis " 
set ylabel " Y axis " 
set xtics 0 ,0.200000, 1  
set ytics 0 ,0.200000, 1  
#
#
splot 'Base2D_f0_P2_2D_ucb.dat' title 'Solution App' with lines
pause -1 
#
splot 'Base2D_f1_P2_2D_ucb.dat' title 'Solution App' with lines
pause -1 
#
splot 'Base2D_f2_P2_2D_ucb.dat' title 'Solution App' with lines
pause -1 
#
splot 'Base2D_f3_P2_2D_ucb.dat' title 'Solution App' with lines
pause -1 
#
splot 'Base2D_f4_P2_2D_ucb.dat' title 'Solution App' with lines
pause -1 
#
splot 'Base2D_f5_P2_2D_ucb.dat' title 'Solution App' with lines
pause -1 
#  
