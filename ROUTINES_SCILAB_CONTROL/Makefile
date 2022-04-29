#! /bin/sh
#
LIBRARY     = libscilabcontrol.a
#
FFLAGS = -DNDEBUG -g -Wall
#
RM = rm -f
AR = ar cru
RANLIB = ranlib
#
SRCS_DIR     = ROUTINES_SCILAB_CONTROL
INSTALL_OBJS = ROUTINES_SCILAB_CONTROL/OBJS
#
#
SRCS = $(SRCS_DIR)/coef.f \
	$(SRCS_DIR)/dgeco.f \
	$(SRCS_DIR)/drref.f \
	$(SRCS_DIR)/fout.f \
	$(SRCS_DIR)/inva.f \
	$(SRCS_DIR)/mzdivq.f \
	$(SRCS_DIR)/proj2.f \
	$(SRCS_DIR)/riccpack.f \
	$(SRCS_DIR)/sszer.f \
	$(SRCS_DIR)/wcerr.f \
	$(SRCS_DIR)/wpade.f \
	$(SRCS_DIR)/arl2.f \
	$(SRCS_DIR)/comqr3.f \
	$(SRCS_DIR)/dgedi.f \
	$(SRCS_DIR)/dsubsp.f \
	$(SRCS_DIR)/front.f \
	$(SRCS_DIR)/invtpl.f \
	$(SRCS_DIR)/newest.f \
	$(SRCS_DIR)/qhesz.f \
	$(SRCS_DIR)/ricd.f \
	$(SRCS_DIR)/storl2.f \
	$(SRCS_DIR)/wclmat.f \
	$(SRCS_DIR)/wpofa.f \
	$(SRCS_DIR)/arl2a.f \
	$(SRCS_DIR)/corth.f \
	$(SRCS_DIR)/dgefa.f \
	$(SRCS_DIR)/dsvdc.f \
	$(SRCS_DIR)/fstair.f \
	$(SRCS_DIR)/irow1.f \
	$(SRCS_DIR)/nextk.f \
	$(SRCS_DIR)/qitz.f \
	$(SRCS_DIR)/rilac.f \
	$(SRCS_DIR)/sybad.f \
	$(SRCS_DIR)/wdegre.f \
	$(SRCS_DIR)/wqrdc.f \
	$(SRCS_DIR)/balanc.f \
	$(SRCS_DIR)/cortr.f \
	$(SRCS_DIR)/dgesl.f \
	$(SRCS_DIR)/dzdivq.f \
	$(SRCS_DIR)/fxshfr.f \
	$(SRCS_DIR)/irow2.f \
	$(SRCS_DIR)/onface.f \
	$(SRCS_DIR)/quad.f \
	$(SRCS_DIR)/rootgp.f \
	$(SRCS_DIR)/sydsr.f \
	$(SRCS_DIR)/wesidu.f \
	$(SRCS_DIR)/wqrsl.f \
	$(SRCS_DIR)/balbak.f \
	$(SRCS_DIR)/dclmat.f \
	$(SRCS_DIR)/dhetr.f \
	$(SRCS_DIR)/ereduc.f \
	$(SRCS_DIR)/giv.f \
	$(SRCS_DIR)/jacl2.f \
	$(SRCS_DIR)/optml2.f \
	$(SRCS_DIR)/quadit.f \
	$(SRCS_DIR)/rpoly.f \
	$(SRCS_DIR)/syhsc.f \
	$(SRCS_DIR)/wexchn.f \
	$(SRCS_DIR)/wrref.f \
	$(SRCS_DIR)/bdiag.f \
	$(SRCS_DIR)/deg1l2.f \
	$(SRCS_DIR)/dlslv.f \
	$(SRCS_DIR)/exch.f \
	$(SRCS_DIR)/hessl2.f \
	$(SRCS_DIR)/lq.f \
	$(SRCS_DIR)/orthes.f \
	$(SRCS_DIR)/quadsd.f \
	$(SRCS_DIR)/rtitr.f \
	$(SRCS_DIR)/tild.f \
	$(SRCS_DIR)/wexpm1.f \
	$(SRCS_DIR)/wshrsl.f \
	$(SRCS_DIR)/bezous.f \
	$(SRCS_DIR)/degl2.f \
	$(SRCS_DIR)/domout.f \
	$(SRCS_DIR)/exchqz.f \
	$(SRCS_DIR)/hhdml.f \
	$(SRCS_DIR)/lrow2.f \
	$(SRCS_DIR)/ortran.f \
	$(SRCS_DIR)/qvalz.f \
	$(SRCS_DIR)/scaleg.f \
	$(SRCS_DIR)/tql2.f \
	$(SRCS_DIR)/wgeco.f \
	$(SRCS_DIR)/wsvdc.f \
	$(SRCS_DIR)/calcsc.f \
	$(SRCS_DIR)/dexpm1.f \
	$(SRCS_DIR)/dpofa.f \
	$(SRCS_DIR)/expan.f \
	$(SRCS_DIR)/hqror2.f \
	$(SRCS_DIR)/lybad.f \
	$(SRCS_DIR)/outl2.f \
	$(SRCS_DIR)/qvecz.f \
	$(SRCS_DIR)/scapol.f \
	$(SRCS_DIR)/tred2.f \
	$(SRCS_DIR)/wgedi.f \
	$(SRCS_DIR)/calsca.f \
	$(SRCS_DIR)/dfrmg.f \
	$(SRCS_DIR)/dqrdc.f \
	$(SRCS_DIR)/feq.f \
	$(SRCS_DIR)/htribk.f \
	$(SRCS_DIR)/lybsc.f \
	$(SRCS_DIR)/pade.f \
	$(SRCS_DIR)/qzk.f \
	$(SRCS_DIR)/shrslv.f \
	$(SRCS_DIR)/watfac.f \
	$(SRCS_DIR)/wgefa.f \
	$(SRCS_DIR)/cbal.f \
	$(SRCS_DIR)/dgbfa.f \
	$(SRCS_DIR)/dqrsl.f \
	$(SRCS_DIR)/find.f \
	$(SRCS_DIR)/htridi.f \
	$(SRCS_DIR)/lydsr.f \
	$(SRCS_DIR)/phi.f \
	$(SRCS_DIR)/realit.f \
	$(SRCS_DIR)/split.f \
	$(SRCS_DIR)/wbalin.f \
	$(SRCS_DIR)/wgesl.f \
	$(SRCS_DIR)/cerr.f \
	$(SRCS_DIR)/dgbsl.f \
	$(SRCS_DIR)/dqrsm.f \
	$(SRCS_DIR)/folhp.f \
	$(SRCS_DIR)/imtql3.f \
	$(SRCS_DIR)/modul.f \
	$(SRCS_DIR)/polmc.f \
	$(SRCS_DIR)/reduc2.f \
	$(SRCS_DIR)/ssxmc.f \
	$(SRCS_DIR)/wbdiag.f \
	$(SRCS_DIR)/wlslv.f \
        $(SRCS_DIR)/dmmul.f
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