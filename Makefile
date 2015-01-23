CXXFLAGS=-O3 -std=c++0x
OBJS=main.o timediff.o implementations_generic.o
TARGET=convTest


all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)