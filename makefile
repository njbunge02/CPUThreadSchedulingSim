CXX = g++
OPTIONS = -Wall
PROGRAM =a.out

all: $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM) -rr < in1.txt

$(PROGRAM): project3.cpp
	$(CXX) $(OPTIONS) project3.cpp -o $(PROGRAM) 

clean:
	rm -f $(PROGRAM)

