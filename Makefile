
PWD              = .
CWD              = .
TOP              = .

INSTALLDIR       = .

include ./LINUX_MAKEFILES/mk.platform

# - where ALL build artefacts go
BUILDDIR        = $(CWD)/0BUILD.$(PLATFORM_KEY)
BUILDDIRS       = $(CWD)/0BUILD.*

LIB_MESCHACH = libmeschach.a
LIB_MESCHACH_ADDS = libmeschach_adds.a
LIB_MESCHACH_EF = libmeschach_ef.a
LIB_MESCHACH_SPOOLES = libmeschach_spooles.a

DRIVER_1D = driver_1D.exe
DRIVER_2D = driver_2D.exe
DRIVER_3D = driver_3D.exe






DEFINES = -DHAVE_LUA -DANSI -DMESCHACH__ITER_EXTENSIONS 

NO_WARNINGS =  -Wno-format \
		-Wno-parentheses \
		-Wno-pointer-sign \
		-Wno-incompatible-pointer-types \
		-Wno-incompatible-pointer-types-discards-qualifiers

CFLAGS += $(DEFINES)  $(NO_WARNINGS) \
	-g \
	-IMESCHACH/INCLUDES \
	-I. \
	-I./EXTERNAL_LIBS/amd \
	-I./EXTERNAL_LIBS/jansson-2.13/src \
	-I./EXTERNAL_LIBS/lua-5.4.3/src \
	-I./EXTERNAL_LIBS \
	-I./EXTERNAL_LIBS/spooles

LDFLAGS = -L./LINUX_MAKEFILES \
	-L./EXTERNAL_LIBS/amd \
	-L./EXTERNAL_LIBS/lua-5.4.3/src \
	-L./EXTERNAL_LIBS/jansson-2.13/src/.libs \
	-L./EXTERNAL_LIBS/spooles/ \
	-L./EXTERNAL_LIBS/spooles/LinSol/srcST \
	-L./EXTERNAL_LIBS/spooles/LinSol/srcMT


all: libmeschach libmeschach_adds libmeschach_ef libmeschach_spooles driver_1D driver_2D driver_3D

clean:
	$(RM) -rf $(BUILDDIR) ./LINUX_MAKEFILES/libmeschach.a ./LINUX_MAKEFILES/libmeschach_adds.a ./LINUX_MAKEFILES/libmeschach_spooles.a ./LINUX_MAKEFILES/libmeschach_ef.a  ./LINUX_MAKEFILESd/driver_1D.exe

libmeschach: $(LIB_MESCHACH)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH) $(LIB_MESCHACH.objs)

libmeschach_adds: $(LIB_MESCHACH_ADDS)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_ADDS) $(LIB_MESCHACH_ADDS.objs)

libmeschach_spooles: $(LIB_MESCHACH_SPOOLES)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_SPOOLES) $(LIB_MESCHACH_SPOOLES.objs)

libmeschach_ef: $(LIB_MESCHACH_EF)
	ar rcs LINUX_MAKEFILES/$(LIB_MESCHACH_EF) $(LIB_MESCHACH_EF.objs)

driver_1D: $(DRIVER_1D)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_1D) $(DRIVER_1D.obj) -lmeschach_ef -lmeschach_adds -lmeschach -lamd -ljansson -llua

driver_2D: $(DRIVER_2D)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_2D) $(DRIVER_2D.obj) -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge

driver_3D: $(DRIVER_3D)
	gcc $(LDFLAGS) -o LINUX_MAKEFILES/$(DRIVER_3D) $(DRIVER_3D.obj) -lmeschach_ef -lmeschach_spooles -lmeschach_adds -lmeschach -lamd -ljansson -llua -lspooles -lBridge


DRIVER_1D.src = DRIVERS_C/1D/PDE_1D_SOLVE.c
DRIVER_2D.src = DRIVERS_C/2D/PDE_2D_SOLVE.c
DRIVER_3D.src = DRIVERS_C/3D/PDE_3D_SOLVE.c

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

LIB_MESCHACH.objs         = $(LIB_MESCHACH.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_ADDS.objs    = $(LIB_MESCHACH_ADDS.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_SPOOLES.objs = $(LIB_MESCHACH_SPOOLES.srcs:%.c=$(BUILDDIR)/%_st.o)
LIB_MESCHACH_EF.objs      = $(LIB_MESCHACH_EF.srcs:%.c=$(BUILDDIR)/%_st.o)

DRIVER_1D.obj     = $(DRIVER_1D.src:%.c=$(BUILDDIR)/%_st.o)
DRIVER_2D.obj     = $(DRIVER_2D.src:%.c=$(BUILDDIR)/%_st.o)
DRIVER_3D.obj     = $(DRIVER_3D.src:%.c=$(BUILDDIR)/%_st.o)

# ------------------------------------------------------------------------

$(LIB_MESCHACH): $(LIB_MESCHACH.objs)
$(LIB_MESCHACH_ADDS): $(LIB_MESCHACH_ADDS.objs)
$(LIB_MESCHACH_SPOOLES): $(LIB_MESCHACH_SPOOLES.objs)
$(LIB_MESCHACH_EF): $(LIB_MESCHACH_EF.objs)

$(DRIVER_1D): $(DRIVER_1D.obj)
$(DRIVER_2D): $(DRIVER_2D.obj)
$(DRIVER_3D): $(DRIVER_3D.obj)

include ./LINUX_MAKEFILES/mk.subdirs
include ./LINUX_MAKEFILES/mk.rules
