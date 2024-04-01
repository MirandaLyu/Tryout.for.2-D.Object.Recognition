#include "task6.h"

double calculateStandardDeviation(const std::vector<double>& data) {
    // Calculate the mean (average)
    double sum = 0.0;
    for (const double& value : data) {
        sum += value;
    }
    double mean = sum / data.size();

    // Calculate the squared differences from the mean
    double squaredDifferencesSum = 0.0;
    for (const double& value : data) {
        double diff = value - mean;
        squaredDifferencesSum += diff * diff;
    }

    // Calculate the average of squared differences
    double variance = squaredDifferencesSum / data.size();

    // Take the square root to get the standard deviation
    double stdev = std::sqrt(variance);

    return stdev;
}