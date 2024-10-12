.PHONY: config clean compiler bin run

RT_SRCS=$(wildcard )

compiler: config
	make -C build -j32

config:
	mkdir -p build
	cd build && cmake ..

clean:
	rm -rf build/*

build/ouput: compiler test/input
	./build/simple test/input > build/output.ll
	llc -relocation-model=pic build/output.ll -o build/output.s
	clang++ -c -fPIE build/output.s -o build/output.o
	clang++ -pie -o build/output build/output.o lib/runtime.cpp

run: build/ouput
	./build/output