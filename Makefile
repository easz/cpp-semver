.SUFFIXES:
.SECONDARY:

CXXFLAGS ?= -pedantic \
            -Wall -Wextra -Wshadow -Werror \
						-std=c++11 -Iinclude

ifdef USE_PEGTL
CXXFLAGS += -IPEGTL/include -D USE_PEGTL
endif

ifdef NDEBUG
CXXFLAGS += -D NDEBUG
endif

HEADER := $(shell find include -name '*.hpp')

SRC_EXAMPLE := $(shell find example -name '*.cpp')
BIN_EXAMPLE := $(SRC_EXAMPLE:%.cpp=build/%)

SRC_TEST := $(shell find test -name '*.cpp')
BIN_TEST := $(SRC_TEST:%.cpp=build/%)

# default
all: .build_test .build_example
.PHONY: all

# help
help:
	@echo "targets:"
	@echo "  all (default): build tests and examples"
	@echo "  test : run tests"
	@echo "  clean : clean everything"
	@echo "  help : show help"
	@echo ""
	@echo "option variables:"
	@echo "  USE_PEGTL : use PEGTL library"
	@echo "  NDEBUG : None Debug mode"
	@echo ""
	@echo "for example, build and run test with PEGTL :"
	@echo "  > make USE_PEGTL=1 test"
	@echo ""
.PHONY: help

# clean build dir
clean:
	@rm -rf build
.PHONY: clean

# build test
.build_test: $(BIN_TEST)
.PHONY: .build_test

# build example
.build_example: $(BIN_EXAMPLE)
.PHONY: .build_example

# run test
test: .build_test
	@echo "Run tests..."
	@set -e; for T in $(BIN_TEST) ; do echo $$T ; $$T > $$T.log ; tail -1 $$T.log ; done
.PHONY: test

# generates include-dependency
build/%.d: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MM -MQ $@ $< -o $@

# compile cpp
build/%: %.cpp build/%.d
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@
