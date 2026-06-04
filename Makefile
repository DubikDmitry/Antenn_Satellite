CXX = g++
CXXFLAGS = -std=c++17 -pthread -Wall -O2
BUILD_DIR = build

INC_DIRS = . AntennaWorker ConcurrentQueue Config DataGenerator Error \
           MainController ReceivingAntenna RowData SatelliteWorker ThreadPool
INCLUDES = $(addprefix -I, $(INC_DIRS))

# Общие объектные файлы (все классы без main.cpp и benchmark.cpp)
COMMON_SOURCES = AntennaWorker/AntennaWorker.cpp \
                 ConcurrentQueue/ConcurrentQueue.cpp \
                 Config/Config.cpp \
                 DataGenerator/DataGenerator.cpp \
                 Error/Error.cpp \
                 MainController/MainController.cpp \
                 ReceivingAntenna/ReceivingAntenna.cpp \
                 RowData/RowData.cpp \
                 SatelliteWorker/SatelliteWorker.cpp \
                 ThreadPool/ThreadPool.cpp

COMMON_OBJECTS = $(addprefix $(BUILD_DIR)/, $(COMMON_SOURCES:.cpp=.o))

all: $(BUILD_DIR) simulation benchmark

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Компиляция общих объектных файлов
$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Программа simulation (обычный main.cpp)
simulation: $(BUILD_DIR)/main.o $(COMMON_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/main.o: main.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Программа benchmark (для замеров производительности)
benchmark: $(BUILD_DIR)/benchmark.o $(COMMON_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/benchmark.o: benchmark.cpp
	mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) simulation benchmark

run-sim: simulation
	./simulation

run-bench: benchmark
	./benchmark

.PHONY: all clean run-sim run-bench