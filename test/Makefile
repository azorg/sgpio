#----------------------------------------------------------------------------
OUT_NAME     := gpio_test
OUT_DIR      := .
CLEAN_FILES := "$(OUT_DIR)/$(OUT_NAME).exe" "data.txt"
#----------------------------------------------------------------------------
# 1-st way to select source files
SRCS := \
  ../sgpio.c \
	sgpio_test.c

HDRS := \
	../sgpio.h

# 2-nd way to select source files
#SRC_DIRS := . ..
#HDR_DIRS := . ..
#----------------------------------------------------------------------------
DEFS    := -DRND_USE_STDLIB
#OPTIM  := -g -O0
OPTIM   := -Os
WARN    := -Wall
CFLAGS  := $(WARN) $(OPTIM) $(DEFS) $(CFLAGS) -pipe
LDFLAGS := -lm $(LDFLAGS)
#----------------------------------------------------------------------------
#_AS  := @as
#_CC  := @gcc
#_CXX := @g++
#_LD  := @g++

#_CC  := @clang
#_CXX := @clang++
#_LD  := @clang
#----------------------------------------------------------------------------
include Makefile.skel
#----------------------------------------------------------------------------

