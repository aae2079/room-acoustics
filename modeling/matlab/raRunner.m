
[y,fs] = audioread("../../wavs/bedroom_RIR.wav");
bufferSize = 1024;
t = 0:1/fs:(length(y)-1)/fs;

figure(1024);
plot(t,y);

rt = reverbTimeAnyl(y,fs,200);
printf("RT60: %f\n RT30: %f\n: RT20: %f\n RT10: %f\n", rt.RT60,rt.RT30,rt.RT20,rt.RT10);
