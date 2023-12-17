#include "mainwindow.h"
#include "file_tree.h"
#include "edit_find.h"
#include "tool_preference.h"
#include <QApplication>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QDir>
#include <QStyleFactory>

int main(int argc, char *argv[]){

    QApplication app(argc, argv);

    MainWindow window;

    window.show();

    if (app.arguments().size() > 1) {

        window.StatusBar->DetermineMessage(QCoreApplication::applicationDirPath());
        QString filePath = QDir::fromNativeSeparators(app.arguments().at(1));
        QFileInfo LoadFile(filePath);
        if(LoadFile.isFile()){
            window.FileMenu->ReOpenProjectOrFile(filePath);
        }else{
            window.StatusBar->DetermineMessage("Not Accept File");
        }

    }

    return app.exec();

}
