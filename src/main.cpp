
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;


typedef struct WAV_HEADER {
    char riff[4];
    int32_t chunkSize;
    char wave[4];
    char fmt[4];
    int32_t subchunk1Size;
    int16_t audioFormat;
    int16_t numChannels;
    int32_t sampleRate;
    int32_t byteRate;
    int16_t blockAlign;
    int16_t bitsPerSample;
    char data[4];
    int32_t subchunk2Size;
} wavHeader;

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


int main(int argc, char* argv[]) {

    vector<double> audioData;


    if(argc < 2) {
        cout << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }

    wav wavFile(argv[1]);
    cout << argv[1] << "Header Info: "<<endl;
    wavFile.print();

    //audioData = vector<double>(wavFile.getData().begin(), wavFile.getData().end());


    return 0;
}