.PHONY: config clean all

all: config
	@make -C build

config:
	@mkdir -p build
	@cd build && cmake ..

clean:
	@rm -rf build/*
