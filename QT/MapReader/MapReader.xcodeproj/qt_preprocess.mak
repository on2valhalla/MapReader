#############################################################################
# Makefile for building: MapReader.app/Contents/MacOS/MapReader
# Generated by qmake (2.01a) (Qt 4.8.4) on: Sat Apr 6 10:04:48 2013
# Project:  MapReader.pro
# Template: app
# Command: /opt/local/bin/qmake -spec /opt/local/share/qt4/mkspecs/macx-xcode -o MapReader.xcodeproj/project.pbxproj MapReader.pro
#############################################################################

MOC       = /opt/local/bin/moc
UIC       = /opt/local/bin/uic
LEX       = flex
LEXFLAGS  = 
YACC      = yacc
YACCFLAGS = -d
DEFINES       = -DQT_NO_DEBUG -DQT_GUI_LIB -DQT_CORE_LIB -DQT_SHARED
INCPATH       = -I/opt/local/share/qt4/mkspecs/macx-xcode -I. -I. -I/opt/local/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/opt/local/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/opt/local/Library/Frameworks/QtGui.framework/Versions/4/Headers -I/opt/local/Library/Frameworks/QtCore.framework/Versions/4/Headers -I/opt/local/include -I. -I/usr/local/include -I/System/Library/Frameworks/CarbonCore.framework/Headers -F/opt/local/Library/Frameworks -F/opt/local/lib
DEL_FILE  = rm -f
MOVE      = mv -f

IMAGES = 
PARSERS =
preprocess: $(PARSERS) compilers
clean preprocess_clean: parser_clean compiler_clean

parser_clean:
check: first

mocclean: compiler_moc_header_clean compiler_moc_source_clean

mocables: compiler_moc_header_make_all compiler_moc_source_make_all

compilers: ./moc_mapreader.cpp ./ui_mapreader.h
compiler_objective_cxx_make_all:
compiler_objective_cxx_clean:
compiler_objective_c_make_all:
compiler_objective_c_clean:
compiler_moc_header_make_all: moc_mapreader.cpp
compiler_moc_header_clean:
	-$(DEL_FILE) moc_mapreader.cpp
moc_mapreader.cpp: mapreader.h
	/opt/local/bin/moc $(DEFINES) $(INCPATH) -D__APPLE__ -D__GNUC__ mapreader.h -o moc_mapreader.cpp

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
ui_mapreader.h: mapreader.ui
	/opt/local/bin/uic mapreader.ui -o ui_mapreader.h

compiler_yacc_decl_make_all:
compiler_yacc_decl_clean:
compiler_yacc_impl_make_all:
compiler_yacc_impl_clean:
compiler_lex_make_all:
compiler_lex_clean:
compiler_clean: compiler_moc_header_clean compiler_uic_clean 

