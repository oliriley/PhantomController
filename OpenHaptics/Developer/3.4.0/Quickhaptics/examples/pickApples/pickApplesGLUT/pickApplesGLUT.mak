CC=g++
LIBDIR = -L../../../uAPI\ V1/lib/
CFLAGS+=-W -O2 -DNDEBUG -Dlinux $(LIBDIR)
LIBS = -lHL -lHLU -lHDU -lHD -lGL -lGLU -lglut -lQH -lQHGLUTWrapper -lrt -lncurses 
#INCLUDES = -I../../../uAPI\ V1/header/ -I../../../uAPI\ V1/header/SOIL -I../../../uAPI\ V1/header/freetype -I../../../3DTouch/libsrc/include

INCLUDES = -I../../../uAPI\ V1/header/ -I../../../3DTouch/libsrc/include

TARGET=pickApples/pickApplesGLUT
HDRS=
SRCS=pickApplesGLUT.cpp
OBJS=$(SRCS:.cpp=.o)

ifndef examples_dir
export examples_dir = /usr/share/3DTouch/examples
endif

.PHONY: all
all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $(SRCS) $(LIBS) $(INCLUDES)

.PHONY: clean
clean:
	-rm -f $(OBJS) $(TARGET)

.PHONY: install
install:
	install -m 755 -o 0 -g 0 -d $(examples_dir)/../QuickHaptics/examples/$(TARGET)
	install -m 755 -o 0 -g 0 -d $(examples_dir)/../QuickHaptics/examples/$(TARGET)/models/appleBasket
	install -m 644 -o 0 -g 0 Makefile $(examples_dir)/../QuickHaptics/examples/$(TARGET)
	install -m 644 -o 0 -g 0 $(SRCS) $(examples_dir)/../QuickHaptics/examples/$(TARGET)
	install -m 644 -o 0 -g 0 models/appleBasket/* $(examples_dir)/../QuickHaptics/examples/$(TARGET)/models/appleBasket
