
[y,fs] = audioread("../../wavs/bedroom_RIR.wav");
t = 0:1/fs:(length(y)-1)/fs;

figure(1024);
plot(t,y);

rt = reverbTimeAnyl(y,fs,200,1);
printf("RT60: %f sec\n RT30: %f sec\n RT20: %f sec\n RT10: %f sec\n", rt.RT60,rt.RT30,rt.RT20,rt.RT10);
