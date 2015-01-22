CXXFLAGS=-O3 -std=c++0x -s -msse2 -mavx
OBJS=main.o timediff.o
TARGET=convTest


all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)