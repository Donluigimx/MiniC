CC = g++
CFLAGS = -std=c++11 -c
SOURCES = $(wildcard src/*.cpp)

compiler: $(SOURCES:src/%.cpp=src/obj/%.o)
	@echo Linking objects.
	@$(CC) $^ -o $@

src/obj/%.o: src/%.cpp
	@echo Compiling and assembling $< file.
	@$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -f src/obj/*.o
	@rm -f compiler
	@echo Objects and executable deleted.

rebuild: clean compiler
