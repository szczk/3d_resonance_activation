DIRS := core tools analysis sim
PROGNAME = main
OBJDIR = obj
.PHONY: all clean run test


CPP = g++ 


INCLUDEFLAGS := $(patsubst %, -I%, $(DIRS))
LIBCC := $(shell find $(DIRS) -maxdepth 1 -name '*.cc')

# CXXFLAGS += $(shell root-config --cflags) -O3 -Wall
CXXFLAGS += -O4 -Wall -std=c++0x -D_GLIBCXX_USE_NANOSLEEP 

# XMLRPC = $(shell xmlrpc-c-config c++ libwww-client --libs)
# OTHERLIBS = -lMinuit -lSpectrum -lpq -lfftw3 -lz

GSLLIBS = -lgsl -lgslcblas 
# -lfftw3_threads -lfftw3 -lfftw3f


#LDFLAGS += $(shell root-config --glibs) $(GSLLIBS) $(XMLRPC) -O3 -Wall
LDFLAGS += $(GSLLIBS) $(XMLRPC) -O3 -Wall -pthread


# LIBOBJ := $(patsubst %.cc, obj/%.o, $(notdir $(LIBCC)) ) $(PROGNAME).o 
LIBOBJ := $(patsubst %.cc, %.o, $(LIBCC)) $(PROGNAME).o 
OBJS = $(addprefix $(OBJDIR)/,$(LIBCC))
GENLIBOBJ := $(patsubst %.cc, %.o, $(LIBCC))


$(PROGNAME).x : $(LIBOBJ)
	$(CPP) $^ -L $(INCLUDEFLAGS) $(LDFLAGS) -o $(PROGNAME).x 

	
%.o : %.cc
	$(CPP) $(INCLUDEFLAGS)  $(CXXFLAGS) $< -c -o $@


all: $(LIBOBJ)
     

run:
	./$(PROGNAME).x

clean:
	rm $(LIBOBJ) *.x
	
delete:
	rm *.txt *.png *.gnu

test:
	$(CPP) test.cc core/Randoms.o $(CXXFLAGS) $(INCLUDEFLAGS) $(LDFLAGS) -o test.x


generator: $(GENLIBOBJ) generator.o
	$(CPP) $^ -L $(INCLUDEFLAGS) $(LDFLAGS) -o generator.x 

analysis: $(GENLIBOBJ) analysis.o
	$(CPP) $^ -L $(INCLUDEFLAGS) $(LDFLAGS) -o analysis.x 
