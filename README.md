
# Measurement of White Point and Optical Black Level from a Reference Object SpyderCube.

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#purpose">Purpose</a></li>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#dependencies">Dependencies</a></li>
        <li><a href="#setting-up-Qt-with-Opencv-in-Windows">Setting up Qt with Opencv in Windows</a></li>
        <li><a href="#running-the-application">Running the Application</a></li>
      </ul>
    </li>
    <li>
      <a href="#usage-of-the-GUI-Tool">Usage of the GUI Tool</a>
    </li>
  </ol>
</details>

## About the Project

SpyderCube is a Color Calibration tool which is used for color correction and white balance adjustment in digital photography and video. 
It is a small cube-shaped device that consists of different surfaces with specific colors and tones. The SpyderCube is primarily used to 
achieve accurate and consistent color reproduction in your photographs. By including the SpyderCube in a reference shot during a photo session, 
you can use it as a reference point for adjusting colors during post-processing. The cube's surfaces include black, white, neutral gray, and a range of other colors, 
which can be used to determine proper exposure, contrast, and color balance.

### Purpose

* Accurate color reproduction:
Achieving accurate color reproduction is crucial for photographers and videographers who want their images or
videos to reflect the true colors of the scene they captured.The black and white surfaces of the SpyderCube help you determine the proper exposure and 
adjust the contrast in your images. By measuring the values and comparing them to the black and white surfaces, you can ensure that your images have the desired tonal range, with deep blacks and pure whites, without losing details. <br>
* White balance adjustment: 
Proper white balance is crucial for accurate color reproduction. The SpyderCube allows you to measure the white 
and neutral gray surfaces, providing a reference point for setting the correct white balance in your images or videos. <br>

### Built With

* [![C++](https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://en.cppreference.com/)
* [![PyQt](https://img.shields.io/badge/PyQt-41CD52?style=for-the-badge&logo=python&logoColor=white)](https://riverbankcomputing.com/software/pyqt/)
* [![OpenCV](https://img.shields.io/badge/OpenCV-5C3EE8?style=for-the-badge&logo=opencv&logoColor=white)](https://opencv.org/)

<!-- GETTING STARTED -->
## Getting Started

### Dependencies

This project depends on the following libraries and tools

* OpenCV: Version 4.6.0
* Qt: Version 5.15.2 With MinGW 8.1.0 32-bit
* CMake: Version 3.26.0

### Setting up Qt with Opencv In Windows

[Watch the video tutorial](https://www.youtube.com/watch?v=0KNh_7fUqrM)

### Running the Application

To run the application, follow these steps:

To run the application, follow these steps:

1. Clone the Repository: Open the Qt software and clone the repository by clicking on File -> New File or Project -> Import Project -> Git -> Clone Repository. Enter the repository URL (https://github.com/Murari007/Measurement-of-White-Point-and-Optical-Black-Level-from-a-Reference-Object-SpyerCube-..git) and choose the destination folder on your local machine.<br>
2. Open the Project: After cloning the repository, you will see the project listed in the "Projects" tab. Click on the project to open it in the Qt software.<br>
3. Build the Project: Build the project by clicking on the build button (hammer icon) or by selecting Build -> Build All from the menu. This will compile the code and generate the executable file.<br>
4. Run the Project: Once the build is successful, you can run the project by clicking on the run button (play icon) or by selecting Build -> Run from the menu. This will execute the code and display the output or the desired functionality of your project.<br>

## Usage of the GUI Tool

As soon as we run the Application the GUI tool Opens.

<p align="center">
  <img src="https://github.com/Murari007/Measurement-of-White-Point-and-Optical-Black-Level-from-a-Reference-Object-SpyerCube./assets/83585005/f3c3886f-e069-40ac-bea9-8917d126e0a9" alt="guitool" width="800">
</p>

Configure the image settings by clicking the "Configuration Settings" option. Enter the Height, Width, Bit Depth, CFA Pattern etc..

<div align="center">
  <img src="https://github.com/Murari007/Measurement-of-White-Point-and-Optical-Black-Level-from-a-Reference-Object-SpyerCube./assets/83585005/85852615-7fda-408d-bae6-7ea38e5988f8" alt="configure" width="400">
</div>

* After configuring the image settings, then read the raw image data from a selected file and perform Black Level Substraction and Demosaicing operation.
* After Demosiacing manually select six region of interest of SpyderCube to measure Statistical Data. 
* Save the Data to a .csv file.
* Apply the gains of respective region gain to white balance the image and proceed with Gamma Correction.
* Use the Auto-select option to perfrom on large image datasets.






