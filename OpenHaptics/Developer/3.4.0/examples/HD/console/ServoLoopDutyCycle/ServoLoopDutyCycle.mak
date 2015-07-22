# Makefile - ServoLoopDutyCycle.dsp

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
CFG=ServoLoopDutyCycle_DEBUG
else
CFG=ServoLoopDutyCycle_RELEASE
endif

CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)

ifeq "$(CFG)"  "ServoLoopDutyCycle_RELEASE"
CFLAGS+=-W -fexceptions -O2 $(INCLUDES) -Dlinux -DNDEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS+=-lHDU -lHD -lrt
else

ifeq "$(CFG)"  "ServoLoopDutyCycle_DEBUG"
CFLAGS+=-W -fexceptions -g -O0 $(INCLUDES) -Dlinux -D_DEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS+=-lHDUD -lHD -lrt
endif
endif

ifndef TARGET
TARGET=ServoLoopDutyCycle
endif

.PHONY: all
all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.cxx
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

%.res: %.rc
	$(RC) $(CPPFLAGS) -o $@ -i $<

SOURCE_FILES= \
	src/ServoLoopDutyCycle.cpp \
	src/StatsSampler.cpp \
	src/conio.c

HEADER_FILES= \
	include/StatsSampler.h \
	src/conio.h

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
	install -m 755 -o 0 -g 0 -d $(examples_dir)/console/$(TARGET)
	install -m 755 -o 0 -g 0 -d $(examples_dir)/console/$(TARGET)/src
	install -m 755 -o 0 -g 0 -d $(examples_dir)/console/$(TARGET)/include
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/console/$(TARGET)
	install -m 644 -o 0 -g 0 $(SOURCE_FILES) $(examples_dir)/console/$(TARGET)/src
	install -m 644 -o 0 -g 0 $(HEADER_FILES) $(examples_dir)/console/$(TARGET)/include
