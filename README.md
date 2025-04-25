# Room Acoustics Measurment (RAM)

## Overview
RAM is a project aimed to gain a comprehensive analysis of any room given it's impulse response and other inputs. At the current stage of the project the only feature we have implemented is the Reverbeation Time Analyzer which returns the reverbation time (RT60, RT30, RT20 and RT10).

## Features
- Reverb Time Analyzer

## System Requirements
- Linux
- or Mac

## Dependencies
- For algorithmic design visualization
  - Python 3.13.2 
  - GNU Octave
- g++ 
- Make
- pkg installer (homebrew (Mac), yum, etc)
- Portaudio
  1. ```brew install portaudio #mac```
  2. ```yum install portaudio #rpm linux ```


## Installation

1. Clone the repository
   ```
   git clone git@github.com:aae2079/room-acoustics.git
   ```
3. cd room-acoustics
4. Run build script
   ```
    ./buildRoomAcoustics.sh
    ```
6. Run Program
   ```
   cd bin/
   ./roomAcoustics_arm64 ../wavs/bedroom_RIR.wav
    ```
7. Feel free to record and use your own impulse response!
   
## Future Work Listed by Priority
- Working on making this available on other platforms
- Continue to optimize and test with various data points.
- Create a web application where this can run, which will allow for visualization and reporting. Includes creating a graphical user interface (preferably using TypeScript) and integrating with the backend C++ code.
- Designing the additional functionalities:
  1. Frequency Response & Room Modes
  2. Noise Floor Analyzer
    - SNR Criterium
- Utilizing machine learning for measurment optomization


