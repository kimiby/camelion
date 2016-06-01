# «Camelion» - Perl storable/C struct back-and-forth translator
#
#  Copyright (C) Alexey Shishkin 2016
#
#  This file is part of Project «Camelion».
#
#  Project «Camelion» is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Project «Camelion» is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with Project «Camelion». If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = lib
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../../[bin]
#QMAKE_POST_LINK  = ../../camelion/scripts/generate_h.sh

# @todo if release strip -s <bin>

SOURCES += \
    translate/nfreeze.c \
    translate/thaw.c \
    reader/read.c \
    nodes/basis.c \
    nodes/find.c \
    helpers/info.c \
    reader/write.c \
    helpers/string.c \
    serials/swrite.c \
    serials/sread.c \
    defines/consts.c \
    memory/alloc.c

HEADERS += \
    defines/types.h \
    defines/xmacro.h \
    translate/nfreeze.h \
    translate/thaw.h \
    libcamelion.h \
    reader/read.h \
    nodes/basis.h \
    nodes/find.h \
    helpers/info.h \
    defines/tools.h \
    reader/write.h \
    helpers/string.h \
    defines/consts.h \
    serials/swrite.h \
    serials/sread.h \
    memory/alloc.h

OTHER_FILES += \
    scripts/generate_h.sh \
    COPYING \
    COPYING.LESSER \
    README.md \
    python/camelion/__init__.py \
    python/camelion/main.py

