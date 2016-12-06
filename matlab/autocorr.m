function y = autocorr(arr, lag)
    y = 0;
    n = length(arr);
    for i = 1+lag : n
        y = y + arr(i) * arr(i-lag);
    end
end