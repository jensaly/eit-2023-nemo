cmake_minimum_required(VERSION 3.20)
project(eit)

find_package(SDL2)

find_package(CURL)

set(CMAKE_CXX_STANDARD 20)

if (SDL2_FOUND)
    find_package(OpenGL)
    set(SOURCES g_main.cpp vehicle.cpp yard.cpp ferry.cpp algorithms.cpp queue.cpp fetchVehicleData.cpp)
    set(HEADERS utility.h vehicle.h yard.h g_utility.h ferry.h algorithms.h filehandler.h filehandler.cpp queue.h fetchVehicleData.cpp)

    include_directories(${OPENGL_INCLUDE_DIRS})
    include_directories(${SDL2_INCLUDE_DIRS})

    set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/imgui)
    set(IMPLOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/implot)

    add_executable(eit_graphical ${SOURCES} ${HEADERS} ${IMGUI_SRC})

    target_link_libraries(eit_graphical PUBLIC sdl2 ${OPENGL_LIBRARIES} curl)

    target_sources( eit_graphical
            PUBLIC
            ${IMGUI_DIR}/imgui_draw.cpp
            ${IMGUI_DIR}/imgui_tables.cpp
            ${IMGUI_DIR}/imgui_widgets.cpp
            ${IMGUI_DIR}/imgui.cpp
            ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
            ${IMGUI_DIR}/backends/imgui_impl_sdl.cpp
            ${IMPLOT_DIR}/implot.cpp
            ${IMPLOT_DIR}/implot_items.cpp)

    target_include_directories( eit_graphical
            PUBLIC ${IMGUI_DIR}
            PUBLIC ${IMPLOT_DIR}
            PUBLIC ${IMGUI_DIR}/backends
            PUBLIC ${SDL2_DIR}/include
            )
else()
    set(SOURCES main.cpp vehicle.cpp yard.cpp ferry.cpp algorithms.cpp queue.cpp fetchVehicleData.cpp)
    set(HEADERS utility.h vehicle.h yard.h ferry.h algorithms.h FileHandler.h filehandler.cpp queue.h fetchVehicleData.cpp randomRegistrationNumber.cpp randomRegistrationNumber.h)
    add_executable(eit ${SOURCES} ${HEADERS})

    target_link_libraries(eit PUBLIC curl)
endif ()
