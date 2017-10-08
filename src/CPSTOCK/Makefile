#
# Makefile
#
# Target: CPSTOCK
#
# Environment
#
WORK_INC =  $(WORK_HOME_STOCK)/inc/
WORK_LIB =  $(WORK_HOME_STOCK)/lib/
WORK_BIN =  $(WORK_HOME_STOCK)/bin/
#abc


#
#----------------------------------------
# $@ : TARGET NAME
# $* : TARGET name except .*
# $< : first item in dependency list
# $? : last renewar file than current target file
#----------------------------------------

#
# Rules
#
DEFINES =
CCOPTS = -c -I$(WORK_INC)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
#LIBS = -pthread -lSane
LIBS = -lStockCL -lSane -pthread
#
#
# Object files
#
OBJS = CPSTOCK.o THRserver.o THRclient.o CLSstockIF.o CLSmap.o
#OBJS = CPVIMS.o 
#OBJS = CLSmap.o
#
# Target Process
#
TARGET = CPSTOCK
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	@mv $@ $(WORK_BIN)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_BIN)$(TARGET) $(OBJS) $(TARGET)
