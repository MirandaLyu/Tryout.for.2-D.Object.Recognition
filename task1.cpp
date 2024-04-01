#include "task1.h"

Mat preprocess(const Mat& img) {
    Mat hsvImg;
    cvtColor(img, hsvImg, COLOR_BGR2HSV);
    for (int x = 0; x < hsvImg.rows; x++) {
        for (int y = 0; y < hsvImg.cols; y++) {
            Vec3b &pixel = hsvImg.at<Vec3b>(x, y);
            int saturation = pixel[1];
            int value = pixel[2];

            // Check if the saturation is high (e.g., above 150)
            if (saturation > 120) {
                // Decrease the value/brightness
                pixel[2] = std::max(value - 60, 0);
            }
        }
    }
    Mat processedImg;
    cvtColor(hsvImg, processedImg, COLOR_HSV2BGR);

        // Blur the image a little bit
    Mat blurImg;
    GaussianBlur(processedImg, blurImg, Size(3, 3), 0);
        // to greyscale
    Mat grayImg;
    cvtColor(blurImg, grayImg, COLOR_BGR2GRAY);
    return grayImg;
}

int calculateBestThreshold(float histogram[], int size) {
    float minWeightedS = std::numeric_limits<float>::max();
    int bestThreshold = 0;
    float totalSum = 0.0;
    std::vector<float> cumulativeSums(size, 0.0);

    // Calculate cumulative sums first
    for (int i = 0; i < 256; i++) {
        totalSum += histogram[i];
        cumulativeSums[i] = totalSum;
    }

    for (int t = 1; t < 255; t++) {
        float group1Sum = cumulativeSums[t - 1];
        float group2Sum = totalSum - group1Sum;

        if (group1Sum == 0.0 || group2Sum == 0.0) continue;

        float group1Mean = 0.0;
        float group2Mean = 0.0;
        for (int i = 0; i < t; i++) {
            group1Mean += i * histogram[i];
        }
        group1Mean /= group1Sum;

        for (int j = t; j < 256; j++) {
            group2Mean += j * histogram[j];
        }
        group2Mean /= group2Sum;

        float group1Var = 0.0;
        float group2Var = 0.0;
        for (int i = 0; i < t; i++) {
            float diff = i - group1Mean;
            group1Var += diff * diff * histogram[i];
        }
        group1Var /= group1Sum;

        for (int j = t; j < 256; j++) {
            float diff = j - group2Mean;
            group2Var += diff * diff * histogram[j];
        }
        group2Var /= group2Sum;

        float weightedSum = group1Sum * group1Var + group2Sum * group2Var;
        if (weightedSum < minWeightedS) {
            minWeightedS = weightedSum;
            bestThreshold = t;
        }
    }

    std::cout << "Best threshold: " << bestThreshold << std::endl;
    return bestThreshold;
}

Mat thresholding(const Mat& grayImg) {
    float histogram[256] = {0.0};
    for (int x = 0; x < grayImg.rows; x++) {
        for (int y = 0; y < grayImg.cols; y++) {
            int pixelVal = grayImg.at<uchar>(x, y);
            histogram[pixelVal]++;
        }
    }
        // normalize the histogram
    for (int i = 0; i < 256; i++) {
        histogram[i] = histogram[i] / (grayImg.rows * grayImg.cols);
    }
        // calculate the min weighted sum of within-group variances
    int bestThreshold = calculateBestThreshold(histogram, 256);
        // Apply the threshold
    Mat binaryImg = Mat::zeros(grayImg.rows, grayImg.cols, CV_8UC1);
    for (int x = 0; x < grayImg.rows; x++) {
        for (int y = 0; y < grayImg.cols; y++) {
            int pixelVal = grayImg.at<uchar>(x, y);
            if (pixelVal < bestThreshold) {
                binaryImg.at<uchar>(x, y) = 255;
            } else {
                binaryImg.at<uchar>(x, y) = 0;
            }
        }
    }
    return binaryImg;
}