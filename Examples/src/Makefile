SDCC=../../sdcc/bin/sdcc
OUTDIR=build

all: PFS154 PFS172 PFS173 PMS150C PMS152 PMS171B

PFS154:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPFS154 -mpdk14 -o $(OUTDIR)/helloworld_pfs154.ihx helloworld.c
	$(SDCC) -DPFS154 -mpdk14 -o $(OUTDIR)/comptest_pfs154.ihx comptest.c

PFS172:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPFS172 -mpdk14 -o $(OUTDIR)/helloworld_pfs172.ihx helloworld.c
	$(SDCC) -DPFS172 -mpdk14 -o $(OUTDIR)/comptest_pfs172.ihx comptest.c

PFS173:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/helloworld_pfs173.ihx helloworld.c
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/comptest_pfs173.ihx comptest.c
	$(SDCC) -DPFS173 -mpdk15 -o $(OUTDIR)/adctest_pfs173.ihx adctest.c

PMS150C:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPMS150C -mpdk13 -o $(OUTDIR)/helloworld_pms150c.ihx helloworld.c
	$(SDCC) -DPMS150C -mpdk13 -o $(OUTDIR)/comptest_pms150c.ihx comptest.c

PMS152:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPMS152 -mpdk14 -o $(OUTDIR)/helloworld_pms152.ihx helloworld.c
	$(SDCC) -DPMS152 -mpdk14 -o $(OUTDIR)/comptest_pms152.ihx comptest.c

PMS171B:
	mkdir -p $(OUTDIR)
	$(SDCC) -DPMS171B -mpdk14 -o $(OUTDIR)/helloworld_pms171b.ihx helloworld.c
	$(SDCC) -DPMS171B -mpdk14 -o $(OUTDIR)/comptest_pms171b.ihx comptest.c

clean:
	rm -rf $(OUTDIR)

.PHONY: all clean
