#include "Region.h"

std::vector<std::vector<double>> Region::getLocalHyperplanes(const std::vector<std::vector<double>> & hyperplanes){

    std::vector<std::vector<double>> myHyperplanes(nbFeatures);
    
	for (int k = 0; k < nbFeatures; k++){
        if(Top[k] == Bottom[k]){
            int l =0;
            while(l<hyperplanes[k].size()) {
                if(hyperplanes[k][l]<Bottom[k]){
                    l++;
                }
                else{
                    myHyperplanes[k].push_back(hyperplanes[k][l]);
                    break;
                }
            }
        }
        else{
            int i = 0; 
            int j = hyperplanes[k].size()-1;
            while(i<j){
                if(hyperplanes[k][i]<Bottom[k]){
                    i++;
                }
                if(hyperplanes[k][j]>Top[k]){
                    j--;
                }
                if((hyperplanes[k][i]>=Bottom[k]) && (hyperplanes[k][j]<=Top[k])){
                    break;
                }
            }
            if(i == 0){
                if(j == hyperplanes[k].size()-1){
                    std::copy(hyperplanes[k].begin(), hyperplanes[k].end(), std::back_inserter(myHyperplanes[k]));
                }
                else{
                    std::copy(hyperplanes[k].begin(), hyperplanes[k].begin()+ j + 2, std::back_inserter(myHyperplanes[k]));
                
                }
            }
            else{
                if(j == hyperplanes[k].size()-1){
                    std::copy(hyperplanes[k].begin()+ i - 1, hyperplanes[k].end(), std::back_inserter(myHyperplanes[k]));
                }
                else{
                    std::copy(hyperplanes[k].begin() + i - 1, hyperplanes[k].begin() + j + 2, std::back_inserter(myHyperplanes[k]));
                }
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