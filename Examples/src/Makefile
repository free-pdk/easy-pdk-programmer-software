DEVICE  ?= PFS154

SDCC    ?= sdcc
OUTDIR  ?= .build
RELDIR  ?= ../pre-compiled

ifeq ($(DEVICE), PFS154)
	ARCH = pdk14
else ifeq ($(DEVICE), PFS172)
	ARCH = pdk14
else ifeq ($(DEVICE), PFS173)
	ARCH = pdk15
else ifeq ($(DEVICE), PMS150C)
	ARCH = pdk13
else ifeq ($(DEVICE), PMS152)
	ARCH = pdk14
else ifeq ($(DEVICE), PMS171B)
 	ARCH = pdk14
endif

all: build

build:
	mkdir -p $(OUTDIR)/$(DEVICE)
	$(SDCC) -D$(DEVICE) -m$(ARCH) -o $(OUTDIR)/$(DEVICE)/helloworld.ihx helloworld.c
	$(SDCC) -D$(DEVICE) -m$(ARCH) -o $(OUTDIR)/$(DEVICE)/comptest.ihx comptest.c
ifeq ($(DEVICE), PFS173)
	$(SDCC) -D$(DEVICE) -m$(ARCH) -o $(OUTDIR)/$(DEVICE)/adctest.ihx adctest.c
	$(SDCC) -D$(DEVICE) -m$(ARCH) -o $(OUTDIR)/$(DEVICE)/polysound.ihx polysound.c
endif

release: build
	mkdir -p $(RELDIR)/$(DEVICE)
	cp $(OUTDIR)/$(DEVICE)/*.ihx $(RELDIR)/$(DEVICE)

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean
