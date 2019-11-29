SDCC=../../sdcc/bin/sdcc
OUTDIR=build

all: PFS154 PFS173 PMS150C

PFS154:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPFS154 -mpdk14 -o $(OUTDIR)/helloworld_pfs154.ihx helloworld.c
	$(SDCC) -DPFS154 -mpdk14 -o $(OUTDIR)/serialecho_pfs154.ihx serialecho.c
	$(SDCC) -DPFS154 -mpdk14 -o $(OUTDIR)/comptest_pfs154.ihx comptest.c

PFS173:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/helloworld_pfs173.ihx helloworld.c
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/serialecho_pfs173.ihx serialecho.c
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/comptest_pfs173.ihx comptest.c
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/adctest_pfs173.ihx adctest.c

PMS150C:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPMS150C -mpdk13 -o $(OUTDIR)/helloworld_pms150c.ihx helloworld.c
	$(SDCC) -DPMS150C -mpdk13 -o $(OUTDIR)/serialecho_pms150c.ihx serialecho.c
	$(SDCC) -DPMS150C -mpdk13 -o $(OUTDIR)/comptest_pms150c.ihx comptest.c

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean
