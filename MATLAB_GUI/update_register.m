function [register, k_low, w] = update_register(register, k_low, w, a, b, q, m, seed)
%UPDATE_REGISTER Update the register according to a new sample with its
%corresponding seed.
rng(seed);
x = 0;
random_shuffle = randperm(m);
for j = 1:m
    x = x + random('Exponential', a) / (m+1-j);
    if x > b^(-k_low)
        break
    end
    k = max([0, min([q+1, floor(1 - log(x) / log(b))])]);
    if k <= k_low
        break
    end
    i = random_shuffle(j);
    if k > register(i)
        register(i) = k;
        w = w + 1;
        if w >= m
            k_low = min(register);
            w = 0;
        end
    end
end
end