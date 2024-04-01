#include "task5.h"

double collectFeatureData(const Mat& stats, int minDistanceLabel, RotatedRect rotatedRect, const Mat& binaryRegionMap, double& percentFilled, double& boundingBoxRatio, double& perimeter) {
    int area = stats.at<int>(minDistanceLabel, cv::CC_STAT_AREA);
    percentFilled = static_cast<double>(area) / (rotatedRect.size.height * rotatedRect.size.width);
    std::cout << "Testing percentFilled: " << percentFilled << std::endl;
    boundingBoxRatio = (rotatedRect.size.height / rotatedRect.size.width) > 1 ?
                                (rotatedRect.size.width / rotatedRect.size.height) :
                                (rotatedRect.size.height / rotatedRect.size.width);
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(binaryRegionMap, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    perimeter = cv::arcLength(contours[0], true);
    double circularity = (4 * CV_PI * area) / (perimeter * perimeter);

    return circularity;
}