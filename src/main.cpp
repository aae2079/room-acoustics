
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "acousDefs.hpp"
#include "wav.hpp"
#include <fftw3.h>
#include <armadillo>
using namespace std;



class wav {
    private:
        wavHeader header;
        vector<int16_t> data;
    public:
        wav(string filename) {
            ifstream file(filename, ios::binary);
            if (file.is_open()) {
                file.read((char*)&header, sizeof(header));
                int16_t buffer;
                while (file.read((char*)&buffer, sizeof(buffer))) {
                    data.push_back(buffer);
                }
            }
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
        const vector<int16_t>& getData() {
            return data;
        }
};


void hilbertTransform(vector<double>& audioData) {
    // fftw_complex *in, *out;
    // fftw_plan p;

    // in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * audioData.size());
    // out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * audioData.size());
    // p = fftw_plan_dft_1d(audioData.size(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // fftw_execute(p);


    //arma::vec audioDataArma(audioData.size());




    // fftw_destroy_plan(p);
    // fftw_free(in);
    // fftw_free(out);

    

    
}

void schroederIntegration(vector<double>& audioData) {
    // Schroeder Integration
    // 1. Calculate the envelope of the signal
    // 2. Square the envelope
    // 3. Integrate the squared envelope

}

REVERB_TIME reverbTimeCalc(vector<double>& audioData, double fs, int window_size, int N) {
    //REVERB_TIME reverbTime; // struct to hold the reverb time values
    
    // 1. Calculate hilbert transform
    // 2. Calculate envelope
    // 3. Filter the signal to remove any noise
    // 4. Integrate using schroeder integration
    // 5. Calculate the reverb time using the schroeder integration (ie. slope of the line) 

    //return reverbTime;

}








int main(int argc, char* argv[]) {

    vector<double> audioData;


    if(argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    wav wavFile(argv[1]);
    cout << argv[1] << "Header Info: "<<endl;
    wavFile.print();

    audioData = vector<double>(wavFile.getData().begin(), wavFile.getData().end());


    return 0;
}
