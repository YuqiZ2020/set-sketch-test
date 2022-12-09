function [kmer_int] = kmer_to_int(kmer)
%KMER_TO_INT View A, C, G, T as 0, 1, 01, 11 in base 2. Turn kmer into an
%integer of 32 bits. The last two bits corresponds to the last base, etc.
%Assume that the kmer has fewer than 16 bases.
%Args:
%   kmer: a character array of sequences of A, T, G, and C
%Returns:
%   encoded integer of base 10
kmer_int = 0;
for i = 1:length(kmer)
    kmer_int = kmer_int * 4;
    switch kmer(i)
        case 'C'
            kmer_int = kmer_int + 1;
        case 'G'
            kmer_int = kmer_int + 2;
        case 'T'
            kmer_int = kmer_int + 3;
    end
end
end