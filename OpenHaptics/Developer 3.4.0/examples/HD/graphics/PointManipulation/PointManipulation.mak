# Makefile - PointManipulation.dsp

INCLUDES=-Iinclude
INCLUDES+=-I../../../../include
INCLUDES+=-I../../../../libsrc/include
INCLUDES+=-I../../../../utilities/include
LDFLAGS=-L../../../../libsrc/lib -L../../../../utilities/lib

#DEBUG = TRUE

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

examples_dir := $(examples_dir)/HD

ifdef DEBUG
CFG=PointManipulation_DEBUG
else
CFG=PointManipulation_RELEASE
endif

CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)

ifeq "$(CFG)"  "PointManipulation_RELEASE"
CFLAGS+=-Wall -fexceptions -O2 $(INCLUDES) -Dlinux -DNDEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS+=-lHD -lHDU -lglut -lGL -lGLU -lrt -lSnapConstraints
else

ifeq "$(CFG)"  "PointManipulation_DEBUG"
CFLAGS+=-Wall -fexceptions -g -O0 $(INCLUDES) -Dlinux -D_DEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS+=-lHD -lHDUD -lglut -lGL -lGLU -lrt -lSnapConstraintsD
endif
endif

ifndef TARGET
TARGET=PointManipulation
endif

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

CFLAGS+=
SOURCE_FILES= \
	src/HapticDeviceManager.cpp \
	src/main.cpp \
	src/PlaneWithAxesConstraint.cpp \
	src/PointManager.cpp \
	src/PointManipulationAfx.cpp \
	src/PointManipulationUtil.cpp \
	src/ViewApparentPointConstraint.cpp

HEADER_FILES= \
	include/HapticDeviceManager.h \
	include/PointManager.h

PRIVATE_HEADER_FILES= \
	src/PlaneWithAxesConstraint.h \
	src/PointManipulationAfx.h \
	src/PointManipulationUtil.h \
	src/ViewApparentPointConstraint.h

RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET)

.PHONY: install
install:
	install -m 755 -o 0 -g 0 -d $(examples_dir)/graphics/$(TARGET)
	install -m 755 -o 0 -g 0 -d $(examples_dir)/graphics/$(TARGET)/src
	install -m 755 -o 0 -g 0 -d $(examples_dir)/graphics/$(TARGET)/include
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/graphics/$(TARGET)
	install -m 644 -o 0 -g 0 $(SOURCE_FILES) $(PRIVATE_HEADER_FILES) $(examples_dir)/graphics/$(TARGET)/src
	install -m 644 -o 0 -g 0 $(HEADER_FILES) $(examples_dir)/graphics/$(TARGET)/include


