#include <QDebug>
#include <QCoreApplication>
#include <QSettings>

#include "mywidget.h"
#include "tools.h"
#include "serial.h"
#include "reg_calc.h"

mywidget::mywidget(QString id, QWidget *parent , tools *Tool)
    : QWidget(parent), m_id(id), Tool(Tool){
}

mywidget::mywidget(QString id, serial *serial)
    :  m_id(id), Serial(serial){
}

mywidget::mywidget(QString id,reg_calc *regcal)
    :  m_id(id),RegCalc(regcal){
}

void mywidget::closeEvent(QCloseEvent *event){

    if(m_id=="LinkWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString BaudRateSelect = Tool->BaudRate->currentText();
        QString DsuComSelect   = Tool->DsuCom->currentText();
        QString DebugToolPath  = Tool->ToolPath->text();
        QString InitFile = Tool->InitFilePath->text();
        QString OtherPar = Tool->ToolVariable->text();
        QString LinkCmd = Tool->ToolShow->text();

        GlobalSetting.beginGroup("LinkSetting");
        GlobalSetting.setValue("BaudRateSelect", BaudRateSelect);
        GlobalSetting.setValue("DsuComSelect", DsuComSelect);
        GlobalSetting.setValue("DebugToolPath", DebugToolPath);
        GlobalSetting.setValue("InitFile", InitFile);
        GlobalSetting.setValue("OtherPar", OtherPar);
        GlobalSetting.setValue("LinkCmd", LinkCmd);
        GlobalSetting.endGroup();

    }else if(m_id=="DownLoadWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString DownLoadFileSelect;
        if(Tool->DownLoadCurrent->isChecked()){
            DownLoadFileSelect = "CurrentProject";
        }else{
            DownLoadFileSelect = "OtherProject";
        }
        QString DownLoadFilePath = Tool->DownLoadPath->text();
        QString DownLoadCMD  = Tool->DownLoadCmd->text();

        GlobalSetting.beginGroup("DownloadSetting");
        GlobalSetting.setValue("DownLoadFileSelect", DownLoadFileSelect);
        GlobalSetting.setValue("DownLoadFilePath", DownLoadFilePath);
        GlobalSetting.setValue("DownLoadCMD", DownLoadCMD);
        GlobalSetting.endGroup();

    }else if(m_id=="VerifyWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString VerifyFileSelect;
        if(Tool->VerifyCurrent->isChecked()){
            VerifyFileSelect = "CurrentProject";
        }else{
            VerifyFileSelect = "OtherProject";
        }
        QString VerifyFilePath = Tool->VerifyPath->text();
        QString VerifyCMDSelect  = Tool->VerifyCmd->text();

        GlobalSetting.beginGroup("VerifySetting");
        GlobalSetting.setValue("VerifyFileSelect", VerifyFileSelect);
        GlobalSetting.setValue("VerifyFilePath", VerifyFilePath);
        GlobalSetting.setValue("VerifyCMDSelect", VerifyCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="RunWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString RunCMDSelect  = Tool->RunCmd->text();

        GlobalSetting.beginGroup("RunSetting");
        GlobalSetting.setValue("RunCMDSelect", RunCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="StopWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString StopCMDSelect  = Tool->StopCmd->text();

        GlobalSetting.beginGroup("StopSetting");
        GlobalSetting.setValue("StopCMDSelect", StopCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="InstWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString InstCMDSelect  = Tool->InstCmd->text();

        GlobalSetting.beginGroup("InstSetting");
        GlobalSetting.setValue("InstCMDSelect", InstCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="StepWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString StepCMDSelect  = Tool->StepCmd->text();

        GlobalSetting.beginGroup("StepSetting");
        GlobalSetting.setValue("StepCMDSelect", StepCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="ContinueWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString ContinueCMDSelect  = Tool->ContinueCmd->text();

        GlobalSetting.beginGroup("ContinueSetting");
        GlobalSetting.setValue("ContinueCMDSelect", ContinueCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="BreakWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString BreakCMDSelect  = Tool->BreakCmd->text();

        GlobalSetting.beginGroup("BreakSetting");
        GlobalSetting.setValue("BreakCMDSelect", BreakCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="QuitWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString QuitCMDSelect  = Tool->QuitCmd->text();

        GlobalSetting.beginGroup("QuitSetting");
        GlobalSetting.setValue("QuitCMDSelect", QuitCMDSelect);
        GlobalSetting.endGroup();

    }else if(m_id=="OpenWidget"){

        QString GlobalConfig = QCoreApplication::applicationDirPath() + "/SourceFile/GlobalConfigFiles/GlobalConfig.ini";
        QSettings GlobalSetting(GlobalConfig,QSettings::IniFormat);

        QString SerialBaudSelect  = Serial->SerialBaud->currentText();
        QString SerialComSelect  = Serial->SerialCom->currentText();
        QString StopBitSelect  = Serial->StopBit->currentText();
        QString DataBitSelect  = Serial->DataBit->currentText();
        QString OddBitSelect  = Serial->OddBit->currentText();
        QString CheckDtrOrRtsSelect  = Serial->CheckDtrOrRts->currentText();
        QString TranHexOrAsciiSelect  = Serial->TranHexOrAscii->currentText();
        QString RecHexOrAsciiSelect  = Serial->RecHexOrAscii->currentText();

        GlobalSetting.beginGroup("UartSetting");
        GlobalSetting.setValue("SerialBaudSelect", SerialBaudSelect);
        GlobalSetting.setValue("SerialComSelect", SerialComSelect);
        GlobalSetting.setValue("StopBitSelect", StopBitSelect);
        GlobalSetting.setValue("DataBitSelect", DataBitSelect);
        GlobalSetting.setValue("OddBitSelect", OddBitSelect);
        GlobalSetting.setValue("CheckDtrOrRtsSelect", CheckDtrOrRtsSelect);
        GlobalSetting.setValue("TranHexOrAsciiSelect", TranHexOrAsciiSelect);
        GlobalSetting.setValue("RecHexOrAsciiSelect", RecHexOrAsciiSelect);
        GlobalSetting.endGroup();

    }

    event->accept();

}
