CC=g++
CFLAGS=-Wall -std=c++1y `Magick++-config --cxxflags --cppflags`

LD=g++
LDFLAGS=-O2 `Magick++-config --ldflags --libs`

EXECUTABLE=SLIP

RM=rm
RMFLAGS=-rf
RMFILES=*.o $(EXECUTABLE)

CPPFILES=$(wildcard *.cpp)

all: $(EXECUTABLE)

$(EXECUTABLE): $(CPPFILES:.cpp=.o)
	@echo Compiling executable: $@
	@$(LD) $^ -o $(EXECUTABLE) $(LDFLAGS)

%.o: %.cpp
	@echo Compiling object file: $@
	@$(CC) -c $< $(CFLAGS) -o $@ 

clean:
	@echo Cleaning files...
	@-$(RM) $(RMFLAGS) $(RMFILES)

remake:
	@$(MAKE) clean
	@$(MAKE)
