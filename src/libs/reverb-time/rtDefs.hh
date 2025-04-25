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
    LinearCurveFit() : slope(0), yInt(0) {}
    ~LinearCurveFit() = default;
    bool fitPoints(const std::vector<Point>& points){
        int nPoints = points.size();
        if (nPoints < 2) {
            return false;
        }
        double sumX = 0, sumY = 0, sumXY = 0, sumXX = 0;
        for (const auto& p : points) {
            sumX += p.x;
            sumY += p.y;
            sumXY += p.x * p.y;
            sumXX += p.x * p.x;
        }
        double xMean = sumX / nPoints;
        double yMean = sumY / nPoints;
        double xyMean = sumXY / nPoints;
        double xxMean = sumXX / nPoints;
    
        double denominator = xxMean - xMean * xMean;
        if (denominator == 0) {
            return false; 
        }
    
        slope = (xyMean - xMean * yMean) / denominator;
        yInt = yMean - slope * xMean;
        return true;
    };
    double getSlope() const{
        return slope;
    };
    double getYInt() const{
        return yInt;
    };

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

        LinearCurveFit curveFit;

    public:
        ReverbAnalyzer() = default;
        ~ReverbAnalyzer() = default;
        REVERB_TIME reverbTimeCalc(std::vector<double>& audioData, double fs, int window_size);
};

#endif // REVERBANALYZER_H
