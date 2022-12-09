function [register] = construct_register(file_path, a, b, q, m, k)
%CONSTRUCT_REGISTER 
%Args:
%   file_path: a character array to the fasqq file
%   a, b, q, m: hyperparameters
%Returns:
%   register values in a 1 * m integer array

register = zeros(1, m); % initializa registers
k_low = 0;
w = 0;

fid = fopen(file_path);
tline = fgetl(fid); % ignore the first row
tline = fgetl(fid);
base_per_row = length(tline);
inputs = zeros(1, base_per_row + k - 1); % store the input
is_first_row = true;
while ischar(tline)
    curr_length = length(tline);
    inputs(k:k+curr_length-1) = tline;
    if is_first_row
        start = k;
        is_first_row = false;
    else
        start = 1;
    end
    for start_pos = start:curr_length
        kmer = inputs(start_pos:(start_pos+k-1));
        curr_seed = kmer_to_int(kmer);
        [register, k_low, w] = update_register(register, k_low, w, a, b, q, m, curr_seed);
    end
    inputs(1:k-1) = inputs(end-k+2:end);
    % read the next line
    tline = fgetl(fid);
end
fclose(fid);

end