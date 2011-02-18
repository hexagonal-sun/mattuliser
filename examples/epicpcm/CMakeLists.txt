# Build a epic pcm example.
#
# Copyright 2011 (c) Matthew Leach.
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

project(epicpcm)
find_package(SDL REQUIRED)
find_package(OpenGL REQUIRED)
include_directories(${SDL_INCLUDE_DIR} ${OpenGL_INCLUDE_DIR})
set(epicpcm_SRCS main.cpp epicpcm.cpp)
	add_executable(epicpcm ${epicpcm_SRCS})
target_link_libraries(epicpcm mattulizer)
add_dependencies(examples epicpcm)
