#
# FreeType 2 configuration rules templates for Unix + configure
#


# Copyright 1996-2017 by
# David Turner, Robert Wilhelm, and Werner Lemberg.
#
# This file is part of the FreeType project, and may only be used, modified,
# and distributed under the terms of the FreeType project license,
# LICENSE.TXT.  By continuing to use, modify, or distribute this file you
# indicate that you have read the license and understand and accept it
# fully.

SHELL := /bin/bash

TOP_DIR := $(shell cd $(TOP_DIR); pwd)

DELETE := rm -f
DELDIR := rm -rf
CAT    := cat
SEP    := /

# this is used for `make distclean' and `make install'
OBJ_BUILD ?= $(BUILD_DIR)

# don't use `:=' here since the path stuff will be included after this file
#
FTSYS_SRC = $(BASE_DIR)/ftsystem.c

INSTALL         := /usr/bin/install -c
INSTALL_DATA    := ${INSTALL} -m 644
INSTALL_PROGRAM := ${INSTALL}
INSTALL_SCRIPT  := ${INSTALL}
MKINSTALLDIRS   := /usr/bin/mkdir -p

CLEAN += $(OBJ_BUILD)/freetype-config \
         $(OBJ_BUILD)/freetype2.pc

DISTCLEAN += $(OBJ_BUILD)/config.cache  \
             $(OBJ_BUILD)/config.log    \
             $(OBJ_BUILD)/config.status \
             $(OBJ_BUILD)/unix-def.mk   \
             $(OBJ_BUILD)/unix-cc.mk    \
             $(OBJ_BUILD)/ftconfig.h    \
             $(LIBTOOL)                 \
             $(OBJ_BUILD)/Makefile


# Standard installation variables.
#
prefix       := /home/hah/Downloads/freetype-2.8/build/install
exec_prefix  := ${prefix}
libdir       := ${exec_prefix}/lib
bindir       := ${exec_prefix}/bin
includedir   := ${prefix}/include
datarootdir  := ${prefix}/share
datadir      := ${datarootdir}
mandir       := ${datarootdir}/man

version_info := 20:0:14

# Variables needed for `freetype-config' and `freetype.pc'.
#
REQUIRES_PRIVATE   := zlib
LIBS_PRIVATE       := 
LIBSSTATIC_CONFIG  := -lfreetype -lz
build_libtool_libs := 
ft_version         := 20.0.14

# The directory where all library files are placed.
#
# By default, this is the same as $(OBJ_DIR); however, this can be changed
# to suit particular needs.
#
LIB_DIR := $(OBJ_DIR)

# The BASE_SRC macro lists all source files that should be included in
# src/base/ftbase.c.  When configure sets up CFLAGS to build ftmac.c,
# ftmac.c should be added to BASE_SRC.
ftmac_c := 

# The SYSTEM_ZLIB macro is defined if the user wishes to link dynamically
# with its system wide zlib. If SYSTEM_ZLIB is 'yes', the zlib part of the
# ftgzip module is not compiled in.
SYSTEM_ZLIB := @SYSTEM_ZLIB@


# The NO_OUTPUT macro is appended to command lines in order to ignore
# the output of some programs.
#
NO_OUTPUT := 2> /dev/null


# To support calls like
#
#   configure --includedir='${libdir}'/freetype2/include
#
# we generate `freetype-config' and `freetype.pc' at compile time so that
# those variables are properly expanded.

$(OBJ_BUILD)/freetype-config: $(TOP_DIR)/builds/unix/freetype-config.in
	rm -f $@ $@.tmp
	sed -e 's|%LIBSSTATIC_CONFIG%|$(LIBSSTATIC_CONFIG)|'   \
	    -e 's|%build_libtool_libs%|$(build_libtool_libs)|' \
	    -e 's|%exec_prefix%|$(exec_prefix)|'               \
	    -e 's|%ft_version%|$(ft_version)|'                 \
	    -e 's|%includedir%|$(includedir)|'                 \
	    -e 's|%libdir%|$(libdir)|'                         \
	    -e 's|%prefix%|$(prefix)|'                         \
	    $<                                                 \
	    > $@.tmp
	chmod +x $@.tmp
	chmod go-w $@.tmp
	mv $@.tmp $@

# To support directory names with spaces (as might easily happen on Windows
# platforms), the right solution would be to surround the pkg-variables in
# `freetype2.pc' with double quotes.  However, doing so ironically disables
# the prefix override mechanism especially written for Windows.  This is a
# bug in pkg-config version 0.28 and earlier.
#
# For this reason, we escape spaces with backslashes.

exec_prefix_x := $(subst $(space),\\$(space),$(exec_prefix))
includedir_x  := $(subst $(space),\\$(space),$(includedir))
libdir_x      := $(subst $(space),\\$(space),$(libdir))
prefix_x      := $(subst $(space),\\$(space),$(prefix))

$(OBJ_BUILD)/freetype2.pc: $(TOP_DIR)/builds/unix/freetype2.in
	rm -f $@ $@.tmp
	sed -e 's|%REQUIRES_PRIVATE%|$(REQUIRES_PRIVATE)|'     \
	    -e 's|%LIBS_PRIVATE%|$(LIBS_PRIVATE)|'             \
	    -e 's|%build_libtool_libs%|$(build_libtool_libs)|' \
	    -e 's|%exec_prefix%|$(exec_prefix_x)|'             \
	    -e 's|%ft_version%|$(ft_version)|'                 \
	    -e 's|%includedir%|$(includedir_x)|'               \
	    -e 's|%libdir%|$(libdir_x)|'                       \
	    -e 's|%prefix%|$(prefix_x)|'                       \
	    $<                                                 \
	    > $@.tmp
	chmod a-w $@.tmp
	mv $@.tmp $@

all install: $(OBJ_BUILD)/freetype-config \
     $(OBJ_BUILD)/freetype2.pc

# EOF
