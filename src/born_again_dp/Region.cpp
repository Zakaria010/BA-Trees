#include "Region.h"

std::vector<std::vector<double>> Region::getLocalHyperplanes(const std::vector<std::vector<double>> & hyperplanes){

    std::vector<std::vector<double>> myHyperplanes(nbFeatures);
    
	for (int k = 0; k < nbFeatures; k++){
        int i = 0; 
        int j = hyperplanes[k].size()-1;
        while(i<j){
            if(hyperplanes[k][i]<Bottom[k]){
                i++;
            }
            if(hyperplanes[k][j]>Top[k]){
                j--;
            }
        }
        if(i == 0){
            if(j == hyperplanes[k].size()-1){
                std::copy(hyperplanes[k].begin(),hyperplanes[k].end(),myHyperplanes[k].begin());
            }
            else{
                std::copy(hyperplanes[k].begin(),hyperplanes[k].begin()+ j + 2,myHyperplanes[k].begin());
            }
        }
        else{
            if(j == hyperplanes[k].size()-1){
                std::copy(hyperplanes[k].begin()+ i - 1,hyperplanes[k].end(),myHyperplanes[k].begin());
            }
            else{
                std::copy(hyperplanes[k].begin() + i - 1,hyperplanes[k].begin() + j + 2,myHyperplanes[k].begin());
            }
        }
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