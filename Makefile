CFLAGS=`llvm-config --cxxflags --ldflags --system-libs --libs core`
LDFLAGS=
SRC=main.cpp read.cpp ast.cpp obj.cpp

all: main.o;
	clang++ $(SRC) -o Bleh -g $(CFLAGS) $(LDFLAGS)

run:
	./Bleh

clean: rm -f *.o Bleh
