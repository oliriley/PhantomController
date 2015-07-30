# Makefile - AnchoredSpringForce

INCLUDES=-I../../../../include
INCLUDES+=-I../../../../libsrc/include
INCLUDES+=-I../../../../utilities/include
LDFLAGS=-L../../../../libsrc/lib -L../../../../utilities/lib

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

examples_dir := $(examples_dir)/HD

#DEBUG = TRUE

ifdef DEBUG
CFG=AnchoredSpringForce_DEBUG
else
CFG=AnchoredSpringForce_RELEASE
endif

CC=gcc
CFLAGS=
CXX=g++
CXXFLAGS=$(CFLAGS)

ifeq "$(CFG)"  "AnchoredSpringForce_RELEASE"
CFLAGS+=-W -fexceptions -O2 $(INCLUDES) -DNDEBUG -Dlinux -D_CONSOLE
LD=$(CXX) $(CXXFLAGS)
LIBS = -lHDU -lHD -lrt

else
ifeq "$(CFG)"  "AnchoredSpringForce_DEBUG"
CFLAGS+=-W -fexceptions -g -O0 $(INCLUDES) -D_DEBUG -Dlinux -D_CONSOLE
LD=$(CXX) $(CXXFLAGS)
LIBS = -lHDUD -lHD -lrt
endif
endif

ifndef TARGET
TARGET=AnchoredSpringForce
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

SOURCE_FILES= \
	AnchoredSpringForce.c \
	conio.c

HEADER_FILES=conio.h

RESOURCE_FILES=

SRCS=$(SOURCE_FILES) $(HEADER_FILES) $(RESOURCE_FILES)

OBJS=$(patsubst %.rc,%.res,$(patsubst %.cxx,%.o,$(patsubst %.cpp,%.o,$(patsubst %.cc,%.o,$(patsubst %.c,%.o,$(filter %.c %.cc %.cpp %.cxx %.rc,$(SRCS)))))))

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.PHONY: install
install:
	install -m 755 -o 0 -g 0 -d $(examples_dir)/console/$(TARGET)
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/console/$(TARGET)
	install -m 644 -o 0 -g 0 $(SRCS) $(examples_dir)/console/$(TARGET)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET)


