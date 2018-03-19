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
DEP_EXAMPLE := $(SRC_EXAMPLE:%.cpp=build/%.d)
BIN_EXAMPLE := $(SRC_EXAMPLE:%.cpp=build/%)

SRC_TEST := $(shell find test -name '*.cpp')
DEP_TEST := $(SRC_TEST:%.cpp=build/%.d)
BIN_TEST := $(SRC_TEST:%.cpp=build/%)

# default
.PHONY: all
all: .build_test .build_example

# help
.PHONY: help
help:
	@echo "target:"
	@echo "  all (default): build tests and examples"
	@echo "  test : run tests"
	@echo "  clean : clean everything"
	@echo "  help : show help"
	@echo ""
	@echo "options variable:"
	@echo "  USE_PEGTL : use PEGTL library"
	@echo "  NDEBUG : None Debug mode"
	@echo ""
	@echo "for example, build and run test with PEGTL :"
	@echo "  > make USE_PEGTL=1 test"
	@echo ""

# clean build dir
.PHONY: clean
clean:
	@rm -rf build

# build test
.PHONY: .build_test
.build_test: $(BIN_TEST)

# build example
.PHONY: .build_example
.build_example: $(BIN_EXAMPLE)

# run test
.PHONY: test
test: $(BIN_TEST)
	@set -e; for T in $(BIN_TEST) ; do echo $$T ; $$T > $$T.log ; tail -1 $$T.log ; done

build/%.d: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -MM -MQ $@ $< -o $@

build/%: %.cpp build/%.d
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $< -o $@

ifeq ($(findstring $(MAKECMDGOALS),clean),)
-include $(DEP_TEST)
-include $(DEP_EXAMPLE)
endif