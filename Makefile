CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -O3
LDFLAGS=-g
LDLIBS=-lwiringPi

BSRCS=decode.cpp send.cpp
OSRCS=Command.cpp GPIOPin.cpp PT22x2.cpp
OBJS=$(subst .cpp,.o,$(OSRCS))

all: decode send

decode: $(OBJS) decode.o
	$(CXX) $(LDFLAGS) -o decode decode.o $(OBJS) $(LDLIBS)

send: $(OBJS) send.o
	$(CXX) $(LDFLAGS) -o send send.o $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend

clean:
	$(RM) $(OBJS) send.o decode.o

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
