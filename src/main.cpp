#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "acousDefs.hpp"
#include "wav.hpp"
#include <fftw3.h>
#include <portaudio.h>
#include <armadillo>
#define ARMA_USE_FFTW3
#define BUFFER_SIZE 1024
using namespace std;


#if 0
class wav {
    private:
        wavHeader header;
        vector<double> data;
    public:
        wav(string filename) {
            ifstream file(filename, ios::binary);
            if(!file.is_open()) {
                cout << "Error: Could not open file " << filename << endl;
                return;
            }
            file.read((char*)&header, sizeof(wavHeader));
            
            //Read the data into a buffer
            std::vector<char> buffer(BUFFER_SIZE);
            while(file.read(buffer.data(), buffer.size())) {
                for(int i = 0; i < BUFFER_SIZE; i++) {
                    data.push_back((double)buffer[i]);
                }
                std::streamsize bytesRead = file.gcount();
                if(bytesRead == 0){
                    break;
                }
            }
            file.close();
            
        }
        void print() {
            cout << "riff: " << header.riff[0] << header.riff[1] << header.riff[2] << header.riff[3] << endl;
            cout << "chunkSize: " << header.chunkSize << endl;
            cout << "wave: " << header.wave[0] << header.wave[1] << header.wave[2] << header.wave[3] << endl;
            cout << "fmt: " << header.fmt[0] << header.fmt[1] << header.fmt[2] << header.fmt[3] << endl;
            cout << "subchunk1Size: " << header.subchunk1Size << endl;
            cout << "audioFormat: " << header.audioFormat << endl;
            cout << "numChannels: " << header.numChannels << endl;
            cout << "sampleRate: " << header.sampleRate << endl;
            cout << "byteRate: " << header.byteRate << endl;
            cout << "blockAlign: " << header.blockAlign << endl;
            cout << "bitsPerSample: " << header.bitsPerSample << endl;
            cout << "data: " << header.data[0] << header.data[1] << header.data[2] << header.data[3] << endl;
            cout << "subchunk2Size: " << header.subchunk2Size << endl;
        }
        const vector<double>& getData() {
            return data;
        }
};
#endif

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


#endif 

int main(int argc, char* argv[]) {

    wavHeader header;
    vector<double>workingBuffer;
    vector<double>audioData;
    //vector<double
    if(argc < 2) {
        cout << "Usage: " << argv[0] << " <wav filename>" << endl;
        return 1;
    }

    string filename;
    filename = argv[1];

    // wav wavFile(argv[1]);
    // cout << argv[1] << "Header Info: "<<endl;
    // wavFile.print();
    // //audioData = vector<double>(wavFile.getData().begin(), wavFile.getData().end());
    // audioData = wavFile.getData();


    //hilbertTransform(audioData);

    ifstream file(filename, ios::binary);
    if(!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return -1;
    }

    //read the header
    file.read((char*)&header, sizeof(header));
    
    //Read the data into a buffer
    std::vector<char> buffer(BUFFER_SIZE);
    // u_int16_t bytesPerSample = header.bitsPerSample / 8;
    // u_int32_t numSamples = header.subchunk2Size / bytesPerSample;
    const int overlap = BUFFER_SIZE / 4;


    while(file.read(buffer.data(), buffer.size())) {
        for(int i = 0; i < BUFFER_SIZE; i++) {
            workingBuffer.push_back(buffer[i]);
        }
        hilbertTransform(workingBuffer, workingBuffer.size());
        std::streamsize bytesRead = file.gcount();
        if(bytesRead == 0){
            break;
        }
    }
    file.close();




    return 0;
}