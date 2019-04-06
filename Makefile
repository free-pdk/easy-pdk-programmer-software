CC    ?= gcc
RM    ?= rm -rf
STRIP ?= strip

CFLAGS += -Wall -O2 -std=c99

all: easypdkprog

ifeq ($(OS),Windows_NT)
    EXE_EXTENSION := .exe
    ARGPSA    = lib/argp-standalone-1.3
    ARGPSALIB = $(ARGPSA)/libargp.a
    CFLAGS  += -I$(ARGPSA)
else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Darwin)
        LDFLAGS += -largp
    endif
endif

DEP=  $(wildcard *.h)
SRC=  serialcom.c fpdkutil.c fpdkcom.c fpdkicdata.c fpdkihex8.c fpdkiccalib.c
OBJ=  $(subst .c,.o,$(SRC))

easypdkprog: $(ARGPSALIB) $(DEP) $(OBJ) easypdkprog.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o easypdkprog easypdkprog.c $(OBJ) $(LIBS) $(ARGPSALIB)
	$(STRIP) easypdkprog$(EXE_EXTENSION)

simpletest: $(DEP) $(OBJ) simpletest.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o simpletest simpletest.c $(OBJ) $(LIBS)

$(ARGPSALIB):
	cd $(ARGPSA) && sh configure
	$(MAKE) -C $(ARGPSA)

clean:
	$(RM) $(OBJ)
	$(RM) easypdkprog$(EXE_EXTENSION)
	$(RM) simpletest$(EXE_EXTENSION)

distclean: clean
ifeq ($(OS),Windows_NT)
	-$(MAKE) distclean -C $(ARGPSA)
endif
