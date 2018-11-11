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
MORE = wrappers/ExampleWrapper
 
$(TARGET).so: $(TARGET).o $(MORE).o
	g++ -shared -Wl,--export-dynamic $(TARGET).o $(MORE).o -L$(BOOST_LIB) -lboost_python$(PYTHON_VERSION_SHORT) -L/usr/lib/python$(PYTHON_VERSION)/config-3.6m-x86_64-linux-gnu -lpython$(PYTHON_VERSION) -o $(TARGET).so
  

$(TARGET).o: $(TARGET).cpp $(MORE).h
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -l$(MORE) -fPIC -c $^
$(MORE).o: $(MORE).cpp
	g++ -I$(PYTHON_INCLUDE) -I$(BOOST_INC) -fPIC -c $(MORE).cpp

clean:
	rm $(TARGET).o $(TARGET).so $(MORE).o $(MORE).h.gch 