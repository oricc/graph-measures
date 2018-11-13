# location of the Python header files
 
PYTHON_VERSION = 3.6
PYTHON_VERSION_SHORT = 36
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
# location of the Boost Python include files and library
 
BOOST_INC =/usr/local/boost_1_67_0 
BOOST_LIB =/usr/local/boost_1_67_0/stage/lib 

# TODO: compile features/wrappers using generic commands, see: https://stackoverflow.com/questions/2908057/can-i-compile-all-cpp-files-in-src-to-os-in-obj-then-link-to-binary-in
FEATURE_SRC_DIR := features
FEATURE_OBJ_DIR := feature_obj
FEATURE_SRC_FILES := $(wildcard $(FEATURE_SRC_DIR)/*.cpp)
FEATURE_OBJ_FILES := $(patsubst $(FEATURE_SRC_DIR)/%.cpp,$(FEATURE_OBJ_DIR)/%.o,$(FEATURE_SRC_FILES))

WRAPPER_SRC_DIR := wrappers
WRAPPER_OBJ_DIR := wrapper_obj
WRAPPER_SRC_FILES := $(wildcard $(WRAPPER_SRC_DIR)/*.cpp)
WRAPPER_OBJ_FILES := $(patsubst $(WRAPPER_SRC_DIR)/%.cpp,$(WRAPPER_OBJ_DIR)/%.o,$(WRAPPER_SRC_FILES))

# compile mesh classes
TARGET = LibExportMain
FEATURE = ExampleFeatureCalculator
WRAPPER = ExampleWrapper

OUTPUT_NAME = _features

CACHE_GRAPH = CacheGraph
CONVERTER = ConvertedGNXReciever
FEATURE_TEMPLATE=FeatureCalculator


accelerated_graph_features/$(OUTPUT_NAME).so: $(TARGET).o $(WRAPPER_OBJ_FILES).o $(CONVERTER).o $(FEATURE_OBJ_FILES).o $(CACHE_GRAPH).o $(FEATURE_TEMPLATE).o
	g++ -shared -Wl,--export-dynamic $^ -L$(BOOST_LIB) -lboost_python$(PYTHON_VERSION_SHORT) -L/usr/lib/python$(PYTHON_VERSION)/config-3.6m-x86_64-linux-gnu -lpython$(PYTHON_VERSION) -o $@
	rm *.o  

################  Changes for each feature #######################
$(FEATURE_OBJ_DIR)/%.o: $(FEATURE_SRC_DIR)/%.cpp
	g++ -fPIC -c -o $@ $^
   
$(WRAPPER_OBJ_DIR)/%.o: $(WRAPPER_SRC_DIR)/%.cpp
	g++ -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -o $@ $^



#$(FEATURE).o: features/$(FEATURE).cpp # includes/$(CACHE_GRAPH).h
#	g++ -fPIC -c -o $@ $^
#	
#$(WRAPPER).o: wrappers/$(WRAPPER).cpp # includes/$(FEATURE).h includes/$(CONVERTER).h
#	g++ -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -o $@ $^

############### From here, always the same #######################
$(TARGET).o: $(TARGET).cpp
	g++  $^  -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC)
	
$(CACHE_GRAPH).o: arch/$(CACHE_GRAPH).cpp
	g++ -fPIC -c $^

$(CONVERTER).o: utils/$(CONVERTER).cpp includes/$(CACHE_GRAPH).h
	g++ $^ -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC) 

$(FEATURE_TEMPLATE).o: arch/$(FEATURE_TEMPLATE).cpp
	g++ -fPIC -c $^

clean:
	rm accelerated_graph_features/$(OUTPUT_NAME).so $(TARGET).o $(WRAPPER_OBJ_FILES).o $(CONVERTER).o $(FEATURE_OBJ_FILES).o $(CACHE_GRAPH).o $(FEATURE_TEMPLATE).o
