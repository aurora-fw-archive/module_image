# ┌─┐┬ ┬┬─┐┌─┐┬─┐┌─┐  ┌─┐┬─┐┌─┐┌┬┐┌─┐┬ ┬┌─┐┬─┐┬┌─
# ├─┤│ │├┬┘│ │├┬┘├─┤  ├┤ ├┬┘├─┤│││├┤ ││││ │├┬┘├┴┐
# ┴ ┴└─┘┴└─└─┘┴└─┴ ┴  └  ┴└─┴ ┴┴ ┴└─┘└┴┘└─┘┴└─┴ ┴
# A Powerful General Purpose Framework
# More information in: https://aurora-fw.github.io/
#
# Copyright (C) 2017 Aurora Framework, All rights reserved.
#
# This file is part of the Aurora Framework. This framework is free
# software; you can redistribute it and/or modify it under the terms of
# the GNU Lesser General Public License version 3 as published by the
# Free Software Foundation and appearing in the file LICENSE included in
# the packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 3 requirements
# will be met: https://www.gnu.org/licenses/lgpl-3.0.html.

message(STATUS "Loading image module...")

find_package(FreeImage REQUIRED)

if (NOT CONFIGURED_ONCE)
	set(AURORAFW_MODULE_IMAGE_SOURCE_DIR ${AURORAFW_MODULE_IMAGE_DIR}/src)
endif()

include_directories(${AURORAFW_MODULE_IMAGE_DIR}/include ${FreeImage_INCLUDE_DIR})

add_library (aurorafw-image SHARED ${AURORAFW_MODULE_IMAGE_SOURCE_DIR}/Image.cpp)

target_link_libraries(aurorafw-image aurorafw-cli aurorafw-gengine-core aurorafw-core freeimage)

set_target_properties(aurorafw-image PROPERTIES OUTPUT_NAME aurorafw-image)