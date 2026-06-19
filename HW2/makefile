CXX := g++
CXXFLAGS := -std=c++17 -O3
SRC := src/main.cpp src/utils.cpp src/hash.cpp

hw2: $(SRC)
	$(CXX) $(CXXFLAGS) -o hw2 $(SRC)

.PHONY: clean
clean:
	rm -f hw2 output/key_query_out1.txt output/key_query_out2.txt output/time_used.txt
