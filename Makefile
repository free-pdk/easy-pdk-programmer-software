CC    ?= gcc
RM    ?= rm -rf
STRIP ?= strip

CFLAGS += -Wall -O2 -std=c99

all: easypdkprog

UNAME_S := $(shell uname -s)
ifneq ($(UNAME_S),Linux)
    ARGPSA    = lib/argp-standalone-1.3
    ARGPSALIB = $(ARGPSA)/libargp.a
    CFLAGS  += -I$(ARGPSA)
endif

ifeq ($(OS),Windows_NT)
    EXE_EXTENSION := .exe
endif

DEP=  $(wildcard *.h)
SRC=  serialcom.c fpdkutil.c fpdkcom.c fpdkicdata.c fpdkihex8.c fpdkiccalib.c
OBJ=  $(subst .c,.o,$(SRC))

easypdkprog: $(ARGPSALIB) $(DEP) $(OBJ) easypdkprog.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o easypdkprog easypdkprog.c $(OBJ) $(LIBS) $(ARGPSALIB)
	$(STRIP) easypdkprog$(EXE_EXTENSION)

easypdkprogtest: $(DEP) $(OBJ) easypdkprogtest.c
	$(CC) $(CFLAGS) $(LDFLAGS) -o easypdkprogtest easypdkprogtest.c $(OBJ) $(LIBS)

$(ARGPSALIB):
	cd $(ARGPSA) && sh configure
	$(MAKE) -C $(ARGPSA)

clean:
	$(RM) $(OBJ)
	$(RM) easypdkprog$(EXE_EXTENSION)
	$(RM) easypdkprogtest$(EXE_EXTENSION)

distclean: clean
ifneq ($(UNAME_S),Linux)
	-$(MAKE) distclean -C $(ARGPSA)
endif
