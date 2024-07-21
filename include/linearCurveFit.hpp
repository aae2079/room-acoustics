#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Point {
	double x;
	double y;
};

class LinearCurveFit{
	private:
		double slope, yInt;
	public:
		LinearCurveFit(): slope(0), yInt(0) {}
		double getYforX(double x) {
			return slope*x + yInt;
		}

		bool fitPoints(const vector<Point>& points) {
			int nPoints = points.size();
			if(nPoints < 2) {
				//Infitenly many lines pass through a single point
				return false;
			}
			double sumX = 0, sumY = 0, sumXY = 0, sumX2 = 0;
			for(auto& p : points) {
				sumX += p.x;
				sumY += p.y;
				sumXY += p.x*p.y;
				sumX2 += p.x*p.x;
			}

			double xMean = sumX/nPoints;
			double yMean = sumY/nPoints;
			double denominator = sumX2 - sumX*sumX/nPoints;

			if(denominator == 0) {
				//The line is vertical
				return false;
			}

			slope = (sumXY - sumX*sumY/nPoints)/denominator;
			yInt = yMean - slope*xMean;
		}
		double getSlope() {
			return slope;
		}
		double getYInt() {
			return yInt;
		}

};
