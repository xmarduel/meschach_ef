
SWIG_DIR = $(HOME)/x_local/bin/swig-1.3.40

SWIG_EXE = $(SWIG_DIR)/swig

SWIG_INC = -I$(TOP)/PYTHON_EXTENSIONS \
	-I$(SWIG_DIR)/Lib \
	-I$(SWIG_DIR)/Lib/python

SWIG_OPTIONS = -noproxy
 
DYLIBS_DIR              = $(BUILT_PRODUCTS_DIR)

LIB_VOGLE                = $(DYLIBS_DIR)/libvogle-2.0.dylib
LIB_VOPL                 = $(DYLIBS_DIR)/libvopl-2.0.dylib
LIB_MESCHACH             = $(DYLIBS_DIR)/libmeschach.dylib
LIB_MESCHACH_SPOOLES     = $(DYLIBS_DIR)/libmeschach_spooles.dylib
LIB_MESCHACH_LAPACK      = $(DYLIBS_DIR)/libmeschach_lapack.dylib
LIB_MESCHACH_ADDS        = $(DYLIBS_DIR)/libmeschach_adds.dylib
LIB_MESCHACH_EF          = $(DYLIBS_DIR)/libmeschach_ef.dylib
LIB_MESCHACH_CPGPLOT     = $(DYLIBS_DIR)/libmeschach_cpgplot.dylib
LIB_MESCHACH_VOGLE       = $(DYLIBS_DIR)/libmeschach_vogle.dylib
LIB_MESCHACH_LIBSCIPLOT  = $(DYLIBS_DIR)/libmeschach_libsciplot.dylib


#CC_ARCH = -arch $(CURRENT_ARCH)

# APPLE PYTHON 2.7 MAC OSX 10.10
#PYTHON_HEADERS       = /System/Library/Frameworks/Python.framework/Headers
#PYTHON_FRAMEWORK_DIR = /System/Library/Frameworks/

PYTHON_HEADERS       = /System/Library/Frameworks/Python.framework/Versions/2.7/include/python2.7
PYTHON_FRAMEWORK_DIR = /System/Library/Frameworks/Python.framework/Versions/2.7


# CFLAGS already set for specific lib
CFLAGS += -g -Wall -no-cpp-precomp -DANSI_C \
	-I../..  \
	-I$(PYTHON_HEADERS) -I/opt/X11/include

INSTALL_DIR      = $(TOP)/PYTHON_EXTENSIONS


$(SRC_WRAP) : $(SWIG_DEPENDANCIES)
	@echo "---> create new wrapper file ...", $(SWIG_INC)
	$(SWIG_EXE) $(SWIG_INC) -python $(SWIG_OPTIONS) $(SWIG_INTERFACE_FILE)

$(OBJ_WRAP) : $(SRC_WRAP)
	@echo "---> and compile it ..."
	$(CC) $(CC_ARCH) -c $(CFLAGS) $(SRC_WRAP) -o $(OBJ_WRAP)

$(PYTHON_EXTENSION) : $(OBJ_WRAP) $(DYLIBS_LINKED_TO_BUNDLE)
	@echo "--->make bundle ..."
	$(CC) $(CC_ARCH) -bundle $(OBJ_WRAP) -o $(PYTHON_EXTENSION) -F$(PYTHON_FRAMEWORK_DIR) -framework Python $(DYLIBS_LINKED_TO_BUNDLE)
	mv $(PYTHON_EXTENSION) $(INSTALL_DIR)


all: $(PYTHON_EXTENSION)

clean:; rm $(OBJ_WRAP) $(SRC_WRAP)
