#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QString>
#include <QDebug>
#include <qformlayout.h>
#include <QDialog>
#include <QScreen>
#include <inputdialog.h>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>


using namespace cv;
using namespace std;

bool leftDown = false, leftup = false;
Point cor1, cor2;
Rect box;
Rect box1;
int MainWindow::height;
int MainWindow::width;
int MainWindow::blackLevel;
double MainWindow::Gamma_value;
Mat img_corrected;
Mat demosaicedImage;
Mat output_image;
Mat img_gamma;
Mat img_with_rect;
QString filename;
vector<String> filenames;
String filepath;
QString folder;
String folder_path;
QString csv_file;
String csv_filepath;
String Autocsv_file;

struct ROI
{
    String region;
    Scalar blue_mean;
    Scalar blue_std;
    Scalar green_mean;
    Scalar green_std;
    Scalar red_mean;
    Scalar red_std;
    Rect coordinates;
    float alpha;
    float beta;
    int peak_value;
};

struct ROI1
{
    Rect box;
    String region;
};

vector<ROI1> regions;

vector<ROI> roi_values;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->addItem("COLOR_BayerGR2BGR");
    ui->comboBox->addItem("COLOR_BayerRG2BGR");
    ui->comboBox->addItem("COLOR_BayerBG2RGB");
    ui->comboBox->addItem("COLOR_BayerGB2BGR");

    ui->comboBox_2->addItem("White Patch (Left)");
    ui->comboBox_2->addItem("White Patch (Right)");
    ui->comboBox_2->addItem("Gray Patch (Left)");
    ui->comboBox_2->addItem("Gray Patch (Right)");
    ui->comboBox_2->addItem("Black Patch");
    ui->comboBox_2->addItem("Black Trap");
    ui->comboBox_2->addItem("Chrome Ball");

    ui->comboBox_3->addItem("White Patch (Left)");
    ui->comboBox_3->addItem("White Patch (Right)");
    ui->comboBox_3->addItem("Gray Patch (Left)");
    ui->comboBox_3->addItem("Gray Patch (Right)");
    ui->comboBox_3->addItem("Black Patch");

    ui->comboBox_4->addItem("Without Preview");
    ui->comboBox_4->addItem("With Preview");

    // Set the image
    QString imagePath = "C:/Users/gmura/OneDrive/Pictures/Screenshots/Screenshot 2023-06-14 101703.png"; // Update with the actual path to your image
    QPixmap pixmap(imagePath);
    ui->label_pic->setPixmap(pixmap);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void mouse_call(int event, int x, int y,int, void*)
{
    if (event == EVENT_LBUTTONDOWN)
    {
        leftDown = true;
        cor1.x = x;
        cor1.y = y;
        cout << "Corner 1: " << cor1 << endl;

    }
    if (event == EVENT_LBUTTONUP)
    {
        if (abs(x - cor1.x) > 20 && abs(y - cor1.y) > 20) //checking whether the region is too small
        {
            leftup = true;
            cor2.x = x;
            cor2.y = y;
            cout << "Corner 2: " << cor2 << endl;
        }
        else
        {
            cout << "Select a region more than 20 pixels" << endl;
        }
    }

    if (leftDown == true && leftup == false) //when the left button is down
    {
        Point pt;
        pt.x = x;
        pt.y = y;
        Mat temp_img = demosaicedImage.clone();
        rectangle(temp_img, cor1, pt, Scalar(0, 0, 65535),5); //drawing a rectangle continuously
        imshow("Demosaiced Image", temp_img);
    }
    if (leftDown == true && leftup == true) //when the selection is done
    {
        box.width = abs(cor1.x - cor2.x);
        box.height = abs(cor1.y - cor2.y);
        box.x = min(cor1.x, cor2.x);
        box.y = min(cor1.y, cor2.y);

        leftDown = false;
        leftup = false;
    }
}

void Averages(Mat img,Rect box,String selectedregion)
{

    ROI roi;

    roi.region = selectedregion;
    qDebug() << roi.region;
    roi.coordinates = box;

    roi.peak_value = 0;
    Mat crop(img, box); //Selecting a ROI(region of interest) from the original pic


    if(roi.region == "Chrome Ball")
    {
        // Convert the ROI to grayscale
        Mat gray;
        cvtColor(crop, gray, COLOR_BGR2GRAY);

        // Calculate the histogram of the grayscale image
        Mat hist;
        int histSize = 65536;
        float range[] = { 0, 65536 };
        const float* histRange = { range };
        calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, &histRange, true, false);

        // Find the peak value in the histogram
        double minVal, maxVal;
        Point minLoc, maxLoc;
        minMaxLoc(hist, &minVal, &maxVal, &minLoc, &maxLoc);

        cout << "Peak value: " << maxLoc.y << endl;

        roi.peak_value = maxLoc.y;
    }
    else
    {
    vector<Mat> channels;
    // Split the image into color channels
    split(crop, channels);

    Scalar mean, std_dev;
    meanStdDev(channels[0], mean, std_dev);
    roi.blue_mean = mean;
    roi.blue_std = std_dev;
    meanStdDev(channels[1], mean, std_dev);
    roi.green_mean = mean;
    roi.green_std = std_dev;
    meanStdDev(channels[2], mean, std_dev);
    roi.red_mean = mean;
    roi.red_std = std_dev;

    roi.alpha = roi.green_mean.val[0] / roi.red_mean.val[0];
    roi.beta = roi.green_mean.val[0] / roi.blue_mean.val[0];
    }
    roi_values.push_back(roi);
}

void readConfiguration()
{
    // Read the JSON file
    QFile configFile("D:/config.json");
    if (!configFile.open(QIODevice::ReadOnly))
    {
        qWarning() << "Unable to open configuration file.";
    }

    // Parse the JSON document from the file
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(configFile.readAll(), &parseError);
    configFile.close();

    if (parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "Error parsing JSON:" << parseError.errorString();
    }

    // Retrieve values from the JSON document
    if (doc.isObject())
    {
        QJsonObject config = doc.object();
        QVariant heightVariant = config["height"].toVariant();
        QVariant widthVariant = config["width"].toVariant();
        QVariant blackVariant = config["black_level"].toVariant();
        QVariant gammaVariant = config["Gammavalue"].toVariant();

        if (heightVariant.canConvert<int>() && widthVariant.canConvert<int>())
        {
            MainWindow::height = heightVariant.toInt();
            MainWindow::width = widthVariant.toInt();
        }

        if(blackVariant.canConvert<int>())
        {
            MainWindow::blackLevel = blackVariant.toInt();
        }
        if(gammaVariant.canConvert<double>())
        {
            MainWindow::Gamma_value = gammaVariant.toDouble();
        }
        QString sensor = config["sensor"].toString();
        // Use the retrieved values as needed
        qDebug() << "Height:" << MainWindow::height;
        qDebug() << "Width:" << MainWindow::width;
        qDebug() << "Sensor:" << sensor;
        qDebug() << "Black Level:" << MainWindow::blackLevel;

    }
    else
    {
        qWarning() << "Invalid JSON document format.";
    }
}
void Output(Mat image, float alpha, float beta)
{
    Scalar b_average, g_average, r_average;

    vector<Mat> Channels;

    // Multiply the pixel values by the average value for each channel
    split(image, Channels);

    cout << endl << "G/R ratio :"<< alpha;
    cout << "G/B ratio :" << beta;

    Channels[0] = Channels[0] * beta;
    Channels[1] = Channels[1];
    Channels[2] = Channels[2] * alpha;

    b_average = mean(Channels[0]);
    g_average = mean(Channels[1]);
    r_average = mean(Channels[2]);



    // Merge the color channels back into a single image
    merge(Channels, output_image);
}


void Gammacorrection(Mat img)
{
    // Normalize pixel values
    img.convertTo(img, CV_32F, 1.0 / 65536.0);

    // Split color channels
    vector<Mat> channels;
    split(img, channels);

    // Apply gamma correction to each channel separately
    double gamma = MainWindow::Gamma_value;
    for (unsigned long long i = 0; i < channels.size(); i++)
    {
        pow(channels[i], gamma, channels[i]);
    }

    // Merge color channels
    merge(channels, img_gamma);

    // Scale pixel values back to [0, 65536] range
    img_gamma.convertTo(img_gamma, CV_16U, 65536);
}

void write_to_csv(const vector<ROI>& roi_values, const string& filename)
{
    ofstream file(filename);

    // Write the header row
    file << "Sl.no,File name,ROI,x,y,width,height,blue_mean,blue_std,green_mean,green_std,red_mean,red_std,G/R ratio,G/B ratio,Peak Value \n";

    // Write the data rows
    file << 1 << "," << filepath << "," << "\n";
    for (const auto& roi : roi_values)
    {
        file << "," << "," << roi.region <<"," << roi.coordinates.x << "," << roi.coordinates.y << ","
             << roi.coordinates.width << "," << roi.coordinates.height << ","
             << roi.blue_mean.val[0] << "," << roi.blue_std.val[0] << ","
             << roi.green_mean.val[0] << "," << roi.green_std.val[0] << ","
             << roi.red_mean.val[0] << "," << roi.red_std.val[0] << ","
             << roi.alpha << "," << roi.beta << "," << roi.peak_value << "\n";
    }
    file.close();
}

void write_to_csv_append(const vector<ROI>& roi_values, const string& filename,const string& imagepath,int j)
{
    ofstream file(filename,ios:: app);


    // Write the data rows
    file << j << "," << imagepath << "," << "\n";
    for (const auto& roi : roi_values)
    {
        file << "," << "," << roi.region << "," << roi.coordinates.x << "," << roi.coordinates.y << ","
             << roi.coordinates.width << "," << roi.coordinates.height << ","
             << roi.blue_mean.val[0] << "," << roi.blue_std.val[0] << ","
             << roi.green_mean.val[0] << "," << roi.green_std.val[0] << ","
             << roi.red_mean.val[0] << "," << roi.red_std.val[0] << ","
             << roi.alpha << "," << roi.beta << "," << roi.peak_value << "\n";
    }

    file.close();
}

void MainWindow::on_pushButton_clicked()
{

    //Code for open the picture file
    filename = QFileDialog::getOpenFileName(
        this,tr("Select Raw Image File"),"C://","All files (*.*) ;;Text File(*.txt);; Music file(*.mp3);;Raw Images(*.raw) "
        );

    filepath = filename.toStdString();

    ui->image_path->setText(filename);

    readConfiguration();

    bayerraw = Mat(height, width, CV_16UC1);

    if (!filename.isEmpty())
    {
        // Read raw Bayer image
        QMessageBox::information(this, tr("File Name"), filename);


        filepath = filename.toStdString();

        qDebug() << filepath;
        ifstream infile(filepath, ios::binary);
        infile.read(reinterpret_cast<char*>(bayerraw.data), height * width * sizeof(uint16_t));
        infile.close();

        int type = bayerraw.type();
        qDebug() << "Image type:" << type;
        int bitDepth = CV_MAT_DEPTH(type);

        qDebug() << "Image bit depth:" << bitDepth;
        qDebug() << "Image rows:" << bayerraw.rows;
        qDebug() << "Image cols:" << bayerraw.cols;

        QImage qimg(bayerraw.data, bayerraw.cols, bayerraw.rows, bayerraw.step, QImage::Format_Grayscale16);
        qimg = qimg.convertToFormat(QImage::Format_Grayscale8); // Convert to 8-bit grayscale for display

        // Create a QPixmap from the QImage
        QPixmap pixmap = QPixmap::fromImage(qimg);

        // Set the QPixmap as the pixmap for the QLabel in the UI
        ui->label_pic->setPixmap(pixmap);

        // Set the QPixmap as the pixmap for the QLabel in the UI
        ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
        ui->label_pic->setAlignment(Qt::AlignCenter);
    }
    else
    {
        QMessageBox::information(this, tr("Error Message"), "Select Image");
    }

}

void MainWindow::on_pushButton_Black_Level_clicked()
{
    // Perform the Black level Substraction
    // Find the minimum pixel value
    if(!bayerraw.empty())
    {

    uint16_t black_level = blackLevel;
    // Subtract the black level from the image
    img_corrected = bayerraw - black_level;


    QImage qimg(img_corrected.data, img_corrected.cols, img_corrected.rows, img_corrected.step, QImage::Format_Grayscale16);

    qimg = qimg.convertToFormat(QImage::Format_Grayscale8); // Convert to 8-bit grayscale for display

    // Create a QPixmap from the QImage
    QPixmap pixmap = QPixmap::fromImage(qimg);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);
    }
    else
    {
    QMessageBox::information(this,"Error Message","Select the Raw Image");
    }
}


void MainWindow::on_pushButton_Demosiac_clicked()
{
    // Get the selected text from the comboBox
    QString selectedText = ui->comboBox->currentText();
    // Map the selected text to the corresponding OpenCV color conversion constant
    std::map<QString, int> colorConversionMap;
    colorConversionMap["COLOR_BayerGR2BGR"] = cv::COLOR_BayerGR2BGR;
    colorConversionMap["COLOR_BayerRG2BGR"] = cv::COLOR_BayerRG2BGR;
    colorConversionMap["COLOR_BayerBG2RGB"] = cv::COLOR_BayerBG2RGB;
    colorConversionMap["COLOR_BayerGB2BGR"] = cv::COLOR_BayerGB2BGR;

    // Perform the selected color conversion
    int conversionConstant = colorConversionMap[selectedText];

    if(!img_corrected.empty())
    {
    // Demosaic the raw image
    cvtColor(img_corrected, demosaicedImage,conversionConstant);
    // Convert the image to 8-bit depth
    Mat img_8bit;
    demosaicedImage.convertTo(img_8bit, CV_8UC3, 1.0 / 256.0);  // Convert 16-bit to 8-bit
    // Create a QImage from the 8-bit color image
    QImage qimg(img_8bit.data, img_8bit.cols, img_8bit.rows, img_8bit.step, QImage::Format_BGR888);
    // Create a QPixmap from the QImage
    QPixmap pixmap = QPixmap::fromImage(qimg);
    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);
    }else
    {
    QMessageBox::information(this,"Error Opening File","Select Raw Image");
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    namedWindow("Demosaiced Image", WINDOW_NORMAL);
    resizeWindow("Demosaiced Image", 800, 600);

    // Get the primary screen
    QScreen *screen = QGuiApplication::primaryScreen();

    // Get the screen geometry
    QRect screenGeometry = screen->geometry();

    // Calculate the position to center the window
    int windowWidth = 800;
    int windowHeight = 600;
    int windowX = (screenGeometry.width() - windowWidth) / 2;
    int windowY = (screenGeometry.height() - windowHeight) / 2;

    moveWindow("Demosaiced Image", windowX, windowY);
    imshow("Demosaiced Image", demosaicedImage);
    setMouseCallback("Demosaiced Image", mouse_call);
    waitKey(0);
    destroyAllWindows();


    QString selectedregion = ui->comboBox_2->currentText();
    String selected;
    selected = selectedregion.toStdString();
    Averages(demosaicedImage, box, selected);
}

void MainWindow::on_pushButton_3_clicked()
{

    if(roi_values.size() == 7)
    {
    //Code for open the picture file
    csv_file = QFileDialog::getOpenFileName(
        this,tr("Open File"),"C://","All files (*.*) ;;Text File(*.txt);; Music file(*.mp3);;Raw Images(*.raw) "
        );

    csv_filepath = csv_file.toStdString();

    ui->csv_filepath->setText(csv_file);

    write_to_csv(roi_values,csv_filepath);

    QMessageBox::information(this,"Confirmation Message","Data Saved Successfully");
    }
    else
    {
    int remainingCount = 6 - roi_values.size();
    QString errorMessage = QString("Select remaining %1 region(s)").arg(remainingCount);
    QMessageBox::information(this,"Error Message", errorMessage);
    }
}

void MainWindow::on_pushButton_4_clicked()
{

    unsigned long long i;
    int region;
    for(i=0;i<roi_values.size();i++)
    {
    if(ui->comboBox_3->currentText().toStdString() == roi_values[i].region)
    {
        region = i;
    }
    }

    Output(demosaicedImage, roi_values[region].alpha, roi_values[region].beta);
    Mat output_8bit;
    output_image.convertTo(output_8bit, CV_8UC3, 1.0 / 256.0);  // Convert 16-bit to 8-bit

    // Create a QImage from the 8-bit color image
    QImage qimg(output_8bit.data, output_8bit.cols, output_8bit.rows, output_8bit.step, QImage::Format_BGR888);

    // Create a QPixmap from the QImage
    QPixmap pixmap = QPixmap::fromImage(qimg);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);
}

void MainWindow::on_pushButton_5_clicked()
{
    Gammacorrection(output_image);
    Mat gamma_8bit;
    img_gamma.convertTo(gamma_8bit, CV_8UC3, 1.0 / 256.0);  // Convert 16-bit to 8-bit

    // Create a QImage from the 8-bit color image
    QImage qimg(gamma_8bit.data, gamma_8bit.cols, gamma_8bit.rows, gamma_8bit.step, QImage::Format_BGR888);

    // Create a QPixmap from the QImage
    QPixmap pixmap = QPixmap::fromImage(qimg);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);
}
void MainWindow::on_pushButton_6_clicked()
{
    // Open a dialog to select a folder
    folder = QFileDialog::getExistingDirectory(nullptr, "Select Folder", QDir::homePath());

    ui->Autofolder_path->setText(folder);

    String folder_path = folder.toStdString();

    glob(folder_path, filenames);
}

void MainWindow::on_pushButton_7_clicked()
{
    roi_values.clear();

    int time;

    QString option = ui->comboBox_4->currentText();

    if(option == "Without Preview")
    {
    time = 0;
    }else
    {
    time = 1;
    }

    ROI1 coordinates;


    for (size_t j = 0; j < filenames.size(); j++)
    {
    vector<vector<string>> data ; // a 2D vector to hold the data
    ifstream file(Autocsv_file);// open the file
    string line;

    readConfiguration();

    bayerraw = Mat(height, width, CV_16UC1);

    ifstream infile(filenames[j], ios::binary);
    infile.read((char*)bayerraw.data, width * height * sizeof(uint16_t));
    infile.close();

    uint16_t black_level = blackLevel;
    // Subtract the black level from the image
    Mat img_corrected = bayerraw - black_level;


    // Demosaic the raw image
    cvtColor(img_corrected, demosaicedImage, COLOR_BayerGR2BGR);

    img_with_rect = demosaicedImage.clone(); // Create a copy of the original image

    while (getline(file, line))
    { // read the file line by line
        stringstream ss(line); // create a stringstream to parse the line

        vector<string> row; // a vector to hold the values of each row

        string value;
        while (getline(ss, value, ','))
        { // parse the line using comma as delimiter
            row.push_back(value);// add each value to the row vector
        }
        data.push_back(row); // add the row vector to the 2D vector
    }

    file.close(); // close the file

    for (int i = 2; i < 9; i++)
    {
        coordinates.region = data[i][2];
        coordinates.box.x = stoi(data[i][3]);
        coordinates.box.y = stoi(data[i][4]);
        coordinates.box.width = stoi(data[i][5]);
        coordinates.box.height = stoi(data[i][6]);
        regions.push_back(coordinates);
        cout << endl;
    }

    for (int i = 0; i < 7; i++)
    {
        box1.x = regions[i].box.x;
        box1.y = regions[i].box.y;
        box1.width = regions[i].box.width;
        box1.height = regions[i].box.height;

        rectangle(img_with_rect, box1, Scalar(0,65535, 0), 3);


        Mat outputimage_8bit;
        img_with_rect.convertTo(outputimage_8bit, CV_8UC3, 1.0 / 256.0);  // Convert 16-bit to 8-bit


        // Create a QImage from the 8-bit color image
        QImage qimg(outputimage_8bit.data, outputimage_8bit.cols, outputimage_8bit.rows, outputimage_8bit.step, QImage::Format_BGR888);

        // Create a QPixmap from the QImage
        QPixmap pixmap = QPixmap::fromImage(qimg);

        // Set the QPixmap as the pixmap for the QLabel in the UI
        ui->label_pic->setPixmap(pixmap);

        // Set the QPixmap as the pixmap for the QLabel in the UI
        ui->label_pic->setPixmap(pixmap.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
        ui->label_pic->setAlignment(Qt::AlignCenter);

        // Wait for a while (optional)
        QApplication::processEvents();
        QThread::sleep(time);
        Averages(demosaicedImage, box1,regions[i].region);
    }

    Output(demosaicedImage, roi_values[0].alpha, roi_values[0].beta);

    Mat output_8bit;
    output_image.convertTo(output_8bit, CV_8UC3, 1.0 / 256.0);  // Convert 16-bit to 8-bit

    // Create a QImage from the 8-bit color image
    QImage qimg1(output_8bit.data, output_8bit.cols, output_8bit.rows, output_8bit.step, QImage::Format_BGR888);

    // Create a QPixmap from the QImage
    QPixmap pixmap1 = QPixmap::fromImage(qimg1);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap1);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap1.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);

    QApplication::processEvents();
    QThread::sleep(time);


    Gammacorrection(output_image);
    Mat gamma_8bit;
    img_gamma.convertTo(gamma_8bit, CV_8UC3, 1.0 / 256.0);  // Convert 16-bit to 8-bit

    // Create a QImage from the 8-bit color image
    QImage qimg3(gamma_8bit.data, gamma_8bit.cols, gamma_8bit.rows, gamma_8bit.step, QImage::Format_BGR888);

    // Create a QPixmap from the QImage
    QPixmap pixmap3 = QPixmap::fromImage(qimg3);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap3);

    // Set the QPixmap as the pixmap for the QLabel in the UI
    ui->label_pic->setPixmap(pixmap3.scaled(ui->label_pic->size(), Qt::KeepAspectRatio));
    ui->label_pic->setAlignment(Qt::AlignCenter);

    QApplication::processEvents();
    QThread::sleep(time);

    write_to_csv_append(roi_values,Autocsv_file,filenames[j],j);

    roi_values.clear();
    }
}


void MainWindow::on_pushButton_8_clicked()
{
    InputDialog inputDialog(this);
    inputDialog.setModal(true);

    QString heightText;
    QString widthText;
    QString SensorInfo;
    QString BlackLevel;
    QString Gammavalue;
    QString CFAPattern;
    QString Bitdepth;

    if (inputDialog.exec() == QDialog::Accepted)
    {
    // Dialog was accepted, handle the input values here
    heightText = inputDialog.getHeight();
    widthText = inputDialog.getWidth();
    SensorInfo = inputDialog.getSensorInformation();
    BlackLevel = inputDialog.getBlackLevel();
    Gammavalue = inputDialog.getGammavalue();
    CFAPattern = inputDialog.getCFAPattern();
    Bitdepth = inputDialog.getBitdepth();
    }
    // Create a JSON object for the configuration
    QJsonObject config;
    config["height"] = heightText;
    config["width"] = widthText;
    config["sensor"] = SensorInfo;
    config["black_level"] = BlackLevel;
    config["Gammavalue"] = Gammavalue;
    config["CFAPattern"] = CFAPattern;
    config["Bitdepth"] = Bitdepth;

    // Create a JSON document and set the config object as its content
    QJsonDocument doc(config);

    // Save the JSON document to a file
    QFile configFile("D:/config.json");
    if (configFile.open(QIODevice::WriteOnly))
    {
    configFile.write(doc.toJson());
    configFile.close();
    qDebug() << "Configuration saved successfully.";
    }
    else
    {
    qWarning() << "Unable to open configuration file.";
    }

}

void MainWindow::on_pushButton_9_clicked()
{
    QString Auto_file;
    //Code for open the picture file
    Auto_file = QFileDialog::getOpenFileName(
        this,tr("Select Raw Image File"),"C://","All files (*.*) ;;Text File(*.txt);; Music file(*.mp3);;Raw Images(*.raw) "
        );

    Autocsv_file = Auto_file.toStdString();

    ui->Autocsv_path->setText(Auto_file);
}

