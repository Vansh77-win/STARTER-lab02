CXX ?= c++
CXXFLAGS ?= -Wall -Wextra -std=c++11 -O2
TARGET ?= lab02
SOURCES := intbst.cpp testbst.cpp
OBJECTS := $(SOURCES:.cpp=.o)
TEST_TARGET := test_intbst

.PHONY: all clean

all: $(TARGET)

$(TEST_TARGET): intbst.o test_intbst.o
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(TARGET): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJECTS) test_intbst.o $(TARGET) $(TEST_TARGET)
