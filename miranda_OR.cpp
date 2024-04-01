/*
a system that can extract objects' features
and identify objects */

#include <sstream>
#include <opencv2/opencv.hpp>
#include <dirent.h>
#include <string>
#include <limits>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "task1.h"
#include "task2.h"
#include "task3.h"
#include "task4.h"
#include "task5.h"
#include "task6.h"

using namespace cv;
using namespace std;

int main() {
    // loop through all images in the directory
    DIR* dir;
    struct dirent* entry;
    std::string inputFolder = "../proj3/testData/";
    std::ofstream outputFile;
    outputFile.open("../proj3/featureVectors.csv", std::ios::app);
    // outputFile << "label,percentFilled,boundingBoxRatio,perimeter,circularity" << std::endl;
    if ((dir = opendir(inputFolder.c_str())) != NULL)  {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_type == DT_REG) { // Check if it's a regular file
            const std::string fileName = inputFolder + entry->d_name;
            
            // for each image
            Mat img = imread(fileName);
            if (img.empty()) {
                std::cerr << "Error: Could not read the image." << fileName << std::endl;
                continue;
            }

            // task 1, not use threshold() function
            Mat grayImg = preprocess(img);
            Mat binaryImg = thresholding(grayImg);
            // display task 1
            std::string windowName1 = fileName + "grayImg";
            std::string windowName2 = fileName + "binaryImg";
            // imshow(windowName1, grayImg);
            imshow(windowName2, binaryImg);


            // task 2
            // close some holes
            Mat cleanedUpImg = cleaningUp(binaryImg);

            std::string windowName3 = fileName + "cleanedUpImg";
            imshow(windowName3, cleanedUpImg);

            // task 3
            // Perform connected components analysis
            Mat regionMap(cleanedUpImg.size(), CV_8UC3, Scalar(0, 0, 0)); // Initialize as black image
            Mat binaryRegionMap(cleanedUpImg.size(), CV_8UC1, Scalar(0)); // Initialize as black image

            Mat stats;
            int minDistanceLabel = imgSegmentation(cleanedUpImg, regionMap, binaryRegionMap, stats);

            std::string windowName4 = fileName + "regionMap";
            cv::imshow(windowName4, regionMap);

            // task 4
            // draw the axis of least central moment and the oriented bounding box
            cv::RotatedRect rotatedRect = showTwoFeatures(regionMap, binaryRegionMap);

            std::string windowName5 = fileName + "task4";
            cv::imshow(windowName5, regionMap);


            // task 5
            // extract features
            double percentFilled, boundingBoxRatio, perimeter;
            double circularity = collectFeatureData(stats, minDistanceLabel, rotatedRect, binaryRegionMap, percentFilled, boundingBoxRatio, perimeter);
            std::cout << "percentFilled: " << percentFilled << std::endl;
            std::cout << "boundingBoxRatio: " << boundingBoxRatio << std::endl;
            std::cout << "perimeter: " << perimeter << std::endl;
            std::cout << "circularity: " << circularity << std::endl;

            cv::waitKey(0);

            // if press 1, collect data; if press 2, identify the object
            std::cout << "Extract of identify? (1 for extract, 2 for identify): ";
            int choice;
            std::cin >> choice;
            if (choice == 1) {
                std::cout << "Enter label for this object: ";
                std::string label;
                std::cin >> label;
                if (!outputFile.is_open()) {
                    std::cerr << "Error: Could not open the write file." << std::endl;
                    return -1;
                } else {
                    outputFile << label << ",";
                    outputFile << percentFilled << ",";
                    outputFile << boundingBoxRatio << ",";
                    outputFile << perimeter << ",";
                    outputFile << circularity << std::endl;
                }
            } else if (choice == 2) {  // task 6
                if (outputFile.is_open()) {
                    outputFile.close(); // close the write file first
                }
                std::ifstream inputFile;
                inputFile.open("../proj3/featureVectors.csv");
                if (!inputFile.is_open()) {
                    std::cerr << "Error: Could not open the read file." << std::endl;
                    return -1;
                }
                std::string line;
                // Read and discard the header line
                if (!std::getline(inputFile, line)) {
                    std::cerr << "Error reading header line." << std::endl;
                    inputFile.close();
                    return 1;
                }
                std::vector<std::string> allLabels;
                std::vector<double> allPf;
                std::vector<double> allBbr;
                std::vector<double> allP;
                std::vector<double> allC;
                while (std::getline(inputFile, line)) {
                    // Use a stringstream to parse the comma-separated values
                    istringstream  iss(line);
                    std::string l, pf, bbr, p, c;
                    std::getline(iss, l, ',');
                    std::cout << "l: " << l << std::endl;
                    std::getline(iss, pf, ',');
                    std::cout << "pf: " << pf << std::endl;
                    std::getline(iss, bbr, ',');
                    std::cout << "bbr: " << bbr << std::endl;
                    std::getline(iss, p, ',');
                    std::cout << "p: " << p << std::endl;
                    std::getline(iss, c);
                    std::cout << "c: " << c << std::endl;
                    allLabels.push_back(l);
                    allPf.push_back(std::stod(pf));
                    allBbr.push_back(std::stod(bbr));
                    allP.push_back(std::stod(p));
                    allC.push_back(std::stod(c));
                }
                inputFile.close();

                // calculate the distances
                std::vector<double> distances;
                double stdev_pf = calculateStandardDeviation(allPf);
                double stdev_bbr = calculateStandardDeviation(allBbr);
                double stdev_p = calculateStandardDeviation(allP);
                double stdev_c = calculateStandardDeviation(allC);
                for (int i = 0; i < allPf.size(); i++) {
                    double delta_pf = (percentFilled - allPf[i]) / stdev_pf;
                    double delta_bbr = (boundingBoxRatio - allBbr[i]) / stdev_bbr;
                    double delta_p = (perimeter - allP[i]) / stdev_p;
                    double delta_c = (circularity - allC[i]) / stdev_c;
                    double distance = delta_pf * delta_pf + delta_bbr * delta_bbr + delta_p * delta_p + delta_c * delta_c;
                    distances.push_back(distance);
                }

                // find the smallest distance
                double minDistance = distances[0];
                int minDistanceIndex = 0;
                for (int i = 1; i < distances.size(); i++) {
                    if (distances[i] < minDistance) {
                        minDistance = distances[i];
                        minDistanceIndex = i;
                    }
                }

                // Set the font and text properties
                int fontFace = cv::FONT_HERSHEY_SIMPLEX;
                double fontScale = 5;
                int fontThickness = 4;
                cv::Scalar textColor(0, 0, 255); // Red color (BGR format)

                // Define the position to place the label
                cv::Point textPosition(200, 600); // (x, y) coordinates

                // Add the label text to the image
                cv::putText(regionMap, allLabels[minDistanceIndex], textPosition, fontFace, fontScale, textColor, fontThickness);
                std::string windowName5 = fileName + "task5";
                cv::imshow(windowName5, regionMap);
                waitKey(0);

            } else {
                std::cout << "Invalid input." << std::endl;
            }

            }
        }
        closedir(dir);
        if (outputFile.is_open()) {
            outputFile.close();
        }
    } else {
        std::cerr << "Error: Input folder not found." << std::endl;
        return -2;
    }

    waitKey(0);

    return 0;
}