CXXFLAGS=-O3 -std=c++0x -msse2 -mavx
OBJS=main.o timediff.o implementations_generic.o implementations_avx.o implementations_sse2.o
TARGET=convTest


all: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)