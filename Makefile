CXX=g++
RM=rm -f
CPPFLAGS=-g -std=c++11 -O3
LDFLAGS=-g
LDLIBS=-lwiringPi

SRCS=decode.cpp Command.cpp GPIOPin.cpp PT22x2Decoder.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: decode

decode: $(OBJS)
	$(CXX) $(LDFLAGS) -o decode $(OBJS) $(LDLIBS)

depend: .depend

.depend: $(SRCS)
	$(RM) ./.depend
	$(CXX) $(CPPFLAGS) -MM $^>>./.depend

clean:
	$(RM) $(OBJS)

dist-clean: clean
	$(RM) *~ .dependtool

include .depend
