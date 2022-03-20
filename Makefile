all:
	rm -rf build
	cmake -B build -DCMAKE_BUILD_TYPE=DEBUG
	make -C build -j8
.PHONY: all
