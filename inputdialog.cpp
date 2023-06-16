#include "ui_inputdialog.h"
#include "inputdialog.h"
#include <QInputDialog>
#include <QFileDialog>
#include <fstream>
#include <QString>


InputDialog::InputDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputDialog)
{
    ui->setupUi(this);


    ui->HeightLineEdit->setPlaceholderText("Enter Height of the image(numbers only)");
    ui->WidthLineEdit->setPlaceholderText("Enter Width of the image(numbers only)");
    ui->BlackLevelLineEdit->setPlaceholderText("Enter black level (numbers only)");
    ui->GammaLineEdit->setPlaceholderText("Enter Gamma Value(Range : 0-2.2)");


}

InputDialog::~InputDialog()
{
    delete ui;
}

QString InputDialog::getHeight() const
{
    return ui->HeightLineEdit->text();
}

QString InputDialog::getWidth() const
{
    return ui->WidthLineEdit->text();
}
QString InputDialog::getBlackLevel() const
{
    return ui->BlackLevelLineEdit->text();
}
QString InputDialog::getSensorInformation() const
{
    return ui->SensorLineEdit->text();
}
QString InputDialog::getGammavalue() const
{
    return ui->GammaLineEdit->text();
}
QString InputDialog::getCFAPattern() const
{
    return ui->CFALineEdit->text();
}
QString InputDialog::getBitdepth() const
{
    return ui->BitDepthLineEdit->text();
}

