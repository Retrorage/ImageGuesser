cmake_minimum_required(VERSION 3.15)

project(ImageGuesser VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)


set(CPP_FILES
	main.cpp
)
list(TRANSFORM CPP_FILES PREPEND src/${PROJECT_NAME}/)

set(SANITIZERS undefined address)



### clang compile commands ###
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

## targets
add_executable(${PROJECT_NAME} ${CPP_FILES})
add_custom_target(${PROJECT_NAME}_assets ALL
	COMMAND ${CMAKE_COMMAND} -E create_symlink ${PROJECT_SOURCE_DIR}/assets ${PROJECT_BINARY_DIR}/assets)
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_assets)



## compiler args
list(APPEND CC_ARGS
	"-Wall" "-Wextra" "-Wno-missing-field-initializers"
)

target_compile_options(${PROJECT_NAME} PUBLIC ${CC_ARGS})
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-O0>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-g>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-ftemplate-backtrace-limit=0>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:-Werror>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:-O3>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:-ffast-math>")

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
	target_compile_options(${PROJECT_NAME} PUBLIC "-fsized-deallocation")
endif()

foreach(SAN IN LISTS SANITIZERS)
	target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-fsanitize=${SAN}>")
	target_link_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:-fsanitize=${SAN}>")
endforeach()



## dependencies
list(APPEND CMAKE_PREFIX_PATH /usr/local)
find_package(PkgConfig REQUIRED)

pkg_search_module(SFML REQUIRED sfml-graphics)
target_link_libraries(${PROJECT_NAME} PUBLIC ${SFML_LIBRARIES})
target_link_directories(${PROJECT_NAME} PUBLIC ${SFML_LIBRARY_DIRS})
target_link_options(${PROJECT_NAME} PUBLIC ${SFML_LDFLAGS})
target_include_directories(${PROJECT_NAME} PUBLIC ${SFML_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${SFML_CFLAGS})

pkg_search_module(JSONCPP REQUIRED jsoncpp)
target_link_libraries(${PROJECT_NAME} PUBLIC ${JSONCPP_LIBRARIES})
target_link_directories(${PROJECT_NAME} PUBLIC ${JSONCPP_LIBRARY_DIRS})
target_link_options(${PROJECT_NAME} PUBLIC ${JSONCPP_LDFLAGS})
target_include_directories(${PROJECT_NAME} PUBLIC ${JSONCPP_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${JSONCPP_CFLAGS})

pkg_search_module(LIBCURL REQUIRED libcurl)
target_link_libraries(${PROJECT_NAME} PUBLIC ${LIBCURL_LIBRARIES})
target_link_directories(${PROJECT_NAME} PUBLIC ${LIBCURL_LIBRARY_DIRS})
target_link_options(${PROJECT_NAME} PUBLIC ${LIBCURL_LDFLAGS})
target_include_directories(${PROJECT_NAME} PUBLIC ${LIBCURL_INCLUDE_DIRS})
target_compile_options(${PROJECT_NAME} PUBLIC ${LIBCURL_CFLAGS})



### include directory ###
target_include_directories(${PROJECT_NAME} BEFORE PUBLIC "${PROJECT_SOURCE_DIR}/include")
