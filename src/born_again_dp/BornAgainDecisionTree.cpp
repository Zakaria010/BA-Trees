#include "BornAgainDecisionTree.h"

unsigned int BornAgainDecisionTree::dynamicProgrammingOptimizeDepth(int indexBottom, int indexTop)
{
	iterationsDP++;
	if (indexBottom == indexTop) return 0;

	int hash = fspaceFinal.keyToHash(indexBottom, indexTop);
	if (regions[indexBottom][hash] != UINT_MAX) return regions[indexBottom][hash];

	unsigned int bestLB = 0;
	unsigned int bestUB = UINT_MAX;
	for (int k = 0; k < params->nbFeatures && bestLB < bestUB; k++)
	{
		const int codeBookValue = fspaceFinal.codeBook[k];
		const int rangeLow = fspaceFinal.keyToCell(indexBottom, k);
		const int rangeUp = fspaceFinal.keyToCell(indexTop, k);
		int tempRangeLow = rangeLow;
		int tempRangeUp = rangeUp;
		while (tempRangeLow < tempRangeUp && bestLB < bestUB)
		{
			int l = (tempRangeLow+tempRangeUp) / 2;
			unsigned int leftResult = dynamicProgrammingOptimizeDepth(indexBottom, indexTop + codeBookValue * (l - rangeUp)); // Index of the new corner: z^R + e_j(l-z^R_j)
			if (leftResult > bestLB) bestLB = leftResult;

			// Small code optimization: We can stop this recursion earlier for large values of leftResult
			if (1 + leftResult >= bestUB) 
				tempRangeUp = l;
			else
			{
				unsigned int rightResult = dynamicProgrammingOptimizeDepth(indexBottom + codeBookValue * (l + 1 - rangeLow), indexTop); // Index of the new corner: z^L + e_j(l+1-z^L_j)
				if (leftResult == 0 && rightResult == 0) // Base case has been attained
				{
					if (fspaceFinal.cells[indexBottom] == fspaceFinal.cells[indexTop])
					{
						regions[indexBottom][hash] = 0;
						regionsMemorizedDP++;
						return 0;
					}
					else
					{
						regions[indexBottom][hash] = 1;
						regionsMemorizedDP++;
						return 1;
					}
				}
				if (rightResult > bestLB)     bestLB = rightResult;
				if (1 + rightResult < bestUB) bestUB = 1 + std::max<unsigned int>(leftResult, rightResult);
				if (1 + leftResult  >= bestUB) tempRangeUp = l; // Left result will only increase to the right (Theorem 4)
				if (1 + rightResult >= bestUB) tempRangeLow = l + 1; // Right result will only increase to the left (Theorem 4)
			}
		}
	}
	regions[indexBottom][hash] = bestUB;
	regionsMemorizedDP++;
	return bestUB;
}

unsigned int BornAgainDecisionTree::dynamicProgrammingOptimizeNbSplits(int indexBottom, int indexTop)
{
	iterationsDP++;
	if (indexBottom == indexTop) return 0;

	int hash = fspaceFinal.keyToHash(indexBottom, indexTop);
	if (regions[indexBottom][hash] != UINT_MAX) return regions[indexBottom][hash];

	unsigned int bestLB = 0;
	unsigned int bestUB = UINT_MAX;
	for (int k = 0; k < params->nbFeatures && bestLB < bestUB; k++)
	{
		const int codeBookValue = fspaceFinal.codeBook[k];
		const int rangeLow = fspaceFinal.keyToCell(indexBottom, k);
		const int rangeUp = fspaceFinal.keyToCell(indexTop, k);
		for (int l = rangeLow ; l < rangeUp && bestLB < bestUB; l++)
		{
			unsigned int leftResult = dynamicProgrammingOptimizeNbSplits(indexBottom, indexTop + codeBookValue * (l - rangeUp));          // Index of the new corner: z^R + e_j(l-z^R_j)
			unsigned int rightResult = dynamicProgrammingOptimizeNbSplits(indexBottom + codeBookValue * (l + 1 - rangeLow), indexTop);    // Index of the new corner: z^L + e_j(l+1-z^L_j)
			if (leftResult == 0 && rightResult == 0) // Base case has been attained
			{
				if (fspaceFinal.cells[indexBottom] == fspaceFinal.cells[indexTop])
				{
					regions[indexBottom][hash] = 0 ;
					regionsMemorizedDP++;
					return 0;
				}
				else
				{
					regions[indexBottom][hash] = 1 ;
					regionsMemorizedDP++;
					return 1;
				}
			}
			else
			{
				if (leftResult  > bestLB) bestLB = leftResult;
				if (rightResult > bestLB) bestLB = rightResult;
				if (1 + rightResult + leftResult < bestUB) bestUB = 1 + leftResult + rightResult;
			}
		}
	}
	regions[indexBottom][hash] = bestUB ;
	regionsMemorizedDP++;
	return bestUB;
}

unsigned int BornAgainDecisionTree::dynamicProgrammingOptimizeDepthThenNbSplits(int indexBottom, int indexTop)
{
	iterationsDP++;
	if (indexBottom == indexTop) return 0;

	int hash = fspaceFinal.keyToHash(indexBottom, indexTop);
	if (regions[indexBottom][hash] != UINT_MAX) return regions[indexBottom][hash];

	unsigned int bestLB = 0;
	unsigned int bestUB = UINT_MAX;
	for (int k = 0; k < params->nbFeatures && bestLB < bestUB; k++)
	{
		const int codeBookValue = fspaceFinal.codeBook[k];
		const int rangeLow = fspaceFinal.keyToCell(indexBottom, k);
		const int rangeUp = fspaceFinal.keyToCell(indexTop, k);
		for (int l = rangeLow; l < rangeUp && bestLB < bestUB; l++)
		{
			unsigned int leftResult = dynamicProgrammingOptimizeDepthThenNbSplits(indexBottom, indexTop + codeBookValue * (l - rangeUp));          // Index of the new corner: z^R + e_j(l-z^R_j)
			unsigned int rightResult = dynamicProgrammingOptimizeDepthThenNbSplits(indexBottom + codeBookValue * (l + 1 - rangeLow), indexTop);    // Index of the new corner: z^L + e_j(l+1-z^L_j)
			if (leftResult == 0 && rightResult == 0) // Base case has been attained
			{
				if (fspaceFinal.cells[indexBottom] == fspaceFinal.cells[indexTop])
				{
					regions[indexBottom][hash] = 0 ;
					regionsMemorizedDP++;
					return 0;
				}
				else
				{
					regions[indexBottom][hash] = BIG_M + 1;
					regionsMemorizedDP++;
					return BIG_M + 1;
				}
			}
			else
			{
				if (leftResult > bestLB)  bestLB = leftResult;
				if (rightResult > bestLB) bestLB = rightResult;
				unsigned int newResult = BIG_M + 1 + BIG_M * std::max<unsigned int>(leftResult/BIG_M,rightResult/BIG_M) + leftResult % BIG_M + rightResult % BIG_M;
				if (newResult < bestUB) bestUB = newResult;
			}
		}
	}
	regions[indexBottom][hash] = bestUB;
	regionsMemorizedDP++;
	return bestUB;
}

int BornAgainDecisionTree::collectResultDP(int indexBottom, int indexTop, unsigned int optValue, unsigned int currentDepth)
{
	if (optValue == 0) 
	{
		finalLeaves++;
		if (currentDepth > finalDepth) finalDepth = currentDepth;
		rebornTree.push_back(Node());
		int nodeID = (int)rebornTree.size()-1;
		rebornTree[nodeID].nodeType = Node::NODE_LEAF;
		rebornTree[nodeID].splitFeature = -1;
		rebornTree[nodeID].splitValue = -1;
		rebornTree[nodeID].classification = fspaceFinal.cells[indexBottom];
		rebornTree[nodeID].nodeID = nodeID;
		rebornTree[nodeID].depth = currentDepth;
		return nodeID;
	}
	else
	{
		// Verify which pair of subproblems was used and calling recursion on each subproblem
		for (int k = 0; k < params->nbFeatures ; k++)
		{
			const int codeBookValue = fspaceFinal.codeBook[k];
			const int rangeLow = fspaceFinal.keyToCell(indexBottom, k);
			const int rangeUp = fspaceFinal.keyToCell(indexTop, k);
			for (int l = rangeLow; l < rangeUp ; l++)
			{
				unsigned int leftResult;
				int indexTopLeft = indexTop + codeBookValue * (l - rangeUp);
				int hash1 = fspaceFinal.keyToHash(indexBottom, indexTopLeft);
				if (indexBottom == indexTopLeft) leftResult = 0; 
				else leftResult = regions[indexBottom][hash1];

				unsigned int rightResult;
				int indexBottomRight = indexBottom + codeBookValue * (l + 1 - rangeLow);
				int hash2 = fspaceFinal.keyToHash(indexBottomRight, indexTop);
				if (indexBottomRight == indexTop) rightResult = 0; 
				else rightResult = regions[indexBottomRight][hash2];

				// Found the optimal split used by the DP
				if (leftResult != UINT_MAX && rightResult != UINT_MAX &&
				   ((params->objectiveFunction == 0 && 1 + std::max<unsigned int>(leftResult, rightResult) == optValue) || 
					(params->objectiveFunction == 1 && 1 + leftResult + rightResult == optValue) ||
					(params->objectiveFunction == 2 && BIG_M + 1 + BIG_M * std::max<unsigned int>(leftResult/BIG_M,rightResult/BIG_M) + leftResult%BIG_M + rightResult%BIG_M == optValue)))
				{
					finalSplits++;
					rebornTree.push_back(Node());
					int nodeID = (int)rebornTree.size() - 1;
					rebornTree[nodeID].nodeType = Node::NODE_INTERNAL;
					rebornTree[nodeID].splitFeature = k;
					rebornTree[nodeID].splitValue = fspaceFinal.orderedHyperplaneLevels[k][l];
					rebornTree[nodeID].nodeID = nodeID;
					int myLeftID =  collectResultDP(indexBottom, indexTopLeft, leftResult, currentDepth + 1);
					int myRightID = collectResultDP(indexBottomRight, indexTop, rightResult, currentDepth + 1);
					rebornTree[nodeID].leftChild = myLeftID;
					rebornTree[nodeID].rightChild = myRightID;
					rebornTree[nodeID].depth = currentDepth;
					return nodeID;
				}
			}
		}
		// Just for safety: This point is never attained since the optimal result is found from one of the subproblems
		throw std::string("Error extraction DP results");
	}
}

int BornAgainDecisionTree::computeRegion(Region * r, int d)
{
	iterationsDP = 0;
	regionsMemorizedDP = 0;
	finalSplits = 0;
	finalLeaves = 0;
	finalDepth = 0;

	// Initialize the cells structures and keep useful hyperplanes
	fspaceOriginal.initializeCells(randomForest->getHyperplanes(),false);
	// fspaceFinal.initializeCells(fspaceOriginal.exportUsefulHyperplanes(),true);
	fspaceFinal.initializeCells(randomForest->getHyperplanes(), false);
	
	// Initialize the memory to store the DP results on the regions
	regions = std::vector<std::vector<unsigned int>>(fspaceFinal.nbCells);
	for (int index = 0; index < fspaceFinal.nbCells; index++){
		regions[index] = std::vector<unsigned int>(fspaceFinal.keyToHash(index,fspaceFinal.nbCells-1)+1,UINT_MAX);
	}
	// Get hyperplanes and hyperplanes importance
	auto myPair = randomForest->getHyperplanesImportance();
	//hyperplanes = std::vector<std::vector<double>>(r->nbFeatures);
	hyperplanes = myPair.first;
	hyperplanes_Importance = myPair.second;

	// Create index of the region bottom and top
	int indexBottom = fspaceFinal.cellToKey(r->Bottom);
	int indexTop = fspaceFinal.cellToKey(r->Top);
	
	// Add first region
	depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);
	if (depthh<=d) usefulRegions.push_back(std::make_pair(std::make_pair(indexBottom,indexTop), r->gain));
	solve(r,d);
	
	std::sort(usefulRegions.begin(), usefulRegions.end(), compareGain);
	auto reg = usefulRegions[0].first;
	indexBottom = reg.first;
	indexTop = reg.second;
	depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);

	// Collect the final solution
	collectResultDP(indexBottom, indexTop, depthh, 0);

	// Collect the region
	for(int i=0; i<r->nbFeatures;i++) {
		r->BottomV[i] = fspaceFinal.keyToCell(indexBottom,i);
		r->TopV[i] = fspaceFinal.keyToCell(indexTop,i);
	}

	return usefulRegions[0].second;
}

double BornAgainDecisionTree::computeBestRegion(Region * r, int d)
{
	iterationsDP = 0;
	regionsMemorizedDP = 0;
	finalSplits = 0;
	finalLeaves = 0;
	finalDepth = 0;

	hyperplanes = randomForest->getHyperplanes();
	// Initialize the cells structures and keep useful hyperplanes
	fspaceOriginal.initializeCells(hyperplanes,false);
	// fspaceFinal.initializeCells(fspaceOriginal.exportUsefulHyperplanes(),true);
	fspaceFinal.initializeCells(hyperplanes, false);
	
	// Initialize the memory to store the DP results on the regions
	regions = std::vector<std::vector<unsigned int>>(fspaceFinal.nbCells);
	gains = std::vector<std::vector<double>>(fspaceFinal.nbCells);
	for (int index = 0; index < fspaceFinal.nbCells; index++){
		regions[index] = std::vector<unsigned int>(fspaceFinal.keyToHash(index,fspaceFinal.nbCells-1)+1,UINT_MAX);
		gains[index] = std::vector<double>(fspaceFinal.keyToHash(index,fspaceFinal.nbCells-1)+1,INT_MIN);
	}
	// Get hyperplanes importance
	hyperplanes_Importance_sum = std::vector<std::map<double,double>>(params->nbFeatures);
	hyperplanes_Importance = calculateFeatureLevelImportance(r->BottomV);
	instance = fspaceFinal.cellToKey(r->Bottom);
	auto solution = solver(0,fspaceFinal.nbCells-1,d);
	auto reg = solution.first;
	int indexBottom = reg.first;
	int indexTop = reg.second;
	depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);

	// Collect the final solution
	collectResultDP(indexBottom, indexTop, depthh, 0);

	// Collect the region
	for(int i=0; i<r->nbFeatures;i++) {
		r->BottomV[i] = fspaceFinal.keyToCell(indexBottom,i);
		r->TopV[i] = fspaceFinal.keyToCell(indexTop,i);
	}
	r->gain = solution.second;

	return solution.second;
}

// Function to calculate Gini index
double BornAgainDecisionTree::calculateGiniIndex(const std::vector<int>& classCounts, int totalSamples)
{
    double giniIndex = 1.0;
    for (int count : classCounts) {
        double probability = static_cast<double>(count) / totalSamples;
        giniIndex -= probability * probability;
    }
    return giniIndex;
}


std::vector<std::map<double, double>> BornAgainDecisionTree::calculateFeatureLevelImportance(const std::vector<double>& inputVector)
{
	std::vector<std::map<double, double>> splitValueImportance(params->nbFeatures);

    int numSamples = fspaceFinal.nbCells;

    for (const std::vector<Node>& tree : randomForest->trees) {
        for (const Node& node : tree) {
            if (node.nodeType == Node::NODE_INTERNAL) {
                int splitFeature = node.splitFeature;
                double splitValue = node.splitValue;

                // Compute class counts for the left and right child nodes
                std::vector<int> leftClassCounts(randomForest->countClasses.size(), 0);
                std::vector<int> rightClassCounts(randomForest->countClasses.size(), 0);
                for (int i = 0; i < numSamples; ++i) {
                    double sample = fspaceFinal.keyToCell(i,splitFeature);
                    int classLabel = fspaceFinal.cells[i];
					if (sample <= splitValue) {
                        leftClassCounts[classLabel]++;
                    } else {
                        rightClassCounts[classLabel]++;
                    }
                }

                // Increment the count for the split value
                splitValueImportance[splitFeature][splitValue] += calculateGiniIndex(leftClassCounts, numSamples) + calculateGiniIndex(rightClassCounts, numSamples);
            }
        }
    }
	// Multiply splitValueImportance by distance
    for (int splitFeature = 0; splitFeature < params->nbFeatures; ++splitFeature) {
    	double featureImportanceSum = 0.0;
    	double lastLevel = 0.0;

    	for (auto& entry : splitValueImportance[splitFeature]) {
    	    double distance = std::exp(-std::abs(inputVector[splitFeature] - entry.first));
    	    entry.second *= distance;
    	    featureImportanceSum += entry.second;

    	    if (lastLevel == 0.0) {
    	        hyperplanes_Importance_sum[splitFeature][entry.first] = entry.second;
    	    } else {
    	        hyperplanes_Importance_sum[splitFeature][entry.first] = entry.second + hyperplanes_Importance_sum[splitFeature][lastLevel];
    	    }

    	    lastLevel = entry.first;
    	    //std::cout << "Feature: " << splitFeature << " - Level: " << entry.first <<" lastlevel : "<<lastLevel<< " - Importance sum: " << hyperplanes_Importance_sum[splitFeature][entry.first] << " - Importance: " << entry.second << std::endl;
    	}

    	featureImportance.emplace_back(splitFeature, featureImportanceSum);
	}	

	std::sort(featureImportance.begin(), featureImportance.end(),
              [](const std::pair<int, double>& a, const std::pair<int, double>& b) {
                  return a.second > b.second;
              });

    return splitValueImportance;
}

double BornAgainDecisionTree::computeSecondRegion(Region * r, int d)
{
	iterationsDP = 0;
	regionsMemorizedDP = 0;
	finalSplits = 0;
	finalLeaves = 0;
	finalDepth = 0;

	hyperplanes = randomForest->getHyperplanes();
	// Initialize the cells structures and keep useful hyperplanes
	fspaceOriginal.initializeCells(hyperplanes,false);
	// fspaceFinal.initializeCells(fspaceOriginal.exportUsefulHyperplanes(),true);
	fspaceFinal.initializeCells(hyperplanes, false);
	
	// Initialize the memory to store the DP results on the regions
	regions = std::vector<std::vector<unsigned int>>(fspaceFinal.nbCells);
	for (int index = 0; index < fspaceFinal.nbCells; index++){
		regions[index] = std::vector<unsigned int>(fspaceFinal.keyToHash(index,fspaceFinal.nbCells-1)+1,UINT_MAX);
	}
	// Get hyperplanes importance
	hyperplanes_Importance_sum = std::vector<std::map<double,double>>(params->nbFeatures);
	hyperplanes_Importance = calculateFeatureLevelImportance(r->BottomV);

	// Create index of the region bottom and top
	int indexBottom = fspaceFinal.cellToKey(r->Bottom);
	int indexTop = fspaceFinal.cellToKey(r->Top);
	
	// Add first region
	depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);
	if (depthh<=d) usefulRegions.push_back(std::make_pair(std::make_pair(indexBottom,indexTop), r->gain));
	solve(r,d);
	
	std::sort(usefulRegions.begin(), usefulRegions.end(), compareGain);
	auto reg = usefulRegions[0].first;
	indexBottom = reg.first;
	indexTop = reg.second;
	// indexBottom = fspaceFinal.cellToKey(r->Bottom);
	// indexTop = fspaceFinal.cellToKey(r->Top);
	depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);

	// Collect the final solution
	collectResultDP(indexBottom, indexTop, depthh, 0);

	// Collect the region
	for(int i=0; i<r->nbFeatures;i++) {
		r->BottomV[i] = fspaceFinal.keyToCell(indexBottom,i);
		r->TopV[i] = fspaceFinal.keyToCell(indexTop,i);
	}

	return usefulRegions[0].second;
}

bool BornAgainDecisionTree::compareGain(const std::pair<std::pair<int,int>, int>& a, const std::pair<std::pair<int,int>, int>& b) {
    return a.second > b.second;
}


void BornAgainDecisionTree::solve(Region * r, int d)
{
	if(depthh> d) return;
	for(int s=0 ; s<r->nbFeatures;s++){
		int k = featureImportance[s].first;
		int i = r->Bottom[k];
		if(i>0){
			r->Bottom[k] -= 1;
			r->gain += hyperplanes_Importance[k][hyperplanes[k][r->Bottom[k]]];
			// Create index of the region bottom and top
			int indexBottom = fspaceFinal.cellToKey(r->Bottom);
			int indexTop = fspaceFinal.cellToKey(r->Top);

			if(std::count(usefulRegions.begin(),usefulRegions.end(),std::make_pair(std::make_pair(indexBottom, indexTop), r->gain))>0) return;
			// Add region
			depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);
			if (depthh<=d){
				usefulRegions.push_back(std::make_pair(std::make_pair(indexBottom, indexTop), r->gain));
				solve(r,d);
			}
			r->gain -= hyperplanes_Importance[k][hyperplanes[k][r->Bottom[k]]];
			r->Bottom[k] += 1;
			solve(r,d);
		}
		
		int j = r->Top[k];
		
		if(j < hyperplanes[k].size()-1){
			r->gain += hyperplanes_Importance[k][hyperplanes[k][r->Top[k]]];
			r->Top[k] += 1;
			
			// Create index of the region bottom and top
			int indexBottom = fspaceFinal.cellToKey(r->Bottom);
			int indexTop = fspaceFinal.cellToKey(r->Top);

			if(std::count(usefulRegions.begin(),usefulRegions.end(),std::make_pair(std::make_pair(indexBottom, indexTop), r->gain))>0) return;
			// Add region
			depthh = dynamicProgrammingOptimizeDepth(indexBottom, indexTop);
			if (depthh<=d){
				usefulRegions.push_back(std::make_pair(std::make_pair(indexBottom, indexTop), r->gain));
				solve(r,d);
			}
			r->Top[k] -= 1;
			r->gain -= hyperplanes_Importance[k][hyperplanes[k][r->Top[k]]];
			solve(r,d);
		}
	}
	return;
}

std::pair<std::pair<int,int>, double> BornAgainDecisionTree::solver(int indexBottom, int indexTop, int maxDepth){
	for (int i = 0; i<params->nbFeatures;i++){
		if ((fspaceFinal.keyToCell(indexBottom,i)>fspaceFinal.keyToCell(instance,i)) || (fspaceFinal.keyToCell(instance,i)> fspaceFinal.keyToCell(indexTop,i)) )
			return std::make_pair(std::make_pair(indexBottom, indexTop), INT_MIN);
	}
	depthh = dynamicProgrammingOptimizeDepth(indexBottom,indexTop);
	if(depthh == 0) return std::make_pair(std::make_pair(indexBottom, indexTop), INT_MIN);
	if(depthh<=maxDepth) {
		int hash = fspaceFinal.keyToHash(indexBottom, indexTop);
		if (gains[indexBottom][hash] != INT_MIN) return std::make_pair(std::make_pair(indexBottom, indexTop),gains[indexBottom][hash]);
		else{
			double gain = 0;
			std::vector<double> TopV;
			std::vector<double> BottomV;
			for (int i = 0; i<params->nbFeatures;i++) {
				BottomV.push_back(fspaceFinal.keyToCell(indexBottom,i));
				TopV.push_back(fspaceFinal.keyToCell(indexTop,i));
			}
			std::vector<int> Bottom;
			std::vector<int> Top;
			Bottom = Region::getCell(BottomV,hyperplanes, params->nbFeatures);
			Top = Region::getCell(TopV,hyperplanes,params->nbFeatures);
			for(int i = 0; i<params->nbFeatures; i++){
				gain +=hyperplanes_Importance[i][hyperplanes[i][Bottom[i]]] +hyperplanes_Importance_sum[i][hyperplanes[i][Top[i]-1]]-hyperplanes_Importance_sum[i][hyperplanes[i][Bottom[i]]];;
	
			}
			gains[indexBottom][hash] = gain;
			return std::make_pair(std::make_pair(indexBottom, indexTop), gain);
		}
	}
	double gain = INT_MIN;
	std::pair<int,int> tmpRegion;
	for (int k = 0; k < params->nbFeatures; k++)
	{
		const int codeBookValue = fspaceFinal.codeBook[k];
		const int rangeLow = fspaceFinal.keyToCell(indexBottom, k);
		const int rangeUp = fspaceFinal.keyToCell(indexTop, k);
		for (int l = rangeLow; l < rangeUp; l++)
		{
			auto leftGain = solver(indexBottom, indexTop + codeBookValue * (l - rangeUp), maxDepth);
			if(leftGain.second>gain) {
				gain = leftGain.second;
				tmpRegion = leftGain.first;
			}
			else {auto rightGain = solver(indexBottom + codeBookValue * (l + 1 - rangeLow), indexTop, maxDepth);
			if(rightGain.second>gain) {
				gain = rightGain.second;
				tmpRegion = rightGain.first;
			}}
		}
	}
	return std::make_pair(tmpRegion,gain);
		
}

void BornAgainDecisionTree::buildOptimal()
{
	iterationsDP = 0;
	regionsMemorizedDP = 0;
	finalSplits = 0;
	finalLeaves = 0;
	finalDepth = 0;

	// Initialize the cells structures and keep useful hyperplanes
	fspaceOriginal.initializeCells(randomForest->getHyperplanes(),false);
	fspaceFinal.initializeCells(fspaceOriginal.exportUsefulHyperplanes(),true);
	
	// Initialize the memory to store the DP results on the regions
	regions = std::vector<std::vector<unsigned int>>(fspaceFinal.nbCells);
	for (int index = 0; index < fspaceFinal.nbCells; index++)
		regions[index] = std::vector<unsigned int>(fspaceFinal.keyToHash(index,fspaceFinal.nbCells-1)+1,UINT_MAX);

	// Launch DP algorithm
	// std::cout << "----- START OF OPTIMIZATION " << std::endl;
	if (params->objectiveFunction == 0)      finalObjective = dynamicProgrammingOptimizeDepth(0, (int)fspaceFinal.nbCells - 1);
	else if (params->objectiveFunction == 1) finalObjective = dynamicProgrammingOptimizeNbSplits(0, (int)fspaceFinal.nbCells - 1);
	else if (params->objectiveFunction == 2) finalObjective = dynamicProgrammingOptimizeDepthThenNbSplits(0, (int)fspaceFinal.nbCells - 1);
	else throw std::string("NON RECOGNIZED OBJECTIVE");

	// Collect the final solution
	collectResultDP(0, (int)fspaceFinal.nbCells - 1, finalObjective, 0);
}

void BornAgainDecisionTree::buildRegionOptimal(Region * r)
{
	iterationsDP = 0;
	regionsMemorizedDP = 0;
	finalSplits = 0;
	finalLeaves = 0;
	finalDepth = 0;

	// Initialize the cells structures and keep useful hyperplanes
	fspaceOriginal.initializeCells(r->getLocalHyperplanes(randomForest->getHyperplanes()),false);
	// fspaceFinal.initializeCells(fspaceOriginal.exportUsefulHyperplanes(),true);
	fspaceFinal.initializeCells(fspaceOriginal.orderedHyperplaneLevels, false);

	
	// Initialize the memory to store the DP results on the regions
	regions = std::vector<std::vector<unsigned int>>(fspaceFinal.nbCells);
	for (int index = 0; index < fspaceFinal.nbCells; index++)
		regions[index] = std::vector<unsigned int>(fspaceFinal.keyToHash(index,fspaceFinal.nbCells-1)+1,UINT_MAX);

	// Launch DP algorithm
	// std::cout << "----- START OF OPTIMIZATION " << std::endl;
	if (params->objectiveFunction == 0)      finalObjective = dynamicProgrammingOptimizeDepth(0, (int)fspaceFinal.nbCells - 1);
	else if (params->objectiveFunction == 1) finalObjective = dynamicProgrammingOptimizeNbSplits(0, (int)fspaceFinal.nbCells - 1);
	else if (params->objectiveFunction == 2) finalObjective = dynamicProgrammingOptimizeDepthThenNbSplits(0, (int)fspaceFinal.nbCells - 1);
	else throw std::string("NON RECOGNIZED OBJECTIVE");

	// Collect the final solution
	collectResultDP(0, (int)fspaceFinal.nbCells - 1, finalObjective, 0);
}


void BornAgainDecisionTree::displayRunStatistics()
{
	std::vector<std::string> objectives = {"Depth","NbLeaves","Depth then NbLeaves","NbLeaves then Depth","Heuristic"};
	std::cout << "----- OPTIMAL SOLUTION FOUND                      " << std::endl;
	std::cout << "----- OBJECTIVE:                                  " << objectives[params->objectiveFunction] << std::endl;
	std::cout << "----- CPU TIME(s):                                " << (double)(params->stopTime - params->startTime) / (double)CLOCKS_PER_SEC << std::endl;
	std::cout << "----- ORIGINAL CELLS:                             " << fspaceOriginal.nbCells << std::endl;
	std::cout << "----- FILTERED CELLS:                             " << fspaceFinal.nbCells << std::endl;
	std::cout << "----- NB SUBPROBLEMS:                             " << (double)regionsMemorizedDP << std::endl;
	std::cout << "----- NB RECURSIVE CALLS:                         " << (double)iterationsDP << std::endl;
	std::cout << "----- BA TREE DEPTH:                              " << finalDepth << std::endl;
	std::cout << "----- BA TREE LEAVES:                             " << finalLeaves << std::endl;
}

void BornAgainDecisionTree::exportRunStatistics(std::string fileName)
{
	std::ofstream myfile;
	std::cout << "----- EXPORTING STATISTICS in " << fileName << std::endl;
	myfile.open(fileName.data());
	if (myfile.is_open())
	{
		myfile << params->datasetName << ",";
		myfile << params->ensembleMethod << ",";
		myfile << params->nbTrees << ",";
		myfile << params->nbFeatures << ",";
		myfile << params->nbClasses << ",";
		myfile << params->objectiveFunction << ",";
		myfile << finalDepth << ",";
		myfile << finalSplits << ",";
		myfile << finalLeaves << ",";
		myfile << 1 << ","; // Only one execution loop has been done
		myfile << (double)(params->stopTime - params->startTime) / (double)CLOCKS_PER_SEC << ",";
		myfile << fspaceFinal.nbCells << ",";
		myfile << regionsMemorizedDP << ",";
		myfile << iterationsDP << std::endl;
		myfile.close();
	}
	else
		std::cout << "PROBLEM OPENING FILE " << fileName << std::endl;
}

void BornAgainDecisionTree::exportBATree(std::string fileName)
{
	std::ofstream myfile;
	std::cout << "----- EXPORTING BA TREE in " << fileName << std::endl;
	myfile.open(fileName.data());
	if (myfile.is_open())
	{
		myfile << "DATASET_NAME: " << params->datasetName << std::endl;
		myfile << "ENSEMBLE: BA" << std::endl;
		myfile << "NB_TREES: " << 1 << std::endl;
		myfile << "NB_FEATURES: " << params->nbFeatures << std::endl;
		myfile << "NB_CLASSES: " << params->nbClasses << std::endl;
		myfile << "MAX_TREE_DEPTH: " << finalDepth << std::endl;
		myfile << "Format: node / node type(LN - leave node, IN - internal node) left child / right child / feature / threshold / node_depth / majority class (starts with index 0)" << std::endl;
		myfile << std::endl;
		myfile << "[TREE 0]" << std::endl;
		myfile << "NB_NODES: " << finalSplits+ finalLeaves << std::endl;

		for (int i = 0; i < (int)rebornTree.size(); i++)
		{
			if (rebornTree[i].nodeType == Node::NODE_INTERNAL)
				myfile << i << " IN " << rebornTree[i].leftChild << " " << rebornTree[i].rightChild << " " << rebornTree[i].splitFeature << " " << rebornTree[i].splitValue << " " << rebornTree[i].depth << " -1" << std::endl;
			else if (rebornTree[i].nodeType == Node::NODE_LEAF)
				myfile << i << " LN -1 -1 -1 -1 " << rebornTree[i].depth << " " << rebornTree[i].classification << std::endl;
			else throw std::string("Error: Unexpected node type");
		}
	}
	else
		std::cout << "PROBLEM OPENING FILE  " << fileName << std::endl;
}

void BornAgainDecisionTree::exportRegionBATree(std::string fileName,Region * r)
{
	std::ofstream myfile;
	std::cout << "----- EXPORTING BA TREE in " << fileName << std::endl;
	myfile.open(fileName.data());
	if (myfile.is_open())
	{
		myfile << "DATASET_NAME: " << params->datasetName << std::endl;
		myfile << "ENSEMBLE: BA" << std::endl;
		myfile << "NB_TREES: " << 1 << std::endl;
		myfile << "NB_FEATURES: " << params->nbFeatures << std::endl;
		myfile << "NB_CLASSES: " << params->nbClasses << std::endl;
		myfile << "MAX_TREE_DEPTH: " << finalDepth << std::endl;
		myfile << "Bottom Vector : "; 
		for(int i = 0;i< r->nbFeatures-1;i++){
			myfile<< r->BottomV[i] << ", ";
		}
		myfile<< r->BottomV[r->nbFeatures-1] << std::endl;
		myfile << "Top Vector : "; 
		for(int i = 0;i< r->nbFeatures-1;i++){
			myfile<< r->TopV[i] << ", ";
		}
		myfile<< r->TopV[r->nbFeatures-1] << std::endl;
		myfile<< "Gain: "<< r->gain << std::endl;
		myfile << "Format: node / node type(LN - leave node, IN - internal node) left child / right child / feature / threshold / node_depth / majority class (starts with index 0)" << std::endl;
		myfile << std::endl;
		myfile << "[TREE 0]" << std::endl;
		myfile << "NB_NODES: " << finalSplits+ finalLeaves << std::endl;

		for (int i = 0; i < (int)rebornTree.size(); i++)
		{
			if (rebornTree[i].nodeType == Node::NODE_INTERNAL)
				myfile << i << " IN " << rebornTree[i].leftChild << " " << rebornTree[i].rightChild << " " << rebornTree[i].splitFeature << " " << rebornTree[i].splitValue << " " << rebornTree[i].depth << " -1" << std::endl;
			else if (rebornTree[i].nodeType == Node::NODE_LEAF)
				myfile << i << " LN -1 -1 -1 -1 " << rebornTree[i].depth << " " << rebornTree[i].classification << std::endl;
			else throw std::string("Error: Unexpected node type");
		}
	}
	else
		std::cout << "PROBLEM OPENING FILE  " << fileName << std::endl;
}

int BornAgainDecisionTree::recursiveHelperHeuristic(unsigned int currentDepth)
{
	// PICK A RANDOM SAMPLE OF CELLS WITHIN THIS REGION
	for (int k : nonTrivialFeatures)
	{
		std::uniform_int_distribution<int> distribution(0, topRightCell[k] - bottomLeftCell[k]);
		for (int s = 0; s < params->nbCellsSampled; s++)
		{
			sampledCellsIndices[s][k] = bottomLeftCell[k] + distribution(params->generator);
			sampledCellsCoords[s][k] = orderedHyperplaneLevelsHeuristic[k][sampledCellsIndices[s][k]];
		}
	}

	// COLLECT SOME STATISTICS
	int nbClassesRepresented = 0;
	std::vector<int> nbSamplesClass = std::vector<int>(params->nbClasses, 0);
	for (int s = 0; s < params->nbCellsSampled; s++)
	{
		classSampledCells[s] = randomForest->majorityClass(sampledCellsCoords[s]);
		if (nbSamplesClass[classSampledCells[s]] == 0) nbClassesRepresented++;
		nbSamplesClass[classSampledCells[s]]++;
	}

	if (nbClassesRepresented == 0)
		throw std::string("ISSUE: NUMBER OF CLASSES REPRESENTED SHOULD BE STRICTLY POSITIVE");

	bool regionIsPure = true;
	if (nbClassesRepresented > 1)
		regionIsPure = false;
#ifdef USING_CPLEX
	else // nbClassesRepresented == 1
	{
		// CALL MIP CERTIFICATE TO VERIFY PURITY (only when the code is linked to CPLEX)
		int pureClassID = classSampledCells[0];
		for (int c = 0; c < params->nbClasses; c++)
		{
			if (c != pureClassID
				&& !myMIPcertificate->preFilterMinMax(nonTrivialFeaturesBeforeSplits, orderedHyperplaneLevelsHeuristic, bottomLeftCell, topRightCell, pureClassID, c)  // Use a pre-filter
				&& !myMIPcertificate->buildAndRunMIP(nonTrivialFeaturesBeforeSplits, nonTrivialFeatures, orderedHyperplaneLevelsHeuristic, bottomLeftCell, topRightCell, pureClassID, c))  // Do an exact check
			{
				regionIsPure = false;
				// cout << "IMPURITY DETECTED BY MIP BUT SAMPLES WERE PURE" << endl;
				break;
			}
		}
	}
#endif

	if (regionIsPure)
	{
		// CREATE A LEAF
		finalLeaves++;
		if (currentDepth > finalDepth) finalDepth = currentDepth;
		rebornTree.push_back(Node());
		int nodeID = (int)rebornTree.size() - 1;
		rebornTree[nodeID].nodeType = Node::NODE_LEAF;
		rebornTree[nodeID].splitFeature = -1;
		rebornTree[nodeID].splitValue = -1;
		rebornTree[nodeID].classification = classSampledCells[0];
		rebornTree[nodeID].nodeID = nodeID;
		rebornTree[nodeID].depth = currentDepth;
		return nodeID;
	}
	else
	{
		// OTHERWISE LOOK FOR THE BEST SPLIT BASED ON THIS RANDOM SAMPLE OF CELLS USING AN INFORMATION GAIN CRITERION
		double bestEntropySub = 1.e30;
		int bestSplitFeature = -1;
		unsigned short int bestSplitLevel = 10000;
		std::uniform_int_distribution<int> distribution(0, params->nbFeatures - 1);
		int kInit = distribution(params->generator); // Avoids possible bias due to the index order when breaking ties
		for (int kk = 0; kk < params->nbFeatures; kk++)
		{
			int k = (kInit + kk) % params->nbFeatures;
			// For each feature which has more than one level
			if (bottomLeftCell[k] != topRightCell[k])
			{
				std::vector<std::pair<unsigned short int, unsigned short int>> orderedSamples;
				for (int s = 0; s < params->nbCellsSampled; s++)
					orderedSamples.push_back(std::pair<unsigned short int, unsigned short int>(sampledCellsIndices[s][k], classSampledCells[s]));
				std::sort(orderedSamples.begin(), orderedSamples.end());

				// Initially all samples are on the right
				std::vector <int> nbSamplesClassLeft = std::vector<int>(params->nbClasses, 0);
				std::vector <int> nbSamplesClassRight = nbSamplesClass;
				int indexSample = 0;
				for (unsigned short int attributeValue = bottomLeftCell[k]; attributeValue < topRightCell[k]; attributeValue++)
				{
					// Iterate on all samples with this attributeValue and switch them to the left
					while (indexSample < params->nbCellsSampled && orderedSamples[indexSample].first <= attributeValue)
					{
						nbSamplesClassLeft[orderedSamples[indexSample].second]++;
						nbSamplesClassRight[orderedSamples[indexSample].second]--;
						indexSample++;
					}

					// No need to consider the case in which all samples have been switched to the left
					if (indexSample != params->nbCellsSampled)
					{
						// Evaluate entropy of the two resulting sample sets
						double entropyLeft = 0.0;
						double entropyRight = 0.0;
						for (int c = 0; c < params->nbClasses; c++)
						{
							// Remark that indexSample contains at this stage the number of samples in the left
							if (nbSamplesClassLeft[c] > 0)
							{
								double fracLeft = (double)nbSamplesClassLeft[c] / (double)(indexSample);
								entropyLeft -= fracLeft * log2(fracLeft);
							}
							if (nbSamplesClassRight[c] > 0)
							{
								double fracRight = (double)nbSamplesClassRight[c] / (double)(params->nbCellsSampled - indexSample);
								entropyRight -= fracRight * log2(fracRight);
							}
						}

						// Evaluate the information gain and store if this is the best option found until now
						double entropySub = ((double)indexSample*entropyLeft + (double)(params->nbCellsSampled - indexSample)*entropyRight) / (double)params->nbCellsSampled;
						if (entropySub < bestEntropySub)
						{
							bestEntropySub = entropySub;
							bestSplitFeature = k;
							bestSplitLevel = attributeValue;
						}
					}
				}
			}
		}

		/* APPLY THE SPLIT AND PERFORM RECURSIVE CALLS */
		finalSplits++;
		rebornTree.push_back(Node());
		int nodeID = (int)rebornTree.size() - 1;
		rebornTree[nodeID].nodeType = Node::NODE_INTERNAL;
		rebornTree[nodeID].splitFeature = bestSplitFeature;
		rebornTree[nodeID].splitValue = orderedHyperplaneLevelsHeuristic[bestSplitFeature][bestSplitLevel];
		rebornTree[nodeID].nodeID = nodeID;

		int tempR = topRightCell[bestSplitFeature];
		topRightCell[bestSplitFeature] = bestSplitLevel;
		// Possibly eliminate this feature from the list of features which need to be considered
		if (bottomLeftCell[bestSplitFeature] == topRightCell[bestSplitFeature])
		{
			nonTrivialFeatures.erase(bestSplitFeature);
			for (int s = 0; s < params->nbCellsSampled; s++)
			{
				sampledCellsIndices[s][bestSplitFeature] = bottomLeftCell[bestSplitFeature];
				sampledCellsCoords[s][bestSplitFeature] = orderedHyperplaneLevelsHeuristic[bestSplitFeature][sampledCellsIndices[s][bestSplitFeature]];
			}
		}
		int myLeftID = recursiveHelperHeuristic(currentDepth + 1);
		topRightCell[bestSplitFeature] = tempR;
		nonTrivialFeatures.insert(bestSplitFeature);

		int tempL = bottomLeftCell[bestSplitFeature];
		bottomLeftCell[bestSplitFeature] = bestSplitLevel + 1;
		// Possibly eliminate this feature from the list of features which need to be considered
		if (bottomLeftCell[bestSplitFeature] == topRightCell[bestSplitFeature])
		{
			nonTrivialFeatures.erase(bestSplitFeature);
			for (int s = 0; s < params->nbCellsSampled; s++)
			{
				sampledCellsIndices[s][bestSplitFeature] = bottomLeftCell[bestSplitFeature];
				sampledCellsCoords[s][bestSplitFeature] = orderedHyperplaneLevelsHeuristic[bestSplitFeature][sampledCellsIndices[s][bestSplitFeature]];
			}
		}
		int myRightID = recursiveHelperHeuristic(currentDepth + 1);
		bottomLeftCell[bestSplitFeature] = tempL;
		nonTrivialFeatures.insert(bestSplitFeature);

		rebornTree[nodeID].leftChild = myLeftID;
		rebornTree[nodeID].rightChild = myRightID;
		rebornTree[nodeID].depth = currentDepth;
		return nodeID;
	}
}

void BornAgainDecisionTree::buildHeuristic()
{
	finalSplits = 0;
	finalLeaves = 0;
	finalDepth = 0;

	// Get the ordered Hyperplanes
	orderedHyperplaneLevelsHeuristic = randomForest->getHyperplanes();

	// Initialize the solver
#ifdef USING_CPLEX
	myMIPcertificate = new MIPCertificate(params, randomForest);
#endif
	
	// Initialize the original region
	for (int k = 0; k < params->nbFeatures; k++)
	{
		bottomLeftCell.push_back(0); // Initializing the bottom left cell
		topRightCell.push_back((int)orderedHyperplaneLevelsHeuristic[k].size()-1); // Initializing the top right cell
		if (bottomLeftCell[k] != topRightCell[k]) nonTrivialFeatures.insert(k); // Keeping track of the features which are non-trivial
	}
	nonTrivialFeaturesBeforeSplits = nonTrivialFeatures;

	// Initialize other data structures and call the recursive construction procedure
	sampledCellsIndices = std::vector<std::vector<unsigned short int>>(params->nbCellsSampled, std::vector<unsigned short int>(params->nbFeatures, 0));
	sampledCellsCoords = std::vector<std::vector<double>>(params->nbCellsSampled, std::vector<double>(params->nbFeatures, 1.e30));
	classSampledCells = std::vector<unsigned short int>(params->nbCellsSampled);
	recursiveHelperHeuristic(0);

#ifdef USING_CPLEX
	delete myMIPcertificate;
#endif
}