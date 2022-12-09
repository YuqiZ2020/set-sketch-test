function [Xm_inv] = estimate_n(registers, a, b, m)
%ESTIMATE_N Estimate the cardinality from the registers
Xm = 0;
for i = 1:length(registers)
    Xm = Xm + a * b^(-registers(i));
end
Xm = log(b) / (1 - 1/b) / m * Xm;
Xm_inv = 1 / Xm;
end