CXX ?= g++
CXXFLAGS = -g -Wall -Wextra

%.o : %.cpp %.h
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -c $<

talon6_emul: talon6_emul.o talon6.o
	$(CXX) $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) talon6_emul.o talon6.o -o $@

all: talon6_emul
