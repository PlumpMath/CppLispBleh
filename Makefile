CFLAGS=`llvm-config --cxxflags --ldflags --system-libs --libs all`
LDFLAGS=

all:
	clang++ main.cpp read.cpp ast.cpp obj.cpp -o Bleh -g $(CFLAGS) $(LDFLAGS)

run:
	./Bleh

clean: rm -f *.o Bleh
