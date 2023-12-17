#include "combobox.h"
#include <QObject>
#include <QWidget>
#include <QApplication>
#include <QComboBox>
#include <QKeyEvent>
#include <QVBoxLayout>

#include <windows.h>
#include <tlhelp32.h>
#include <unistd.h>
#include <csignal>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

combobox::combobox(QWidget *parent):QComboBox(parent){}

void combobox::CtrlCEvent(QProcess *Process){

    if(Process!=NULL){

        qint64 PID =  Process->processId();
        AttachConsole(PID);
        SetConsoleCtrlHandler(NULL, TRUE);
        GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0);

        HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, PID);
        if (INVALID_HANDLE_VALUE == hProcess)
        {
            return ;
        }
        WaitForSingleObject(hProcess, INFINITE);
        CloseHandle(hProcess);
        Sleep(20);

        FreeConsole();
        SetConsoleCtrlHandler(NULL, FALSE);

    }

}
