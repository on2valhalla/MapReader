#############################################################################
# Makefile for building: MapReader.app/Contents/MacOS/MapReader
# Generated by qmake (2.01a) (Qt 4.8.4) on: Tue Apr 9 12:09:37 2013
# Project:  ../MapReader/MapReader.pro
# Template: app
# Command: /usr/bin/qmake -spec /usr/local/Qt4.8/mkspecs/macx-g++ CONFIG+=x86_64 CONFIG+=declarative_debug -o Makefile ../MapReader/MapReader.pro
#############################################################################

####### Compiler, tools and options

CC            = gcc
CXX           = g++
DEFINES       = -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
CFLAGS        = -pipe -g -gdwarf-2 -arch x86_64 -m64 -mmacosx-version-min=10.5 -Wall -W $(DEFINES)
CXXFLAGS      = -fpermissive -std=c++11 -g -gdwarf-2 -arch x86_64 -m64 -mmacosx-version-min=10.5 -Wall -W $(DEFINES)
INCPATH       = -I/usr/local/Qt4.8/mkspecs/macx-g++ -I../MapReader -I/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/usr/include/QtCore -I/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/usr/include/QtGui -I/usr/include -I/usr/local/include -I. -I. -I../MapReader -I. -F/Library/Frameworks
LINK          = g++
LFLAGS        = -headerpad_max_install_names -arch x86_64 -m64 -mmacosx-version-min=10.5
LIBS          = $(SUBLIBS) -F/Library/Frameworks -L/Library/Frameworks -L/usr/local/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_objdetect -lopencv_calib3d -framework QtGui -framework QtCore 
AR            = ar cq
RANLIB        = ranlib -s
QMAKE         = /usr/bin/qmake
TAR           = tar -cf
COMPRESS      = gzip -9f
COPY          = cp -f
SED           = sed
COPY_FILE     = cp -f
COPY_DIR      = cp -f -R
STRIP         = 
INSTALL_FILE  = $(COPY_FILE)
INSTALL_DIR   = $(COPY_DIR)
INSTALL_PROGRAM = $(COPY_FILE)
DEL_FILE      = rm -f
SYMLINK       = ln -f -s
DEL_DIR       = rmdir
MOVE          = mv -f
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p
export MACOSX_DEPLOYMENT_TARGET = 10.4

####### Output directory

OBJECTS_DIR   = ./

####### Files

SOURCES       = ../MapReader/main.cpp \
		../MapReader/mapreader.cpp moc_mapreader.cpp \
		moc_clicklabel.cpp
OBJECTS       = main.o \
		mapreader.o \
		moc_mapreader.o \
		moc_clicklabel.o
DIST          = /usr/local/Qt4.8/mkspecs/common/unix.conf \
		/usr/local/Qt4.8/mkspecs/common/mac.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base-macx.conf \
		/usr/local/Qt4.8/mkspecs/common/g++-base.conf \
		/usr/local/Qt4.8/mkspecs/common/g++-macx.conf \
		/usr/local/Qt4.8/mkspecs/qconfig.pri \
		/usr/local/Qt4.8/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt4.8/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.8/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.8/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.8/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/dwarf2.prf \
		/usr/local/Qt4.8/mkspecs/features/debug.prf \
		/usr/local/Qt4.8/mkspecs/features/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/x86_64.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.8/mkspecs/features/declarative_debug.prf \
		/usr/local/Qt4.8/mkspecs/features/warn_on.prf \
		/usr/local/Qt4.8/mkspecs/features/qt.prf \
		/usr/local/Qt4.8/mkspecs/features/unix/thread.prf \
		/usr/local/Qt4.8/mkspecs/features/moc.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/rez.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/sdk.prf \
		/usr/local/Qt4.8/mkspecs/features/resources.prf \
		/usr/local/Qt4.8/mkspecs/features/uic.prf \
		/usr/local/Qt4.8/mkspecs/features/yacc.prf \
		/usr/local/Qt4.8/mkspecs/features/lex.prf \
		/usr/local/Qt4.8/mkspecs/features/include_source_dir.prf \
		../MapReader/MapReader.pro
QMAKE_TARGET  = MapReader
DESTDIR       = 
TARGET        = MapReader.app/Contents/MacOS/MapReader

####### Custom Compiler Variables
QMAKE_COMP_QMAKE_OBJECTIVE_CFLAGS = -pipe \
		-g \
		-arch \
		x86_64 \
		-m64 \
		-mmacosx-version-min=10.5 \
		-Wall \
		-W


first: all
####### Implicit rules

.SUFFIXES: .o .c .cpp .cc .cxx .C

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cc.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.cxx.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.C.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o "$@" "$<"

.c.o:
	$(CC) -c $(CFLAGS) $(INCPATH) -o "$@" "$<"

####### Build rules

all: Makefile MapReader.app/Contents/PkgInfo MapReader.app/Contents/Resources/empty.lproj MapReader.app/Contents/Info.plist $(TARGET)

$(TARGET): ui_mapreader.h $(OBJECTS)  
	@$(CHK_DIR_EXISTS) MapReader.app/Contents/MacOS/ || $(MKDIR) MapReader.app/Contents/MacOS/ 
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

Makefile: ../MapReader/MapReader.pro  /usr/local/Qt4.8/mkspecs/macx-g++/qmake.conf /usr/local/Qt4.8/mkspecs/common/unix.conf \
		/usr/local/Qt4.8/mkspecs/common/mac.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base.conf \
		/usr/local/Qt4.8/mkspecs/common/gcc-base-macx.conf \
		/usr/local/Qt4.8/mkspecs/common/g++-base.conf \
		/usr/local/Qt4.8/mkspecs/common/g++-macx.conf \
		/usr/local/Qt4.8/mkspecs/qconfig.pri \
		/usr/local/Qt4.8/mkspecs/modules/qt_webkit_version.pri \
		/usr/local/Qt4.8/mkspecs/features/qt_functions.prf \
		/usr/local/Qt4.8/mkspecs/features/qt_config.prf \
		/usr/local/Qt4.8/mkspecs/features/exclusive_builds.prf \
		/usr/local/Qt4.8/mkspecs/features/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_pre.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/dwarf2.prf \
		/usr/local/Qt4.8/mkspecs/features/debug.prf \
		/usr/local/Qt4.8/mkspecs/features/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/default_post.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/x86_64.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/objective_c.prf \
		/usr/local/Qt4.8/mkspecs/features/declarative_debug.prf \
		/usr/local/Qt4.8/mkspecs/features/warn_on.prf \
		/usr/local/Qt4.8/mkspecs/features/qt.prf \
		/usr/local/Qt4.8/mkspecs/features/unix/thread.prf \
		/usr/local/Qt4.8/mkspecs/features/moc.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/rez.prf \
		/usr/local/Qt4.8/mkspecs/features/mac/sdk.prf \
		/usr/local/Qt4.8/mkspecs/features/resources.prf \
		/usr/local/Qt4.8/mkspecs/features/uic.prf \
		/usr/local/Qt4.8/mkspecs/features/yacc.prf \
		/usr/local/Qt4.8/mkspecs/features/lex.prf \
		/usr/local/Qt4.8/mkspecs/features/include_source_dir.prf \
		/Library/Frameworks/QtGui.framework/QtGui.prl \
		/Library/Frameworks/QtCore.framework/QtCore.prl
	$(QMAKE) -spec /usr/local/Qt4.8/mkspecs/macx-g++ CONFIG+=x86_64 CONFIG+=declarative_debug -o Makefile ../MapReader/MapReader.pro
/usr/local/Qt4.8/mkspecs/common/unix.conf:
/usr/local/Qt4.8/mkspecs/common/mac.conf:
/usr/local/Qt4.8/mkspecs/common/gcc-base.conf:
/usr/local/Qt4.8/mkspecs/common/gcc-base-macx.conf:
/usr/local/Qt4.8/mkspecs/common/g++-base.conf:
/usr/local/Qt4.8/mkspecs/common/g++-macx.conf:
/usr/local/Qt4.8/mkspecs/qconfig.pri:
/usr/local/Qt4.8/mkspecs/modules/qt_webkit_version.pri:
/usr/local/Qt4.8/mkspecs/features/qt_functions.prf:
/usr/local/Qt4.8/mkspecs/features/qt_config.prf:
/usr/local/Qt4.8/mkspecs/features/exclusive_builds.prf:
/usr/local/Qt4.8/mkspecs/features/default_pre.prf:
/usr/local/Qt4.8/mkspecs/features/mac/default_pre.prf:
/usr/local/Qt4.8/mkspecs/features/mac/dwarf2.prf:
/usr/local/Qt4.8/mkspecs/features/debug.prf:
/usr/local/Qt4.8/mkspecs/features/default_post.prf:
/usr/local/Qt4.8/mkspecs/features/mac/default_post.prf:
/usr/local/Qt4.8/mkspecs/features/mac/x86_64.prf:
/usr/local/Qt4.8/mkspecs/features/mac/objective_c.prf:
/usr/local/Qt4.8/mkspecs/features/declarative_debug.prf:
/usr/local/Qt4.8/mkspecs/features/warn_on.prf:
/usr/local/Qt4.8/mkspecs/features/qt.prf:
/usr/local/Qt4.8/mkspecs/features/unix/thread.prf:
/usr/local/Qt4.8/mkspecs/features/moc.prf:
/usr/local/Qt4.8/mkspecs/features/mac/rez.prf:
/usr/local/Qt4.8/mkspecs/features/mac/sdk.prf:
/usr/local/Qt4.8/mkspecs/features/resources.prf:
/usr/local/Qt4.8/mkspecs/features/uic.prf:
/usr/local/Qt4.8/mkspecs/features/yacc.prf:
/usr/local/Qt4.8/mkspecs/features/lex.prf:
/usr/local/Qt4.8/mkspecs/features/include_source_dir.prf:
/Library/Frameworks/QtGui.framework/QtGui.prl:
/Library/Frameworks/QtCore.framework/QtCore.prl:
qmake:  FORCE
	@$(QMAKE) -spec /usr/local/Qt4.8/mkspecs/macx-g++ CONFIG+=x86_64 CONFIG+=declarative_debug -o Makefile ../MapReader/MapReader.pro

MapReader.app/Contents/PkgInfo: 
	@$(CHK_DIR_EXISTS) MapReader.app/Contents || $(MKDIR) MapReader.app/Contents 
	@$(DEL_FILE) MapReader.app/Contents/PkgInfo
	@echo "APPL????" >MapReader.app/Contents/PkgInfo
MapReader.app/Contents/Resources/empty.lproj: 
	@$(CHK_DIR_EXISTS) MapReader.app/Contents/Resources || $(MKDIR) MapReader.app/Contents/Resources 
	@touch MapReader.app/Contents/Resources/empty.lproj
	
MapReader.app/Contents/Info.plist: 
	@$(CHK_DIR_EXISTS) MapReader.app/Contents || $(MKDIR) MapReader.app/Contents 
	@$(DEL_FILE) MapReader.app/Contents/Info.plist
	@sed -e "s,@SHORT_VERSION@,1.0,g" -e "s,@TYPEINFO@,????,g" -e "s,@ICON@,,g" -e "s,@EXECUTABLE@,MapReader,g" -e "s,@TYPEINFO@,????,g" /usr/local/Qt4.8/mkspecs/macx-g++/Info.plist.app >MapReader.app/Contents/Info.plist
dist: 
	@$(CHK_DIR_EXISTS) .tmp/MapReader1.0.0 || $(MKDIR) .tmp/MapReader1.0.0 
	$(COPY_FILE) --parents $(SOURCES) $(DIST) .tmp/MapReader1.0.0/ && $(COPY_FILE) --parents ../MapReader/mapreader.h ../MapReader/clicklabel.h .tmp/MapReader1.0.0/ && $(COPY_FILE) --parents ../MapReader/main.cpp ../MapReader/mapreader.cpp .tmp/MapReader1.0.0/ && $(COPY_FILE) --parents ../MapReader/mapreader.ui .tmp/MapReader1.0.0/ && (cd `dirname .tmp/MapReader1.0.0` && $(TAR) MapReader1.0.0.tar MapReader1.0.0 && $(COMPRESS) MapReader1.0.0.tar) && $(MOVE) `dirname .tmp/MapReader1.0.0`/MapReader1.0.0.tar.gz . && $(DEL_FILE) -r .tmp/MapReader1.0.0


clean:compiler_clean 
	-$(DEL_FILE) $(OBJECTS)
	-$(DEL_FILE) *~ core *.core


####### Sub-libraries

distclean: clean
	-$(DEL_FILE) -r MapReader.app
	-$(DEL_FILE) Makefile


check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_mapreader.cpp moc_clicklabel.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mapreader.cpp moc_clicklabel.cpp
moc_mapreader.cpp: ../MapReader/mapreader.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ ../MapReader/mapreader.h -o moc_mapreader.cpp

moc_clicklabel.cpp: ../MapReader/mapreader.h \
		../MapReader/clicklabel.h
	/Developer/Tools/Qt/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ ../MapReader/clicklabel.h -o moc_clicklabel.cpp

compiler_rcc_make_all:
compiler_rcc_clean:
compiler_image_collection_make_all: qmake_image_collection.cpp
compiler_image_collection_clean:
	-$(DEL_FILE) qmake_image_collection.cpp
compiler_moc_source_make_all:
compiler_moc_source_clean:
compiler_rez_source_make_all:
compiler_rez_source_clean:
compiler_uic_make_all: ui_mapreader.h
compiler_uic_clean:
	-$(DEL_FILE) ui_mapreader.h
ui_mapreader.h: ../MapReader/mapreader.ui \
		../MapReader/clickablelabel.h \
		../MapReader/mapreader.h
	/Developer/Tools/Qt/uic ../MapReader/mapreader.ui -o ui_mapreader.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

####### Compile

main.o: ../MapReader/main.cpp ../MapReader/mapreader.h \
		../MapReader/clicklabel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o main.o ../MapReader/main.cpp

mapreader.o: ../MapReader/mapreader.cpp ../MapReader/mapreader.h \
		ui_mapreader.h \
		../MapReader/clickablelabel.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o mapreader.o ../MapReader/mapreader.cpp

moc_mapreader.o: moc_mapreader.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_mapreader.o moc_mapreader.cpp

moc_clicklabel.o: moc_clicklabel.cpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o moc_clicklabel.o moc_clicklabel.cpp

####### Install

install:   FORCE

uninstall:   FORCE

FORCE:

