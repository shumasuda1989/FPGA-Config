TARGET = fpga_configuration

VPATH = source/erase:source/mcs_convert:source/reboot:source/verify:source/write_file

SUBTARGET = erase_spi \
            mcs_convert \
            reboot_start \
            spi_verify \
            write_file_to_spi

SUBSUBSRC = erase_cmd \
            read_buf_to_file \
            read_spi_cmd \
            filedata_to_buf \
            read_buf_cmd \
            status_cmd \
            wren_cmd \
            write_spi_cmd

RBCP = rbcp_com


CURDIR = $(shell pwd)
INCLUDEDIR=source/include

DEP = dep.d
TARGETSRC = $(addsuffix .cc, $(basename $(TARGET)))
TARGETOBJ = $(addsuffix .o, $(basename $(TARGET)))
SUBSRCS = $(addsuffix .cc, $(basename $(SUBTARGET)))
SUBOBJS = $(addsuffix .o, $(basename $(SUBTARGET)))
SUBSUBSRCS = $(addsuffix .cc, $(basename $(SUBSUBSRC)))
SUBSUBOBJS = $(addsuffix .o, $(basename $(SUBSUBSRC)))
RBCPSRC = $(addsuffix .c, $(basename $(RBCP)))
RBCPOBJ = $(addsuffix .o, $(basename $(RBCP)))

CXXFLAGS      = -O2 -Wall -fPIC -I$(INCLUDEDIR)

all: dep $(TARGET)

$(TARGET): % : %.o $(SUBOBJS) $(SUBSUBOBJS) $(RBCPOBJ)
	$(CXX) $(CXXFLAGS) $^ $(LIBS) -o $@

$(SUBTARGET): % : %.cc $(SUBSUBOBJS) $(RBCPOBJ)
	if [ -z "$(EXTDEF)" ]; then echo ""; echo CANNOT MAKE; else \
   $(CXX) $(CXXFLAGS) $(EXTDEF) $^ $(LIBS) -o $@; fi

$(SUBSUBSRC): % : %.cc $(RBCPOBJ)
	if [ -z "$(EXTDEF)" ]; then echo ""; echo CANNOT MAKE; else \
   $(CXX) $(CXXFLAGS) $(EXTDEF) $< $(RBCPOBJ) $(LIBS) -o $@; fi

### If you want to make only a part of the TARGET binary, 
### execute "make srcname EXTDEF=-D(macro)" 
###    e.g.: make reboot_start EXTDEF=-DTEST_REBOOT

.cc.o :
	$(CXX) $(CXXFLAGS) $(EXTDEF) -c $<

.c.o:
	$(CC) $(CXXFLAGS) -c $<

dep: $(TARGETSRC) $(SUBSRCS) $(SUBSUBSRCS) $(RBCPSRC)
	@$(CXX) -MM $(CXXFLAGS) $^ > $(DEP)

-include dep.d

clean:
	$(RM) $(TARGET) $(SUBTARGET) $(SUBSUBSRC) *.o $(DEP) *~

echo:
	@echo $(CXXFLAGS)

.PHONY: all clean echo