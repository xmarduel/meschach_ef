#! /usr/bin/env sh
#
# all metrics
#
#rm -i metrics1.txt
#

mcstrip ../SRCS/boundary_conditions.c    | cyclo -c
mcstrip ../SRCS/boundary_conditions_1D.c | cyclo -c
mcstrip ../SRCS/boundary_conditions_2D.c | cyclo -c
mcstrip ../SRCS/boundary_conditions_3D.c | cyclo -c

mcstrip ../SRCS/graphics_output_1D.c | cyclo -c
mcstrip ../SRCS/graphics_output_2D.c | cyclo -c
mcstrip ../SRCS/graphics_output_3D.c | cyclo -c
mcstrip ../SRCS/graphics_geom_view.c | cyclo -c
mcstrip ../SRCS/graphics_matrix_patterns.c | cyclo -c

mcstrip ../SRCS/adv.c    | cyclo -c
mcstrip ../SRCS/adv_1D.c | cyclo -c
mcstrip ../SRCS/adv_2D.c | cyclo -c
mcstrip ../SRCS/adv_3D.c | cyclo -c

mcstrip ../SRCS/finite_elements.c    | cyclo -c
mcstrip ../SRCS/finite_elements_1D.c | cyclo -c
mcstrip ../SRCS/finite_elements_2D.c | cyclo -c
mcstrip ../SRCS/finite_elements_3D.c | cyclo -c

mcstrip ../SRCS/miscellaneous_1D.c | cyclo -c
mcstrip ../SRCS/miscellaneous_2D.c | cyclo -c
mcstrip ../SRCS/miscellaneous_3D.c | cyclo -c

mcstrip ../SRCS/algo_stokes.c | cyclo -c
mcstrip ../SRCS/algo_navierstokes.c | cyclo -c

#mcstrip ../SRCS/problems_functions.c | cyclo -c
#
mcstrip ../SRCS/problems_solvers1.c | cyclo -c
mcstrip ../SRCS/problems_solvers2.c | cyclo -c
mcstrip ../SRCS/problems_solvers3.c | cyclo -c
mcstrip ../SRCS/problems_solvers4.c | cyclo -c
mcstrip ../SRCS/problems_solvers5.c | cyclo -c

#mcstrip ../SRCS/functions_definitions.c | cyclo -c

mcstrip ../SRCS/all_params.c               | cyclo -c
mcstrip ../SRCS/all_params_get_param_val.c | cyclo -c
mcstrip ../SRCS/all_params_set_param_val.c | cyclo -c

mcstrip ../SRCS/functions_structs.c    | cyclo -c
mcstrip ../SRCS/functions_structs_1D.c | cyclo -c
mcstrip ../SRCS/functions_structs_2D.c | cyclo -c
mcstrip ../SRCS/functions_structs_3D.c | cyclo -c

mcstrip ../SRCS/apply_boundaryconditions_1D.c | cyclo -c
mcstrip ../SRCS/apply_boundaryconditions_2D.c | cyclo -c
mcstrip ../SRCS/apply_boundaryconditions_3D.c | cyclo -c

mcstrip ../SRCS/rhs.c    | cyclo -c
mcstrip ../SRCS/rhs_1D.c | cyclo -c
mcstrip ../SRCS/rhs_2D.c | cyclo -c
mcstrip ../SRCS/rhs_3D.c | cyclo -c

mcstrip ../SRCS/assemblage_objects_1D.c | cyclo -c
mcstrip ../SRCS/assemblage_objects_2D.c | cyclo -c
mcstrip ../SRCS/assemblage_objects_3D.c | cyclo -c

mcstrip ../SRCS/geometry.c    | cyclo -c
mcstrip ../SRCS/geometry_1D.c | cyclo -c
mcstrip ../SRCS/geometry_2D.c | cyclo -c
mcstrip ../SRCS/geometry_3D.c | cyclo -c
