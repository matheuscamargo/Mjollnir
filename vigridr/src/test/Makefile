CC = g++ -Wall -Werror -std=c++0x -DGTEST_USE_OWN_TR1_TUPLE=0
INCLUDES = -I ../../third-parties/cpp/include
LFLAGS = -L../../third-parties/cpp/lib
LIBRARIES = -pthread -lgtest
TARGETS = GameTest
OBJECTS_GameTest = ../../obj/server/GameLogic.o ../../obj/thrifts/gen-cpp/Command_constants.o ../../obj/thrifts/gen-cpp/Command_types.o ../../obj/thrifts/gen-cpp/GameDescription_types.o ../../obj/thrifts/gen-cpp/WorldModel_types.o ../../obj/thrifts/gen-cpp/GameResult_types.o
BIN := ../../bin/cpp

all: $(BIN) $(BIN)/$(TARGETS)

$(BIN):
	@mkdir -p $(BIN)
	@echo "Created test bin directory"

$(BIN)/$(TARGETS): $(BIN)/% : %.cpp
	$(CC) $< $(OBJECTS_$(<:%.cpp=%)) $(INCLUDES) $(LFLAGS) -o $@ $(LIBRARIES)

remove:
	rm -rf bin

clean: remove

test: $(BIN)/$(TARGETS)
	$(foreach target, $(TARGETS), ./$(BIN)/$(target);)
