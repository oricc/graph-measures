# location of the Python header files
 
PYTHON_VERSION = 3.6
PYTHON_VERSION_SHORT = 36
PYTHON_INCLUDE = /usr/include/python$(PYTHON_VERSION)
 
# location of the Boost Python include files and library
 
BOOST_INC =/usr/local/boost_1_67_0 
BOOST_LIB =/usr/local/boost_1_67_0/stage/lib 
 
# compile mesh classes
TARGET = LibExportMain
# TODO: add FEATURE to compile list
FEATURE = ExampleFeatureCalculator
WRAPPER = ExampleWrapper

OUTPUT_NAME = _features

CACHE_GRAPH = CacheGraph
CONVERTER = ConvertedGNXReciever
FEATURE_TEMPLATE=FeatureCalculator
 
$(OUTPUT_NAME).so: $(TARGET).o $(WRAPPER).o $(CONVERTER).o $(FEATURE).o $(CACHE_GRAPH).o $(FEATURE_TEMPLATE).o
	g++ -shared -Wl,--export-dynamic $^ -L$(BOOST_LIB) -lboost_python$(PYTHON_VERSION_SHORT) -L/usr/lib/python$(PYTHON_VERSION)/config-3.6m-x86_64-linux-gnu -lpython$(PYTHON_VERSION) -o $(OUTPUT_NAME).so
  

$(TARGET).o: $(TARGET).cpp
	g++  $^  -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC)
	
$(FEATURE).o: features/$(FEATURE).cpp includes/$(CACHE_GRAPH).h
	g++ -fPIC -c  $^

$(CACHE_GRAPH).o: arch/$(CACHE_GRAPH).cpp
	g++ -fPIC -c $^

$(WRAPPER).o: wrappers/$(WRAPPER).cpp includes/$(FEATURE).h includes/$(CONVERTER).h
	g++ $^ -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC) 

$(CONVERTER).o: utils/$(CONVERTER).cpp includes/$(CACHE_GRAPH).h
	g++ $^ -fPIC -c -I$(PYTHON_INCLUDE) -I$(BOOST_INC) 

$(FEATURE_TEMPLATE).o: arch/$(FEATURE_TEMPLATE).cpp
	g++ -fPIC -c $^

clean:
	rm $(OUTPUT_NAME).so $(TARGET).o $(WRAPPER).o $(FEATURE).o $(CONVERTER).o $(CACHE_GRAPH).o $(FEATURE_TEMPLATE).o
