# CMakeList.txt : CMake project for Sulpur, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Sulpur")
find_package(Vulkan REQUIRED)
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

#glfw
add_subdirectory(External/glfw-3.4)
add_subdirectory(External/glm)

#Resource path
set(RESOURCE_PATH ${CMAKE_SOURCE_DIR})

# Add source to this project's executable.
add_executable(Sulpur ${SOURCES})
target_compile_definitions(Sulpur PUBLIC RESOURCE_PATH="${RESOURCE_PATH}/assets")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sulpur PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(Sulpur Vulkan::Vulkan glfw glm)


# TODO: Add tests and install targets if needed.
