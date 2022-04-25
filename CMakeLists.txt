cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 11)

project(qdma_test LANGUAGES CXX)

add_definitions(-mavx512f)

add_executable(qdma_test
	main.cpp
	mmio.cpp
	dma.cpp
)
target_link_libraries(qdma_test
	qdma
	pthread
)