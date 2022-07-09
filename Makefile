
PWD              = .
CWD              = .
TOP              = .

# - where ALL build artefacts go
BUILDDIR        = $(CWD)/0BUILD.x86-linux64

LIB_MESCHACH            = libmeschach.a
LIB_MESCHACH_ADDS       = libmeschach_adds.a
LIB_MESCHACH_SPOOLES    = libmeschach_spooles.a
LIB_MESCHACH_EF         = libmeschach_ef.a
LIBSCIPLOT              = libsciplot.a
LIB_MESCHACH_LIBSCIPLOT = libmeschach_libsciplot.a
LIBVOGLE                = libvogle.a
LIBVOPL                 = libvopl.a
LIB_MESCHACH_VOGLE      = libmeschach_vogle.a
LIB_MESCHACH_CPGPLOT    = libmeschach_cpgplot.a

DRIVER_1D = driver_1D.exe
DRIVER_2D = driver_2D.exe
DRIVER_3D = driver_3D.exe
DRIVER_1D_TRANSIENT = driver_1D_TRANSIENT.exe
DRIVER_2D_TRANSIENT = driver_2D_TRANSIENT.exe

# --------------------------------------------------------------

DEFINES = -DHAVE_X11 -DHAVE_CPGPLOT -DHAVE_VOGLE -DHAVE_LIBSCIPLOT -DHAVE_LUA -DANSI -DMESCHACH__ITER_EXTENSIONS

NO_WARNINGS =  -Wno-format \
	-Wno-parentheses \
	-Wno-pointer-sign \
	-Wno-incompatible-pointer-types \
	-Wno-incompatible-pointer-types-discards-qualifiers

#CFLAGS_OPT = -g
CFLAGS_OPT = -O3

CFLAGS += $(CFLAGS_OPT) \
	$(DEFINES)  $(NO_WARNINGS) \
	-I./MESCHACH/INCLUDES \
	-I. \
	-I./EXTERNAL_LIBS/amd \
	-I./EXTERNAL_LIBS/jansson-2.13/src \
	-I./EXTERNAL_LIBS/lua-5.4.3/src \
	-I./EXTERNAL_LIBS \
	-I./EXTERNAL_LIBS/spooles \
	-I./EXTERNAL_LIBS/pgplot_srcs \
	-I/opt/X11/include \
	-I./MESCHACH_LIBSCIPLOT/LIBSCIPLOT \
	-I./EXTERNAL_LIBS/plotutils-2.6 \
	-I./EXTERNAL_LIBS/plotutils-2.6/include \
	-I./MESCHACH_VOGLE/VOGLE/src \
	-I./MESCHACH_VOGLE/VOPL/SRC

LDFLAGS = -L./LINUX_MAKEFILES \
	-L./EXTERNAL_LIBS/amd \
	-L./EXTERNAL_LIBS/lua-5.4.3/src \
	-L./EXTERNAL_LIBS/jansson-2.13/src/.libs \
	-L./EXTERNAL_LIBS/spooles/ \
	-L./EXTERNAL_LIBS/spooles/LinSol/srcST \
	-L./EXTERNAL_LIBS/spooles/LinSol/srcMT \
	-L./EXTERNAL_LIBS/plotutils-2.6/lib \
	-L./EXTERNAL_LIBS/plotutils-2.6/libplot/.libs \
	-L./EXTERNAL_LIBS/pgplot \
	-L/usr/local/gfortran/lib \
	-L/opt/X11/lib 


all: libmeschach libmeschach_adds libmeschach_spooles libmeschach_ef libsciplot libmeschach_libsciplot libvogle libvopl libmeschach_vogle libmeschach_cpgplot driver_1D driver_2D driver_3D driver_1D_TRANSIENT driver_2D_TRANSIENT

clean:
	$(RM) -rf $(BUILDDIR) ./LINUX_MAKEFILES/libmeschach.a ./LINUX_MAKEFILES/libmeschach_adds.a ./LINUX_MAKEFILES/libmeschach_spooles.a ./LINUX_MAKEFILES/libmeschach_ef.a ./LINUX_MAKEFILES/libsciplot.a ./LINUX_MAKEFILES/libmeschach_libsciplot.a ./LINUX_MAKEFILES/libvogle.a  ./LINUX_MAKEFILES/libvopl.a ./LINUX_MAKEFILES/libmeschach_vogle.a ./LINUX_MAKEFILES/libmeschach_cpgplot.a ./LINUX_MAKEFILES/driver_1D.exe ./LINUX_MAKEFILES/driver_2D.exe ./LINUX_MAKEFILES/driver_3D.exe ./LINUX_MAKEFILES/driver_1D_TRANSIENT.exe ./LINUX_MAKEFILES/driver_2D_TRANSIENT.exe

libmeschach: $(LIB_MESCHACH)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH) $(LIB_MESCHACH.objs)

libmeschach_adds: $(LIB_MESCHACH_ADDS)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_ADDS) $(LIB_MESCHACH_ADDS.objs)

libmeschach_spooles: $(LIB_MESCHACH_SPOOLES)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_SPOOLES) $(LIB_MESCHACH_SPOOLES.objs)

libmeschach_ef: $(LIB_MESCHACH_EF)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_EF) $(LIB_MESCHACH_EF.objs)

libsciplot: $(LIBSCIPLOT)
	ar rcs LINUX_MAKEFILES/$(LIBSCIPLOT) $(LIBSCIPLOT.objs)

libmeschach_libsciplot: $(LIB_MESCHACH_LIBSCIPLOT)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_LIBSCIPLOT) $(LIB_MESCHACH_LIBSCIPLOT.objs)

libvogle: $(LIBVOGLE)
	ar rcs LINUX_MAKEFILES/$(LIBVOGLE) $(LIBVOGLE.objs)

libvopl: $(LIBVOPL)
	ar rcs LINUX_MAKEFILES/$(LIBVOPL) $(LIBVOPL.objs)

libmeschach_vogle: $(LIB_MESCHACH_VOGLE)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_VOGLE) $(LIB_MESCHACH_VOGLE.objs)

libmeschach_cpgplot: $(LIB_MESCHACH_CPGPLOT)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_CPGPLOT) $(LIB_MESCHACH_CPGPLOT.objs)

driver_1D: $(DRIVER_1D)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_1D) $(DRIVER_1D.obj) -lmeschach_cpgplot -lXaPgplot -lcpgplot -lpgplot -lgfortran -lmeschach_vogle -lvopl -lvogle -lmeschach_libsciplot -lsciplot -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge -lplot -lcommon -lXaw -lXt -lX11

driver_2D: $(DRIVER_2D)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_2D) $(DRIVER_2D.obj) -lmeschach_cpgplot -lXaPgplot -lcpgplot -lpgplot -lgfortran  -lmeschach_vogle -lvopl -lvogle -lmeschach_libsciplot -lsciplot -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge -lplot -lcommon -lXaw -lXt -lX11

driver_3D: $(DRIVER_3D)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_3D) $(DRIVER_3D.obj) -lmeschach_cpgplot -lXaPgplot -lcpgplot -lpgplot -lgfortran -lmeschach_vogle -lvopl -lvogle -lmeschach_libsciplot -lsciplot -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge -lplot -lcommon -lXaw -lXt -lX11

driver_1D_TRANSIENT: $(DRIVER_1D_TRANSIENT)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_1D_TRANSIENT) $(DRIVER_1D_TRANSIENT.obj) -lmeschach_cpgplot -lXaPgplot -lcpgplot -lpgplot -lgfortran -lmeschach_vogle -lvopl -lvogle -lmeschach_libsciplot -lsciplot -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge -lplot -lcommon -lXaw -lXt -lX11

driver_2D_TRANSIENT: $(DRIVER_2D_TRANSIENT)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_2D_TRANSIENT) $(DRIVER_2D_TRANSIENT.obj) -lmeschach_cpgplot -lXaPgplot -lcpgplot -lpgplot -lgfortran -lmeschach_vogle -lvopl -lvogle -lmeschach_libsciplot -lsciplot -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge -lplot -lcommon -lXaw -lXt -lX11


DRIVER_1D.src = DRIVERS_C/1D/PDE_1D_SOLVE.c
DRIVER_2D.src = DRIVERS_C/2D/PDE_2D_SOLVE.c
DRIVER_3D.src = DRIVERS_C/3D/PDE_3D_SOLVE.c
DRIVER_1D_TRANSIENT.src = DRIVERS_C/1D_TRANSIENT/HEAT_1D.c
DRIVER_2D_TRANSIENT.src = DRIVERS_C/2D_TRANSIENT/HEAT_2D.c

LIB_MESCHACH.srcs        = \
	MESCHACH/SRCS/bdfactor.c \
	MESCHACH/SRCS/bkpfactor.c \
	MESCHACH/SRCS/chfactor.c \
	MESCHACH/SRCS/copy.c \
	MESCHACH/SRCS/err.c \
	MESCHACH/SRCS/extra.c \
	MESCHACH/SRCS/fft.c \
	MESCHACH/SRCS/givens.c \
	MESCHACH/SRCS/hessen.c \
	MESCHACH/SRCS/hsehldr.c \
	MESCHACH/SRCS/init.c \
	MESCHACH/SRCS/iter0.c \
	MESCHACH/SRCS/iternsym.c \
	MESCHACH/SRCS/itersym.c \
	MESCHACH/SRCS/ivecop.c \
	MESCHACH/SRCS/lufactor.c \
	MESCHACH/SRCS/machine.c \
	MESCHACH/SRCS/matlab.c \
	MESCHACH/SRCS/matop.c \
	MESCHACH/SRCS/matrixio.c \
	MESCHACH/SRCS/mfunc.c \
	MESCHACH/SRCS/meminfo.c \
	MESCHACH/SRCS/memory.c \
	MESCHACH/SRCS/memstat.c\
	MESCHACH/SRCS/norm.c \
	MESCHACH/SRCS/otherio.c \
	MESCHACH/SRCS/pxop.c \
	MESCHACH/SRCS/qrfactor.c \
	MESCHACH/SRCS/schur.c \
	MESCHACH/SRCS/solve.c \
	MESCHACH/SRCS/sparse.c \
	MESCHACH/SRCS/sparseio.c \
	MESCHACH/SRCS/spbkp.c \
	MESCHACH/SRCS/spchfctr.c \
	MESCHACH/SRCS/splufctr.c \
	MESCHACH/SRCS/spswap.c \
	MESCHACH/SRCS/sprow.c \
	MESCHACH/SRCS/submat.c \
	MESCHACH/SRCS/svd.c \
	MESCHACH/SRCS/symmeig.c \
	MESCHACH/SRCS/update.c \
	MESCHACH/SRCS/vecop.c \
	MESCHACH/SRCS/version.c \
	MESCHACH/SRCS/zcopy.c \
	MESCHACH/SRCS/zfunc.c \
	MESCHACH/SRCS/zgivens.c \
	MESCHACH/SRCS/zhessen.c \
	MESCHACH/SRCS/zhsehldr.c \
	MESCHACH/SRCS/zlufctr.c \
	MESCHACH/SRCS/zmachine.c \
	MESCHACH/SRCS/zmatio.c \
	MESCHACH/SRCS/zmatlab.c \
	MESCHACH/SRCS/zmatop.c \
	MESCHACH/SRCS/zmemory.c \
	MESCHACH/SRCS/znorm.c \
	MESCHACH/SRCS/zqrfctr.c \
	MESCHACH/SRCS/zschur.c \
	MESCHACH/SRCS/zsolve.c \
	MESCHACH/SRCS/zvecop.c

LIB_MESCHACH_ADDS.srcs        = \
        MESCHACH_ADDS/SRCS/amd_wrapper.c \
        MESCHACH_ADDS/SRCS/control.c \
        MESCHACH_ADDS/SRCS/eigen.c \
        MESCHACH_ADDS/SRCS/gpskmesh.c \
        MESCHACH_ADDS/SRCS/iter_adds.c \
        MESCHACH_ADDS/SRCS/lock.c \
        MESCHACH_ADDS/SRCS/matrix_adds.c \
        MESCHACH_ADDS/SRCS/queue.c \
        MESCHACH_ADDS/SRCS/sparse2_adds.c \
        MESCHACH_ADDS/SRCS/spmat2formats.c \
        MESCHACH_ADDS/SRCS/sparse_adds.c \
        MESCHACH_ADDS/SRCS/strfuncs.c

LIB_MESCHACH_SPOOLES.srcs        = \
        MESCHACH_SPOOLES/SRCS/spooles_wrapper.c \
        MESCHACH_SPOOLES/SRCS/spooles_wrapper_eigen.c \
        MESCHACH_SPOOLES/SRCS/spooles_wrapper_factor.c

LIB_MESCHACH_EF.srcs        = \
        MESCHACH_EF/SRCS/adv.c \
        MESCHACH_EF/SRCS/adv_1D.c \
        MESCHACH_EF/SRCS/adv_2D.c \
        MESCHACH_EF/SRCS/adv_3D.c \
        MESCHACH_EF/SRCS/algo_navierstokes.c \
        MESCHACH_EF/SRCS/algo_stokes.c \
        MESCHACH_EF/SRCS/all_params.c \
        MESCHACH_EF/SRCS/all_params_get_param_val.c \
        MESCHACH_EF/SRCS/all_params_json_config.c \
        MESCHACH_EF/SRCS/all_params_json_utils.c \
        MESCHACH_EF/SRCS/all_params_set_param_val.c \
        MESCHACH_EF/SRCS/apply_boundaryconditions_1D.c \
        MESCHACH_EF/SRCS/apply_boundaryconditions_2D.c \
        MESCHACH_EF/SRCS/apply_boundaryconditions_3D.c \
        MESCHACH_EF/SRCS/assemblage_objects_1D.c \
        MESCHACH_EF/SRCS/assemblage_objects_2D.c \
        MESCHACH_EF/SRCS/assemblage_objects_3D.c \
        MESCHACH_EF/SRCS/boundary_conditions.c \
        MESCHACH_EF/SRCS/boundary_conditions_1D.c \
        MESCHACH_EF/SRCS/boundary_conditions_2D.c \
        MESCHACH_EF/SRCS/boundary_conditions_3D.c \
        MESCHACH_EF/SRCS/finite_elements.c \
        MESCHACH_EF/SRCS/finite_elements_1D.c \
        MESCHACH_EF/SRCS/finite_elements_2D.c \
        MESCHACH_EF/SRCS/finite_elements_3D.c \
        MESCHACH_EF/SRCS/functions_definitions.c \
        MESCHACH_EF/SRCS/functions_structs.c \
        MESCHACH_EF/SRCS/functions_structs_1D.c \
        MESCHACH_EF/SRCS/functions_structs_2D.c \
        MESCHACH_EF/SRCS/functions_structs_3D.c \
        MESCHACH_EF/SRCS/geometry.c \
        MESCHACH_EF/SRCS/geometry_1D.c \
        MESCHACH_EF/SRCS/geometry_2D.c \
        MESCHACH_EF/SRCS/geometry_3D.c \
        MESCHACH_EF/SRCS/graphics_geom_view.c \
        MESCHACH_EF/SRCS/graphics_matrix_patterns.c \
        MESCHACH_EF/SRCS/graphics_output.c \
        MESCHACH_EF/SRCS/graphics_output_1D.c \
        MESCHACH_EF/SRCS/graphics_output_2D.c \
        MESCHACH_EF/SRCS/graphics_output_3D.c \
        MESCHACH_EF/SRCS/miscellaneous_1D.c \
        MESCHACH_EF/SRCS/miscellaneous_2D.c \
        MESCHACH_EF/SRCS/miscellaneous_3D.c \
        MESCHACH_EF/SRCS/problems_solvers1.c \
        MESCHACH_EF/SRCS/problems_solvers2.c \
        MESCHACH_EF/SRCS/problems_solvers3.c \
        MESCHACH_EF/SRCS/problems_solvers4.c \
        MESCHACH_EF/SRCS/problems_solvers5.c \
        MESCHACH_EF/SRCS/rhs.c \
        MESCHACH_EF/SRCS/rhs_1D.c \
        MESCHACH_EF/SRCS/rhs_2D.c \
        MESCHACH_EF/SRCS/rhs_3D.c \
        MESCHACH_EF/SRCS/time_scheme.c

LIBSCIPLOT.srcs        = \
        MESCHACH_LIBSCIPLOT/LIBSCIPLOT/libsciplot.c \
        MESCHACH_LIBSCIPLOT/LIBSCIPLOT/linemode.c \
        MESCHACH_LIBSCIPLOT/LIBSCIPLOT/misc.c \
        MESCHACH_LIBSCIPLOT/LIBSCIPLOT/fontlist.c \
        MESCHACH_LIBSCIPLOT/LIBSCIPLOT/plotter.c \
        MESCHACH_LIBSCIPLOT/LIBSCIPLOT/reader.c

LIB_MESCHACH_LIBSCIPLOT.srcs        = \
        MESCHACH_LIBSCIPLOT/SRCS/graphics1D_libsciplot.c \
        MESCHACH_LIBSCIPLOT/SRCS/graphics1D_libsciplot_svqueue.c \
        MESCHACH_LIBSCIPLOT/SRCS/graphics1D_libsciplot_stationnary.c \
        MESCHACH_LIBSCIPLOT/SRCS/graphics1D_libsciplot_stationnary_xt_window.c \
        MESCHACH_LIBSCIPLOT/SRCS/graphics1D_libsciplot_transient.c \
        MESCHACH_LIBSCIPLOT/SRCS/graphics1D_libsciplot_transient_xt_window.c \

LIBVOGLE.srcs        = \
        MESCHACH_VOGLE/VOGLE/drivers/X11.c \
        MESCHACH_VOGLE/VOGLE/drivers/gif.c \
        MESCHACH_VOGLE/VOGLE/drivers/ppm.c \
        MESCHACH_VOGLE/VOGLE/src/arcs.c \
        MESCHACH_VOGLE/VOGLE/src/aspect.c \
        MESCHACH_VOGLE/VOGLE/src/attr.c \
        MESCHACH_VOGLE/VOGLE/src/buffer.c \
        MESCHACH_VOGLE/VOGLE/src/clip.c \
        MESCHACH_VOGLE/VOGLE/src/curves.c \
        MESCHACH_VOGLE/VOGLE/src/draw.c \
        MESCHACH_VOGLE/VOGLE/src/drivers.c \
        MESCHACH_VOGLE/VOGLE/src/getgp.c \
        MESCHACH_VOGLE/VOGLE/src/getstr.c \
        MESCHACH_VOGLE/VOGLE/src/getstring.c \
        MESCHACH_VOGLE/VOGLE/src/mapping.c \
        MESCHACH_VOGLE/VOGLE/src/matrix.c \
        MESCHACH_VOGLE/VOGLE/src/move.c \
        MESCHACH_VOGLE/VOGLE/src/newtoken.c \
        MESCHACH_VOGLE/VOGLE/src/objects.c \
        MESCHACH_VOGLE/VOGLE/src/patches.c \
        MESCHACH_VOGLE/VOGLE/src/points.c \
        MESCHACH_VOGLE/VOGLE/src/polygons.c \
        MESCHACH_VOGLE/VOGLE/src/pref.c \
        MESCHACH_VOGLE/VOGLE/src/rect.c \
        MESCHACH_VOGLE/VOGLE/src/scale.c \
        MESCHACH_VOGLE/VOGLE/src/tensor.c \
        MESCHACH_VOGLE/VOGLE/src/text.c \
        MESCHACH_VOGLE/VOGLE/src/trans.c \
        MESCHACH_VOGLE/VOGLE/src/valloc.c \
        MESCHACH_VOGLE/VOGLE/src/verror.c \
        MESCHACH_VOGLE/VOGLE/src/viewing.c \
        MESCHACH_VOGLE/VOGLE/src/viewp.c \
        MESCHACH_VOGLE/VOGLE/src/window.c \
        MESCHACH_VOGLE/VOGLE/src/yobbaray.c

LIBVOPL.srcs        = \
        MESCHACH_VOGLE/VOPL/SRC/adjustscale.c \
        MESCHACH_VOGLE/VOPL/SRC/axic.c \
        MESCHACH_VOGLE/VOPL/SRC/contour.c \
        MESCHACH_VOGLE/VOPL/SRC/cubicsp.c \
        MESCHACH_VOGLE/VOPL/SRC/ilinfit.c \
        MESCHACH_VOGLE/VOPL/SRC/lsfit.c \
        MESCHACH_VOGLE/VOPL/SRC/mem.c \
        MESCHACH_VOGLE/VOPL/SRC/plot.c \
        MESCHACH_VOGLE/VOPL/SRC/savestr.c \
        MESCHACH_VOGLE/VOPL/SRC/scalers.c \
        MESCHACH_VOGLE/VOPL/SRC/spline.c \
        MESCHACH_VOGLE/VOPL/SRC/switches.c \
        MESCHACH_VOGLE/VOPL/SRC/vopl.c \
        MESCHACH_VOGLE/VOPL/SRC/vopl_error.c

LIB_MESCHACH_VOGLE.srcs        = \
        MESCHACH_VOGLE/SRCS/graphics1D_vogle.c \
        MESCHACH_VOGLE/SRCS/graphics_vogle_svqueue.c \
        MESCHACH_VOGLE/SRCS/graphics1D_vogle_stationnary.c \
        MESCHACH_VOGLE/SRCS/graphics1D_vogle_stationnary_xt_window.c \
        MESCHACH_VOGLE/SRCS/graphics1D_vogle_transient.c \
        MESCHACH_VOGLE/SRCS/graphics1D_vogle_transient_xt_window.c \
        MESCHACH_VOGLE/SRCS/graphics2D_vogle.c \
        MESCHACH_VOGLE/SRCS/graphics2D_vogle_stationnary.c \
        MESCHACH_VOGLE/SRCS/graphics2D_vogle_stationnary_xt_window.c \
        MESCHACH_VOGLE/SRCS/graphics2D_vogle_transient.c \
        MESCHACH_VOGLE/SRCS/graphics2D_vogle_transient_xt_window.c \
        MESCHACH_VOGLE/SRCS/graphics1D_voplcontour_stationnary_xt_window.c \
        MESCHACH_VOGLE/SRCS/graphics1D_voplcontour_transient_xt_window.c

LIB_MESCHACH_CPGPLOT.srcs        = \
        MESCHACH_CPGPLOT/SRCS/graphics1D_pgplot.c \
	MESCHACH_CPGPLOT/SRCS/graphics1D_pgplot_stationnary.c \
	MESCHACH_CPGPLOT/SRCS/graphics1D_pgplot_stationnary_xt_window.c \
	MESCHACH_CPGPLOT/SRCS/graphics1D_pgplot_svqueue.c \
	MESCHACH_CPGPLOT/SRCS/graphics1D_pgplot_transient.c \
	MESCHACH_CPGPLOT/SRCS/graphics1D_pgplot_transient_xt_window.c


LIB_MESCHACH.objs            = $(LIB_MESCHACH.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_ADDS.objs       = $(LIB_MESCHACH_ADDS.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_SPOOLES.objs    = $(LIB_MESCHACH_SPOOLES.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_EF.objs         = $(LIB_MESCHACH_EF.srcs:%.c=$(BUILDDIR)/%_st.o)
LIBSCIPLOT.objs              = $(LIBSCIPLOT.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_LIBSCIPLOT.objs = $(LIB_MESCHACH_LIBSCIPLOT.srcs:%.c=$(BUILDDIR)/%_st.o)
LIBVOGLE.objs                = $(LIBVOGLE.srcs:%.c=$(BUILDDIR)/%_st.o)
LIBVOPL.objs                 = $(LIBVOPL.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_VOGLE.objs      = $(LIB_MESCHACH_VOGLE.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_CPGPLOT.objs    = $(LIB_MESCHACH_CPGPLOT.srcs:%.c=$(BUILDDIR)/%_st.o)

DRIVER_1D.obj           = $(DRIVER_1D.src:%.c=$(BUILDDIR)/%_st.o)
DRIVER_2D.obj           = $(DRIVER_2D.src:%.c=$(BUILDDIR)/%_st.o)
DRIVER_3D.obj           = $(DRIVER_3D.src:%.c=$(BUILDDIR)/%_st.o)
DRIVER_1D_TRANSIENT.obj = $(DRIVER_1D_TRANSIENT.src:%.c=$(BUILDDIR)/%_st.o)
DRIVER_2D_TRANSIENT.obj = $(DRIVER_2D_TRANSIENT.src:%.c=$(BUILDDIR)/%_st.o)

# ------------------------------------------------------------------------

$(LIB_MESCHACH): $(LIB_MESCHACH.objs)
$(LIB_MESCHACH_ADDS): $(LIB_MESCHACH_ADDS.objs)
$(LIB_MESCHACH_SPOOLES): $(LIB_MESCHACH_SPOOLES.objs)
$(LIB_MESCHACH_EF): $(LIB_MESCHACH_EF.objs)
$(LIBSCIPLOT): $(LIBSCIPLOT.objs)
$(LIB_MESCHACH_LIBSCIPLOT): $(LIB_MESCHACH_LIBSCIPLOT.objs)
$(LIBVOGLE): $(LIBVOGLE.objs)
$(LIBVOPL): $(LIBVOPL.objs)
$(LIB_MESCHACH_VOGLE): $(LIB_MESCHACH_VOGLE.objs)
$(LIB_MESCHACH_CPGPLOT): $(LIB_MESCHACH_CPGPLOT.objs)

$(DRIVER_1D): $(DRIVER_1D.obj)
$(DRIVER_2D): $(DRIVER_2D.obj)
$(DRIVER_3D): $(DRIVER_3D.obj)
$(DRIVER_1D_TRANSIENT): $(DRIVER_1D_TRANSIENT.obj)
$(DRIVER_2D_TRANSIENT): $(DRIVER_2D_TRANSIENT.obj)

include ./LINUX_MAKEFILES/mk.rules
