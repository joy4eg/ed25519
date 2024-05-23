cmake_minimum_required(VERSION 3.18)
project(ed25519 C)

cmake_policy(SET CMP0048 NEW)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS "-Wall -Wextra -Wno-unused-parameter -pipe -fstack-protector-all")
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -ggdb -gdwarf-2 -gstrict-dwarf -fno-omit-frame-pointer -fno-inline")
set(CMAKE_C_FLAGS_RELEASE "-O2")

option(ED25519_BUILD_LIBRARY_SHARED "Build shared library"   ON)
option(ED25519_BUILD_LIBRARY_STATIC "Build static library"   OFF)
option(ED25519_BUILD_TEST           "Build test application" ON)
option(ED25519_GEN_SEED             "Build with seed generation support" ON)

set(ED25519_SOURCE_FILES
	src/add_scalar.c
	src/fe.c
	src/ge.c
	src/key_exchange.c
	src/keypair.c
	src/sc.c
	src/seed.c
	src/sha512.c
	src/sign.c
	src/verify.c
)

MESSAGE(STATUS "Build type:                    " ${CMAKE_BUILD_TYPE})
MESSAGE(STATUS "Build shared library:          " ${ED25519_BUILD_LIBRARY_SHARED})
MESSAGE(STATUS "Build static library:          " ${ED25519_BUILD_LIBRARY_STATIC})
MESSAGE(STATUS "Build test app:                " ${ED25519_BUILD_TEST})

add_library(ed25519 OBJECT ${ED25519_SOURCE_FILES})
set_target_properties(ed25519 PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(ed25519 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if (NOT ED25519_GEN_SEED)
	target_compile_definitions(ed25519 PRIVATE ED25519_NO_SEED=1)
endif()

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/ed25519.pc.in"
	"${CMAKE_CURRENT_BINARY_DIR}/ed25519.pc"
	@ONLY
)

# Shared lib
if (ED25519_BUILD_LIBRARY_SHARED)
	add_library(ed25519_shared SHARED $<TARGET_OBJECTS:ed25519>)
	set_target_properties(ed25519_shared PROPERTIES OUTPUT_NAME ed25519)
	install(TARGETS ed25519_shared LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

# Static lib
if (ED25519_BUILD_LIBRARY_STATIC)
	add_library(ed25519_static STATIC $<TARGET_OBJECTS:ed25519>)
	set_target_properties(ed25519_static PROPERTIES OUTPUT_NAME ed25519)
	install(TARGETS ed25519_static LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif()

if (ED25519_BUILD_TEST)
	add_executable(ed25519_bin test.c)
	target_link_libraries(ed25519_bin ed25519)
	set_target_properties(ed25519_bin PROPERTIES OUTPUT_NAME ed25519test)
endif()

if (ED25519_BUILD_LIBRARY_SHARED OR ED25519_BUILD_LIBRARY_STATIC)
    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ed25519.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
    install(FILES ${CMAKE_SOURCE_DIR}/include/ed25519.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ed25519)
endif()
