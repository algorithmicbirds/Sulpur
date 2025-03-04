cmake_minimum_required (VERSION 3.10)


# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Sulpur")
find_package(Vulkan REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.hpp")

#External libs
add_subdirectory(External/glfw-3.4)
add_subdirectory(External/glm)

set(RESOURCE_PATH ${CMAKE_SOURCE_DIR})

# Add source to this project's executable.
add_executable(Sulpur ${SOURCES})
add_compile_definitions(RESOURCE_PATH="${RESOURCE_PATH}/assets/")


if(UNIX)
    add_custom_command(
        TARGET Sulpur PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan "Compiling shaders using compile.sh"        
        COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --cyan ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_SOURCE_DIR}/assets/shaders/compile.sh
        COMMENT "Running shader compilation on Unix"
    )
endif()

if(MSVC)

	set_target_properties(Sulpur PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") 
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Sulpur PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(Sulpur Vulkan::Vulkan glfw glm)


# TODO: Add tests and install targets if needed.
