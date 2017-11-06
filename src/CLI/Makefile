#
# Makefile
#
# Target: SCLI
#
# Environment
#
WORK_INC =  $(WORK_HOME_STOCK)/inc/
WORK_INC2 = $(WORK_HOME_STOCK)/src/PRC_THR/
WORK_LIB =  $(WORK_HOME_STOCK)/lib/
WORK_BIN =  $(WORK_HOME_STOCK)/bin/
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
CCOPTS = -c -I$(WORK_INC) -I$(WORK_INC2)
LDOPTS = $(DEFINES) -L$(WORK_LIB)

CC = g++ $(CCOPTS) $(DEFINES)
LINK = g++ $(LDOPTS)

.cpp.o:	; $(CC) $<
#
# Libraries used for linking
#
#LIBS = -lpthread -lSane
LIBS = -lpthread -lSane -lStockCL
#
#
# Object files
#
OBJS = CLI.o CLScommand.o CLIfunction.o
#
# Target Process
#
TARGET = SCLI
#
# Make the Executalbe Program
#
default:	$(TARGET)

$(TARGET) : $(OBJS)
	$(LINK) $(OBJS) $(LIBS) -o $@
	#@mv CLSFILETEST $(WORK_BIN)
	@mv $@ $(WORK_BIN)
	#@rm -rf  $(OBJS)
	@rm -rf core $(TARGET) $(OBJS)

clean :	
	rm -rf $(WORK_BIN)$(TARGET) $(OBJS)
