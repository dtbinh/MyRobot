# Make command to use for dependencies
MAKE=make
RM=rm
MKDIR=mkdir

# -----Begin user-editable area-----
player_inc=/usr/local/include/player-3.0
player_lib=-L/usr/local/lib -lplayerc++ -lboost_thread -lboost_signals -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lgeos -lgeos_c -lplayercommon 

compiler=g++
linker=g++
# -----End user-editable area-----

OUTDIR=bin
OUTFILE=./$(OUTDIR)/robot
CFG_INC=-I$(player_inc) 
CFG_LIB=$(player_lib) 
CFG_OBJ=
COMMON_OBJ=$(OUTDIR)/MyRobot.o $(OUTDIR)/LaserRobot.o $(OUTDIR)/Leader.o $(OUTDIR)/Follower.o $(OUTDIR)/Communicate.o $(OUTDIR)/TimeRecorder.o 
OBJ=$(COMMON_OBJ) $(CFG_OBJ)
ALL_OBJ=$(COMMON_OBJ) $(player_lib) 

COMPILE=$(compiler) -c -g -o "$(OUTDIR)/$(*F).o" $(CFG_INC) $<
LINK=$(linker)  -g -o "$(OUTFILE)" $(ALL_OBJ)

# Pattern rules
$(OUTDIR)/%.o : %.cc
	$(COMPILE)

# Build rules
all: $(OUTFILE)

$(OUTFILE): $(OUTDIR)  $(OBJ)
	$(LINK)

$(OUTDIR):
	$(MKDIR) -p "$(OUTDIR)"

# Rebuild this project
rebuild: cleanall all

# Clean this project
clean:
	$(RM) -f $(OUTFILE)
	$(RM) -f $(OBJ)

# Clean this project and all dependencies
cleanall: clean