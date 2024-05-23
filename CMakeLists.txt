cmake_minimum_required(VERSION 3.14)
project(DateProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(date)
add_subdirectory(test)

add_executable(
    ${PROJECT_NAME}
    main.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    date
)

if (NOT googletest) # is googletest defined if already built?
	include(FetchContent)
	FetchContent_Declare(
		googletest
		# Specify the commit you depend on and update it regularly.
		URL https://github.com/google/googletest/archive/refs/heads/master.zip
	)
	# For Windows: Prevent overriding the parent project's compiler/linker settings
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
	FetchContent_MakeAvailable(googletest)
endif()