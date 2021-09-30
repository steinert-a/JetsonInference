cmake_minimum_required(VERSION 3.10.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

# Qt
set(CMAKE_AUTOMOC ON)

# CUDA
set(PATH_CUDA "/usr/local/cuda")
link_directories(${PATH_CUDA}/lib64)

# target
project(jetson_inference VERSION 1.0.0)
add_executable(${PROJECT_NAME} "")

# standard
target_link_libraries(${PROJECT_NAME} -lpthread -lm -lstdc++)

# CUDA
target_include_directories(${PROJECT_NAME} PRIVATE ${PATH_CUDA}/include)
target_link_libraries(${PROJECT_NAME} -lnvinfer -lnvparsers -lnvinfer_plugin -lnvonnxparser -lcudnn -lcublas -lcudart -lrt -ldl)

# Open CV
find_package(OpenCV REQUIRED PATHS "/usr/local/lib/cmake/opencv4")
target_include_directories(${PROJECT_NAME} PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

# Qt
find_package(Qt5Core COMPONENTS Core REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Qt5Core_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Qt5Core_LIBRARIES})

add_subdirectory(src)
