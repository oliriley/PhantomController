# Makefile - CustomShape

INCLUDES=-I../../../../include
INCLUDES+=-I../../../../libsrc/include
INCLUDES+=-I../../../../utilities/include
LDFLAGS=-L../../../../lib -L../../../../utilities/lib

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

examples_dir := $(examples_dir)/HL

#DEBUG = TRUE

ifdef DEBUG 
CFG=CustomShape_Debug
else
CFG=CustomShape_Release
endif

CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)

ifeq "$(CFG)"  "CustomShape_Release"
CFLAGS+=-W -fexceptions -O2 $(INCLUDES) -Dlinux -DNDEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS = -lHL -lHD -lHDU -lrt
else

ifeq "$(CFG)"  "CustomShape_Debug"
CFLAGS+=-W -fexceptions -g -O0 $(INCLUDES) -Dlinux -D_DEBUG
LD=$(CXX) $(CXXFLAGS)
LIBS = -lHL -lHD -lHDUD -lrt
endif
endif

ifndef TARGET
TARGET=CustomShape
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

SOURCE_FILES= \
	CustomShape.cpp \
	HapticRings.cpp \
	HapticSphere.cpp \
	conio.c

INCLUDE_FILES= \
	HapticRings.h \
	HapticSphere.h \
	conio.h

SRCS=$(SOURCE_FILES) $(INCLUDE_FILES) 

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET)

.PHONY: install
install:
	install -m 755 -o 0 -g 0 -d $(examples_dir)/console/$(TARGET)
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/console/$(TARGET)
	install -m 644 -o 0 -g 0 $(SRCS) $(examples_dir)/console/$(TARGET)


