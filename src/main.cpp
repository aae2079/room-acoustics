#include <iostream>
#include <fstream>
#include <cstdio>
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

REVERB_TIME reverbTimeCalc(std::vector<double>& audioData, std::vector<double>& buffer, double fs, int window_size) {
    REVERB_TIME reverbTime; // struct to hold the reverb time values
    
    //buffer.insert(audioData.begin(), audioData.end());

    if(buffer.size() >= 19900) {
        // Hilbert Transform
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
 
    }

    

    return reverbTime;

}

#endif

    

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }


    /*
    Read the WAV file
    */
    std::ifstream file(argv[1], std::ios::binary);
    
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return -1;
    }
    
    wavHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    if (header.riff[0] != 'R' || header.riff[1] != 'I' ||
        header.riff[2] != 'F' || header.riff[3] != 'F') {
        std::cerr << "Invalid WAV file format: " << argv[1] << std::endl;
        return -1;
    }

    const size_t bytes_per_sample = header.bitsPerSample / 8;
    std::vector<char> buffer(BUFFER_SIZE*bytes_per_sample);

    int msgCount = 1;

    while (file.read(buffer.data(), BUFFER_SIZE*bytes_per_sample)) {
        std::cout << "Read " << file.gcount() << " bytes." << std::endl;
        
        
        if (bytes_per_sample == 2) { // 16-bit sample
            std::vector<int16_t> audioChunk(BUFFER_SIZE);
            std::memcpy(audioChunk.data(), buffer.data(), BUFFER_SIZE * bytes_per_sample);
            int a = 0;

        } else if (bytes_per_sample == 4) { // 32-bit sample
            int32_t sample = *reinterpret_cast<int32_t*>(buffer.data());
            std::cout << "First sample: " << sample << std::endl;
        }
        msgCount++;
    }

    if (file.gcount() > 0) {
        std::cout << "Read " << file.gcount() << " bytes (final block)." << std::endl;
        
        // Process remaining buffer here
        msgCount++;
    }

    std::cout<< "Number of messages: " << msgCount << std::endl;
    file.close();





    return 0;
}