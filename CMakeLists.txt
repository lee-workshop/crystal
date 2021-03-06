cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(PROJECT_NAME crystal)
project(${PROJECT_NAME})
set(CMAKE_SYSTEM_NAME Linux)

#Pretty colors
set(CMAKE_COLOR_MAKEFILE ON)
#Dont force verbose
set(CMAKE_VERBOSE_MAKEFILE ON)
#Include current dir
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

INCLUDE(CheckIncludeFile)
INCLUDE(CheckCCompilerFlag)
INCLUDE(CheckCXXCompilerFlag)
INCLUDE(CheckTypeSize)

# Version
set(CRYSTAL_VERSION_MAJOR "0")
set(CRYSTAL_VERSION_MINOR "1")
set(CRYSTAL_VERSION_PATCH "0")
set(CRYSTAL_VERSION "${CRYSTAL_VERSION_MAJOR}.${CRYSTAL_VERSION_MINOR}.${CRYSTAL_VERSION_PATCH}")

set(CMAKE_INSTALL_PREFIX "/usr/local" CACHE PATH "Installation Directory")
set(PKGDATADIR "${CMAKE_INSTALL_PREFIX}/share/crystal")
set(SYSCONFDIR "/etc")
set(LIBDIR "/usr/lib")
set(MANDIR "${CMAKE_INSTALL_PREFIX}/share/man")

set(CRYSTAL_DEFINITIONS)
if(${CMAKE_SYSTEM_NAME} MATCHES "FreeBSD" OR
		${CMAKE_SYSTEM_NAME} MATCHES "NetBSD" OR
		${CMAKE_SYSTEM_NAME} MATCHES "OpenBSD"
		)
	set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DNEEDS_BASENAME")
else()
	set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DHAVE_SHADOW")
endif()

set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DPACKAGE=\"crystal\"")
set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DVERSION=\"${CRYSTAL_VERSION}\"")
set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DPKGDATADIR=\"${PKGDATADIR}\"")
set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DSYSCONFDIR=\"${SYSCONFDIR}\"")

# Flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O2")
set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -Wall -g -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -O2")

# source 
set(crystal_srcs
	main.cc
	app.cc
	numlock.cc
	switchuser.cc
	png.c
	jpeg.c
)

set(crystallock_srcs
	crystallock.cc
)

set(common_srcs
    cfg.cc
    image.cc
    log.cc
    panel.cc
    util.cc
)
if(USE_PAM)
	set(common_srcs ${common_srcs} PAM.cc)
    # for now, only build crystallock if we are using PAM.
    set(BUILD_CRYSTALLOCK 1)
endif(USE_PAM)

# Build common library
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries")

if (BUILD_SHARED_LIBS)
	message(STATUS "Enable shared library building")
	add_library(libcrystal ${common_srcs})
else(BUILD_SHARED_LIBS)
	message(STATUS "Disable shared library building")
	add_library(libcrystal STATIC ${common_srcs})
endif(BUILD_SHARED_LIBS)

if(USE_CONSOLEKIT)
	set(crystal_srcs ${crystal_srcs} Ck.cpp)
endif(USE_CONSOLEKIT)

add_executable(${PROJECT_NAME} ${crystal_srcs})
if(BUILD_CRYSTALLOCK)
    add_executable(crystallock ${crystallock_srcs})
endif(BUILD_CRYSTALLOCK)

#Set the custom CMake module directory where our include/lib finders are
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(X11 REQUIRED)
find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)

# Fontconfig
set(FONTCONFIG_DIR ${CMAKE_MODULE_PATH})
find_package(FONTCONFIG REQUIRED)
if(FONTCONFIG_FOUND)
	message("\tFontConfig Found")
	target_link_libraries(${PROJECT_NAME} ${FONTCONFIG_LIBRARY})
	include_directories(${FONTCONFIG_INCLUDE_DIR})
endif(FONTCONFIG_FOUND)

# PAM
if(USE_PAM)
	message("\tPAM Enabled")
	find_package(PAM)
	if(PAM_FOUND)
		message("\tPAM Found")
		set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DUSE_PAM")
		target_link_libraries(libcrystal ${PAM_LIBRARY})
		target_link_libraries(${PROJECT_NAME} ${PAM_LIBRARY})
		target_link_libraries(crystallock ${PAM_LIBRARY})
		include_directories(${PAM_INCLUDE_DIR})
	else(PAM_FOUND)
		message("\tPAM Not Found")
	endif(PAM_FOUND)
else(USE_PAM)
	message("\tPAM disabled")
endif(USE_PAM)

# ConsoleKit
if(USE_CONSOLEKIT)
	find_package(CkConnector)
	message("\tConsoleKit Enabled")
	if(CKCONNECTOR_FOUND)
		message("\tConsoleKit Found")
		# DBus check
		find_package(DBus REQUIRED)
		if(DBUS_FOUND)
			message("\tDBus Found")
			target_link_libraries(${PROJECT_NAME} ${DBUS_LIBRARIES})
			include_directories(${DBUS_ARCH_INCLUDE_DIR})
			include_directories(${DBUS_INCLUDE_DIR})
			set(CRYSTAL_DEFINITIONS ${CRYSTAL_DEFINITIONS} "-DUSE_CONSOLEKIT")
			target_link_libraries(${PROJECT_NAME} ${CKCONNECTOR_LIBRARIES})
			include_directories(${CKCONNECTOR_INCLUDE_DIR})
		else(DBUS_FOUND)
			message("\tDBus Not Found")
		endif(DBUS_FOUND)
	else(CKCONNECTOR_FOUND)
		message("\tConsoleKit Not Found")
		message("\tConsoleKit disabled")
	endif(CKCONNECTOR_FOUND)
else(USE_CONSOLEKIT)
	message("\tConsoleKit disabled")
endif(USE_CONSOLEKIT)

# system librarys
find_library(M_LIB m)
find_library(RT_LIB rt)
find_library(CRYPTO_LIB crypt)
find_package(Threads)

add_definitions(${CRYSTAL_DEFINITIONS})

#Set up include dirs with all found packages
include_directories(
	${X11_INCLUDE_DIR}
	${X11_Xft_INCLUDE_PATH}
	${X11_Xrender_INCLUDE_PATH}
	${X11_Xrandr_INCLUDE_PATH}
	${FREETYPE_INCLUDE_DIR_freetype2}
	${X11_Xmu_INCLUDE_PATH}
	${ZLIB_INCLUDE_DIR}
	${JPEG_INCLUDE_DIR}
	${PNG_INCLUDE_DIR}
)

target_link_libraries(libcrystal
	${X11_Xft_LIB}
	${X11_Xrandr_LIB}
    ${JPEG_LIBRARIES}
	${PNG_LIBRARIES}
)

#Set up library with all found packages for crystal
target_link_libraries(${PROJECT_NAME}
	${M_LIB}
	${RT_LIB}
	${CRYPTO_LIB}
	${X11_X11_LIB}
	${X11_Xft_LIB}
	${X11_Xrender_LIB}
	${X11_Xrandr_LIB}
	${X11_Xmu_LIB}
	${FREETYPE_LIBRARY}
	${JPEG_LIBRARIES}
	${PNG_LIBRARIES}
    libcrystal
)

if(BUILD_CRYSTALLOCK)
    #Set up library with all found packages for crystallock
    target_link_libraries(crystallock
        ${M_LIB}
        ${RT_LIB}
        ${CRYPTO_LIB}
        ${X11_X11_LIB}
        ${X11_Xft_LIB}
        ${X11_Xrender_LIB}
        ${X11_Xrandr_LIB}
        ${X11_Xmu_LIB}
        ${X11_Xext_LIB}
        ${FREETYPE_LIBRARY}
        ${JPEG_LIBRARIES}
        ${PNG_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
        libcrystal
    )
endif(BUILD_CRYSTALLOCK)

####### install
# crystal
install(TARGETS crystal RUNTIME DESTINATION bin)
install(TARGETS crystallock RUNTIME DESTINATION bin)

if (BUILD_SHARED_LIBS)
	set_target_properties(libcrystal PROPERTIES
		OUTPUT_NAME crystal
		SOVERSION ${CRYSTAL_VERSION})

	install(TARGETS libcrystal
		LIBRARY DESTINATION lib
		ARCHIVE DESTINATION lib
	)
endif (BUILD_SHARED_LIBS)

# man file
install(FILES crystal.1 DESTINATION ${MANDIR}/man1/)
install(FILES crystallock.1 DESTINATION ${MANDIR}/man1/)
# configure
install(FILES crystal.conf DESTINATION ${SYSCONFDIR})
# systemd service file
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	install(FILES crystal.service DESTINATION ${LIBDIR}/systemd/system)
endif (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
# themes directory
subdirs(themes)
