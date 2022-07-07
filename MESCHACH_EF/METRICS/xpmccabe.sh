#! /usr/bin/env sh
#
# all metrics
#
#rm -i metrics1.txt
#

pmccabe ../SRCS/boundary_conditions.c
pmccabe ../SRCS/boundary_conditions_1D.c  
pmccabe ../SRCS/boundary_conditions_2D.c  
pmccabe ../SRCS/boundary_conditions_3D.c  

pmccabe ../SRCS/graphics_output_1D.c  
pmccabe ../SRCS/graphics_output_2D.c  
pmccabe ../SRCS/graphics_output_3D.c  
pmccabe ../SRCS/graphics_geom_view.c  
pmccabe ../SRCS/graphics_matrix_patterns.c  

pmccabe ../SRCS/adv.c     
pmccabe ../SRCS/adv_1D.c  
pmccabe ../SRCS/adv_2D.c  
pmccabe ../SRCS/adv_3D.c  

pmccabe ../SRCS/finite_elements.c     
pmccabe ../SRCS/finite_elements_1D.c  
pmccabe ../SRCS/finite_elements_2D.c  
pmccabe ../SRCS/finite_elements_3D.c  

pmccabe ../SRCS/miscellaneous_1D.c  
pmccabe ../SRCS/miscellaneous_2D.c  
pmccabe ../SRCS/miscellaneous_3D.c  

pmccabe ../SRCS/algo_stokes.c  
pmccabe ../SRCS/algo_navierstokes.c  

#pmccabe ../SRCS/problems_functions.c  
#
pmccabe ../SRCS/problems_solvers1.c  
pmccabe ../SRCS/problems_solvers2.c  
pmccabe ../SRCS/problems_solvers3.c  
pmccabe ../SRCS/problems_solvers4.c  
pmccabe ../SRCS/problems_solvers5.c  

#pmccabe ../SRCS/functions_definitions.c  

pmccabe ../SRCS/all_params.c                
pmccabe ../SRCS/all_params_get_param_val.c  
pmccabe ../SRCS/all_params_set_param_val.c  

pmccabe ../SRCS/functions_structs.c     
pmccabe ../SRCS/functions_structs_1D.c  
pmccabe ../SRCS/functions_structs_2D.c  
pmccabe ../SRCS/functions_structs_3D.c  

pmccabe ../SRCS/apply_boundaryconditions_1D.c  
pmccabe ../SRCS/apply_boundaryconditions_2D.c  
pmccabe ../SRCS/apply_boundaryconditions_3D.c  

pmccabe ../SRCS/rhs.c     
pmccabe ../SRCS/rhs_1D.c  
pmccabe ../SRCS/rhs_2D.c  
pmccabe ../SRCS/rhs_3D.c  

pmccabe ../SRCS/assemblage_objects_1D.c  
pmccabe ../SRCS/assemblage_objects_2D.c  
pmccabe ../SRCS/assemblage_objects_3D.c  

pmccabe ../SRCS/geometry.c     
pmccabe ../SRCS/geometry_1D.c  
pmccabe ../SRCS/geometry_2D.c  
pmccabe ../SRCS/geometry_3D.c  
