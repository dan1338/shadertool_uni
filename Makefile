all:
	rm -rf build
	cmake -B build -DCMAKE_BUILD_TYPE=DEBUG
	make -C build -j8

test-copy:
	./build/shadertool scripts/copy.script -i data/cars3.webm -o copy.webm

test-stereo:
	./build/shadertool scripts/stereo.script -a data/cars3.webm -b data/city.mp4 -o stereo.webm

test-blur:
	./build/shadertool scripts/blur.script -i data/cars3.webm -o blur.webm

test-blend:
	./build/shadertool scripts/blend.script -a data/cars3.webm -b data/city.mp4 -o blend.webm

test-waves:
	./build/shadertool scripts/waves.script -i data/lores/cars.webm -o waves.webm

.PHONY: all test
