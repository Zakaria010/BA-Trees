#include "Commandline.h"
#include "Params.h"
#include "RandomForest.h"
#include "BornAgainDecisionTree.h"
#include "Region.h"

int main(int argc, char** argv)
{
	Commandline c(argc, argv);
	if (c.command_ok)
	{
		std::ifstream inputFile(c.instance_name.c_str());
		if (inputFile.is_open())
		{
			/* READING INPUT RANDOM FOREST */
			std::cout << "----- READING RANDOM FOREST from " << c.instance_name << std::endl;
			Params params(inputFile, c.nbTrees, c.objectiveFunction,c.seed);
			RandomForest randomForest(&params, inputFile);

			bool useRegion = !c.instance.empty();
			if(useRegion){ 
				/* CONSTRUCTING THE BORN-AGAIN TREE */
				Region region(c.instance,c.instance,randomForest.getHyperplanes());
				params.startTime = clock();
				BornAgainDecisionTree bornAgainTree(&params, &randomForest);
				double gain;
				if (c.method == 1) 
					gain = bornAgainTree.computeBestRegion(&region,c.depth);
				else 
					gain = bornAgainTree.computeSecondRegion(&region,c.depth);
				params.stopTime = clock();
				std::cout<<"gain : "<<gain<<std::endl;
				
				/* EXPORTING STATISTICS AND RESULTS */
				bornAgainTree.displayRunStatistics();
				bornAgainTree.exportRunStatistics(c.output_name + ".out");
				bornAgainTree.exportRegionBATree(c.output_name + ".tree", &region);
			}
			
			else{
				/* CONSTRUCTING THE BORN-AGAIN TREE */
				params.startTime = clock();
				BornAgainDecisionTree bornAgainTree(&params, &randomForest);
				if (c.objectiveFunction == 0 || c.objectiveFunction == 1 || c.objectiveFunction == 2)
					if(!c.region_file.empty()){
						std::ifstream regionFile(c.region_file.c_str());
						if(regionFile.is_open()){
							Region region(params.nbFeatures, regionFile);
							bornAgainTree.buildRegionOptimal(&region);
						}					
					}
					else{
						bornAgainTree.buildOptimal();
					}
				else if (c.objectiveFunction == 4)
					bornAgainTree.buildHeuristic();
				params.stopTime = clock();
				
				/* EXPORTING STATISTICS AND RESULTS */
				bornAgainTree.displayRunStatistics();
				bornAgainTree.exportRunStatistics(c.output_name + ".out");
				bornAgainTree.exportBATree(c.output_name + ".tree");
			}
			
			
		}
		else
		{
			std::cout << "----- IMPOSSIBLE TO READ RANDOM FOREST from: " << c.instance_name << std::endl;
		}
	}
	else
	{
		std::cout << "----- INCORRECT COMMANDLINE " << std::endl;
	}
	std::cout << "----- END OF ALGORITHM " << std::endl << std::endl;
	return 0;
}
