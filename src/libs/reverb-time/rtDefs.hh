#ifndef REVERBANALYZER_H
#define REVERBANALYZER_H

#include <vector>
#include <armadillo>

struct Point {
    double x;
    double y;
};

struct REVERB_TIME {
    double RT60;
    double RT30;
    double RT20;
    double RT10;
};




class LinearCurveFit {
public:
    LinearCurveFit();
    bool fitPoints(const std::vector<Point>& points);
    double getSlope() const;
    double getYInt() const;

private:
    double slope;
    double yInt;
};

class ReverbAnalyzer {

    private:
        arma::cx_vec hilbertTransform(std::vector<double>& data);
        void movingAverage(std::vector<double>& data, int k);
        std::vector<double> linspace(double start, double end, int num);
        void schroederIntegration(std::vector<double>& data);

    public:
        ReverbAnalyzer() = default;
        ~ReverbAnalyzer() = default;
        REVERB_TIME reverbTimeCalc(std::vector<double>& audioData, double fs, int window_size);
};

#endif // REVERBANALYZER_H
