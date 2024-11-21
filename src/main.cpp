#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include "wav.hpp"
#include "rtDefs.hh"
//#include <fftw3.h>
//#include <portaudio.h>
#include <armadillo>
#define ARMA_USE_FFTW3
#define BUFFER_SIZE 1024
#define OVERLAP BUFFER_SIZE/2
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

int main(int argc, char* argv[]) {
    REVERB_TIME reverbTime;
    ReverbAnalyzer rA;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << "<Optional:Path to save data>" << std::endl;
        return 1;
    }
    if (argc == 3) {
        std::string path = argv[2];
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

    size_t bytesPerSamp = header.bitsPerSample / 8;
    size_t overlapBytes = OVERLAP * bytesPerSamp;
    std::vector<char> buffer(BUFFER_SIZE*bytesPerSamp);

    std::vector<char> overlapBuffer(overlapBytes,0);
    std::vector<double> timeArray;
    std::vector<double> audioData;

    int msgCount = 0;
    double time = 0;
    while (file.read(buffer.data(), BUFFER_SIZE*bytesPerSamp)) {
        //std::cout << "Read " << file.gcount() << " bytes." << std::endl;
        
        // Combine overlap with current buffer
        //std::memcpy(buffer.data(), overlapBuffer.data(), overlapBytes);
        
        if (bytesPerSamp == 2){// 16-bit sample
            std::vector<int16_t> audioChunk(BUFFER_SIZE);
            std::memcpy(audioChunk.data(), buffer.data(), BUFFER_SIZE * bytesPerSamp);
            //timeArray = arma::conv_to<std::vector<double>>::from(timeArray_arma);
            if(msgCount <= 39) {
                audioData.insert(audioData.end(), audioChunk.begin(), audioChunk.end());
            }  

        } 
        else if (bytesPerSamp == 4) { // 32-bit sample
            std::vector<int32_t> audioChunk(BUFFER_SIZE);
            std::memcpy(audioChunk.data(), buffer.data(), BUFFER_SIZE * bytesPerSamp);
            while (msgCount != 38) {
                audioData.insert(audioData.end(), audioChunk.begin(), audioChunk.end());
            }

        }
        time += static_cast<double>(BUFFER_SIZE)/header.sampleRate;
        msgCount++;
    
    }
    //Normalize audio data
    double max = *std::max_element(audioData.begin(), audioData.end());
    for (size_t ii = 0; ii < audioData.size(); ii++) {
        audioData[ii] /= max;
    }

    reverbTime = rA.reverbTimeCalc(audioData, header.sampleRate, 200);
    std::cout << "RT60: " << reverbTime.RT60 << std::endl;
    std::cout << "RT30: " << reverbTime.RT30 << std::endl;
    std::cout << "RT20: " << reverbTime.RT20 << std::endl;
    std::cout << "RT10: " << reverbTime.RT10 << std::endl;

    if (file.gcount() > 0) {
        std::cout << "Read " << file.gcount() << " bytes (final block)." << std::endl;
        
        // Process remaining buffer here
        msgCount++;
    }

    std::cout<< "Number of messages: " << msgCount << std::endl;
    file.close();

    return 0;
}
