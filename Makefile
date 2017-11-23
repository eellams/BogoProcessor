CC := g++
SRCDIR := src
BUILDDIR := build
BINDIR := bin
TESTSDIR := tests
TARGET := BogoProcessor

CFLAGS=-std=c++98

# File extensions etc
SOURCEEXT := .cpp
OBJECTEXT := .o
SOURCEMAIN := src/main.cpp

LIBFOLDER :=

# Include and library directories
SYSTEMC_HOME := /usr/local/systemc-2.3.1
SYSTEMCINCLUDE := $(SYSTEMC_HOME)/include
SYSTEMCLIB := $(SYSTEMC_HOME)/lib-linux64

LIB := -L$(SYSTEMCLIB) -lsystemc -lm
INC := -I$(SYSTEMCINCLUDE) -Iinclude

SOURCES := $(shell find $(SRCDIR) -type f -name *$(SOURCEEXT))
SOURCES := $(filter-out $(SOURCEMAIN), $(SOURCES))

TESTSOURCES := $(shell find $(TESTSDIR) -type f -name *$(SOURCEEXT))

OBJECTS := $(patsubst $(SRCDIR)/%, $(BUILDDIR)/%, $(SOURCES:$(SOURCEEXT)=$(OBJECTEXT)))
TESTOBJECTS :=

$(TARGET): $(OBJECTS)
	@echo " Linking..."
	@mkdir -p $(BINDIR)
	@echo " $(CC) $(CFLAGS) $(INC) $^ $(SOURCEMAIN) -o $(BINDIR)/$(TARGET) $(LIB)"; $(CC) $(CFLAGS) $(INC) $^ $(SOURCEMAIN) -o $(BINDIR)/$(TARGET) $(LIB)

build/%.o : src/%.cpp
	@echo " Building..."
	@mkdir -p $(BUILDDIR)
	@echo " $(CC) $(CFLAGS) $(INC) -c -o $@ $<"; $(CC) $(CFLAGS) $(INC) -c -o $@ $<

tests : $(TESTSOURCES)
	@echo " Making tests...";
	@mkdir -p $(BINDIR)/$(TESTSDIR)
	@$(foreach t, $^, \
		echo " $(CC) $(CFLAGS) $(INC) $(t) -o$(patsubst $(TESTSDIR)/%, $(BINDIR)/$(TESTSDIR)/%, $(t:$(SOURCEEXT)=)) $(LIB)" ; \
		$(CC) $(CFLAGS) $(INC) $(t) -o$(patsubst $(TESTSDIR)/%, $(BINDIR)/$(TESTSDIR)/%, $(t:$(SOURCEEXT)=)) $(LIB) ; \
	)


clean:
	@echo " Cleaning...";
	@echo " $(RM) -r $(BUILDDIR) $(TARGET)"; $(RM) -r $(BUILDDIR) $(BINDIR)/$(TARGET)
