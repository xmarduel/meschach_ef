#! /bin/sh
#
LIBRARY     = libslicot.a
#
FFLAGS = -DNDEBUG -g -Wall
#
RM = rm -f
AR = ar cru
RANLIB = ranlib
#
SRCS_DIR     = ROUTINES_SLICOT
INSTALL_OBJS = ROUTINES_SLICOT/OBJS
#
#
SRCS =  $(SRCS_DIR)/ab01nd.f \
	$(SRCS_DIR)/ib01my.f \
	$(SRCS_DIR)/ib01qd.f \
	$(SRCS_DIR)/mb01rd.f \
	$(SRCS_DIR)/mb01vd.f \
	$(SRCS_DIR)/mb04id.f \
	$(SRCS_DIR)/sb02mr.f \
	$(SRCS_DIR)/sb02oy.f \
	$(SRCS_DIR)/sb03mw.f \
	$(SRCS_DIR)/sb03ov.f \
	$(SRCS_DIR)/sb04mr.f \
	$(SRCS_DIR)/sb04nx.f \
	$(SRCS_DIR)/sb04qu.f \
	$(SRCS_DIR)/sb10dd.f \
	$(SRCS_DIR)/ab01od.f \
	$(SRCS_DIR)/ib01nd.f \
	$(SRCS_DIR)/ib01rd.f \
	$(SRCS_DIR)/mb01ru.f \
	$(SRCS_DIR)/mb02pd.f \
	$(SRCS_DIR)/mb04iy.f \
	$(SRCS_DIR)/sb02ms.f \
	$(SRCS_DIR)/sb02qd.f \
	$(SRCS_DIR)/sb03mx.f \
	$(SRCS_DIR)/sb03oy.f \
	$(SRCS_DIR)/sb04mu.f \
	$(SRCS_DIR)/sb04ny.f \
	$(SRCS_DIR)/sb04qy.f \
	$(SRCS_DIR)/sb10fd.f \
	$(SRCS_DIR)/ab13md.f \
	$(SRCS_DIR)/ib01od.f \
	$(SRCS_DIR)/ma02ad.f \
	$(SRCS_DIR)/mb01rx.f \
	$(SRCS_DIR)/mb02qy.f \
	$(SRCS_DIR)/mb04kd.f \
	$(SRCS_DIR)/sb02mt.f \
	$(SRCS_DIR)/sb02rd.f \
	$(SRCS_DIR)/sb03my.f \
	$(SRCS_DIR)/sb03qx.f \
	$(SRCS_DIR)/sb04mw.f \
	$(SRCS_DIR)/sb04pd.f \
	$(SRCS_DIR)/sb04rd.f \
	$(SRCS_DIR)/sb10pd.f \
	$(SRCS_DIR)/ib01ad.f \
	$(SRCS_DIR)/ib01oy.f \
	$(SRCS_DIR)/ma02ed.f \
	$(SRCS_DIR)/mb01ry.f \
	$(SRCS_DIR)/mb02ud.f \
	$(SRCS_DIR)/mb04nd.f \
	$(SRCS_DIR)/sb02nd.f \
	$(SRCS_DIR)/sb02ru.f \
	$(SRCS_DIR)/sb03od.f \
	$(SRCS_DIR)/sb03qy.f \
	$(SRCS_DIR)/sb04my.f \
	$(SRCS_DIR)/sb04px.f \
	$(SRCS_DIR)/sb04rv.f \
	$(SRCS_DIR)/sb10qd.f \
	$(SRCS_DIR)/ib01bd.f \
	$(SRCS_DIR)/ib01pd.f \
	$(SRCS_DIR)/ma02fd.f \
	$(SRCS_DIR)/mb01sd.f \
	$(SRCS_DIR)/mb03od.f \
	$(SRCS_DIR)/mb04ny.f \
	$(SRCS_DIR)/sb02od.f \
	$(SRCS_DIR)/sb02sd.f \
	$(SRCS_DIR)/sb03or.f \
	$(SRCS_DIR)/sb03sx.f \
	$(SRCS_DIR)/sb04nd.f \
	$(SRCS_DIR)/sb04py.f \
	$(SRCS_DIR)/sb04rw.f \
	$(SRCS_DIR)/sb10rd.f \
	$(SRCS_DIR)/ib01cd.f \
	$(SRCS_DIR)/ib01px.f \
	$(SRCS_DIR)/mb01pd.f \
	$(SRCS_DIR)/mb01td.f \
	$(SRCS_DIR)/mb03oy.f \
	$(SRCS_DIR)/mb04od.f \
	$(SRCS_DIR)/sb02ou.f \
	$(SRCS_DIR)/sb03md.f \
	$(SRCS_DIR)/sb03ot.f \
	$(SRCS_DIR)/sb03sy.f \
	$(SRCS_DIR)/sb04nv.f \
	$(SRCS_DIR)/sb04qd.f \
	$(SRCS_DIR)/sb04rx.f \
	$(SRCS_DIR)/select.f \
	$(SRCS_DIR)/ib01md.f \
	$(SRCS_DIR)/ib01py.f \
	$(SRCS_DIR)/mb01qd.f \
	$(SRCS_DIR)/mb01ud.f \
	$(SRCS_DIR)/mb03ud.f \
	$(SRCS_DIR)/mb04oy.f \
	$(SRCS_DIR)/sb02ov.f \
	$(SRCS_DIR)/sb03mv.f \
	$(SRCS_DIR)/sb03ou.f \
	$(SRCS_DIR)/sb04md.f \
	$(SRCS_DIR)/sb04nw.f \
	$(SRCS_DIR)/sb04qr.f \
	$(SRCS_DIR)/sb04ry.f \
	$(SRCS_DIR)/tb01wd.f \
	$(SRCS_DIR)/lsame.f \
	$(SRCS_DIR)/Ex-schur.f
#
OBJS = $(SRCS:.f=.o)
#
#
all:: $(LIBRARY)
#
$(LIBRARY): $(OBJS)
	@echo Creation of $(LIBRARY)
	@$(RM) $@
	@$(AR) $@ $(OBJS)
	@$(RANLIB) $@
	mv $@ $(SRCS_DIR)
	mv $(OBJS) $(INSTALL_OBJS)