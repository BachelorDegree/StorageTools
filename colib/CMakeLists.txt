cmake_minimum_required(VERSION 2.8)
project(libco)

# This for mac osx only
set(CMAKE_MACOSX_RPATH 0)

# Set lib version
set(LIBCO_VERSION   0.5)

# Set cflags
set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -g -fno-strict-aliasing -O2 -Wall -export-dynamic -Wall -pipe  -D_GNU_SOURCE -D_REENTRANT -fPIC -Wno-deprecated -m64)

# Use c and asm
enable_language(C ASM)

# Add source files
set(SOURCE_FILES
        co_hook_sys_call.cpp
        co_routine.cpp
	co_routine_specific.cpp
	coctx_mov.cpp
	coctx_swap_mov.S
	co_aio.cpp
	co_comm.cpp
	co_routine_pool.cpp
	sync/co_condition_variable.cpp
	sync/co_mutex.cpp
)
set(HEADER_FILES
	co_aio.h
	co_aio_utils.h
	co_channel.h
	co_comm.h
	co_hook_sys_call.h
	co_routine.h
	co_routine_inner.h
	co_routine_lambda.h
	co_routine_pool.h
	co_routine_specific.h
	coctx.h
	cpp_awk.hpp
	demangle.h
	macros.h
	sync/co_mutex.h
	sync/co_condition_variable.h
	sync/co_rwmutex.h
	sync/lock_entry.h
	sync/ref_object.h
	sync/spin_lock.h
)

include_directories(.)

# Add static and shared library target
add_library(colib_static STATIC ${SOURCE_FILES})
add_library(colib_shared SHARED ${SOURCE_FILES})
target_link_libraries(colib_static aio)
target_link_libraries(colib_shared aio)

# Set library output name
set_target_properties(colib_static PROPERTIES OUTPUT_NAME colib)
set_target_properties(colib_shared PROPERTIES OUTPUT_NAME colib)

set_target_properties(colib_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)
set_target_properties(colib_shared PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# Set shared library version, will generate libcolib.${LIBCO_VERSION}.so and a symbol link named libcolib.so
# For mac osx, the extension name will be .dylib
set_target_properties(colib_shared PROPERTIES VERSION ${LIBCO_VERSION} SOVERSION ${LIBCO_VERSION})

INSTALL(TARGETS colib_static LIBRARY DESTINATION lib)
INSTALL(TARGETS colib_shared LIBRARY DESTINATION lib)
INSTALL(FILES ${HEADER_FILES} DESTINATION include/colib)
