#
# Makefile
#
# Target: STOCKMAIN
#
# Environment
#
WORK_INC = $(WORK_HOME_STOCK)/inc/
PROC_INC = $(WORK_HOME_STOCK)/src/PROC
PROC_INC2 = /home/ygd/test/work/GD/CLSlib/src/PROCPP
WORK_LIB = $(WORK_HOME_STOCK)/lib/
WORK_BIN = $(WORK_HOME_STOCK)/bin/
ORA_INC = $(ORACLE_INC)
ORA_LIB = /usr/lib/oracle/11.2/client64/lib
#


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
#CCOPTS = -c -I$(WORK_INC) -I$(PROC_INC) -I$(PROC_INC2) -I$(ORA_INC)
CCOPTS = -c -I$(WORK_INC)
#LDOPTS = $(DEFINES) -L$(WORK_LIB) -L$(ORA_LIB)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
LIBS = -pthread -lStockCL -lSane
#
#
# Object files
#
OBJS = STOCKMAIN.o MngDatabase.o
#
# Target Process
#
TARGET = STOCKMAIN
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	@mv $@ $(WORK_BIN)
	@rm -rf core $(TARGET) $(OBJS)

clean :
	rm -rf $(WORK_BIN)$(TARGET) $(TARGET) $(OBJS)
