#include "task4.h"

RotatedRect showTwoFeatures(Mat& regionMap, const Mat& binaryRegionMap) {
    cv::Moments moments = cv::moments(binaryRegionMap);
        // Calculate the orientation angle (axis of least central moment)
    double angle = 0.5 * atan2(2 * moments.mu11, moments.mu20 - moments.mu02);
        // draw the major and minor axis
    cv::Point startPoint;
    cv::Point endPointMinor;
    cv::Point endPointMajor;
    float length = 1000.0; // Length of the axis (can adjust this)
    // double cx = centroids.at<double>(minDistanceLabel, 0);
    // double cy = centroids.at<double>(minDistanceLabel, 1);
    startPoint.x = moments.m10 / moments.m00;
    startPoint.y = moments.m01 / moments.m00;
    // startPoint.x = cx;
    // startPoint.y = cy;
    endPointMinor.x = startPoint.x + length * cos(angle);
    endPointMinor.y = startPoint.y + length * sin(angle);
    int thickness = 3;
    cv::line(regionMap, startPoint, endPointMinor, cv::Scalar(0, 255, 0), thickness);
    endPointMajor.x = startPoint.x + length * cos(angle + CV_PI / 2);
    endPointMajor.y = startPoint.y + length * sin(angle + CV_PI / 2);
    cv::line(regionMap, startPoint, endPointMajor, cv::Scalar(0, 255, 0), thickness);

        // draw oriented bouding box using minAreaRect()
    std::vector<cv::Point> nonZeroPoints;
    cv::findNonZero(binaryRegionMap, nonZeroPoints);
    cv::RotatedRect rotatedRect = cv::minAreaRect(nonZeroPoints);
    cv::Point2f vertices[4];
    rotatedRect.points(vertices);
    for (int i = 0; i < 4; i++) {
        cv::line(regionMap, vertices[i], vertices[(i + 1) % 4], cv::Scalar(255, 0, 0), thickness);
    }
    
    return rotatedRect;
}