#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = Modulay

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	DistrhoPluginMaxGen.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

ifeq ($(LINUX),true)
TARGETS += ladspa
endif

TARGETS += lv2_dsp
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
