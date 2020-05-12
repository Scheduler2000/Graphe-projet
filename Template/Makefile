########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++17 -Wall
LDFLAGS = -lwsock32

# Makefile settings - Can be customized.
APPNAME = prog
EXT = .cpp
SRCDIR = src
INCDIR = inc
############## Do not change anything from here downwards or be careful ! #############
SRC = $(wildcard $(SRCDIR)/*$(EXT)) \
	  $(wildcard $(SRCDIR)/console/*$(EXT)) \
	  $(wildcard $(SRCDIR)/logger/*$(EXT)) 

OBJ = $(SRC:.$(EXT)=.o)

# Windows OS variables & settings
DEL = del
EXE = .exe

########################################################################
####################### Targets beginning here #########################
########################################################################

all: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS) -I $(INCDIR)/


#################### Cleaning rules for Windows OS && winrar command #####################
# Cleans complete project
mrproper: clean
	$(DEL) $(APPNAME)$(EXE) $(APPNAME).rar

# Cleans only all files with the extension .o
clean: 
	$(DEL) /S *.o

rar : 
	rar a $(APPNAME).rar
