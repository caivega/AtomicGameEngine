
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/rapidjson/include
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/kNet/include
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/FreeType/include
                    ${CMAKE_SOURCE_DIR}/Source/ThirdParty/Box2D)

file (GLOB CONTAINER_SOURCE Container/*.cpp Container/*.h)
file (GLOB CORE_SOURCE Core/*.cpp Core/*.h)
file (GLOB MATH_SOURCE Math/*.cpp Math/*.h)
file (GLOB ENGINE_SOURCE Engine/*.cpp Engine/*.h)
file (GLOB INPUT_SOURCE Input/*.cpp Input/*.h)
file (GLOB IO_SOURCE IO/*.cpp IO/*.h)
file (GLOB RESOURCE_SOURCE Resource/*.cpp Resource/*.h)
file (GLOB AUDIO_SOURCE Audio/*.cpp Audio/*.h)
file (GLOB NETWORK_SOURCE Network/*.cpp Network/*.h)

if (NOT EMSCRIPTEN)
  file (GLOB IPC_SOURCE IPC/*.cpp IPC/*.h)
endif()

file (GLOB ATOMIC2D_SOURCE Atomic2D/*.cpp Atomic2D/*.h)
file (GLOB SCENE_SOURCE Scene/*.cpp Scene/*.h)
file (GLOB UI_SOURCE UI/*.cpp UI/*.h)

if (NOT ATOMIC_BUILD_2D)
  file (GLOB ATOMIC3D_SOURCE Atomic3D/*.cpp Atomic3D/*.h)
  file (GLOB PHYSICS_SOURCE Physics/*.cpp Physics/*.h)
  file (GLOB NAVIGATION_SOURCE Navigation/*.cpp Navigation/*.h)
  file (GLOB ENVIRONMENT_SOURCE Environment/*.cpp Environment/*.h)
endif()

file (GLOB GRAPHICS_SOURCE Graphics/*.cpp Graphics/*.h)

if (MSVC)
    file (GLOB GRAPHICS_IMPL_SOURCE Graphics/Direct3D9/*.cpp Graphics/Direct3D9/*.h)
else()

# for kNet
    add_definitions (-DUNIX)
    file (GLOB GRAPHICS_IMPL_SOURCE Graphics/OpenGL/*.cpp Graphics/OpenGL/*.h)
endif()

if (ANDROID)

endif()

if (APPLE)
    if (NOT IOS)
      set (PLATFORM_SOURCE IO/MacFileWatcher.mm)
    else()
    endif()
endif()

if (EMSCRIPTEN)

endif()

set (SOURCE_FILES ${CONTAINER_SOURCE} ${CORE_SOURCE} ${ENGINE_SOURCE} ${INPUT_SOURCE}
                  ${AUDIO_SOURCE} ${IO_SOURCE} ${MATH_SOURCE}
                  ${RESOURCE_SOURCE} ${PHYSICS_SOURCE} ${IPC_SOURCE}
                  ${GRAPHICS_SOURCE} ${GRAPHICS_IMPL_SOURCE}
                  ${ATOMIC3D_SOURCE}
                  ${ATOMIC2D_SOURCE} ${ENVIRONMENT_SOURCE}
                  ${SCENE_SOURCE} ${UI_SOURCE}
                  ${PLATFORM_SOURCE})

if (NOT EMSCRIPTEN)
  set (SOURCE_FILES ${SOURCE_FILES} ${NETWORK_SOURCE} ${NAVIGATION_SOURCE})
endif()

GroupSources("Atomic2D")
GroupSources("Atomic3D")
GroupSources("Audio")
GroupSources("Container")
GroupSources("Core")
GroupSources("Engine")
GroupSources("Environment")
GroupSources("Graphics")
GroupSources("Input")
GroupSources("IO")
GroupSources("IPC")
GroupSources("Math")
GroupSources("Navigation")
GroupSources("Network")
GroupSources("Physics")
GroupSources("Resource")
GroupSources("Scene")
GroupSources("UI")

add_library(Atomic ${SOURCE_FILES})
