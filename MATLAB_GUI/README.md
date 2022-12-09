# MATLAB GUI for SetSketch

This is a MATLAB Graphical User Interface (GUI) that can run SetKetch. An simplified MATLAB version of SetSketch algorithm is implemented. The Jaccard estimation results will be compared with true values for accuracy assessment.


The GUI has an intuitive workflow with the key steps shown in the `Workflow` folder. The main steps include selecting input files through an pop-up window, setting the parameters, running the algirthm, and examing the displayed results. User-friendly features, such as an manually-implemented progress bar and easily adjustable parameters, are also involved.


The original algorithm by Otmar Ertl was presented in the paper ["SetSketch: Filling the Gap between MinHash and HyperLogLog"](http://vldb.org/pvldb/vol14/p2244-ertl.pdf) which was accepted at [VLDB 2021](https://vldb.org/2021/). An [extended paper version](https://arxiv.org/abs/2101.00314) that includes mathematical proofs and additional results is available on arXiv. The author's [original implementation](https://github.com/dynatrace-research/set-sketch-paper) in C++ and additional Python visualization tools is available on Github.
