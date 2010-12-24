# Build mattulizer.
#
# Copyright 2010 (c) Matthew Leach.
#
# This file is part of mattulizer.
#
# Mattulizer is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# Mattulizer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Mattulizer.  If not, see <http://www.gnu.org/licenses/>.#

cmake_minimum_required(VERSION 2.8)

project(mattulizer)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/src/cmake/")

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(FFTW)

# add the FFTW library to the build only if it is found
if(${FFTW_FOUND})
set(include_DIR ${include_DIR} ${FFTW_INCLUDE_DIR})
set(library_LIB ${library_LIB} ${FFTW_LIBRARY})
endif(${FFTW_FOUND})

include_directories(${SDL_INCLUDE_DIR}
                    ${SDLMIXER_INCLUDE_DIR}
                    ${OPENGL_INCLUDE_DIR}
                    ${include_DIR})

set(mattulizer_SRCS src/visualiser.cpp
                    src/visualiserWin.cpp
                    src/sdlexception.cpp
                    src/dspmanager.cpp
                    src/dsp/pcm.cpp)

add_library(mattulizer SHARED ${mattulizer_SRCS})
target_link_libraries(mattulizer
                      ${SDL_LIBRARY}
                      ${SDLMIXER_LIBRARY}
                      ${OPENGL_LIBRARIES}
                      ${CMAKE_THREADS_LIBS_INIT}
                      ${library_LIB})
