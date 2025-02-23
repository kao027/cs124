# Compiler
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

# Source files
SRCS = randmst.cpp mst.cpp tests.cpp
HEADERS = mst.h

# Output executables
TARGET = randmst
TESTS_TARGET = tests

# Build the main executable
$(TARGET): randmst.cpp mst.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) randmst.cpp mst.cpp

# Build the tests executable
$(TESTS_TARGET): tests.cpp mst.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TESTS_TARGET) tests.cpp mst.cpp

# Clean the build
clean:
	rm -f $(TARGET) $(TESTS_TARGET)
