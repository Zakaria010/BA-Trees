#include "Region.h"

std::vector<std::vector<double>> Region::getLocalHyperplanes(const std::vector<std::vector<double>> & hyperplanes){

    std::vector<std::vector<double>> myHyperplanes(nbFeatures);
    
	for (int k = 0; k < nbFeatures; k++){
        int j = hyperplanes[k].size() - 1;
        while (j >= 1 && hyperplanes[k][j-1] > Top[k]) j--;

        int i = hyperplanes[k].size() - 1;
        while (i >= 1 && hyperplanes[k][i-1] > Bottom[k]) i--;
        myHyperplanes[k] = std::vector<double>(hyperplanes[k].begin() + i, hyperplanes[k].begin() + j + 1);
    }

    return myHyperplanes;

}

void Region::readVector(std::ifstream &inputFile, std::vector<double>& v) {
    std::string line;
    if (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, ',')) {
            v.push_back(std::stoi(token));
        }
    }
}


Region::Region(int nbF ,std::ifstream & inputFile) {
    
    nbFeatures = nbF;
    readVector(inputFile, Bottom);
    readVector(inputFile, Top);
    inputFile.close();

    if(Bottom.size() != Top.size()){
        std::cout << "Vectors don't have the same size" << std::endl;
    }

    else {
        for(int i = 0;i<Top.size();i++){
            if (Top[i]<Bottom[i]){
                std::cout << "Undefined region, the first vector must be less or equal second vector" << std::endl;
            }
        }
    }

}