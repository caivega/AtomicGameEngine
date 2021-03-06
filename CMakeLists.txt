
project (Atomic)

cmake_minimum_required (VERSION 2.8)

set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)

include(AtomicUtils)


add_definitions(-DATOMIC_ROOT_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -DATOMIC_ROOT_BUILD_DIR="${CMAKE_BINARY_DIR}")

add_definitions( -DATOMIC_API= -DATOMIC_STATIC_DEFINE -DATOMIC_ATOMIC2D -DATOMIC_LOGGING)

add_definitions( -DATOMIC_DEV_BUILD )

# this is here as QtCreator is having trouble picking up #include <Atomic/*> without it
include_directories(${CMAKE_SOURCE_DIR}/Source ${CMAKE_SOURCE_DIR}/Source/AtomicEditor/Source)

set (ATOMIC_LINK_LIBRARIES Atomic Box2D Duktape TurboBadger FreeType JO LZ4 PugiXml STB)

if (NOT ATOMIC_BUILD_2D)
    add_definitions( -DATOMIC_PHYSICS -DATOMIC_3D)
    set (ATOMIC_LINK_LIBRARIES ${ATOMIC_LINK_LIBRARIES} Bullet StanHull)
endif()

if (NOT EMSCRIPTEN)
    add_definitions( -DATOMIC_NETWORK)
    set (ATOMIC_LINK_LIBRARIES ${ATOMIC_LINK_LIBRARIES} SDL Civetweb Recast Detour kNet )
endif()

if (MSVC)

    include(AtomicWindows)

elseif(APPLE)

    if (IOS)
        include(AtomicIOS)
    else()
        include(AtomicMac)
    endif()

elseif(LINUX)

  include(AtomicLinux)

elseif(ANDROID)

    include(AtomicAndroid)

elseif(EMSCRIPTEN)

	include(AtomicWeb)

endif()

add_subdirectory(Source)
