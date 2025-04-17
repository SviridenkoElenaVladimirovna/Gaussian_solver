PROJECT = gaussian_solver
LIBPROJECT = $(PROJECT).a
TESTPROJECT = test-$(PROJECT)

CXX = g++
AR = ar
ARFLAGS = rsv

EIGEN_PATH = /home/vboxuser/Desktop/eigen-3.4.0
CXXFLAGS = -I. -I$(EIGEN_PATH) -std=c++20 -Wall -Wextra -O3 -march=native
LDFLAGS = -L. -l:$(LIBPROJECT)
LDFLAGS_TEST = $(LDFLAGS) -lgtest -lgtest_main -lpthread

DEPS = gaussian_solver.h
OBJ = gaussian_solver.o main.o
TEST_OBJ = tests.o

.PHONY: default
default: all

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(LIBPROJECT): gaussian_solver.o
	$(AR) $(ARFLAGS) $@ $^

$(PROJECT): main.o $(LIBPROJECT)
	$(CXX) -o $@ main.o $(LDFLAGS)

$(TESTPROJECT): $(TEST_OBJ) $(LIBPROJECT)
	$(CXX) -o $@ $(TEST_OBJ) $(LDFLAGS_TEST)

test: $(TESTPROJECT)

all: $(PROJECT)

.PHONY: clean
clean:
	rm -f *.o

.PHONY: cleanall
cleanall: clean
	rm -f $(PROJECT) $(LIBPROJECT) $(TESTPROJECT)
