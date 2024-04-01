#include "task2.h"

Mat cleaningUp(Mat& binaryImg) {
    int morph_size = 6;
    Mat element = getStructuringElement(MORPH_RECT, Size(2 * morph_size + 1, 2 * morph_size + 1), 
                                        Point(morph_size, morph_size));

    Mat cleanedUpImg;
    dilate(binaryImg, binaryImg, element);
    erode(binaryImg, binaryImg, element);
    cleanedUpImg = binaryImg;
    return cleanedUpImg;
}