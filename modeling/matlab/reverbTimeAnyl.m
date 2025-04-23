%{
    The reverbTimeAnalyzer function calculates the respecrtive reverb times
    [rt60,rt30,rt20,rt10] of the input to an acousitc signal, this servces the purpose
    to determing the sound quality of a room. It uses the MR Schroeder inegration method.
    
    params
    ------
    audioData: the audio signal
    fs: the sampling frequency
    window_size: the window size for the moving average

    returns
    -------
    reverbTime: a struct containing the reverb time values for 60 dB, 30 dB, 20 dB, and 10 dB}
%}

function reverbTime = reverbTimeCalc(audioData, fs, window_size)
    % Initialize return structure
    reverbTime = struct('RT60', NaN, 'RT30', NaN, 'RT20', NaN, 'RT10', NaN);

    % Hilbert Transform
    hilbertTransformedData = hilbert(audioData);
    envelope = abs(hilbertTransformedData);

    % Moving Average
    envelope = movingAverage(envelope, window_size);

    % Debugging: Convert envelope to dB
    debug = 20 * log10(envelope);

    % Find the point where the average falls below threshold
    CHANGE_RATE_TH = 0.0008;
    ii = 1;
    sumVal = 0;
    avg = 0;
    while ii <= numel(envelope) - 1023
        sumVal = sum(envelope(ii:ii+1023));
        avg = sumVal / 1024;
        if avg < CHANGE_RATE_TH
            break;
        end
        ii = ii + 1024;
    end

    % Calculate integral size
    integralSize = ii;

    % Schroeder Integration
    xSch = schroederIntegrral(flip(envelope(1:integralSize))^2);

    % Find the slope of the curve
    f = polyfit((1:integralSize), xSch, 1);
    slope = f(1);
    b = f(2);

    % Calculate RT values
    reverbTime.RT60 = -60 / slope;
    reverbTime.RT30 = -30 / slope;
    reverbTime.RT20 = -20 / slope;
    reverbTime.RT10 = -10 / slope;
end

function y = movingAverage(x,k)
    %{
        params:
            x: input signal
            k: window size
        returns:
            y: moving average of x
    %}

    pad_width = floor(k/2);
    x_pad = padarray(x,pad_width,'replicate','both');

    y = [];
    for ii = 1:length(x)
        y = [y, mean(x_pad(ii:ii+k-1))];
    end
    
end

function sch = schroederIntegrral(signal)
    %{
        This function implements M.R. Schroeders reverb time integral. This method modernizes
        the constant need of averaging over many reverb curve collections. This methods uses
        the cumulative sum.
        
        params
        ------
        signal

        returns
        -------
        sch: the M.R. Schroeder reverb time integral of the signal
    %}

    cum_sum = cumsum(signal);
    sch = 10.0 * log10(flip(cum_sum) / max(cum_sum));
end

function y = hilbert(x)
    cx = fft(x);
    N = length(cx);
    if(mod(N,2) == 0)
        for ii = 1:N/2
            cx(ii) = cx(ii)*2;
        end
    else
        for ii = 1:(N+1)/2
            cx(ii) = cx(ii)*2;
        end
    end

    y = ifft(cx);
end