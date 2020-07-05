DEVICE  ?= PFS154

#-------------------------------------------------------------

SDCC    ?= sdcc
OUTDIR  ?= .build
RELDIR  ?= ../pre-compiled

BUILD_DIR = $(OUTDIR)/$(DEVICE)
RELEASE_DIR = $(RELDIR)/$(DEVICE)

SOURCES = helloworld.c comptest.c calib-and-fuse-demo.c

ifeq ($(DEVICE), PFS154)
	ARCH = pdk14
else ifeq ($(DEVICE), PFS172)
	ARCH = pdk14
	SOURCES += adctest.c
else ifeq ($(DEVICE), PFS173)
	ARCH = pdk15
	SOURCES += adctest.c polysound.c
else ifeq ($(DEVICE), PMS150C)
	ARCH = pdk13
else ifeq ($(DEVICE), PMS152)
	ARCH = pdk14
else ifeq ($(DEVICE), PMS171B)
 	ARCH = pdk14
	SOURCES += adctest.c
endif

RELEASES = $(patsubst %.c,$(BUILD_DIR)/%.ihx,$(SOURCES))

all: build

print-%: ; @echo $* = $($*)

$(BUILD_DIR)/%.ihx: %.c
	@mkdir -p $(BUILD_DIR)
	$(SDCC) -D$(DEVICE) -m$(ARCH) -o $@ $<

build: $(RELEASES)

release: build
	@mkdir -p $(RELEASE_DIR)
	cp $(BUILD_DIR)/*.ihx $(RELEASE_DIR)

PMS152:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPMS152 -mpdk14 -o $(OUTDIR)/helloworld_pms152.ihx helloworld.c
	$(SDCC) -DPMS152 -mpdk14 -o $(OUTDIR)/comptest_pms152.ihx comptest.c

PMS171B:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPMS171B -mpdk14 -o $(OUTDIR)/helloworld_pms171b.ihx helloworld.c
	$(SDCC) -DPMS171B -mpdk14 -o $(OUTDIR)/comptest_pms171b.ihx comptest.c

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
