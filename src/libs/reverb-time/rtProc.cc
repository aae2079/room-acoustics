#include "rtDefs.hh"
#include <cmath>
#include <algorithm>

LinearCurveFit::LinearCurveFit() : slope(0), yInt(0) {}

bool LinearCurveFit::fitPoints(const std::vector<Point>& points) {
    int nPoints = points.size();
    if (nPoints < 2) {
        return false;
    }
    double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
    for (const auto& p : points) {
        sumX += p.x;
        sumY += p.y;
        sumXY += p.x * p.y;
        sumX2 += p.x * p.x;
    }
    double xMean = sumX / nPoints;
    double yMean = sumY / nPoints;
    double denominator = sumX2 - (sumX * sumX) / nPoints;

    if (denominator == 0) {
        return false;
    }

    slope = (sumXY - (sumX * sumY) / nPoints) / denominator;
    yInt = yMean - slope * xMean;
    return true;
}

double LinearCurveFit::getSlope() const { return slope; }
double LinearCurveFit::getYInt() const { return yInt; }

arma::cx_vec ReverbAnalyzer::hilbertTransform(std::vector<double>& data) {
    int N = data.size();
    arma::vec dataArma(data);
    arma::cx_vec dataFFT = arma::fft(dataArma);

    if (N % 2 == 0) {
        for (int ii = 1; ii < N / 2; ii++) {
            dataFFT(ii) *= 2;
        }
    } else {
        for (int ii = 1; ii < (N + 1) / 2; ii++) {
            dataFFT(ii) *= 2;
        }
    }

    return arma::ifft(dataFFT);
}

void ReverbAnalyzer::movingAverage(std::vector<double>& data, int k) {
    std::vector<double> frame;
    int dataSize = data.size();

    for (int ii = 0; ii < dataSize - k + 1; ++ii) {
        double sum = 0;
        for (int jj = 0; jj < k; ++jj) {
            sum += data[ii + jj];
        }
        frame.push_back(sum / k);
    }

    data = frame;
}

std::vector<double> ReverbAnalyzer::linspace(double start, double end, int num) {
    std::vector<double> result(num);
    double step = (end - start) / (num - 1);

    for (int i = 0; i < num; ++i) {
        result[i] = start + i * step;
    }

    return result;
}

void ReverbAnalyzer::schroederIntegration(std::vector<double>& data) {
    std::vector<double> cumsum;
    double sum = 0;

    for (double val : data) {
        sum += val;
        cumsum.push_back(sum);
    }

    double max_cumsum = *std::max_element(cumsum.begin(), cumsum.end());

    for (double& val : data) {
        val = 10 * log10(cumsum[val] / max_cumsum);
    }
}

REVERB_TIME ReverbAnalyzer::reverbTimeCalc(std::vector<double>& audioData, double fs, int window_size) {
    REVERB_TIME reverbTime;
    LinearCurveFit curveFit;
    std::vector<Point> curveFitData;

    // Hilbert Transform
    arma::cx_vec hilbertTransformedData = hilbertTransform(audioData);
    std::vector<double> envelope = arma::conv_to<std::vector<double>>::from(arma::abs(hilbertTransformedData));

    // Moving Average
    movingAverage(envelope, window_size);

    // Schroeder Integration
    std::vector<double> schIntegralInput(envelope.begin(), envelope.end());
    std::reverse(schIntegralInput.begin(), schIntegralInput.end());
    std::transform(schIntegralInput.begin(), schIntegralInput.end(), schIntegralInput.begin(), [](double x) { return x * x; });
    schroederIntegration(schIntegralInput);

    // Calculate the reverb time
    int N = schIntegralInput.size();
    std::vector<double> time = linspace(0, N / fs, N);

    for (int ii = 0; ii < N; ++ii) {
        curveFitData.push_back({ time[ii], schIntegralInput[ii] });
    }

    curveFit.fitPoints(curveFitData);
    double slope = curveFit.getSlope();

    reverbTime.RT60 = -60 / slope;
    reverbTime.RT30 = -30 / slope;
    reverbTime.RT20 = -20 / slope;
    reverbTime.RT10 = -10 / slope;

    return reverbTime;
}
