#include "rtDefs.hh"
#include <cmath>
#include <algorithm>
#include <fstream>
#define CHANGE_RATE_TH 0.0008

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
    int revIx = data.size()-1;
    for (size_t ii = 0; ii < data.size(); ii++) {
        data[ii] = 10 * log10(cumsum[revIx] / max_cumsum);
        revIx--;
    }
}

REVERB_TIME ReverbAnalyzer::reverbTimeCalc(std::vector<double>& audioData, double fs, int window_size) {
    REVERB_TIME reverbTime;
    std::vector<Point> curveFitData;
    double sum,avg;
    int integralSize,ii;

    // Hilbert Transform
    arma::cx_vec hilbertTransformedData = hilbertTransform(audioData);
    std::vector<double> envelope = arma::conv_to<std::vector<double>>::from(arma::abs(hilbertTransformedData));

    // Moving Average
    movingAverage(envelope, window_size);

    std::vector<double> debug;
    debug.resize(envelope.size());
    for (int gdb = 0; gdb < envelope.size(); gdb++) {
        debug[gdb] = 20 * log10(envelope[gdb]);
    }
    for (ii = 0; ii < envelope.size(); ii += 1024){
        sum = std::accumulate(envelope.begin() + ii, envelope.begin() + ii + 1024, 0.0);
        avg = sum / 1024;
        if (avg < CHANGE_RATE_TH){
            break;
        }
    }
    integralSize = ii;
    // Schroeder Integration
    std::vector<double> schIntegralInput(envelope.begin(), integralSize + envelope.begin());
    //schIntegralInput.erase(schIntegralInput.begin() + integralSize, schIntegralInput.end());
    
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
