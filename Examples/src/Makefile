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
	SOURCES += adctest.c
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

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
