import numpy as np
import functions.py
import matplotlib.pyplot as plt
import librosa as lr
import librosa.display as ld
import scipy.signal as sc
import matplotlib.colors as mcolors
from scipy.optimize import curve_fit

def reverbTimeCalc(signal, int window_size, int N):
    
    x_anyl = abs(sc.hilbert(x))/np.max(abs(x)) #Analytical signal removes any phase information
    x_log = 20.0*np.log10(x_anyl) #Take the log to get the power singal

    ma_filter = np.ones(window_size) / window_size
    x_ma = np.convolve(x_anyl, ma_filter, mode='same') #Filter the signal to remove noise

    x_ma1 = x_ma[:N]

    sch = 10.0 * np.log10(np.cumsum(x_ma1[::-1]**2)[::-1] / np.max(np.cumsum(x_ma1[::-1]**2))) #Integrate using Schroders integral
    
    ##ANALYSIS
    popt,pconv = curve_fit(func,t[:N],sch)
    m,b = popt
    y = m * t + b
    
    reverbTimeArr = [-60/m,-30/m,-20/m,-10/m]
    
    return reverbTimeArr

