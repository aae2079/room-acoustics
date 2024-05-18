#include <vector>
#include <algorithm>
#include <cmath>
#include <armadillo>
#include <acousDefs.hpp>

class reverbAnalyzer{
    public: 
        reverbAnalyzer(std::vector<double>& audioData, double fs, int window_size);
        REVERB_TIME getReverbTime();
}
arma::cx_vec hilbertTransform(std::vector<double>& data, int N) {
    /*
    computes the Hilbert Transform
    data: input d data (real)
    N: size of the data
    */

    N = data.size();
    arma::vec dataArma(data);
    arma::cx_vec dataFFT = arma::fft(dataArma);

    if (N % 2 == 0) {
        for (int ii = 1; ii < N/2; ii++) {
            dataFFT(ii) *= 2;
        }

    } else {
        for (int ii = 1; ii < (N+1)/2; ii++) {
            dataFFT(ii) *= 2;
        }
    }

    arma::cx_vec dataIFFT = arma::ifft(dataFFT);

    //data = arma::conv_to<std::vector<complex<double>>>::from(arma::abs(dataIFFT));
    
    return dataIFFT;
    
}

#if 1
void movingAverage(std::vector<double>& data, int k) {
    /*
    Computes the moving average of the data, which uses a reflective padding method
    data: input data (real)
    k: size of the window
    */

    //int pad_width = k / 2;
    std::vector<double> frame;

    for (int ii = 0; ii < data.size(); ii++){
        double sum = 0;
        for (int jj = 0; jj < k; jj++) {
            sum += data[ii + jj];
        }
        frame.push_back(sum/k);
        
    }


}
void schroederIntegration(std::vector<double>& data) {

    /* 
    Computes the Schroeder Integration
    data: input data (real)
    */

    std::vector<double> cumsum;
    int sum = 0;
    for(int i = 0; i < data.size(); i++) {
        sum += data[i];
        cumsum.push_back(sum);
    }

    double max_cumsum = *std::max_element(cumsum.begin(), cumsum.end());

    for (int ii = 0; ii < data.size(); ii++) {
        data[ii] = 10 * log10(cumsum[ii] / max_cumsum);
    }



}

REVERB_TIME reverbTimeCalc(std::vector<double>& audioData, double fs, int window_size) {
    REVERB_TIME reverbTime; // struct to hold the reverb time values
    
    arma::cx_vec hilbertTransformedData = hilbertTransform(audioData, audioData.size());
    std::vector<double> envelope = arma::conv_to<std::vector<double>>::from(arma::abs(hilbertTransformedData));
    int N = 7000;

    movingAverage(envelope, window_size);

    // Schroeder Integration Proc
    std::vector<double> schIntegralInput(envelope.begin(), envelope.end() + N);
    std::reverse(schIntegralInput.begin(), schIntegralInput.end());
    std::transform(schIntegralInput.begin(), schIntegralInput.end(), schIntegralInput.begin(), [](double x) {return x*x;});
    schroederIntegration(schIntegralInput);

    // Calculate the reverb time Curve Fitting
    

    return reverbTime;

}