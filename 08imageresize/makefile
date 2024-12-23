# 編譯器與選項
CXX = g++
CXXFLAGS = -g -I./include -std=c++17 -Wall -Wextra -fdiagnostics-color=always
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:src/%.cpp=build/%.o)
TARGET = build/main

# 編譯與連結
all: build_dir $(TARGET)

build_dir:
	mkdir -p build
$(TARGET): $(OBJ)
	$(CXX) -o $@ $^

build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理建置檔案
clean:
	rm -rf build
