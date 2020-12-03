#include "MPFA_loop_functions.h"
#include <assert.h> 
MPFA_loop_functions::MPFA_loop_functions() :
	RNG(argos::CRandom::CreateRNG("argos")),
        SimTime(0),
	MaxSimTime(3600 * GetSimulator().GetPhysicsEngine("dyn2d").GetInverseSimulationClockTick()),
        CollisionTime(0), 
        lastNumCollectedFood(0),
        currNumCollectedFood(0),
	ResourceDensityDelay(0),
	RandomSeed(GetSimulator().GetRandomSeed()),
	SimCounter(0),
	MaxSimCounter(1),
	VariableFoodPlacement(0),
	OutputData(0),
	DrawDensityRate(4),
	DrawIDs(1),
	DrawTrails(1),
	DrawTargetRays(1),
	FoodDistribution(2),
	FoodItemCount(256),
	PowerlawFoodUnitCount(256),
	NumberOfClusters(4),
	ClusterWidthX(8),
	ClusterWidthY(8),
	PowerRank(4),
	ProbabilityOfSwitchingToSearching(0.0),
	ProbabilityOfReturningToNest(0.0),
	UninformedSearchVariation(0.0),
	RateOfInformedSearchDecay(0.0),
	RateOfSiteFidelity(0.0),
	RateOfLayingPheromone(0.0),
	RateOfPheromoneDecay(0.0),
	FoodRadius(0.05),
	FoodRadiusSquared(0.0025),
	NestRadius(0.25),
	//NestRadiusSquared(0.0625),
	NestElevation(0.01),
    BacktrackDelivery(1),
	// We are looking at a 4 by 4 square (3 targets + 2*1/2 target gaps)
	SearchRadiusSquared((4.0 * FoodRadius) * (4.0 * FoodRadius)),
        NumDistributedFood(0),
	score(0),
    DeliveryFlag(0),
	PrintFinalScore(0)
{}

void MPFA_loop_functions::Init(argos::TConfigurationNode &node) {
    CVector2		NestPosition; //qilu 09/06
 
	argos::CDegrees USV_InDegrees;
	argos::TConfigurationNode MPFA_node = argos::GetNode(node, "MPFA");

	argos::GetNodeAttribute(MPFA_node, "ProbabilityOfSwitchingToSearching", ProbabilityOfSwitchingToSearching);
	argos::GetNodeAttribute(MPFA_node, "ProbabilityOfReturningToNest",      ProbabilityOfReturningToNest);
	argos::GetNodeAttribute(MPFA_node, "UninformedSearchVariation",         USV_InDegrees);
	argos::GetNodeAttribute(MPFA_node, "RateOfInformedSearchDecay",         RateOfInformedSearchDecay);
	argos::GetNodeAttribute(MPFA_node, "RateOfSiteFidelity",                RateOfSiteFidelity);
	argos::GetNodeAttribute(MPFA_node, "RateOfLayingPheromone",             RateOfLayingPheromone);
	argos::GetNodeAttribute(MPFA_node, "RateOfPheromoneDecay",              RateOfPheromoneDecay);
	argos::GetNodeAttribute(MPFA_node, "PrintFinalScore",                   PrintFinalScore);

	UninformedSearchVariation = ToRadians(USV_InDegrees);
	argos::TConfigurationNode settings_node = argos::GetNode(node, "settings");

	argos::GetNodeAttribute(settings_node, "MaxSimTimeInSeconds", MaxSimTime);

	MaxSimTime *= GetSimulator().GetPhysicsEngine("dyn2d").GetInverseSimulationClockTick();

	argos::GetNodeAttribute(settings_node, "MaxSimCounter", MaxSimCounter);
	argos::GetNodeAttribute(settings_node, "VariableFoodPlacement", VariableFoodPlacement);
    argos::GetNodeAttribute(settings_node, "VaryForwardSpeedFlag", VaryForwardSpeedFlag);
    argos::GetNodeAttribute(settings_node, "VaryCapacityFlag", VaryCapacityFlag);
	argos::GetNodeAttribute(settings_node, "OutputData", OutputData);
	argos::GetNodeAttribute(settings_node, "DrawIDs", DrawIDs);
	argos::GetNodeAttribute(settings_node, "DrawTrails", DrawTrails);
	argos::GetNodeAttribute(settings_node, "DrawTargetRays", DrawTargetRays);
	argos::GetNodeAttribute(settings_node, "FoodDistribution", FoodDistribution);
	argos::GetNodeAttribute(settings_node, "FoodItemCount", FoodItemCount);
	argos::GetNodeAttribute(settings_node, "PowerlawFoodUnitCount", PowerlawFoodUnitCount);
	argos::GetNodeAttribute(settings_node, "NumberOfClusters", NumberOfClusters);
	argos::GetNodeAttribute(settings_node, "ClusterWidthX", ClusterWidthX);
	argos::GetNodeAttribute(settings_node, "ClusterWidthY", ClusterWidthY);
    argos::GetNodeAttribute(settings_node, "FoodRadius", FoodRadius);
	argos::GetNodeAttribute(settings_node, "NestRadius", NestRadius);
	argos::GetNodeAttribute(settings_node, "NestElevation", NestElevation);
    argos::GetNodeAttribute(settings_node, "BacktrackDelivery", BacktrackDelivery);
    argos::GetNodeAttribute(settings_node, "NumOfBranches", numBranch);
    
    // calculate the forage range and compensate for the robot's radius of 0.085m
	argos::CVector3 ArenaSize = GetSpace().GetArenaSize();
	argos::Real rangeX = (ArenaSize.GetX() / 2.0) - 0.085;
	argos::Real rangeY = (ArenaSize.GetY() / 2.0) - 0.085;
	ForageRangeX.Set(-rangeX, rangeX);
	ForageRangeY.Set(-rangeY, rangeY);

    ArenaWidth = ArenaSize[0];
    
    string PosStrRegionNest;
    size_t numNestsInPreviousLevels =0, level=0; 
    size_t levelDiff;    
    for(int i=0; i<1024; i++)
    {
	    PosStrRegionNest = "NestPosition_"+ to_string(i);
	    if(argos::NodeAttributeExists(settings_node, PosStrRegionNest))
	    {
<<<<<<< HEAD
		argos::GetNodeAttribute(settings_node, PosStrRegionNest, NestPosition);
                Nests[i] = Nest(NestPosition);
        	Nests[i].SetNestIdx(i);
               if(i==0)
	       {
		    numNestsInPreviousLevels++;
		    Nests[i].SetLevel(level);
	       level++;
	       }
	       else
	       {
               levelDiff = (i - numNestsInPreviousLevels)/pow(numBranch, level); 	
               if(levelDiff == 1)
               {
                   numNestsInPreviousLevels += pow(numBranch, level);
                   level++;
                }
	         Nests[i].SetLevel(level);
	       }
            
           Nests[i].SetParentNestIdx_with_backtrack(i, numBranch, &Nests);
           if(!BacktrackDelivery){
               size_t parent_id = Nests[i].GetParentNestIdx();    
               vector<Nest*> parentNests;
               size_t gens = 1; //we only consider two generations of parents
               parentNests.push_back(&Nests[parent_id]);
               while (parent_id != 0 && gens < 2){
                   parent_id = Nests[parent_id].GetParentNestIdx();
                   parentNests.push_back(&Nests[parent_id]);
                   gens++;
                   }//end while
               Nests[i].SetParentNestIdx_no_backtrack(parentNests); 
           }//end if
         }// end if
     }//end for
     
    //set capacity for delivery robots
    size_t revLevel =0;
    if(!BacktrackDelivery){
        if(numBranch == 4){
            numBranch = 3;
            }
        /*if(numBranch == 9){
            numBranch = 5;
           }*/
    }
    if (Nests.size() == 1) level =0;
    
    Real basicWidth = 1.0;
    ActualArenaWidth = ArenaWidth;
    if(Nests[0].GetLocation().GetX() < -1)//quad arena
    {
        ActualArenaWidth = ArenaWidth*2;
=======
			argos::GetNodeAttribute(settings_node, PosStrRegionNest, NestPosition);
			Nests.push_back(Nest(NestPosition));
        	Nests[i].SetRegionFlag(1);
        	
		}
		else if(argos::NodeAttributeExists(settings_node, PosStrDepot))
	    {
			argos::GetNodeAttribute(settings_node, PosStrDepot, NestPosition);
			Nests.push_back(Nest(NestPosition));
        	Nests[i].SetRegionFlag(0);
		}
        
        Nests[i].SetNestIdx(i);
        //Nests[i].SetRadius(i);
        //cylId = "cyl"+to_string(i);
        //Cylinders.push_back(dynamic_cast<CCylinderEntity&>(GetSpace().GetEntity(cylId)));
>>>>>>> development
    }
    ostringstream arena_width;
    arena_width << GetSpace().GetArenaSize()[0];
         
    string header = "./results/MPFA_transport_" +arena_width.str()+"by"+arena_width.str()+"_";
    
    //size_t initCapcity = 1;
    size_t initCapcity = 4;
    ofstream CapacityDataOutput((header+"CapacityData.txt").c_str(), ios::app);
    
    for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){
        revLevel = level - it->second.GetLevel();
        //argos::LOG<<"revLevel="<<revLevel<<endl;
        if(VaryCapacityFlag){//vary capacity
            it->second.SetDeliveryCapacity(initCapcity*pow(sqrt(numBranch)*numBranch, revLevel));//initial capacity is 4
        }
        else{
            it->second.SetDeliveryCapacity(initCapcity);
            }
        it->second.SetNestRadius(revLevel, NestRadius);
        //it->second.SetNestRadius(NestRadius, ArenaWidth, Nests.size());
        CapacityDataOutput<<it->second.GetDeliveryCapacity()<<" ";
    }
            
	CapacityDataOutput<<"\n";
    CapacityDataOutput.close();
        
        	
    //NestRadiusSquared = NestRadius*NestRadius;
	FoodRadiusSquared = FoodRadius*FoodRadius;
        //Number of distributed foods
    if (FoodDistribution == 1){
        NumDistributedFood = ClusterWidthX*ClusterWidthY*NumberOfClusters;
    }
    else{
        NumDistributedFood = FoodItemCount;  
    }

 
    
	// Send a pointer to this loop functions object to each controller.
	argos::CSpace::TMapPerType& footbots = GetSpace().GetEntitiesByType("foot-bot");
	argos::CSpace::TMapPerType::iterator it;
    Num_robots = footbots.size();
    argos::LOG<<"Number of robots="<<Num_robots<<endl;
    for(it = footbots.begin(); it != footbots.end(); it++) {
   	    argos::CFootBotEntity& footBot = *argos::any_cast<argos::CFootBotEntity*>(it->second);
	    BaseController& c = dynamic_cast<BaseController&>(footBot.GetControllableEntity().GetController());
	    MPFA_controller& c2 = dynamic_cast<MPFA_controller&>(c);
        c2.SetLoopFunctions(this);
        if(c2.GetId().compare(0, 1, "D")==0){ //check wether there are delivering robots or not 
            DeliveryFlag = 1;
        }
	}
    SetFoodDistribution();
 ForageList.clear(); //qilu 09/13/2016
 	
    last_time_in_minutes=0; //qilu 09/13/2016
}


void MPFA_loop_functions::Reset() {
	   if(VariableFoodPlacement == 0) {
		      RNG->Reset();
	   }

    GetSpace().Reset();
    GetSpace().GetFloorEntity().Reset();
    MaxSimCounter = SimCounter;
    SimCounter = 0;
    score = 0;
   
    FoodList.clear();
    FoodColoringList.clear();
	
    TargetRayList.clear();
    
    SetFoodDistribution();
    for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){
      it->second.FidelityList.clear();
      it->second.DensityOnFidelity.clear(); //qilu 09/11/2016
      it->second.PheromoneList.clear();
      }
    
    argos::CSpace::TMapPerType& footbots = GetSpace().GetEntitiesByType("foot-bot");
    argos::CSpace::TMapPerType::iterator it;
   
    Num_robots = footbots.size();
    for(argos::CSpace::TMapPerType::iterator it = footbots.begin(); it != footbots.end(); it++) {
        argos::CFootBotEntity& footBot = *argos::any_cast<argos::CFootBotEntity*>(it->second);
        BaseController& c = dynamic_cast<BaseController&>(footBot.GetControllableEntity().GetController());
        MPFA_controller& c2 = dynamic_cast<MPFA_controller&>(c);
        MoveEntity(footBot.GetEmbodiedEntity(), c2.GetStartPosition(), argos::CQuaternion(), false);
    }
}

void MPFA_loop_functions::PreStep() {
    SimTime++;
    curr_time_in_minutes = getSimTimeInSeconds()/60.0;
    if(curr_time_in_minutes - last_time_in_minutes==1){
		      
        ForageList.push_back(currNumCollectedFood - lastNumCollectedFood);
        lastNumCollectedFood = currNumCollectedFood;
        last_time_in_minutes++;
    }

    UpdatePheromoneList();
    if(GetSpace().GetSimulationClock() > ResourceDensityDelay) //update the sensed targets color to be black.
        for(size_t i = 0; i < FoodColoringList.size(); i++)
            FoodColoringList[i] = argos::CColor::BLACK; 	
  	
    if(FoodList.size() == 0) {
        for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){
            it->second.PheromoneList.clear();//qilu 09/11/16
            it->second.FidelityList.clear();//qilu 09/11/16
            it->second.DensityOnFidelity.clear(); //qilu 09/11/2016
        }
        TargetRayList.clear();//qilu 07/13 
    }
}

void MPFA_loop_functions::PostStep() {
	// nothing... yet...
}


bool MPFA_loop_functions::IsExperimentFinished() {
	bool isFinished = false;

	if(FoodList.size() == 0 || GetSpace().GetSimulationClock() >= MaxSimTime) {
		isFinished = true;
	}
        if(score >= NumDistributedFood){
		isFinished = true;
	}


	if(isFinished == true && MaxSimCounter > 1) {
		size_t newSimCounter = SimCounter + 1;
		size_t newMaxSimCounter = MaxSimCounter - 1;
		PostExperiment();
		Reset();

		SimCounter    = newSimCounter;
		MaxSimCounter = newMaxSimCounter;
		isFinished    = false;
	}

	return isFinished;
}

void MPFA_loop_functions::PostExperiment() {
	  
     printf("%d, %f, %lu\n", score, getSimTimeInSeconds(), RandomSeed);
       
                  
    if (PrintFinalScore == 1) {
	 string type="", backtrack="";
         if (FoodDistribution == 0) type = "random";
        else if (FoodDistribution == 1) type = "cluster";
        else type = "powerlaw";
         if(!BacktrackDelivery) backtrack = "no_backtrack";
         else  backtrack = "with_backtrack";
         
         
         ostringstream num_tag;         
         num_tag << FoodItemCount;
	 

        ostringstream num_nests;
        num_nests <<  Nests.size();
    
        ostringstream num_robots;
        num_robots <<  Num_robots;
         
        ostringstream arena_width;
        arena_width << ActualArenaWidth;
        
	ostringstream varySpeed;
        varySpeed << VaryForwardSpeedFlag;
        
        ostringstream quardArena;
        if(Nests[0].GetLocation().GetX() < -1){ //the central nest is not in the center, this is a quard arena
             quardArena << 1;
         }
         else{
             quardArena << 0;
             }
        
        ostringstream delivery;
        delivery << DeliveryFlag;
        
        string header = "./results/"+type+"_MPFA_transport_" + backtrack +"_n"+num_nests.str()+"_r"+num_robots.str()+"_tag"+num_tag.str()+"_"+arena_width.str()+"by"+arena_width.str()+"_vary_speed_"+ varySpeed.str()+"_quard_arena_" + quardArena.str() +"_";
        
        unsigned int ticks_per_second = GetSimulator().GetPhysicsEngine("Default").GetInverseSimulationClockTick();
        
       /* Real total_travel_time=0;
        Real total_search_time=0;
        ofstream travelSearchTimeDataOutput((header+"TravelSearchTimeData.txt").c_str(), ios::app);
        */
        
        argos::CSpace::TMapPerType& footbots = GetSpace().GetEntitiesByType("foot-bot");
         
        for(argos::CSpace::TMapPerType::iterator it = footbots.begin(); it != footbots.end(); it++) {
            argos::CFootBotEntity& footBot = *argos::any_cast<argos::CFootBotEntity*>(it->second);
            BaseController& c = dynamic_cast<BaseController&>(footBot.GetControllableEntity().GetController());
            MPFA_controller& c2 = dynamic_cast<MPFA_controller&>(c);
            CollisionTime += c2.GetCollisionTime();
            
            /*if(c2.GetStatus() == "SEARCHING"){
                total_search_time += SimTime-c2.GetTravelingTime();
                total_travel_time += c2.GetTravelingTime();
	        }
            else{
		        total_search_time += c2.GetSearchingTime();
		        total_travel_time += SimTime-c2.GetSearchingTime();
            }*/
        }
        //travelSearchTimeDataOutput<< total_travel_time/ticks_per_second<<", "<<total_search_time/ticks_per_second<<endl;
        //travelSearchTimeDataOutput.close();
        
        ofstream dataOutput( (header+ "iAntTagData.txt").c_str(), ios::app);
        // output to file
        if(dataOutput.tellp() == 0) {
            dataOutput << "tags_collected, collisions_in_seconds, time_in_minutes, random_seed\n";
        }
    
        dataOutput << Score() << ", "<<CollisionTime/(2*ticks_per_second)<< ", "<< curr_time_in_minutes <<", "<<RandomSeed<<endl;
        dataOutput.close();
    
        ofstream forageDataOutput((header+"ForageData.txt").c_str(), ios::app);
        
        for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){
            forageDataOutput<<it->second.FoodList.size()<<" ";
            }
        forageDataOutput<<"\n";
        forageDataOutput.close();
        Nests.clear();
      }  

}


argos::CColor MPFA_loop_functions::GetFloorColor(const argos::CVector2 &c_pos_on_floor) {
	return argos::CColor::WHITE;
}

void MPFA_loop_functions::UpdatePheromoneList() {
	// Return if this is not a tick that lands on a 0.5 second interval
	if ((int)(GetSpace().GetSimulationClock()) % ((int)(GetSimulator().GetPhysicsEngine("dyn2d").GetInverseSimulationClockTick()) / 2) != 0) return;
	
	std::vector<Pheromone> new_p_list; 

	argos::Real t = GetSpace().GetSimulationClock() / GetSimulator().GetPhysicsEngine("dyn2d").GetInverseSimulationClockTick();
         
   for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){  
	    for(size_t i = 0; i < it->second.PheromoneList.size(); i++) {

		        it->second.PheromoneList[i].Update(getSimTimeInSeconds());//qilu 09/24/2016 let it decays in every second
		        if(it->second.PheromoneList[i].IsActive()) {
			          new_p_list.push_back(it->second.PheromoneList[i]);
		        }
      }
      it->second.PheromoneList.clear();
      it->second.PheromoneList = new_p_list;
      new_p_list.clear();//qilu 09/08/2016
 }
}
void MPFA_loop_functions::SetFoodDistribution() {
	switch(FoodDistribution) {
		case 0:
			RandomFoodDistribution();
			break;
		case 1:
			ClusterFoodDistribution();
			break;
		case 2:
			PowerLawFoodDistribution();
			break;
  case 3:
			GaussianFoodDistribution();
			break;
		default:
			argos::LOGERR << "ERROR: Invalid food distribution in XML file.\n";
	}
}

void MPFA_loop_functions::RandomFoodDistribution() {
	FoodList.clear();
        FoodColoringList.clear();
	argos::CVector2 placementPosition;

	for(size_t i = 0; i < FoodItemCount; i++) {
		placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));

		while(IsOutOfBounds(placementPosition, 1, 1)) {
			placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));
		}

		FoodList.push_back(placementPosition);
		FoodColoringList.push_back(argos::CColor::BLACK);
	}
}

void MPFA_loop_functions::GaussianFoodDistribution() {
 FoodList.clear();
 FoodColoringList.clear();
 argos::CVector2 centerPosition;
 argos::CVector2 placementPosition;
 
 for(size_t i = 0; i < NumberOfClusters; i++) {
		  centerPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));

		  while(IsOutOfArena(centerPosition) || IsCollidingWithNest(centerPosition, NestRadius)) {
			  centerPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));
		  }
    for(size_t i=0; i<32; i++){
       placementPosition.Set(centerPosition.GetX() + RNG->Gaussian(0.15, 0), centerPosition.GetY() + RNG->Gaussian(0.15, 0));
       while(IsOutOfArena(placementPosition, FoodRadius)){
            placementPosition.Set(centerPosition.GetX() + RNG->Gaussian(0.15, 0), centerPosition.GetY() + RNG->Gaussian(0.15, 0));
        }
       
       FoodList.push_back(placementPosition);
       FoodColoringList.push_back(argos::CColor::BLACK);
     }
   }
 }
 
void MPFA_loop_functions::ClusterFoodDistribution() {
    FoodList.clear();
    FoodColoringList.clear();
	assert(FoodItemCount == NumberOfClusters*ClusterWidthX*ClusterWidthY);
	
	argos::Real     foodOffset  = 3.0 * FoodRadius;
	size_t          foodToPlace = NumberOfClusters * ClusterWidthX * ClusterWidthY;
	size_t          foodPlaced = 0;
	argos::CVector2 placementPosition;

	FoodItemCount = foodToPlace;

	for(size_t i = 0; i < NumberOfClusters; i++) {
		placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));

		while(IsOutOfBounds(placementPosition, ClusterWidthY, ClusterWidthX)) {
			placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));
		}

		for(size_t j = 0; j < ClusterWidthY; j++) {
			for(size_t k = 0; k < ClusterWidthX; k++) {
				foodPlaced++;
				FoodList.push_back(placementPosition);
				FoodColoringList.push_back(argos::CColor::BLACK);
				placementPosition.SetX(placementPosition.GetX() + foodOffset);
			}

			placementPosition.SetX(placementPosition.GetX() - (ClusterWidthX * foodOffset));
			placementPosition.SetY(placementPosition.GetY() + foodOffset);
		}
	}
}


void MPFA_loop_functions::PowerLawFoodDistribution() {
 FoodList.clear();
    FoodColoringList.clear();
	argos::Real foodOffset     = 3.0 * FoodRadius;
	size_t      foodPlaced     = 0;
	size_t      powerLawLength = 1;
	size_t      maxTrials      = 200;
	size_t      trialCount     = 0;

	std::vector<size_t> powerLawClusters;
	std::vector<size_t> clusterSides;
	argos::CVector2     placementPosition;

    //-----Wayne: Dertermine PowerRank and food per PowerRank group
    size_t priorPowerRank = 0;
    size_t power4 = 0;
    size_t FoodCount = 0;
    size_t TotalFoodCount = 0;
    size_t DistributedCount = 0;
    size_t diffFoodCount = 0;
    size_t singleClusterCount = 0;
    size_t otherClusterCount = 0;
    size_t modDiff = 0;
    size_t TotalDistributedFood = 0;
    
    //Wayne: priorPowerRank is determined by what power of 4
    //plus a multiple of power4 increases the food count passed required count
    //this is how powerlaw works to divide up food into groups
    //the number of groups is the powerrank
    while (TotalFoodCount < FoodItemCount){
	if (FoodItemCount - TotalFoodCount < PowerlawFoodUnitCount){
	    DistributedCount = FoodItemCount - TotalFoodCount;
	}
	else DistributedCount = PowerlawFoodUnitCount;
	    TotalFoodCount += DistributedCount;
		
        while (FoodCount < DistributedCount){
        priorPowerRank++;
        power4 = pow (4.0, priorPowerRank);
        FoodCount = power4 + priorPowerRank * power4;
    }
    
    //Wayne: Actual powerRank is prior + 1
    PowerRank = priorPowerRank + 1;
    
    //Wayne: Equalizes out the amount of food in each group, with the 1 cluster group taking the
    //largest loss if not equal, when the powerrank is not a perfect fit with the amount of food.
        diffFoodCount = FoodCount - DistributedCount;
    modDiff = diffFoodCount % PowerRank;
    
        if (DistributedCount % PowerRank == 0){
            singleClusterCount = DistributedCount / PowerRank;
        otherClusterCount = singleClusterCount;
    }
    else {
            otherClusterCount = DistributedCount / PowerRank + 1;
        singleClusterCount = otherClusterCount - modDiff;
    }
    //End of PowerRank and food per PowerRank group
    
	for(size_t i = 0; i < PowerRank; i++) {
		powerLawClusters.push_back(powerLawLength * powerLawLength);
		powerLawLength *= 2;
	}

	for(size_t i = 0; i < PowerRank; i++) {
		powerLawLength /= 2;
		clusterSides.push_back(powerLawLength);
	}
    /*Wayne: Modified to break from loops if food count reached.
     Provides support for unequal clusters and odd food numbers.
     Necessary for DustUp and Jumble Distribution changes. */
    
	for(size_t h = 0; h < powerLawClusters.size(); h++) {
		for(size_t i = 0; i < powerLawClusters[h]; i++) {
			placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));

			while(IsOutOfBounds(placementPosition, clusterSides[h], clusterSides[h])) {
				trialCount++;
				placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));

				if(trialCount > maxTrials) {
					argos::LOGERR << "PowerLawDistribution(): Max trials exceeded!\n";
					break;
				}
			}

            trialCount = 0;
			for(size_t j = 0; j < clusterSides[h]; j++) {
				for(size_t k = 0; k < clusterSides[h]; k++) {
					foodPlaced++;
					FoodList.push_back(placementPosition);
					FoodColoringList.push_back(argos::CColor::BLACK);
					placementPosition.SetX(placementPosition.GetX() + foodOffset);
                    if (foodPlaced == singleClusterCount + h * otherClusterCount) break;
				}

				placementPosition.SetX(placementPosition.GetX() - (clusterSides[h] * foodOffset));
				placementPosition.SetY(placementPosition.GetY() + foodOffset);
                if (foodPlaced == singleClusterCount + h * otherClusterCount) break;
			}
            if (foodPlaced == singleClusterCount + h * otherClusterCount) break;
	        } //end for
        }//end for

	TotalDistributedFood += foodPlaced;
	FoodCount = 0;
	priorPowerRank =0;
	foodPlaced =0;
	powerLawClusters.clear();
	clusterSides.clear();
    }
    assert(TotalDistributedFood == FoodItemCount);
}
 
bool MPFA_loop_functions::IsOutOfArena(argos::CVector2 p){
  argos::Real x = p.GetX();
	 argos::Real y = p.GetY();
  
  if((x < ForageRangeX.GetMin())
			|| (x > ForageRangeX.GetMax()) ||
			(y < ForageRangeY.GetMin()) ||
			(y > ForageRangeY.GetMax()))		return true;
    return false;
   }
 
 bool MPFA_loop_functions::IsOutOfArena(argos::CVector2 p, argos::Real radius){
  argos::Real x_min = p.GetX() - radius;
	 argos::Real x_max = p.GetX() + radius;

	 argos::Real y_min = p.GetY() - radius;
	 argos::Real y_max = p.GetY() + radius;
  if((x_min < (ForageRangeX.GetMin() + radius))
			|| (x_max > (ForageRangeX.GetMax() - radius)) ||
			(y_min < (ForageRangeY.GetMin() + radius)) ||
			(y_max > (ForageRangeY.GetMax() - radius)))		return true;
     return false;
   
   }
   
bool MPFA_loop_functions::IsOutOfBounds(argos::CVector2 p, argos::Real radius){ //qilu 07/26/2016
  /*
  argos::Real x_min = p.GetX() - radius;
	 argos::Real x_max = p.GetX() + radius;

	 argos::Real y_min = p.GetY() - radius;
	 argos::Real y_max = p.GetY() + radius;
  if((x_min < (ForageRangeX.GetMin() + radius))
			|| (x_max > (ForageRangeX.GetMax() - radius)) ||
			(y_min < (ForageRangeY.GetMin() + radius)) ||
			(y_max > (ForageRangeY.GetMax() - radius)))		return true;*/
  if (IsOutOfArena(p, radius)) return true;
  
  if(IsCollidingWithFood(p, radius)) return true;
  
  if(IsCollidingWithNest(p, radius)) return true;

  return false;
}
bool MPFA_loop_functions::IsOutOfBounds(argos::CVector2 p, size_t length, size_t width) {
	argos::CVector2 placementPosition = p;

	argos::Real foodOffset   = 3.0 * FoodRadius;
	argos::Real widthOffset  = 3.0 * FoodRadius * (argos::Real)width;
	argos::Real lengthOffset = 3.0 * FoodRadius * (argos::Real)length;

	argos::Real x_min = p.GetX() - FoodRadius;
	argos::Real x_max = p.GetX() + FoodRadius + widthOffset;

	argos::Real y_min = p.GetY() - FoodRadius;
	argos::Real y_max = p.GetY() + FoodRadius + lengthOffset;

	if((x_min < (ForageRangeX.GetMin() + FoodRadius))
			|| (x_max > (ForageRangeX.GetMax() - FoodRadius)) ||
			(y_min < (ForageRangeY.GetMin() + FoodRadius)) ||
			(y_max > (ForageRangeY.GetMax() - FoodRadius)))
	{
		return true;
	}

	for(size_t j = 0; j < length; j++) {
		for(size_t k = 0; k < width; k++) {
			if(IsCollidingWithFood(placementPosition)) return true;
			if(IsCollidingWithNest(placementPosition)) return true;
			placementPosition.SetX(placementPosition.GetX() + foodOffset);
		}

		placementPosition.SetX(placementPosition.GetX() - (width * foodOffset));
		placementPosition.SetY(placementPosition.GetY() + foodOffset);
	}

	return false;
}

bool MPFA_loop_functions::IsCollidingWithNest(argos::CVector2 p, argos::Real radius){ //qilu 07/26/2016 for nest
 argos::Real nestRadiusPlusBuffer = NestRadius + radius;
	argos::Real NRPB_squared = nestRadiusPlusBuffer * nestRadiusPlusBuffer;
 if(Nests.size()==0) return false; //qilu 07/26/2016
 
 for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){
          if( (p - it->second.GetLocation()).SquareLength() < NRPB_squared) return true;
  }
    return false;
 }
  
bool MPFA_loop_functions::IsCollidingWithNest(argos::CVector2 p) {
	argos::Real nestRadiusPlusBuffer = NestRadius + FoodRadius;
	argos::Real NRPB_squared = nestRadiusPlusBuffer * nestRadiusPlusBuffer;

 if(Nests.size()==0) return false; //qilu 07/26/2016
	//return ((p - NestPosition).SquareLength() < NRPB_squared);
 
 for(map<int, Nest>:: iterator it= Nests.begin(); it!= Nests.end(); it++){
     if( (p - it->second.GetLocation()).SquareLength() < NRPB_squared) return true;
  }
  return false;
}

bool MPFA_loop_functions::IsCollidingWithFood(argos::CVector2 p, argos::Real radius){ //qilu 07/26/2016 for nest
 argos::Real foodRadiusPlusBuffer = FoodRadius + radius;
	argos::Real FRPB_squared = foodRadiusPlusBuffer * foodRadiusPlusBuffer;
 
 for(size_t i=0; i < FoodList.size(); i++){ //qilu 07/26/2016
      if( (p - FoodList[i]).SquareLength() < FRPB_squared) return true;
  }
    return false;
}

bool MPFA_loop_functions::IsCollidingWithFood(argos::CVector2 p) {
	argos::Real foodRadiusPlusBuffer = 2.0 * FoodRadius;
	argos::Real FRPB_squared = foodRadiusPlusBuffer * foodRadiusPlusBuffer;

	for(size_t i = 0; i < FoodList.size(); i++) {
		if((p - FoodList[i]).SquareLength() < FRPB_squared) return true;
	}

	return false;
}

unsigned int MPFA_loop_functions::getNumberOfRobots() {
	return GetSpace().GetEntitiesByType("foot-bot").size();
}

unsigned int MPFA_loop_functions::getNumberOfDepots() {
	return Nests.size();
}



double MPFA_loop_functions::getProbabilityOfSwitchingToSearching() {
	return ProbabilityOfSwitchingToSearching;
}

double MPFA_loop_functions::getProbabilityOfReturningToNest() {
	return ProbabilityOfReturningToNest;
}

// Value in Radians
double MPFA_loop_functions::getUninformedSearchVariation() {
	return UninformedSearchVariation.GetValue();
}

double MPFA_loop_functions::getRateOfInformedSearchDecay() {
	return RateOfInformedSearchDecay;
}

double MPFA_loop_functions::getRateOfSiteFidelity() {
	return RateOfSiteFidelity;
}

double MPFA_loop_functions::getRateOfLayingPheromone() {
	return RateOfLayingPheromone;
}

double MPFA_loop_functions::getRateOfPheromoneDecay() {
	return RateOfPheromoneDecay;
}

argos::Real MPFA_loop_functions::getSimTimeInSeconds() {
	unsigned int ticks_per_second = GetSimulator().GetPhysicsEngine("Default").GetInverseSimulationClockTick();
	float sim_time = GetSpace().GetSimulationClock();
	return sim_time/ticks_per_second;
}

void MPFA_loop_functions::SetTrial(unsigned int v) {
}

void MPFA_loop_functions::setScore(unsigned int s) {
	score = s;
    if (score >= NumDistributedFood) {
		PostExperiment();
	}
}

int MPFA_loop_functions::Score() {	
	return score;
}

void MPFA_loop_functions::increaseNumDistributedFoodByOne(){
    NumDistributedFood++;
}
void MPFA_loop_functions::ConfigureFromGenome(Real* g)
{
	// Assign genome generated by the GA to the appropriate internal variables.
	ProbabilityOfSwitchingToSearching = g[0];
	ProbabilityOfReturningToNest      = g[1];
	UninformedSearchVariation.SetValue(g[2]);
	RateOfInformedSearchDecay         = g[3];
	RateOfSiteFidelity                = g[4];
	RateOfLayingPheromone             = g[5];
	RateOfPheromoneDecay              = g[6];
}

REGISTER_LOOP_FUNCTIONS(MPFA_loop_functions, "MPFA_loop_functions")
