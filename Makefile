.SUFFIXES:
.SECONDARY:

BUILD := build

CXXFLAGS ?= -pedantic \
            -Wall -Wextra -Wshadow -Werror \
						-std=c++11 -Iinclude

ifdef NDEBUG
CXXFLAGS += -D NDEBUG
BUILD := $(BUILD)/ndebug
else
BUILD := $(BUILD)/debug
endif

ifdef USE_PEGTL
CXXFLAGS += -IPEGTL/include -D USE_PEGTL
BUILD := $(BUILD)/use_pegtl
else
BUILD := $(BUILD)/no_pegtl
endif

HEADER := $(shell find include -name '*.hpp')

SRC_EXAMPLE := $(shell find example -name '*.cpp')
DEP_EXAMPLE := $(SRC_EXAMPLE:%.cpp=$(BUILD)/%.d)
BIN_EXAMPLE := $(SRC_EXAMPLE:%.cpp=$(BUILD)/%)

SRC_TEST := $(shell find test -name '*.cpp')
DEP_TEST := $(SRC_TEST:%.cpp=$(BUILD)/%.d)
BIN_TEST := $(SRC_TEST:%.cpp=$(BUILD)/%)

# default
all: .build_test .build_example
.PHONY: all

# help
help:
	@echo "goals:"
	@echo "  all (default): build tests and examples"
	@echo "  test : run tests"
	@echo "  clean : clean everything"
	@echo "  help : show help"
	@echo ""
	@echo "optional variables:"
	@echo "  USE_PEGTL : build with PEGTL library"
	@echo "  NDEBUG : build in none Debug mode"
	@echo ""
	@echo "for example, build and run test with PEGTL :"
	@echo "  $ make USE_PEGTL=1 test"
	@echo ""

# clean build dir
clean:
	@rm -rf build/

# build test
.build_test: $(BIN_TEST)

# build example
.build_example: $(BIN_EXAMPLE)

# run test
test: .build_test
	@echo "Run tests..."
	@set -e; for T in $(BIN_TEST) ; do echo $$T ; $$T > $$T.log ; tail -1 $$T.log ; done

# compile cpp
$(BUILD)/%: %.cpp $(BUILD)/%.d
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

# generates include-dependency on the fly
$(BUILD)/%.d: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MM -MQ $@ $< -o $@

# include generated include-dependency if possible
ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEP_EXAMPLE)
-include $(DEP_TEST)
endif