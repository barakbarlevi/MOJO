#XENO_CONFIG := /usr/xenomai/bin/xeno-config

CC := g++
CODEDIRS=.
INCDIRS=.

#CFLAGS := $(shell $(XENO_CONFIG)  --posix --alchemy --cflags) $(foreach D,$(INCDIRS),-I$(D))
#LDFLAGS := $(shell $(XENO_CONFIG)  --posix --alchemy --ldflags --auto-init-solib)

#CFLAGS := $(foreach D,$(INCDIRS),-I$(D)) // ORIG
LDFLAGS := -g -lX11 

CFLAGS :=-Wall -Wextra -g -std=c++20 -pthread $(foreach D,$(INCDIRS),-I$(D))


CFILES = $(foreach D,$(CODEDIRS),$(wildcard $(D)/*.cpp))
#CFILES = IPC_Client.cpp
#CFILES = swap.cc

OBJECTS = $(patsubst %.cpp,%.o,$(CFILES))

EXECUTABLE := MOJO
#EXECUTABLE := swap

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# XXXX missing %.h? check
%.o: %.cpp
		echo $@
		echo $<
		$(CC) -c -o $@ $< $(CFLAGS)

clean: 
		rm -rf $(EXECUTABLE) $(OBJECTS)
