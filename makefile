# Make command to use for dependencies
MAKE=make
RM=rm
MKDIR=mkdir

# -----Begin user-editable area-----
preDef=BOOST_SIGNALS_NO_DEPRECATION_WARNING 
player_inc=/usr/local/include/player-3.0
player_lib=-L/usr/local/lib -lplayerc++ -lboost_thread -lboost_signals -lplayerc -lm -lz -lplayerinterface -lplayerwkb -lgeos -lgeos_c -lplayercommon 
boost_inc=/media/sf_DistributedIntelligenceInRobotics/boost_1_55_0 
boost_lib=-L/media/sf_DistributedIntelligenceInRobotics/boost_1_55_0/stage/lib -lboost_system 

compiler=g++
linker=g++
# -----End user-editable area-----

OUTDIR=bin
OUTFILE=./$(OUTDIR)/robot
CFG_INC=-I$(player_inc) -I$(boost_inc) 
CFG_LIB=$(player_lib) $(boost_lib) 
CFG_OBJ=
COMMON_OBJ=$(OUTDIR)/MyRobot.o $(OUTDIR)/LaserRobot.o $(OUTDIR)/Follower.o $(OUTDIR)/Leader.o $(OUTDIR)/Communicate.o $(OUTDIR)/TimeRecorder.o \
$(OUTDIR)/UdpSession.o $(OUTDIR)/CommPoint.o 

OBJ=$(COMMON_OBJ) $(CFG_OBJ)
ALL_OBJ=$(COMMON_OBJ) $(CFG_LIB) 

COMPILE=$(compiler) -c "-D$(preDef)" -g -o "$(OUTDIR)/$(*F).o" $(CFG_INC) $<
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