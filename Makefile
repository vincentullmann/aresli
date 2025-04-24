
CXX ?= g++

working_dir	:= ${shell pwd}
BUILD_PATH = build
dist_dir  ?= dist
SRC_PATH := src
SRC_EXT = cpp

name = MyAssetResolver


# code lists #
# Find all source files in the source directory, sorted by
# most recently modified
SOURCES = $(shell find $(SRC_PATH) -name '*.cpp' | sort -k 1nr | cut -f2-)
# Set the object file names, with the source directory stripped
# from the path, and the build path prepended in its place
OBJECTS = $(SOURCES:$(SRC_PATH)/%.$(SRC_EXT)=$(BUILD_PATH)/%.o)
# Set the dependency files that will be used to add header dependencies
DEPS = $(OBJECTS:.o=.d)


# flags #
CXXFLAGS = -fPIC -std=c++17 -Wall -Wextra -g
INCLUDES = -I $(SRC_PATH) -I include/ -I /usr/local/include
# Space-separated pkg-config libraries used by this project
LIBS = 

# Python
INCLUDES += -I/usr/include/python3.11/
LIBS += -lboost_python


# USD
USD_ROOT ?= /software/packages/usd/24.11/_v/a
INCLUDES += -I$(USD_ROOT)/include

# LIBS += -L$(USD_ROOT)/lib -lusd_usd -lusd_ar -lusd_tf

USD_LIBS = $(wildcard $(USD_ROOT)/lib/*.so)
USD_LIBS := $(patsubst lib%.so,%,$(notdir $(USD_LIBS)))
USD_LIBS := $(addprefix -l,$(USD_LIBS))
LIBS += -L$(USD_ROOT)/lib $(USD_LIBS)
# -lusd_usd -lusd_ar -lusd_tf


# LDFLAGS = \
# 	-L$$(ILMBASE_LIB_PATH) \
# 	-L$$(BOOST_LIB_PATH) \
# 	-lMillGlProcedural -lMillIOProcedural -lMillProcedural \
# 	-lMillUtils -lMillUuid \
# 	-Wl,-Bstatic -lImath -lIex \
# 	-lboost_system-$$(BOOST_LIB_SUFFIX) -Wl,-Bdynamic \
# 	-lz -ldl -lGL -lpthread -lyaml \
# 	-lhboost_filesystem -lhboost_system -ltbb \
# 	-lHoudiniAPPS3
# 
# CXXFLAGS = \
# 	-DBUILD_NUMBER=\"$(BUILD_NUMBER)\" \
# 	-DPROFILE=0 \
# 	-I$(ILMBASE_INCLUDE_PATH) \
# 	-I$(BOOST_INCLUDE_PATH) \
# 	-I$(EXPAT_INCLUDE_PATH) \
# 	-D_GLIBCXX_USE_CXX11_ABI=0


################################################################################

$(BUILD_PATH)/%.o: $(SRC_PATH)/%.$(SRC_EXT)
	mkdir -p $(BUILD_PATH)
	@echo "Compiling: $< -> $@"
	$(CXX) $(CXXFLAGS) $(INCLUDES) -MP -MMD -c $< -o $@


# build library
$(BUILD_PATH)/$(name).so: $(OBJECTS)
	@echo "Linking: $@"
	$(CXX) $(CXXFLAGS) $(OBJECTS) -shared -o $@ ${LIBS}


build: dist/$(name).so
	@echo "build"


install: $(BUILD_PATH)/$(name).so
	rm -rf $(dist_dir)
	mkdir -p $(dist_dir)

	cp $(BUILD_PATH)/$(name).so $(dist_dir)/
	cp $(SRC_PATH)/plugInfo.json $(dist_dir)/


clean:
	rm -rf build *.so


test: install
	@echo "Running tests..."
	rez env usd pytest -- TF_DEBUG=AR_RESOLVER_INIT PXR_PLUGINPATH_NAME=${working_dir}/$(dist_dir) pytest tests

