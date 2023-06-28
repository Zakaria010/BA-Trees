/*MIT License

Copyright(c) 2020 Thibaut Vidal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.*/

#ifndef REGION_H
#define REGION_H

#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>

class Region{

public:
    
    // indexBottom of the region
    std::vector<int> Bottom;
    
    // indexTop of the region
    std::vector<int> Top;

    // Bottom 
    std::vector<double> BottomV;

    // Top
    std::vector<double> TopV;

    // Number of features
    int nbFeatures;

    // Region Gain
    double gain;

    //parse vector
    std::vector<double> parse_vector(std::string str, char delimiter);

    // FeatureLevel Importance
    std::vector<std::map<double,int>> hyperplaneLevelsImportance;

    // Local Hyperplanes
    std::vector<std::vector<double>> localHyperplanes;

    // Collects local hyperplane levels w.r.t the region
	std::vector<std::vector<double>> getLocalHyperplanes(const std::vector<std::vector<double>> & hyperplanes);

    // Compute Region Gain
    //int computeRegionGain();

    //getCell
    static std::vector<int> getCell(std::vector<double> v, std::vector<std::vector<double>> & hyperplanes, int nbFeatures);

    // Read a vector from a file    
    void readVector(std::ifstream &inputFile, std::vector<double>& v);
    
    //Constructor
    Region(int nbFeatures , std::ifstream & inputFile);
    Region(std::string Bottom,  std::string Top, std::vector<std::vector<double>> hyperplanes);

};
#endif