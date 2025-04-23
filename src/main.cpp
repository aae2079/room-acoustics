#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <vector>
#include "wav.hpp"
#include "rtDefs.hh"
//#include <fftw3.h>
#include <portaudio.h>
#include <armadillo>
#include "roomAcoustics.hh"
#define ARMA_USE_FFTW3
#define BUFFER_SIZE 1024
#define OVERLAP BUFFER_SIZE/2
#define SAMPLE_RATE 44100
using namespace std;

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 1024
#define NUM_CHANNELS 1
#define SAMPLE_SIZE sizeof(float)  // We are using 32-bit float for audio data
#define PA_SAMPLE_TYPE paFloat32  // PortAudio sample type for float data

bool is_recording = false;
ROOM_ACOUSTICS_CTRL *ctrlMem;

static int audioRecordingCB(const void* inputBuffer, void* outputBuffer, unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData ){
    if(is_recording){
        const float* input = static_cast<const float*>(inputBuffer);
        ctrlMem->buffer.insert(ctrlMem->buffer.end(), input, input + framesPerBuffer);
    }

    return paContinue;

}

void cleanup(){
    ctrlMem->err = Pa_StopStream(ctrlMem->stream);
    if (ctrlMem->err != paNoError) {
        std::cerr << "Error stopping stream: " << Pa_GetErrorText(ctrlMem->err) << std::endl;
    }
    ctrlMem->err = Pa_CloseStream(ctrlMem->stream);
    if (ctrlMem->err != paNoError) {
        std::cerr << "Error closing stream: " << Pa_GetErrorText(ctrlMem->err) << std::endl;
    }
    Pa_Terminate(); 
    std::cout << "PortAudio terminated." << std::endl;
    delete ctrlMem;
}

int main(int argc, char* argv[]) {
    ctrlMem = new ROOM_ACOUSTICS_CTRL;

    REVERB_TIME reverbTime;
    ReverbAnalyzer rA;

    

    ctrlMem->err = Pa_Initialize();
    if (ctrlMem->err != paNoError){
        std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(ctrlMem->err) << std::endl;
        return 1;
    }
     ctrlMem->err = Pa_OpenDefaultStream(&ctrlMem->stream,
                               NUM_CHANNELS,      // Number of input channels
                               0,                  // Number of output channels
                               PA_SAMPLE_TYPE,     // Sample format (float)
                               SAMPLE_RATE,        // Sample rate
                               FRAMES_PER_BUFFER,  // Frames per buffer
                               audioRecordingCB,      // Callback function
                               nullptr);           // User data (none in this case)

    if (ctrlMem->err != paNoError) {
        std::cerr << "Error opening stream: " << Pa_GetErrorText(ctrlMem->err) << std::endl;
        return 1;
    }

    ctrlMem->err = Pa_StartStream(ctrlMem->stream);
    if (ctrlMem->err != paNoError) {
        std::cerr << "Error starting stream: " << Pa_GetErrorText(ctrlMem->err) << std::endl;
        return 1;
    }

    char buttonInput;
    while(1){
        std::cout << "Press 'r' to start recording, 'q' to quit: ";
        std::cin >> buttonInput;
        if(buttonInput == 'r'){
            if(is_recording){
                is_recording = false;
            }else{
                is_recording = true;
            }
        }else if(buttonInput == 'q'){
            //run reverb analyzer
            double sum = 0;
            for (size_t ii = 0; ii < ctrlMem->buffer.size(); ii++) {
                sum += ctrlMem->buffer[ii];
            }
            if(sum == 0){
                std::cerr << "No audio data recorded." << std::endl;
                break;
            }
            reverbTime = rA.reverbTimeCalc(ctrlMem->buffer, (double)SAMPLE_RATE, 200);
            std::cout << "RT60: " << reverbTime.RT60 << std::endl;
            std::cout << "RT30: " << reverbTime.RT30 << std::endl;
            std::cout << "RT20: " << reverbTime.RT20 << std::endl;
            std::cout << "RT10: " << reverbTime.RT10 << std::endl;
            
            break;
        }else{
            std::cout << "Try again...";
        }
    }

    cleanup();
    
    #if _WAV_READER_
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
    #endif // _WAV_READER_
    return 0;
}
