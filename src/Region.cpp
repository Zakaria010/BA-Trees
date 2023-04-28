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
                std::copy(hyperplanes[k].begin(),j+2,myHyperplanes[k].begin());
            }
        }
        else{
            if(j == hyperplanes[k].size()-1){
                std::copy(i-1,hyperplanes[k].end(),myHyperplanes[k].begin());
            }
            else{
                std::copy(i-1,j+2,myHyperplanes[k].begin());
            }
        }
    }

    return myHyperplanes;

}