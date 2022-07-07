cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(qdma_benchmark LANGUAGES CXX VERSION 0.1)

add_definitions(-mavx512f)
add_compile_options(-O3 -march=native)


add_executable(qdma_throughput
	qdma_throughput.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mmio/mmio.cpp
)
target_include_directories(qdma_throughput PUBLIC
	${CMAKE_SOURCE_DIR}/dma
	${CMAKE_CURRENT_SOURCE_DIR}/mmio
)
target_link_libraries(qdma_throughput
	qdma
	pthread
)

add_executable(qdma_random
	qdma_random.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mmio/mmio.cpp
)
target_include_directories(qdma_random PUBLIC
	${CMAKE_SOURCE_DIR}/dma
	${CMAKE_CURRENT_SOURCE_DIR}/mmio
)
target_link_libraries(qdma_random
	qdma
	pthread
)

add_executable(qdma_latency
	qdma_latency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/dma/dma.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/mmio/mmio.cpp
)
target_include_directories(qdma_latency PUBLIC
	${CMAKE_SOURCE_DIR}/dma
	${CMAKE_CURRENT_SOURCE_DIR}/mmio
)
target_link_libraries(qdma_latency
	qdma
	pthread
)

add_executable(axil_latency
	axil_latency.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/load/load.cpp
)
target_include_directories(axil_latency PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/load
)
target_link_libraries(axil_latency
	qdma
	pthread
)