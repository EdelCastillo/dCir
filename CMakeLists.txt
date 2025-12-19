project(dCir)
set(PROJECT_NAME dCir)
cmake_minimum_required(VERSION 3.1.0)
  
 
set(TOP ./${CMAKE_CURRENT_DIR})
#path to root of boost installation
set (BOOST_ROOT_DIR /usr/include/boost/)

set(CMAKE_INCLUDE_CURRENT_DIR YES) 

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)


INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/src 
    ${KDE4_INCLUDES} 
#    ${QT_INCLUDE_DIR}
    ${Qt5Widgets_INCLUDE_DIRS}
    )
     
 add_executable(${PROJECT_NAME}
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
        
TARGET_LINK_LIBRARIES(${PROJECT_NAME} 
PUBLIC
#            ${QT_LIBRARIES} 
            ${Qt5Widgets_LIBRARIES}
           )
message(STATUS "baseDirectory: ${CMAKE_CURRENT_SOURCE_DIR}")        
message(STATUS "LIBRARIES:
            QT: ${QT_LIBRARIES}  
            QT: ${QT_INCLUDE_DIR}
            Qt:${Qt5Widgets_INCLUDE_DIRS}
            ")
 
            
#message(WARNING ${CMAKE_INSTALL_PREFIX})
#INSTALL(TARGETS ${PROJECT_NAME}
#          DESTINATION ${CMAKE_INSTALL_PREFIX}/bin/bfl 
#          PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE #GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)

 
set (CMAKE_CXX_FLAGS "-O1") 
set (CMAKE_CXX_FLAGS_DEBUG "-g") 
set (CMAKE_CXX_FLAGS "-fexceptions -O2")
 #si esta linea no está, el compilador da un mensaje de error con la librería Armadillo:
  #/usr/include/armadillo_bits/arma_rng.hpp:113:11: error: exception handling disabled, use -fexceptions to enable
  #     catch(...) {}
#set (CMAKE_CXX_FLAGS_DEBUG "-pthread") 
#set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og")
#set (CMAKE_CXX_FLAGS_DEBUG -Og)
#adddebug 
set (CMAKE_BUILD_TYPE Debug)
#set (CMAKE_BUILD_TYPE Release)

