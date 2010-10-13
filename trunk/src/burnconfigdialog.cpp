#include "burnconfigdialog.h"
#include "ui_burnconfigdialog.h"
#include "avroutputdialog.h"
#include <QtGui>

burnConfigDialog::burnConfigDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::burnConfigDialog)
{
    ui->setupUi(this);
    getSettings();
}

burnConfigDialog::~burnConfigDialog()
{
    delete ui;
}

QString burnConfigDialog::getTempDir()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("temp_directory", QDir("./").absolutePath()).toString();
}

QString burnConfigDialog::getAVRDUDE()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("avrdude_location", QFileInfo("avrdude.exe").absoluteFilePath()).toString();
}

QString burnConfigDialog::getAVRArgs()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("avr_arguments").toString();
}

QString burnConfigDialog::getProgrammer()
{
    QSettings settings("er9x-eePe", "eePe");
    return settings.value("programmer", QString("usbasp")).toString();
}

void burnConfigDialog::getSettings()
{
    ui->avrdude_location->setText(getAVRDUDE());
    ui->temp_location->setText(getTempDir());
    ui->avrArgs->setText(getAVRArgs());
    int idx = ui->avrdude_programmer->findText(getProgrammer());
    if(idx>=0) ui->avrdude_programmer->setCurrentIndex(idx);
}

void burnConfigDialog::putSettings()
{
    QSettings settings("er9x-eePe", "eePe");
    settings.setValue("avrdude_location", ui->avrdude_location->text());
    settings.setValue("temp_directory", ui->temp_location->text());
    settings.setValue("programmer", ui->avrdude_programmer->currentText());
    settings.setValue("avr_arguments", ui->avrArgs->text());
}


void burnConfigDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void burnConfigDialog::on_avrdude_programmer_currentIndexChanged(QString )
{
    putSettings();
}

void burnConfigDialog::on_avrdude_location_editingFinished()
{
    putSettings();
}

void burnConfigDialog::on_temp_location_editingFinished()
{
    putSettings();
}

void burnConfigDialog::on_avrArgs_editingFinished()
{
    putSettings();
}

void burnConfigDialog::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Location"),ui->avrdude_location->text());

    if(!fileName.isEmpty())
    {
        ui->avrdude_location->setText(fileName);
        putSettings();
    }
}

void burnConfigDialog::on_pushButton_2_clicked()
{
    QString dirName = QFileDialog::getExistingDirectory(this, tr("Select Location"),ui->temp_location->text());

    if(!dirName.isEmpty())
    {
        ui->temp_location->setText(dirName);
        putSettings();
    }
}

void burnConfigDialog::on_pushButton_3_clicked()
{
    QStringList arguments;
    arguments << "-c?";

    avrOutputDialog ad(this, ui->avrdude_location->text(), arguments, AVR_DIALOG_KEEP_OPEN);
    ad.exec();
}



void burnConfigDialog::on_pushButton_4_clicked()
{
    QStringList arguments;
    arguments << "-?";

    avrOutputDialog ad(this, ui->avrdude_location->text(), arguments, AVR_DIALOG_KEEP_OPEN);
    ad.exec();
}




void burnConfigDialog::on_resetFuses_clicked()
{
    //fuses
    //avrdude -c usbtiny -p attiny2313 -U lfuse:w:<0x0e>:m
    //avrdude -c usbtiny -p attiny2313 -U hfuse:w:<0x89>:m
    //avrdude -c usbtiny -p attiny2313 -U efuse:w:<0xFF>:m

    QMessageBox::StandardButton ret = QMessageBox::No;

    ret = QMessageBox::warning(this, tr("eePe"),
                               tr("<b><u>WARNING!</u></b><br>Writing fuses can mess up your radio.<br>Do this only if you are sure they are wrong!<br>Are you sure you want to continue?"),
                               QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        QString avrdudeLoc = ui->avrdude_location->text();
        QString programmer = ui->avrdude_programmer->currentText();
        QString args = ui->avrArgs->text();

        QString str1 = "lfuse:w:<0x0e>:m";
        QString str2 = "hfuse:w:<0x89>:m";
        QString str3 = "efuse:w:<0xFF>:m";

        QStringList arguments;
        arguments << "-c" << programmer << "-p" << "m64" << "-U" << str1 << args;

        avrOutputDialog ad(this, avrdudeLoc, arguments, AVR_DIALOG_KEEP_OPEN);
        ad.show();
        ad.waitForFinish();

        arguments.clear();
        arguments << "-c" << programmer << "-p" << "m64" << "-U" << str2 << args;
        ad.runAgain(avrdudeLoc, arguments, AVR_DIALOG_KEEP_OPEN);
        ad.show();
        ad.waitForFinish();

        arguments.clear();
        arguments << "-c" << programmer << "-p" << "m64" << "-U" << str3 << args;
        ad.runAgain(avrdudeLoc, arguments, AVR_DIALOG_KEEP_OPEN);
        ad.exec();

    }

}
