project(event_camera_comb_filter)
cmake_minimum_required(VERSION 3.0)

# import external dependencies
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 17)

find_package(yaml-cpp REQUIRED)


include_directories(include)
add_executable(event_camera_comb_filter main.cpp comb_filter.cpp)

set(EXECUTABLES_LIST
    event_camera_comb_filter
)

foreach(EXEC_NAME ${EXECUTABLES_LIST})
    target_include_directories(${EXEC_NAME} PRIVATE
        include
        ${MODULE_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${YAML_CPP_INCLUDE_DIR}
    )
    if(UNIX) 
        message(STATUS "Unix detected")
        target_link_libraries(${EXEC_NAME}
            ${MODULE_LIBS}
            ${OpenCV_LIBS}
            # clang
            pthread
            yaml-cpp
        )
    endif()
    if(WIN32) 
        message(STATUS "Windows detected")
        target_link_libraries(${EXEC_NAME}
            ${MODULE_LIBS}
            ${OpenCV_LIBS}
            pthreadVC3
        )
    endif()
endforeach()
