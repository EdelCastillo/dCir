#only for Windows
cmake_minimum_required(VERSION 3.16)
project(dCir)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/msvc2022_64/lib/cmake")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.18/msvc2019_64/lib/cmake")

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)
qt_standard_project_setup()
 
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/msvc2022_64")
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.0/msvc2022_64/bin")
#set(CMAKE_PREFIX_PATH "C:/Qt/5.15.18/msvc2019_64/bin")

#find_package(Qt6Core REQUIRED)
#find_package(Qt6Widgets REQUIRED)
#find_package(Qt6Gui REQUIRED)
include_directories (${CMAKE_SOURCE_DIR}/src)
#target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

#qt_add_executable(dCir WIN32 
add_executable(dCir WIN32 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
 ${CMAKE_CURRENT_SOURCE_DIR}/src/dCir.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/image.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/blocks.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/paths.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/mouse.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/common.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/project.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/extractfromfile.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/pathDialog.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/blockDialog.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/newProjectDialog.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/paramsDialog.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/fileNameDialog.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/dirDialog.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/combinationals.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/sequentials.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/numeric.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/files.cpp 
 ${CMAKE_CURRENT_SOURCE_DIR}/src/addrDialog.cpp 
${CMAKE_CURRENT_SOURCE_DIR}/src/dialogs.cpp 
)
 target_compile_options(dCir PUBLIC "/Zc:__cplusplus")
  
target_link_libraries(${PROJECT_NAME} PRIVATE
	Qt6::Core
	Qt6::Gui
	Qt6::Widgets
)
set_target_properties(${PROJECT_NAME} PROPERTIES
    ${BUNDLE_ID_OPTION}
    WIN32_EXECUTABLE TRUE
)
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

