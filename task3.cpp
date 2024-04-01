#include "task3.h"

bool compareComponents(const Component& a, const Component& b) {
    return a.area > b.area;
}

int imgSegmentation(const Mat& cleanedUpImg, Mat& regionMap, Mat& binaryRegionMap, Mat& stats) {
    Mat labels, centroids;
    int numComponents = connectedComponentsWithStats(cleanedUpImg, labels, stats, centroids);
    // std::cout << "Number of components: " << numComponents << std::endl;

    // Create a list of random colors (256 unique colors)
    std::vector<cv::Vec3b> colors;
    for (int i = 0; i < 256; i++) {
        colors.push_back(cv::Vec3b(rand() % 256, rand() % 256, rand() % 256));
    }

    // Create a vector to store the components and their areas
    std::vector<Component> components;

    // Iterate through the components
    for (int componentIndex = 1; componentIndex < numComponents; componentIndex++) {
        int area = stats.at<int>(componentIndex, cv::CC_STAT_AREA);

        // Store the component index and its area
        Component component;
        component.index = componentIndex;
        component.area = area;

        components.push_back(component);
    }

    // Sort the components based on area in descending order
    std::sort(components.begin(), components.end(), compareComponents);

    // Get the three largest components
    int numLargestComponents = std::min(3, static_cast<int>(components.size()));

    // get the region closest to the center of the image
    int minDistanceLabel = 0;
    float minDistance = std::numeric_limits<float>::max();
    for (int i = 0; i < numLargestComponents; i++) {
        int label = components[i].index;
        double cx = centroids.at<double>(label, 0);
        double cy = centroids.at<double>(label, 1);
        double distance = (cx - cleanedUpImg.rows / 2) * (cx - cleanedUpImg.rows / 2)
                        + (cy - cleanedUpImg.cols / 2) * (cy - cleanedUpImg.cols / 2);
        if (distance < minDistance) {
            minDistance = distance;
            minDistanceLabel = label;
        }
    }

    cv::Vec3b color = colors[minDistanceLabel % 256]; // Assign a unique color based on label
    for (int x = 0; x < cleanedUpImg.rows; x++) {
        for (int y = 0; y < cleanedUpImg.cols; y++) {
            if (labels.at<int>(x, y) == minDistanceLabel) {
                binaryRegionMap.at<uchar>(x, y) = 255; // Set pixel color in region map
                regionMap.at<cv::Vec3b>(x, y) = color; // Set pixel color in region map
            }
        }
    }
    return minDistanceLabel;
}