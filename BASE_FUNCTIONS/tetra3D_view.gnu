# ----------------------------------------------------------- 
# Ceci est un programme GNUPLOT nomme <file_name>.gnu'        
# ----------------------------------------------------------- 
# 
# 
# 
pause 0 " " 
pause 0 " " 
pause 0 "reading the file tetra3D_view_surf.dat ..." 
# 
set title "MESH (tetra3D_view.gnu)" 
set nokey 
#set size square 
set size ratio -1 
#  
splot 'tetra3D_view_surf.dat' with lines
#  
pause 0 "...done" 
pause -1 
# 
# 
pause 0 " " 
pause 0 " " 
pause 0 "reading the file tetra3D_view_mesh.dat ..." 
# 
set title "MESH (tetra3D_view.gnu)" 
set nokey 
#set size square 
set size ratio -1 
#  
splot 'tetra3D_view_mesh.dat' with lines
#  
pause 0 "...done" 
pause -1 
