set ( prj Surface_mesher )

project ( Surface_mesher_Demo )

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

cmake_minimum_required(VERSION 2.8.11)
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 OLD)
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 OLD)
endif()
if(POLICY CMP0071)
  cmake_policy(SET CMP0071 NEW)
endif()

set(PACKAGE_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)

# Add several CGAL packages to the include and link paths,
# if they lie in ${PACKAGE_ROOT}/.
foreach(INC_DIR ${PACKAGE_ROOT}/include ${PACKAGE_ROOT}/../Mesh_2/include ${PACKAGE_ROOT}/../Data_structure_for_queries_3/include ${PACKAGE_ROOT}/../Marching_cube/include ${PACKAGE_ROOT}/../CGAL_ImageIO/include ${PACKAGE_ROOT}/../GraphicsView/include)
  if (EXISTS ${INC_DIR})
    include_directories (BEFORE ${INC_DIR})
  endif()
endforeach()
foreach(LIB_DIR ${PACKAGE_ROOT}/../CGAL_ImageIO/src/CGAL_ImageIO ${PACKAGE_ROOT}/../GraphicsView/src/CGALQt5)
  if (EXISTS ${LIB_DIR})
    link_directories (${LIB_DIR})
  endif()
endforeach()

include_directories( ./ )

# QGLViwer needs Qt5 configured with QtOpenGL and QtXml support

set( QT_USE_QTOPENGL TRUE )
set( QT_USE_QTXML    TRUE )
set( QT_USE_QTMAIN   TRUE )

find_package(CGAL COMPONENTS ImageIO Qt5)
if ( CGAL_FOUND AND CGAL_Qt5_FOUND AND CGAL_ImageIO_FOUND)

  add_definitions(-DQT_NO_KEYWORDS)
  include( ${CGAL_USE_FILE} )

  find_package(Qt5 QUIET COMPONENTS OpenGL Xml Svg)
  find_package(QGLViewer )
  find_package(OpenGL    )

  if ( QGLVIEWER_FOUND AND Qt5_FOUND AND OPENGL_FOUND AND OPENGL_GLU_FOUND )


    include_directories( ${QGLVIEWER_INCLUDE_DIR} )
    add_definitions(${QGLVIEWER_DEFINITIONS})

    set( sources Raw_image_dialog.cpp colorlisteditor.cpp values_list.cpp mainwindow.cpp Surface_mesher.cpp viewer.cpp volume.cpp )

    if(EXISTS ${PACKAGE_ROOT}/../Marching_cube)
      option(SURFACE_MESH_DEMO_USE_MARCHING_CUBE "Embed a marching cube implementation in the Surface Mesh demo." ON)
      mark_as_advanced(SURFACE_MESH_DEMO_USE_MARCHING_CUBE)
    endif()

    if(EXISTS ${PACKAGE_ROOT}/include/CGAL/Polyhedral_surface_3.h)
      option(SURFACE_MESH_DEMO_WITH_POLYHEDRAL_SURFACE "Compile the support for polyhedral surfaces." OFF)
      mark_as_advanced(SURFACE_MESH_DEMO_WITH_POLYHEDRAL_SURFACE)
    endif()

    option(SURFACE_MESH_DEMO_VERBOSE "Set this option if you want the Surface Mesh demo to display messages on standard output." OFF)
    mark_as_advanced(SURFACE_MESH_DEMO_VERBOSE)

    if(SURFACE_MESH_DEMO_VERBOSE)
      add_definitions(-DCGAL_SURFACE_MESHER_VERBOSE)
    endif()

    if(SURFACE_MESH_DEMO_WITH_POLYHEDRAL_SURFACE)
      set(sources ${sources} polyhedral_surface.cpp)
    else(SURFACE_MESH_DEMO_WITH_POLYHEDRAL_SURFACE)
      add_definitions(-DCGAL_DO_NOT_USE_POLYHEDRAL_SURFACE)
    endif(SURFACE_MESH_DEMO_WITH_POLYHEDRAL_SURFACE)

    if(SURFACE_MESH_DEMO_USE_MARCHING_CUBE)
      set(sources ${sources} ${PACKAGE_ROOT}/../Marching_cube/src/mc/ply.c)
      add_definitions(-DCGAL_SURFACE_MESH_DEMO_USE_MARCHING_CUBE)
    endif()


    qt5_generate_moc( "surface.h" "${CMAKE_CURRENT_BINARY_DIR}/moc_surface.cpp" )

    set( sources ${sources} moc_surface.cpp)

    qt5_wrap_ui( uis ui/values_list.ui ui/mainwindow.ui ui/optionsdialog.ui ui/raw_image.ui )

    qt5_add_resources( CGAL_Qt5_RESOURCE_FILES values_list.qrc surface_mesher.qrc  )

    add_executable  ( ${prj}  ${sources} ${uis} ${CGAL_Qt5_RESOURCE_FILES} ${CGAL_Qt5_MOC_FILES})

    qt5_use_modules(${prj} OpenGL Xml Svg)

    add_to_cached_list( CGAL_EXECUTABLE_TARGETS ${prj} )

    set(VTK_LIBS "")
    find_package(VTK QUIET COMPONENTS vtkImagingGeneral  vtkIOImage NO_MODULE)
    if(VTK_FOUND)
      include(${VTK_USE_FILE})
      if ("${VTK_VERSION_MAJOR}" GREATER "5")
        message(STATUS "VTK found")
        add_definitions(-DCGAL_USE_VTK)
        set(VTK_LIBS vtkImagingGeneral vtkIOImage)
      else()
        message(STATUS "Vtk must be at least Rel 6")
      endif()
    else()
      message(STATUS "For reading Dicom files install VTK first")
    endif()

    # Link the executable to CGAL and third-party libraries
    target_link_libraries( ${prj} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES})

    target_link_libraries( ${prj} ${QT_LIBRARIES} ${QGLVIEWER_LIBRARIES} ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY} ${VTK_LIBS})

  else( QGLVIEWER_FOUND AND Qt5_FOUND AND OPENGL_FOUND AND OPENGL_GLU_FOUND )
    message(STATUS "NOTICE: This demo needs libQGLViewer, and will not be compiled.")
  endif( QGLVIEWER_FOUND AND Qt5_FOUND AND OPENGL_FOUND AND OPENGL_GLU_FOUND )
else(CGAL_FOUND AND CGAL_Qt5_FOUND AND CGAL_ImageIO_FOUND)
  if(RUNNING_CGAL_AUTO_TEST)
    # Just to avoid a warning from CMake if that variable is set on the command line...
  endif()

  message(STATUS "NOTICE: This demo needs Qt5, and will not be compiled.")
endif(CGAL_FOUND AND CGAL_Qt5_FOUND AND CGAL_ImageIO_FOUND)
