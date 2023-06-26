#include "Region.h"

std::vector<std::vector<double>> Region::getLocalHyperplanes(const std::vector<std::vector<double>> & hyperplanes){

    std::vector<std::vector<double>> myHyperplanes(nbFeatures);
    
	for (int k = 0; k < nbFeatures; k++){
        int j = hyperplanes[k].size() - 1;
        while (j >= 1 && hyperplanes[k][j-1] > TopV[k]) j--;

        int i = hyperplanes[k].size() - 1;
        while (i >= 1 && hyperplanes[k][i-1] > BottomV[k]) i--;
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

//parse vector function
std::vector<double> Region::parse_vector(std::string str, char delimiter) {
	std::vector<double> result;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(delimiter)) != std::string::npos) {
	    token = str.substr(0, pos);
	    result.push_back(std::stod(token));
	    str.erase(0, pos + 1);
	}
	result.push_back(std::stod(str));
	return result;
}	


int Region::computeRegionGain(std::vector<std::map<double,double>> hyperplanes_Importance)
{
    int G = 0;
    for (int k=0; k< nbFeatures; k++){
        for (size_t i=0; i< localHyperplanes[k].size()-1; i++){
            G = G + hyperplanes_Importance[k][localHyperplanes[k][i]];
        }
    } 
    return G;
}

std::vector<int> Region::getCell(std::vector<double> v, std::vector<std::vector<double>> & hyperplanes)
{
    std::vector<int> Cell;
    for (int k=0; k < nbFeatures;k++){
        int j = hyperplanes[k].size() - 1;
        while (j >= 1 && hyperplanes[k][j-1] > v[k]) j--;
        Cell.push_back(j);
    }
    return Cell;
}


Region::Region(int nbF ,std::ifstream & inputFile) {
    
    nbFeatures = nbF;
    readVector(inputFile, BottomV);
    readVector(inputFile, TopV);
    inputFile.close();

    if(Bottom.size() != Top.size()){
        std::cout << "Vectors don't have the same size" << std::endl;
    }

    else {
        for(int i = 0;i<Top.size();i++){
            if (TopV[i]<BottomV[i]){
                std::cout << "Undefined region, the first vector must be less or equal second vector" << std::endl;
            }
        }
    }

}
Region::Region(std::string Bot, std::string T, std::vector<std::vector<double>>  hyperplanes){
    BottomV = parse_vector(Bot,',');
    TopV = parse_vector(T,',');
    if(BottomV.size() != TopV.size()){
        std::cout << "Vectors don't have the same size" << std::endl;
    }

    else {
        for(int i = 0;i<TopV.size();i++){
            if (TopV[i]<BottomV[i]){
                std::cout << "Undefined region, the first vector must be less or equal second vector" << std::endl;
            }
        }
    }
    nbFeatures = BottomV.size();
    Bottom = getCell(BottomV, hyperplanes);
    Top = getCell(TopV, hyperplanes);
    //hyperplaneLevelsImportance = hyperplaneLevelsImportance;
    localHyperplanes = getLocalHyperplanes(hyperplanes);
    gain = 0.0;

}