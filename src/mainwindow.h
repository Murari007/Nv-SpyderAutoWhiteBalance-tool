#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>

using namespace cv;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static int height;
    static int width;
    static int blackLevel;
    static double Gamma_value;


private:
    // Declare the bayerraw matrix as a member variable
    Mat bayerraw;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_Black_Level_clicked();

    void on_pushButton_Demosiac_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();


    void on_pushButton_9_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
