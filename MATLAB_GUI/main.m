folder_path = fullfile('D:', 'JHU', 'Fall2022', '601.447Computational Genomics_Sequences', 'project', 'data');
file_path1 = fullfile(folder_path, 'sarscov2_alpha.fa.txt');
file_path2 = fullfile(folder_path, 'sarscov2_wuhan.fa.txt');

% hyperparameter
k = 10; % k-mer
m = 4096; % number of registers
b = 1.001;
a = 20;
q = 2^16-2;

% Assume that each line has the same number of bases, except for the last
% row
% Assume that each line has at least 16 bases.
% Assume the k in kmer is in the range of [1, 16].

% construct the registers
[register1] = construct_register(file_path1, a, b, q, m, k); %u
[register2] = construct_register(file_path2, a, b, q, m, k); %v

% estimate the cardinality of the sets of kmers
n1 = estimate_n(register1, a, b, m); %u
n2 = estimate_n(register2, a, b, m); %v

% estimate the Jaccard similarity
u = n1 / (n1+n2);
v = n2 / (n1+n2);
D_plus = sum(register1>register2);
D_minus = sum(register1<register2);
D_0 = sum(register1==register2);
J_max = min([u/v, v/u]);

% J = optimvar('J', 'LowerBound',0,'UpperBound',J_max);
% obj = objfunx;
% prob = optimproblem('Objective',obj, 'ObjectiveSense','max');
% sol = solve(prob);
% 
% function pb = pbfunx(x, b)
% pb = -log(1 - x * (b-1)/b) / log(b);
% end
% 
% function f = objfunx(J)
% f = D_plus * log(pbfunx(u-v*J, b)) + D_minus * log(pbfunx(v-u*J, b)) + D_0 * log(1 - pbfunx(u-v*J, b) - pbfunx(v-u*J, b));
% end


