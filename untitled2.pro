QT       += core gui
QT       += printsupport
QT       += xml
QT       += widgets
QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS += -Werror=return-type

RC_ICONS += SDK.ico

TARGET = SDK

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    combobox.cpp \
    compile.cpp \
    config_file.cpp \
    edit.cpp \
    edit_find.cpp \
    edit_widget.cpp \
    file.cpp \
    file_tree.cpp \
    jump_file.cpp \
    main.cpp \
    mainwindow.cpp \
    mywidget.cpp \
    new_project_or_file.cpp \
    new_project_or_file_message.cpp \
    qsci_editor.cpp \
    reg_calc.cpp \
    serial.cpp \
    status_bar.cpp \
    tool_preference.cpp \
    tools.cpp \
    windowset.cpp

HEADERS += \
    Qsci/qsciabstractapis.h \
    Qsci/qsciapis.h \
    Qsci/qscicommand.h \
    Qsci/qscicommandset.h \
    Qsci/qscidocument.h \
    Qsci/qsciglobal.h \
    Qsci/qscilexer.h \
    Qsci/qscilexeravs.h \
    Qsci/qscilexerbash.h \
    Qsci/qscilexerbatch.h \
    Qsci/qscilexercmake.h \
    Qsci/qscilexercoffeescript.h \
    Qsci/qscilexercpp.h \
    Qsci/qscilexercsharp.h \
    Qsci/qscilexercss.h \
    Qsci/qscilexercustom.h \
    Qsci/qscilexerd.h \
    Qsci/qscilexerdiff.h \
    Qsci/qscilexerfortran.h \
    Qsci/qscilexerfortran77.h \
    Qsci/qscilexerhtml.h \
    Qsci/qscilexeridl.h \
    Qsci/qscilexerjava.h \
    Qsci/qscilexerjavascript.h \
    Qsci/qscilexerjson.h \
    Qsci/qscilexerlua.h \
    Qsci/qscilexermakefile.h \
    Qsci/qscilexermarkdown.h \
    Qsci/qscilexermatlab.h \
    Qsci/qscilexeroctave.h \
    Qsci/qscilexerpascal.h \
    Qsci/qscilexerperl.h \
    Qsci/qscilexerpo.h \
    Qsci/qscilexerpostscript.h \
    Qsci/qscilexerpov.h \
    Qsci/qscilexerproperties.h \
    Qsci/qscilexerpython.h \
    Qsci/qscilexerruby.h \
    Qsci/qscilexerspice.h \
    Qsci/qscilexersql.h \
    Qsci/qscilexertcl.h \
    Qsci/qscilexertex.h \
    Qsci/qscilexerverilog.h \
    Qsci/qscilexervhdl.h \
    Qsci/qscilexerxml.h \
    Qsci/qscilexeryaml.h \
    Qsci/qscimacro.h \
    Qsci/qsciprinter.h \
    Qsci/qsciscintilla.h \
    Qsci/qsciscintillabase.h \
    Qsci/qscistyle.h \
    Qsci/qscistyledtext.h \
    combobox.h \
    compile.h \
    config_file.h \
    edit.h \
    edit_find.h \
    edit_widget.h \
    file.h \
    file_tree.h \
    jump_file.h \
    mainwindow.h \
    mybutton.h \
    mytextedit.h \
    mywidget.h \
    new_project_or_file.h \
    new_project_or_file_message.h \
    qsci_editor.h \
    reg_calc.h \
    serial.h \
    status_bar.h \
    tool_preference.h \
    tools.h \
    windowset.h

FORMS += \
    mainwindow.ui \
    new_project_or_file.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lqscintilla2_qt5d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lqscintilla2_qt5d
else:unix: LIBS += -L$$PWD/./ -lqscintilla2_qt5

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

RESOURCES += \
    resource.qrc
