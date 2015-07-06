#include "widget_simpledriverbackup.h"
#include "ui_widget_simpledriverbackup.h"
#include <QtCore>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QPixmap>
#include "aboutdialog.h"

QString simpledriverbackupVersion = "0.0.1";
QString sourceFolder = "C:/WINDOWS/system32/DriverStore";
QString destFolder = "C:/";

//QString destFolder = QDir::currentPath();

Widget_SimpleDriverBackup::Widget_SimpleDriverBackup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget_SimpleDriverBackup)
{
    ui->setupUi(this);


    Widget_SimpleDriverBackup::setWindowTitle("SimpleDriverBackup - v" + simpledriverbackupVersion);
    ui->label_version->setText("v" + simpledriverbackupVersion);

    ui->lineEdit_destFolder->setText(destFolder);
    ui->progressBar->hide();
    ui->label_currentFileCopying->hide();
    ui->pushButton_stop->hide();

    ui->label_help->setCursor(Qt::WhatsThisCursor);
    ui->pushButton_destFolder->setCursor(Qt::PointingHandCursor);
    ui->pushButton_start->setCursor(Qt::PointingHandCursor);
    ui->pushButton_about->setCursor(Qt::PointingHandCursor);

    QPixmap pixHDD("hdd.png");
    ui->label_pixHDD->setPixmap(pixHDD);

}

Widget_SimpleDriverBackup::~Widget_SimpleDriverBackup()
{
    delete ui;
}

void Widget_SimpleDriverBackup::on_pushButton_destFolder_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select destination folder"),destFolder,
                                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        if(dir[dir.length()-1] != '/')
            dir += '/';

        ui->lineEdit_destFolder->setText(dir);
        destFolder = dir;
    }
}

void Widget_SimpleDriverBackup::on_pushButton_start_clicked()
{
    ui->progressBar->show();
    ui->label_currentFileCopying->show();
    //ui->pushButton_stop->show();

    if(destFolder[destFolder.length()-1] != '/')
        destFolder += '/';

    QString processLog = "";
    QDir source(sourceFolder);
    QDir dest(destFolder);

    source.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

    int numberOfFiles = 0;
    QDirIterator itCounter(source, QDirIterator::Subdirectories);
    while(itCounter.hasNext())
    {
        numberOfFiles++;
        //qDebug() << "processing..." + QString::number(numberOfFiles);
        itCounter.next();
    }

    qDebug() << numberOfFiles;

    int intProgress = 0;
    ui->progressBar->setMaximum(numberOfFiles);

    QDirIterator it(source, QDirIterator::Subdirectories);
    while (it.hasNext()) {

        QFileInfo fileinfo = it.fileInfo();



        intProgress++;
        ui->progressBar->setValue(intProgress);
        ui->label_currentFileCopying->setText(fileinfo.absoluteFilePath());

        if(fileinfo.isDir())
        {
            int filesInDir = fileinfo.dir().count();
            ui->progressBar->setMaximum(numberOfFiles + filesInDir);

            QString pathToDirSource = fileinfo.absoluteFilePath();
            QString pathToDirSourceRelative = fileinfo.absoluteFilePath().replace(sourceFolder, QString(""));

            QString pathToDirDest = destFolder + pathToDirSourceRelative;

            QDir copyToDir(pathToDirDest);
            if(copyToDir.mkpath(copyToDir.absolutePath()))
            {
                processLog += QString::number(intProgress) + " Dir created: " + copyToDir.absolutePath() + "\r\n";
            }
            else
            {
                processLog += QString::number(intProgress) + " Error, could not create dir: " + copyToDir.absolutePath() + "\r\n";
            }
        }

        else if(fileinfo.isFile() && fileinfo.isReadable())
        {
            QString pathToFileSource = fileinfo.absoluteFilePath();
            QString pathToFileSourceRelative = fileinfo.absoluteFilePath().replace(sourceFolder, QString(""));

            QString pathToFileDest = destFolder + pathToFileSourceRelative;

            QFile copyToFile(pathToFileDest);

            if(copyToFile.copy(pathToFileSource, pathToFileDest))
            {
                // success, dir/subdir etc created
                processLog += QString::number(intProgress) + " File copied: " + pathToFileDest + "\r\n";
            }
            else
            {
                if(copyToFile.exists())
                    processLog += QString::number(intProgress) + " File already exists: " + pathToFileDest + "\r\n";
                else
                    processLog += QString::number(intProgress) + " Error, could not copy file: " + pathToFileDest + "\r\n";
            }
        }

        else
        {
            processLog += QString::number(intProgress) + " Error: " + fileinfo.absoluteFilePath() + fileinfo.baseName() + " unknown type of file\r\n";
        }

        //QThread::msleep(2);

        it.next();
    }

    ui->label_currentFileCopying->setText("Finish!");
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this,
                          "Finish",
                          "Backup completed.\n\nDrivers destination folder:\n"+destFolder+"\n\nWould you like to save the log file?",
                          QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QTime time = QTime::currentTime();
        QString strTime = time.toString("HH:mm:ss");
        strTime = strTime.replace(":", "");

        QString saveLogfilename = QFileDialog::getSaveFileName(this, tr("Save log file"),
                                   destFolder + "SimpleDriverBackup_log_"+strTime,
                                   tr("Text (*.txt)"));
        QFile logFile(saveLogfilename);
        logFile.open(QIODevice::WriteOnly);
            QTextStream stream(&logFile);
            stream << processLog;
        logFile.close();

        QProcess *proc = new QProcess(this);
        proc->start("notepad.exe "+saveLogfilename);
    }
    ui->progressBar->hide();
}

void Widget_SimpleDriverBackup::on_pushButton_about_clicked()
{
    aDialog = new AboutDialog(this);
    aDialog->show();
}

void Widget_SimpleDriverBackup::on_pushButton_stop_clicked()
{
//    QMessageBox::StandardButton reply;
//    reply = QMessageBox::question(this,
//                          "Finish",
//                          "Backup completed.\n\nDrivers destination folder:\n"+destFolder+"\n\nWould you like to save the log file?",
//                          QMessageBox::Yes | QMessageBox::No);
}
