# Makefile - PointSnapping.dsp

INCLUDES=-Iinclude
INCLUDES+=-I../../../../include
INCLUDES+=-I../../../../libsrc/include
INCLUDES+=-I../../../../utilities/include
LDFLAGS=-L../../../../libsrc/lib -L../../../../utilities/lib

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

examples_dir := $(examples_dir)/HD

ifdef DEBUG
CFG=PointSnapping_DEBUG
else
CFG=PointSnapping_RELEASE
endif

CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)

ifeq "$(CFG)"  "PointSnapping_RELEASE"
CFLAGS+=-W -fexceptions -O2 $(INCLUDES) -Dlinux -DNDEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS+=-lHD -lHDU -lSnapConstraints -lglut -lGL -lGLU -lrt
else

ifeq "$(CFG)"  "PointSnapping_DEBUG"
CFLAGS+=-W -fexceptions -g -O0 $(INCLUDES) -Dlinux -D_DEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS+=-lHD -lHDUD -lSnapConstraintsD -lglut -lGL -lGLU -lrt
endif
endif

ifndef TARGET
TARGET=PointSnapping
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
	src/PointManager.cpp \
	src/PointSnappingAfx.cpp \
	src/PointSnappingUtil.cpp \
	src/ViewApparentPointConstraint.cpp

HEADER_FILES= \
	include/HapticDeviceManager.h \
	include/PointManager.h

PRIVATE_HEADER_FILES= \
	src/PointSnappingAfx.h \
	src/PointSnappingUtil.h \
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


