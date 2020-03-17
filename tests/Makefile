CXX = g++ -g -std=c++17
OBJECTS = test.o ../src/cellkernel.o
TARGET = run_tests


all: $(TARGET)

test: $(TARGET)
	./$(TARGET)

$(TARGET) : $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)
