n = length(src);
a = zeros(1,n-1);
samplingrate = 48000; % Hz

parfor i = 1:n-1
    a(i) = autocorr(src, i);
end
% 
% fou_f = fft(src);
% s_f = fou_f .* conj(fou_f);
% r_f = ifft(s_f);

%  DOI: 10.1109/89.876309 
% Enhanced Standard Auto Correlation Function

a_clip = a; % Bruteforce autocorrelation
% a_clip = r_f; % FFT autocorrelation
a_clip(a_clip<0) = 0;
a_expanded = zeros(1,2*length(a_clip)-1);
l_a_expanded = length(a_expanded);

for i = 1:l_a_expanded
    if (mod(i,2) == 1)
        a_expanded(i) = a_clip((i+1)/2);
    end
end

for i = 1:l_a_expanded
    if (mod(i,2) == 0)
        a_expanded(i) = (a_expanded(i-1) + a_expanded(i+1))/2;
    end
end

parfor i = 1:length(a_clip)
    a_clip(i) = a_clip(i) - a_expanded(i);
end

a_clip(a_clip<0) = 0;

% engine rpm limits
ulim = samplingrate * 0.12; % 500rpm
llim = samplingrate * 0.005; % 11000rpm
% lim_a = a(llim:ulim);
% [maxval, maxidx] = max(lim_a);
lim_a = a_clip(llim:ulim);
[maxval, maxidx] = max(lim_a);
f_freq = samplingrate / (llim + maxidx - 1);

[esacf_maxval, esacf_maxidx] = max(a_clip); 
esacf_f_freq = 48000 / esacf_maxidx;