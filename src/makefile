OS_NAME = $(shell uname -o)

# make test
test: test.cpp carbon.cpp carbon.hpp carbon_coef.hpp
	g++ -std=c++23 -Wall -O3 test.cpp carbon.cpp -o test

# make dll
dll: carbon.cpp carbon.hpp carbon_coef.hpp
	g++ -c -std=c++23 -Wall -O3 carbon.cpp
ifeq ($(OS_NAME),Linux)
	g++ -shared carbon.o -o carbon.a
else
	g++ -shared carbon.o -o carbon.dll
endif	
	
# run the test
RUN: 
	./test
	