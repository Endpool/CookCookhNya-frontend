SHELL := /bin/bash

.PHONY: all
all: build-debug

# Install dependencies and make CMake presets
.PHONY: conan-debug conan-release
conan-debug:
	conan install . --build=missing --settings=build_type=Debug
conan-release:
	conan install . --build=missing --settings=build_type=Release

# Run cmake
.PHONY: cmake-debug cmake-release
cmake-debug cmake-release: cmake-%: conan-%
	cmake --preset=conan-$* $(CMAKE_FLAGS)

build/Debug/CMakeCache.txt: conanfile.txt
	rm -f build/Debug/CMakeCache.txt
	$(MAKE) cmake-debug
build/Release/CMakeCache.txt: conanfile.txt
	rm -f build/Release/CMakeCache.txt
	$(MAKE) cmake-release

# Build using cmake
.PHONY: build-debug build-release
build-debug: build/Debug/CMakeCache.txt
	cmake --build --preset=conan-debug -- -j5
build-release: build/Release/CMakeCache.txt
	cmake --build --preset=conan-release -- -j5

# Run bot
.PHONY: start-debug gdb start-release
start-debug: build-debug
	set -a && source .env && ./build/Debug/main
gdb: build-debug
	set -a && source .env && gdb ./build/Debug/main
start-release: build-release
	set -a && source .env && ./build/Release/main

.PHONY: format-commit format-all
format-commit:
	find $$(git diff-index --cached --name-only --diff-filter=AM HEAD) \( -name '*.cpp' -o -name '*.hpp' \) -exec clang-format -i {} \;
format-all:
	find src \( -name '*.cpp' -o -name '*.hpp' \) -exec clang-format -i {} \;
