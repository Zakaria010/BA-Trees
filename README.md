
# Born-Again Tree Ensembles

This repository contains the source code and data associated to the paper "Born Again Tree Ensembles", by Thibaut Vidal and Maximilian Schiffer, with the help of Toni Pacheco who contributed during the paper submission and revision period. This paper has been presented at the 37th International Conference on Machine Learning (ICML 2020).

## Test Environment

This code has been tested on Ubuntu 18.04 using GCC compiler v9.2.0 for the C++ algorithm, as well as on Windows 10 using MinGW or Visual Studio 2017 for compilation. 
We used Anaconda distribution with Python 3.7 for all other codes and scripts.
We recommend using a similar configuration to avoid any possible compilation issue.

## Folder Structure

The repository contains the following folders:

docs<br>
src<br>      |-------born_again_dp<br>     |-------resources<br>

### docs:

* The docs folder contains the pdf files of paper (born-again.pdf) and its supplementary material (born-again-supplementary.pdf).
* This folder also contains a jupyter notebook (illustrative_example.ipynb) containing a working example of the code pipeline, including some visualization and evaluation scripts.
* The file requirements.txt contains the list of packages that must be installed to run the notebook. If a package is missing, you can easily add it via pip or anaconda using either <em>pip install "package"</em> or <em>conda install "package"</em>.
* Important: Graphviz installation sometimes fails to correctly update the "PATH" variable in the system. To circumvent this issue, we recommend to install this package via anaconda using <em>conda install python-graphviz</em> 

#### src\born_again_dp:

* This folder contains the C++ implementation of the optimal and heuristic BA-Tree algorithms.
* This folder contains also the C++ implementation of optimal and heuristic local explanation using BA-Tree algorithm.
* This folder also contains a bash script (runAllDatasets.sh) which can be executed to run the algorithm on all datasets, folds, for all objective functions, and considering a different number of trees as input. The results of this script are stored in the folder src\output. Due to the number of datasets and tests, this experiment requires some CPU time (approximately 24h).
* This folder also contains a bash script (runOptimal.sh) which can be executed to run the optimal algorithm on all datasets for some given instance and depth, folds, for depth objective functions. The results of this script are stored in the folder src\outputDeter. Due to the number of datasets and tests, this experiment requires some CPU time.
* This folder also contains a bash script (runHeuristic.sh) which can be executed to run the Heuristic approach on all datasets for some given instance and depth, folds, for depth objective functions. The results of this script are stored in the folder src\outputHeuristic. Due to the number of datasets and tests, this experiment requires some CPU time.

#### src\resources

* This folder contains two subfolders, one (datasets) contains the original data sets as well as the training data and test data subsets created by ten-fold cross validiation. 
* The other (forests) contains the random forests which have been generated with scikit-learn v0.22.1 from these data sets. These forests are used as input to the BA-trees algorithms.

## Installation and Usage

The code, located in `src\born_again_dp`, can be built by simply calling the <em>make</em> command.
This requires the availability of the g++ compiler.<br> 

![Getting Started GIF](docs/Getting-Started.gif)

By default, the simple makefile provided in this project does not link with CPLEX to facilitate installation and portability.
As a consequence, the call to the MIP solver to prove faithfulness of a region in the heuristic BA-tree is deactivated (USING_CPLEX = false).
To compile with CPLEX and guarantee faithfulness in the heuristic, make sure that CPLEX is installed in your system, adapt the makefile with the correct library path, and run the command "make withCPLEX=1".

### Using the C++ algorithm

After compilation, the executable can be directly run on any input file representing a tree ensemble for a giving instance picked from the dataset with the following command line:

```
Usage:
   ./bornAgain input_ensemble_path output_BAtree_path -obj 0 -instance instance [list of options]
Available options:
  -depth X	     Number used to control the maximal depth of the resulting tree
  -method X      Method used in the algorithm: 1 = Optimal ; 0 = Heuristic (defaults to 0)
```
Examples: <br>
`./bornAgain ../resources/forests/FICO/FICO.RF1.txt new_method -obj 0 -depth 9 -instance 1.0,1.0,1.0,0.0,0.0,1.0,0.0,0.0,0.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0 -method 1`<br>
`./bornAgain ../resources/forests/Breast-Cancer-Wisconsin/Breast-Cancer-Wisconsin.RF1.txt new_method -obj 0 -depth 5 -instance 5.0,10.0,6.0,1.0,10.0,4.0,4.0,10.0,10.0`

## Contributing

If you wish to contribute to this project, e.g;, to the code, portability or integration, we encourage you to contact us by email:<br> ``vidalt AT inf.puc-rio.br``

## Team

Contributors to this code:
* <a href="https://github.com/vidalt" target="_blank">`Thibaut Vidal`</a>
* <a href="https://github.com/youssouf1994" target="_blank">`Youssouf Emine`</a>
* <a href="https://github.com/Zakaria010" target="_blank">`Zakaria El Kassimi`</a>

## License

[![License](http://img.shields.io/:license-mit-blue.svg?style=flat-square)](http://badges.mit-license.org)

- **[MIT license](http://opensource.org/licenses/mit-license.php)**
