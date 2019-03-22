#include "MPFA_controller.h"
#include <unistd.h>

MPFA_controller::MPFA_controller() :
	RNG(argos::CRandom::CreateRNG("argos")),
	isInformed(false),
	isHoldingFood(false),
	numHeldFood(0),
	isUsingSiteFidelity(false),
	isGivingUpSearch(false),
	ResourceDensity(0),
	MaxTrailSize(50),
	SearchTime(0),
	MPFA_state(DEPARTING),
	LoopFunctions(NULL),
    ClosestNest(NULL),
    TargetNest(NULL), 
	survey_count(0),
	isUsingPheromone(0),
    SiteFidelityPosition(1000, 1000), 
    searchingTime(0),
    travelingTime(0),
    startTime(0),
    m_pcLEDs(NULL),
    updateFidelity(false)
{
}

void MPFA_controller::Init(argos::TConfigurationNode &node) {
    compassSensor   = GetSensor<argos::CCI_PositioningSensor>("positioning");
	wheelActuator   = GetActuator<argos::CCI_DifferentialSteeringActuator>("differential_steering");
	proximitySensor = GetSensor<argos::CCI_FootBotProximitySensor>("footbot_proximity");
	argos::TConfigurationNode settings = argos::GetNode(node, "settings");

	argos::GetNodeAttribute(settings, "FoodDistanceTolerance",   FoodDistanceTolerance);
	argos::GetNodeAttribute(settings, "TargetDistanceTolerance", TargetDistanceTolerance);
	argos::GetNodeAttribute(settings, "NestDistanceTolerance", NestDistanceTolerance);
	argos::GetNodeAttribute(settings, "NestAngleTolerance",    NestAngleTolerance);
	argos::GetNodeAttribute(settings, "TargetAngleTolerance",    TargetAngleTolerance);
	argos::GetNodeAttribute(settings, "SearchStepSize",          SearchStepSize);
	argos::GetNodeAttribute(settings, "RobotForwardSpeed",       RobotForwardSpeed);
	argos::GetNodeAttribute(settings, "RobotRotationSpeed",      RobotRotationSpeed);
	argos::GetNodeAttribute(settings, "ResultsDirectoryPath",      results_path);
	argos::GetNodeAttribute(settings, "DestinationNoiseStdev",      DestinationNoiseStdev);
	argos::GetNodeAttribute(settings, "PositionNoiseStdev",      PositionNoiseStdev);
    //argos::GetNodeAttribute(settings, "QuardArena",      QuardArena);

	argos::CVector2 p(GetPosition());
	SetStartPosition(argos::CVector3(p.GetX(), p.GetY(), 0.0));

    FoodDistanceTolerance *= FoodDistanceTolerance;
	SetIsHeadingToNest(true);
	SetTarget(p);
	
    m_pcLEDs   = GetActuator<CCI_LEDsActuator>("leds");
    controllerID= GetId();//qilu 07/26/2016
    assert(controllerID.compare(0, 1, "D") != 0);
    m_pcLEDs->SetAllColors(CColor::GREEN);
}

void MPFA_controller::ControlStep() {
	/*
	ofstream log_output_stream;
	log_output_stream.open("MPFA_log.txt", ios::app);

	// depart from nest after food drop off or simulation start
	if (isHoldingFood) log_output_stream << "(Carrying) ";
	
	switch(MPFA_state)  {
		case DEPARTING:
			if (isUsingSiteFidelity) {
				log_output_stream << "DEPARTING (Fidelity): "
					<< GetTarget().GetX() << ", " << GetTarget().GetY()
					<< endl;
			} else if (isInformed) {
				log_output_stream << "DEPARTING (Waypoint): "
				<< GetTarget().GetX() << ", " << GetTarget().GetY() << endl;
			} else {
				log_output_stream << "DEPARTING (Searching): "
				<< GetTarget().GetX() << ", " << GetTarget().GetY() << endl;
			}
			break;
		// after departing(), once conditions are met, begin searching()
		case SEARCHING:
			if (isInformed) log_output_stream << "SEARCHING: Informed" << endl;     
			else log_output_stream << "SEARCHING: UnInformed" << endl;
			break;
		// return to nest after food pick up or giving up searching()
		case RETURNING:
			log_output_stream << "RETURNING" << endl;
			break;
		case SURVEYING:
			log_output_stream << "SURVEYING" << endl;
			break;
		default:
			log_output_stream << "Unknown state" << endl;
	}
	*/

	// Add line so we can draw the trail

	CVector3 position3d(GetPosition().GetX(), GetPosition().GetY(), 0.00);
	CVector3 target3d(GetTarget().GetX(), GetTarget().GetY(), 0.00);
	CRay3 targetRay(target3d, position3d);
	//myTrail.push_back(targetRay);
	LoopFunctions->TargetRayList.push_back(targetRay);
	LoopFunctions->TargetRayColorList.push_back(TrailColor);

	previous_position = GetPosition();
    //UpdateTargetRayList();
	MPFA();
	Move();
}

void MPFA_controller::Reset() {
    total_targets_collected =0;
    isHoldingFood   = false;
    numHeldFood = 0;
    isInformed      = false;
    SearchTime      = 0;
    ResourceDensity = 0;
    collisionDelay = 0;
    
  	LoopFunctions->CollisionTime=0; //qilu 09/26/2016
	   //LoopFunctions->currCollisionTime =0; //qilu 09/26/2016
    //LoopFunctions->lastCollisionTime =0; //qilu 09/26/2016
    
    
    /* Set LED color */
    /* m_pcLEDs->SetAllColors(CColor::BLACK); //qilu 09/04 */
    SetTarget(ClosestNest->GetLocation()); //qilu 09/08
    SiteFidelityPosition = ClosestNest->GetLocation();//qilu 09/08
    
    TrailToShare.clear();
    TrailToFollow.clear();
}

bool MPFA_controller::IsHoldingFood() {
		return isHoldingFood;
}

bool MPFA_controller::IsUsingSiteFidelity() {
		return isUsingSiteFidelity;
}

void MPFA_controller::MPFA() {
	
	switch(MPFA_state) {
		// depart from nest after food drop off or simulation start
		case DEPARTING:
			//argos::LOG << "DEPARTING" << std::endl;
			Departing();
			break;
		// after departing(), once conditions are met, begin searching()
		case SEARCHING:
			//argos::LOG << "SEARCHING" << std::endl;
			if((SimulationTick() % (SimulationTicksPerSecond() / 2)) == 0) {
				Searching();
			}
			break;
		// return to nest after food pick up or giving up searching()
		case RETURNING:
			//argos::LOG << "RETURNING" << std::endl;
			Returning();
			break;
		case SURVEYING:
			//argos::LOG << "SURVEYING" << std::endl;
			Surveying();
			break;
	    case DEPOT_DELIVERING:
    		//SetIsHeadingToNest(true);
		    Delivering();
		    break;
	}
}

bool MPFA_controller::IsInTargetNest() {
        if ((GetPosition() - TargetNest->GetLocation()).SquareLength()<TargetNest->GetNestRadiusSquared()) {
            return true;
        }
  return false;
}


bool MPFA_controller::IsInTheNest() {
  if ((GetPosition() - ClosestNest->GetLocation()).SquareLength()<ClosestNest->GetNestRadiusSquared()) {
            return true;
        }
  return false;
}

void MPFA_controller::SetLoopFunctions(MPFA_loop_functions* lf) {
	LoopFunctions = lf;
    Real basicWidth = 1.0;
    
    if(lf->VaryForwardSpeedFlag == 1 && controllerID.compare(0, 1, "D")==0)
    {
        //if(lf->Nests[0].GetLocation().GetX() < -1)
        //{
        //    RobotForwardSpeed *= pow(lf->ArenaWidth*2/basicWidth, 1/3.0);
        //}
        //else{
            RobotForwardSpeed *= pow(lf->ActualArenaWidth/basicWidth, 1/3.0);
        //}
    }
	// Initialize the SiteFidelityPosition
    SiteFidelityPosition = CVector2(0,0); //qilu 07/26/2016

	// Create the output file here because it needs LoopFunctions
		
	// Name the results file with the current time and date
	time_t t = time(0);   // get time now
	struct tm * now = localtime(&t);
	stringstream ss;

	char hostname[1024];                                                   
	hostname[1023] = '\0';                    
	gethostname(hostname, 1023);  

	ss << "MPFA-"<<GIT_BRANCH<<"-"<<GIT_COMMIT_HASH<<"-"
		<< hostname << '-'
		<< getpid() << '-'
		<< (now->tm_year) << '-'
		<< (now->tm_mon + 1) << '-'
		<<  now->tm_mday << '-'
		<<  now->tm_hour << '-'
		<<  now->tm_min << '-'
		<<  now->tm_sec << ".csv";

		string results_file_name = ss.str();
		results_full_path = results_path+"/"+results_file_name;

	// Only the first robot should do this:	 
	if (GetId().compare("MPFA_0") == 0) {
		/*
		ofstream results_output_stream;
		results_output_stream.open(results_full_path, ios::app);
		results_output_stream << "NumberOfRobots, "
			<< "TargetDistanceTolerance, "
			<< "TargetAngleTolerance, "
			<< "FoodDistanceTolerance, "
			<< "RobotForwardSpeed, "
			<< "RobotRotationSpeed, "
			<< "RandomSeed, "
			<< "ProbabilityOfSwitchingToSearching, "
			<< "ProbabilityOfReturningToNest, "
			<< "UninformedSearchVariation, "   
			<< "RateOfInformedSearchDecay, "   
			<< "RateOfSiteFidelity, "          
			<< "RateOfLayingPheromone, "       
			<< "RateOfPheromoneDecay" << endl
			<< LoopFunctions->getNumberOfRobots() << ", "
			<< CSimulator::GetInstance().GetRandomSeed() << ", "  
			<< TargetDistanceTolerance << ", "
			<< TargetAngleTolerance << ", "
			<< FoodDistanceTolerance << ", "
			<< RobotForwardSpeed << ", "
			<< RobotRotationSpeed << ", "
			<< LoopFunctions->getProbabilityOfSwitchingToSearching() << ", "
			<< LoopFunctions->getProbabilityOfReturningToNest() << ", "
			<< LoopFunctions->getUninformedSearchVariation() << ", "
			<< LoopFunctions->getRateOfInformedSearchDecay() << ", "
			<< LoopFunctions->getRateOfSiteFidelity() << ", "
			<< LoopFunctions->getRateOfLayingPheromone() << ", "
			<< LoopFunctions->getRateOfPheromoneDecay()
			<< endl;
				
			results_output_stream.close();
		*/
	}

}




void MPFA_controller::Delivering(){
	//argos::LOG<<"controllerID = "<<controllerID<<", Delivering ..."<<endl;
	if(IsInTargetNest()) {
		isHoldingFood = false;
		
		//drop off the food and display in the nest 
		argos::CVector2 placementPosition;
        for(int i =0; i < numHeldFood; i++){
            
            placementPosition.Set(TargetNest->GetLocation().GetX()+RNG->Gaussian(TargetNest->GetNestRadius()/1.2, 0.5), TargetNest->GetLocation().GetY()+RNG->Gaussian(TargetNest->GetNestRadius()/1.2, 0.5));
          
            while((placementPosition-TargetNest->GetLocation()).SquareLength()>pow(LoopFunctions->NestRadius-LoopFunctions->FoodRadius, 2)){
              placementPosition.Set(TargetNest->GetLocation().GetX()+RNG->Gaussian(TargetNest->GetNestRadius()/1.2, 0.5), TargetNest->GetLocation().GetY()+RNG->Gaussian(TargetNest->GetNestRadius()/1.2, 0.5));
            }
            TargetNest->FoodList.push_back(placementPosition);
		}
        if(TargetNest->GetNestIdx() == 0) //center nest
        {
            total_targets_collected += numHeldFood;
            //ClosestNest->num_collected_tags++;
            LoopFunctions->setScore(total_targets_collected);
            LoopFunctions->currNumCollectedFood += numHeldFood;  
        }
        
        //TargetNest->num_collected_tags++;
        SetIsHeadingToNest(true);
        SetTarget(ClosestNest->GetLocation()); 
        MPFA_state = DEPOT_RETURNING;  
        numHeldFood = 0;   
    }
	
}


/*void MPFA_controller::Idling()
{
	//argos::LOG<<"Idling ..."<<endl;
	if(ClosestNest == NULL){
		SetClosestNest();
		int parent_idx = ClosestNest->GetParentNestIdx();
		TargetNest = &LoopFunctions->Nests[parent_idx];
	}
	
	size_t numCollectedFood = ClosestNest->FoodList.size();
	size_t packSize = ClosestNest->GetDeliveryCapacity(); 
    if(numCollectedFood >= packSize){
		ClosestNest->FoodList.erase(ClosestNest->FoodList.begin(), ClosestNest->FoodList.begin()+packSize);
		isHoldingFood = true;
		numHeldFood = packSize;
		SetIsHeadingToNest(true);
        SetTarget(TargetNest->GetLocation());
	    MPFA_state = DEPOT_DELIVERING;	        
	}	
}



void MPFA_controller::DepotReturning(){
	if(IsInTheNest()) {
		 MPFA_state = DEPOT_IDLING; 
	}		
}
*/

void MPFA_controller::Departing()
{
    //LOG<<"Departing..."<<endl;
    if(ClosestNest == NULL){
		SetClosestNest();
	}
	argos::Real distanceToTarget = (GetPosition() - GetTarget()).Length();
    argos::Real randomNumber = RNG->Uniform(argos::CRange<argos::Real>(0.0, 1.0));

    /*
    ofstream log_output_stream;
    log_output_stream.open("MPFA_log.txt", ios::app);
    log_output_stream << "Distance to target: " << distanceToTarget << endl;
    log_output_stream << "Current Position: " << GetPosition() << ", Target: " << GetTarget() << endl;
    log_output_stream.close();
    */

	/* When not informed, continue to travel until randomly switching to the searching state. */
    if((SimulationTick() % (SimulationTicksPerSecond() / 2)) == 0) {
       if(isInformed == false){
           if(SimulationTick()%(5*SimulationTicksPerSecond())==0 && randomNumber < LoopFunctions->ProbabilityOfSwitchingToSearching){
		//LOG<<"Switch to search..."<<endl;
		 Stop();
		 SearchTime = 0;
		 MPFA_state = SEARCHING;
		 travelingTime+=SimulationTick()-startTime;//qilu 10/22
		 startTime = SimulationTick();//qilu 10/22
		
		 argos::Real USV = LoopFunctions->UninformedSearchVariation.GetValue();
		 argos::Real rand = RNG->Gaussian(USV);
		 argos::CRadians rotation(rand);
		 argos::CRadians angle1(rotation.UnsignedNormalize());
		 argos::CRadians angle2(GetHeading().UnsignedNormalize());
		 argos::CRadians turn_angle(angle1 + angle2);
		 argos::CVector2 turn_vector(SearchStepSize, turn_angle);
		 SetIsHeadingToNest(false);
                 //argos::LOG<<"heading to false in switching to search"<<endl;
		 SetTarget(turn_vector + GetPosition());
                 
		 }
		 else if(distanceToTarget < TargetDistanceTolerance){
		   SetRandomSearchLocation();
		 }
	}
     }
	
     /* Are we informed? I.E. using site fidelity or pheromones. */	
     if(isInformed && distanceToTarget < TargetDistanceTolerance) {
          //ofstream log_output_stream;
          //log_output_stream.open("MPFA_log.txt", ios::app);
          //log_output_stream << "Reached waypoint: " << SiteFidelityPosition << endl;
        
          SearchTime = 0;
          MPFA_state = SEARCHING;
          travelingTime+=SimulationTick()-startTime;//qilu 10/22
          startTime = SimulationTick();//qilu 10/22
            
          if(isUsingSiteFidelity) {
               isUsingSiteFidelity = false;
               SetFidelityList();
               //log_output_stream << "After SetFidelityList: " << SiteFidelityPosition << endl;
               //log_output_stream.close();
          }
     }


}

void MPFA_controller::Searching() {
 //LOG<<"Searching..."<<endl;
	// "scan" for food only every half of a second
	if((SimulationTick() % (SimulationTicksPerSecond() / 2)) == 0) {
        //argos::LOG<<"before set holding food ...it is"<<isHoldingFood<<endl;
		SetHoldingFood();
	}
	// When not carrying food, calculate movement.
	if(IsHoldingFood() == false) 
    {
		 argos::CVector2 distance = GetPosition() - GetTarget();
		 argos::Real     random   = RNG->Uniform(argos::CRange<argos::Real>(0.0, 1.0));
     
         // If we reached our target search location, set a new one. The 
	     // new search location calculation is different based on whether
	     // we are currently using informed or uninformed search.
	     if(distance.SquareLength() < TargetDistanceTolerance) 
         {
	         // randomly give up searching
	         if(SimulationTick()%(5*SimulationTicksPerSecond())==0 && random < LoopFunctions->ProbabilityOfReturningToNest) 
             {
	             //SetClosestNest();//qilu 07/26/2016
	             SetIsHeadingToNest(true);
	             //argos::LOG<<"heading to true in giveup"<<endl;
	             SetTarget(ClosestNest->GetLocation());
                 isGivingUpSearch = true;
	             ClosestNest->FidelityList.erase(controllerID); 
	             ClosestNest->DensityOnFidelity.erase(controllerID); 
	             SiteFidelityPosition= CVector2(10000,10000); 
	             isUsingSiteFidelity = false;
	             updateFidelity = false; 
	             MPFA_state = RETURNING;
	             searchingTime+=SimulationTick()-startTime;
	             startTime = SimulationTick();
	             /*
	             ofstream log_output_stream;
	             log_output_stream.open("giveup.txt", ios::app);
	             log_output_stream << "Give up: " << SimulationTick() / SimulationTicksPerSecond() << endl;
	             log_output_stream.close();
	             */
	             return; 
	         }
	         argos::Real USCV = LoopFunctions->UninformedSearchVariation.GetValue();
	         argos::Real rand = RNG->Gaussian(USCV);
	
	         // uninformed search
	         if(isInformed == false) 
             {
		          argos::CRadians rotation(rand);
		          argos::CRadians angle1(rotation);
		          argos::CRadians angle2(GetHeading());
		          argos::CRadians turn_angle(angle1 + angle2);
		          argos::CVector2 turn_vector(SearchStepSize, turn_angle);
		      
		          //argos::LOG << "UNINFORMED SEARCH: rotation: " << angle1 << std::endl;
		          //argos::LOG << "UNINFORMED SEARCH: old heading: " << angle2 << std::endl;
		      
		          /*
		          ofstream log_output_stream;
		          log_output_stream.open("uninformed_angle1.log", ios::app);
		          log_output_stream << angle1.GetValue() << endl;
		          log_output_stream.close();
		      
		          log_output_stream.open("uninformed_angle2.log", ios::app);
		          log_output_stream << angle2.GetValue() << endl;
		          log_output_stream.close();
		      
		          log_output_stream.open("uninformed_turning_angle.log", ios::app);
		          log_output_stream << turn_angle.GetValue() << endl;
		          log_output_stream.close();
		          */
		          SetIsHeadingToNest(false);
		          //argos::LOG<<"heading to false in uninformed search"<<endl;
		          SetTarget(turn_vector + GetPosition());
             }
	         // informed search
	         else{
	              SetIsHeadingToNest(false);
                  if(IsAtTarget()) 
                  {
	                  size_t          t           = SearchTime++;
	                  argos::Real     twoPi       = (argos::CRadians::TWO_PI).GetValue();
	                  argos::Real     pi          = (argos::CRadians::PI).GetValue();
	                  argos::Real     isd         = LoopFunctions->RateOfInformedSearchDecay;
	                  /*argos::Real     correlation = GetExponentialDecay((2.0 * twoPi) - LoopFunctions->UninformedSearchVariation.GetValue(), t, isd);
	                  argos::Real     rand = RNG->Gaussian(correlation + LoopFunctions->UninformedSearchVariation.GetValue());
	                       */ //qilu 09/24/2016
	                  Real correlation = GetExponentialDecay(rand, t, isd);
	                  //argos::CRadians rotation(GetBound(rand, -pi, pi));
	                  argos::CRadians rotation(GetBound(correlation, -pi, pi));//qilu 09/24/2016
	                  argos::CRadians angle1(rotation);
	                  argos::CRadians angle2(GetHeading());
	                  argos::CRadians turn_angle(angle2 + angle1);
	                  argos::CVector2 turn_vector(SearchStepSize, turn_angle);
	          
	                  //argos::LOG << "INFORMED SEARCH: rotation: " << angle1 << std::endl;
	                  //argos::LOG << "INFORMED SEARCH: old heading: " << angle2 << std::endl;
	          
	                  /*
	                  ofstream log_output_stream;
	                  log_output_stream.open("informed_angle1.log", ios::app);
	                  log_output_stream << angle1.GetValue() << endl;
	                  log_output_stream.close();
	          
	                  log_output_stream.open("informed_angle2.log", ios::app);
	                  log_output_stream << angle2.GetValue() << endl;
	                  log_output_stream.close();
	          
	                  log_output_stream.open("informed_turning_angle.log", ios::app);
	                  log_output_stream << turn_angle.GetValue() << endl;
	                  log_output_stream.close();
	                  */
	                  SetTarget(turn_vector + GetPosition());
	              }
             }
		 }
		 else {
			      //argos::LOG << "SEARCH: Haven't reached destination. " << GetPosition() << "," << GetTarget() << std::endl;
		 }
	}
	else {
		   //argos::LOG << "SEARCH: Carrying food." << std::endl;
	}

	// Food has been found, change state to RETURNING and go to the nest
	//else {
	//	SetTarget(LoopFunctions->NestPosition);
	//	MPFA_state = RETURNING;
	//}
}

// Cause the robot to rotate in place as if surveying the surrounding targets
// Turns 36 times by 10 degrees
void MPFA_controller::Surveying() {
 //LOG<<"Surveying..."<<endl;
	if (survey_count <= 4) { 
		CRadians rotation(survey_count*3.14/2); // divide by 10 so the vecot is small and the linear motion is minimized
		argos::CVector2 turn_vector(SearchStepSize, rotation.SignedNormalize());
			
		SetIsHeadingToNest(true); // Turn off error for this
		SetTarget(turn_vector + GetPosition());
        
		/*
		ofstream log_output_stream;
		log_output_stream.open("log.txt", ios::app);
		log_output_stream << (GetHeading() - rotation ).SignedNormalize() << ", "  << SearchStepSize << ", "<< rotation << ", " <<  turn_vector << ", " << GetHeading() << ", " << survey_count << endl;
		log_output_stream.close();
		*/
		if(fabs((GetHeading() - rotation).SignedNormalize().GetValue()) < TargetAngleTolerance.GetValue()) survey_count++;
			//else Keep trying to reach the turning angle
        
	}
	// Set the survey countdown
	else {
		SetIsHeadingToNest(false); // Turn on error for this
		SetTarget(ClosestNest->GetLocation()); //qilu 07/26/2016
        MPFA_state = RETURNING;
		survey_count = 0; // Reset
        searchingTime+=SimulationTick()-startTime;//qilu 10/22
        startTime = SimulationTick();//qilu 10/22
        
	}
}




/*****
 * RETURNING: Stay in this state until the robot has returned to the nest.
 * This state is triggered when a robot has found food or when it has given
 * up on searching and is returning to the nest.
 *****/
void MPFA_controller::Returning() 
{
   //LOG<<"Returning..."<<endl;
   //SetHoldingFood();
   //SetTarget(LoopFunctions->NestPosition);

  // Are we there yet? (To the nest, that is.)
  if(IsInTheNest()) 
  {
    // Based on a Poisson CDF, the robot may or may not create a pheromone
	// located at the last place it picked up food.
    argos::Real poissonCDF_pLayRate    = GetPoissonCDF(ResourceDensity, LoopFunctions->RateOfLayingPheromone);
	argos::Real poissonCDF_sFollowRate = GetPoissonCDF(ResourceDensity, LoopFunctions->RateOfSiteFidelity);
    argos::Real r1 = RNG->Uniform(argos::CRange<argos::Real>(0.0, 1.0));
    argos::Real r2 = RNG->Uniform(argos::CRange<argos::Real>(0.0, 1.0));
    ClosestNest->visited_time_point_in_minute = (argos::Real)(SimulationTick() / SimulationTicksPerSecond())/60;
    if (isHoldingFood) 
    { 
	  		       
      //drop off the food and display in the nest 
      argos::CVector2 placementPosition;
          
      placementPosition.Set(ClosestNest->GetLocation().GetX()+RNG->Gaussian(0.2, 0.5), ClosestNest->GetLocation().GetY()+RNG->Gaussian(0.2, 0.5));
          
      while((placementPosition-ClosestNest->GetLocation()).SquareLength()>pow(LoopFunctions->NestRadius/2.0-LoopFunctions->FoodRadius, 2))
          placementPosition.Set(ClosestNest->GetLocation().GetX()+RNG->Gaussian(0.2, 0.5), ClosestNest->GetLocation().GetY()+RNG->Gaussian(0.2, 0.5));
     
      ClosestNest->FoodList.push_back(placementPosition);
      
      if(ClosestNest->GetNestIdx() == 0) //center nest
      {
		total_targets_collected ++;
        LoopFunctions->setScore(total_targets_collected);
        LoopFunctions->currNumCollectedFood ++;  
      }
    
                	      
	  //LoopFunctions->setScore(total_targets_collected);
      if(poissonCDF_pLayRate > r1 && updateFidelity) 
      {
		TrailToShare.push_back(ClosestNest->GetLocation()); //qilu 07/26/2016
        argos::Real timeInSeconds = (argos::Real)(SimulationTick() / SimulationTicksPerSecond());
	    Pheromone sharedPheromone(SiteFidelityPosition, TrailToShare, timeInSeconds, LoopFunctions->RateOfPheromoneDecay, ResourceDensity);
        ClosestNest->PheromoneList.push_back(sharedPheromone);//qilu 09/08/2016
        ClosestNest->DensityOnFidelity.erase(controllerID); //09/11/2016 if it creates a pheromone trail, the sensed density on site fidelity should be removed. Otherwise, there is a repeated information.
                
        sharedPheromone.Deactivate(); // make sure this won't get re-added later...
       }
       TrailToShare.clear();
    }
        
    // use site fidelity
    if(updateFidelity && poissonCDF_sFollowRate > r2) 
    {
      //log_output_stream << "Using site fidelity" << endl;
      SetIsHeadingToNest(false);
      //argos::LOG<<"heading to false in using site fidelity"<<endl;
      SetTarget(SiteFidelityPosition);
      isInformed = true;
    }
    // use pheromone waypoints
    else if(SetTargetPheromone()) 
    {
      //log_output_stream << "Using site pheremone" << endl;
      isInformed = true;
      isUsingSiteFidelity = false;
    }
    // use random search
    else 
    {
      //log_output_stream << "Using random search" << endl;
      SetRandomSearchLocation();
      isInformed = false;
      isUsingSiteFidelity = false;
    }

    isGivingUpSearch = false;
    MPFA_state = DEPARTING;
    isHoldingFood = false;
    numHeldFood = 0;
    travelingTime+=SimulationTick()-startTime;//qilu 10/22
    startTime = SimulationTick();//qilu 10/22
  }
  // Take a small step towards the nest so we don't overshoot by too much if we miss it
  else 
  {
    if(IsAtTarget())
    {
      //SetIsHeadingToNest(false); // Turn off error for this
      //SetTarget(ClosestNest->GetLocation());
        argos::Real USCV = LoopFunctions->UninformedSearchVariation.GetValue();
        argos::Real rand = RNG->Gaussian(USCV);

        argos::CRadians rotation(rand);
        argos::CRadians angle1(rotation);
        argos::CRadians angle2(GetHeading());
        argos::CRadians turn_angle(angle1 + angle2);
        argos::CVector2 turn_vector(SearchStepSize, turn_angle);
        SetIsHeadingToNest(false);
        SetTarget(turn_vector + GetPosition());
    }
  }		
}

void MPFA_controller::SetRandomSearchLocation() {
	argos::Real random_wall = RNG->Uniform(argos::CRange<argos::Real>(0.0, 1.0));
	argos::Real x = 0.0, y = 0.0;

	/* north wall */
	if(random_wall < 0.25) {
		x = RNG->Uniform(RegionRangeX);
		y = RegionRangeY.GetMax();
	}
	/* south wall */
	else if(random_wall < 0.5) {
		x = RNG->Uniform(RegionRangeX);
		y = RegionRangeY.GetMin();
	}
	/* east wall */
	else if(random_wall < 0.75) {
		x = RegionRangeX.GetMax();
		y = RNG->Uniform(RegionRangeY);
	}
	/* west wall */
	else {
		x = RegionRangeX.GetMin();
		y = RNG->Uniform(RegionRangeY);
	}
	//the global location is relative to the current location 	
	x += GetPosition().GetX();
	y += GetPosition().GetY();
		
	SetIsHeadingToNest(false); // Turn on error for this
    SetTarget(argos::CVector2(x, y));
}


size_t MPFA_controller::GetNumHeldFood(){
	return numHeldFood;
	
	}

/*****
 * Check if the iAnt is finding food. This is defined as the iAnt being within
 * the distance tolerance of the position of a food item. If the iAnt has found
 * food then the appropriate boolean flags are triggered.
 *****/
void MPFA_controller::SetHoldingFood() {
	// Is the iAnt already holding food?
	if(IsHoldingFood() == false) {
		// No, the iAnt isn't holding food. Check if we have found food at our
		// current position and update the food list if we have.
	
	    std::vector<argos::CVector2> newFoodList;
	    std::vector<argos::CColor> newFoodColoringList;
	    size_t i = 0, j = 0;
		//if(MPFA_state != RETURNING){
			for(i = 0; i < LoopFunctions->FoodList.size(); i++) {
			    if((GetPosition() - LoopFunctions->FoodList[i]).SquareLength() < FoodDistanceTolerance ) {
	               // We found food! Calculate the nearby food density.
	               isHoldingFood = true;
	               numHeldFood = 1;
	               MPFA_state = SURVEYING;
	               //searchingTime+=SimulationTick()-startTime;//qilu 10/22
				   //startTime = SimulationTick();//qilu 10/22
	               j = i + 1;
				   searchingTime+=SimulationTick()-startTime;//qilu 10/22
				   startTime = SimulationTick();//qilu 10/22
				   //distribute a new food 
			         argos::CVector2 placementPosition;
			         placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));
			          
			         while(LoopFunctions->IsOutOfBounds(placementPosition, 1, 1)){
			             placementPosition.Set(RNG->Uniform(ForageRangeX), RNG->Uniform(ForageRangeY));
			         }
			         newFoodList.push_back(placementPosition);
					 newFoodColoringList.push_back(LoopFunctions->FoodColoringList[i]);
                    LoopFunctions->increaseNumDistributedFoodByOne(); //the total number of cubes in the arena should be updated. qilu 11/15/2018
					 //end
			 
				   break;
				 } else {
					//Return this unfound-food position to the list
					newFoodList.push_back(LoopFunctions->FoodList[i]);
					newFoodColoringList.push_back(LoopFunctions->FoodColoringList[i]);
				 }
			}
		//}
		if(j>0){
			for(; j < LoopFunctions->FoodList.size(); j++) {
			  newFoodList.push_back(LoopFunctions->FoodList[j]);
			  newFoodColoringList.push_back(LoopFunctions->FoodColoringList[j]);
			}
		}
   
	    // We picked up food. Update the food list minus what we picked up.
	    if(IsHoldingFood()) {
	         //SetClosestNest();//qilu 07/26/2016
	         //SetIsHeadingToNest(true);
	         //SetTarget(ClosestNest->GetLocation()); //qilu 07/26/2016
	         
         
	         
			LoopFunctions->FoodList.clear();
			LoopFunctions->FoodColoringList.clear();
	         LoopFunctions->FoodList = newFoodList;
	         LoopFunctions->FoodColoringList = newFoodColoringList; //qilu 09/12/2016
         SetLocalResourceDensity();
      }
	}
		
	// This shouldn't be checked here ---
	// Drop off food: We are holding food and have reached the nest.
	//else if((GetPosition() - LoopFunctions->NestPosition).SquareLength() < LoopFunctions->NestRadiusSquared) {
	//    isHoldingFood = false;
	// }

	// We are carrying food and haven't reached the nest, keep building up the
	// pheromone trail attached to this found food item.
 /* if(IsHoldingFood() && SimulationTick() % LoopFunctions->DrawDensityRate == 0) {
        TrailToShare.push_back(GetPosition());
  }*/
}

/*****
 * If the robot has just picked up a food item, this function will be called
 * so that the food density in the local region is analyzed and saved. This
 * helps facilitate calculations for pheromone laying.
 *
 * Ideally, given that: [*] is food, and [=] is a robot
 *
 * [*] [*] [*] | The maximum resource density that should be calculated is
 * [*] [=] [*] | equal to 9, counting the food that the robot just picked up
 * [*] [*] [*] | and up to 8 of its neighbors.
 *
 * That being said, the random and non-grid nature of movement will not
 * produce the ideal result most of the time. This is especially true since
 * item detection is based on distance calculations with circles.
 *****/
void MPFA_controller::SetLocalResourceDensity() {
    //argos::LOG<<"set local resource density ..."<<endl;
	argos::CVector2 distance;

	// remember: the food we picked up is removed from the foodList before this function call
	// therefore compensate here by counting that food (which we want to count)
	ResourceDensity = 1;

	/* Calculate resource density based on the global food list positions. */
	for(size_t i = 0; i < LoopFunctions->FoodList.size(); i++) {
		   distance = GetPosition() - LoopFunctions->FoodList[i];

		   if(distance.SquareLength() < LoopFunctions->SearchRadiusSquared*2) {
			      ResourceDensity++;
			      LoopFunctions->FoodColoringList[i] = argos::CColor::ORANGE;
			      LoopFunctions->ResourceDensityDelay = SimulationTick() + SimulationTicksPerSecond() * 10;
		   }
	}
 
	/* Set the fidelity position to the robot's current position. */
    SiteFidelityPosition = GetPosition();
    isUsingSiteFidelity = true;
    updateFidelity = true; //qilu 09/07/2016
    TrailToShare.push_back(SiteFidelityPosition);//qilu 09/07/2016
    ClosestNest->FidelityList[controllerID] = SiteFidelityPosition; //qilu 09/07/2016
    ClosestNest->DensityOnFidelity[controllerID]= ResourceDensity; //09/11/2016
	/* Delay for 4 seconds (simulate iAnts scannning rotation). */
	//  Wait(4); // This function is broken. It causes the rover to move in the wrong direction after finishing its local resource density test 

	//ofstream log_output_stream;
	//log_output_stream.open("MPFA_log.txt", ios::app);
	//log_output_stream << "(Survey): " << ResourceDensity << endl;
	//log_output_stream << "SiteFidelityPosition: " << SiteFidelityPosition << endl;
	//log_output_stream.close();
}

/*****
 * Update the global site fidelity list for graphics display and add a new fidelity position.
 *****/
void MPFA_controller::SetFidelityList(argos::CVector2 newFidelity) {
	std::vector<argos::CVector2> newFidelityList;

	/* Remove this robot's old fidelity position from the fidelity list. */
	/*for(size_t i = 0; i < LoopFunctions->FidelityList.size(); i++) {
  if((LoopFunctions->FidelityList[i] - SiteFidelityPosition).SquareLength() != 0.0) {
			newFidelityList.push_back(LoopFunctions->FidelityList[i]);
		}
	} */


	/* Update the global fidelity list. */
	//LoopFunctions->FidelityList = newFidelityList;

 ClosestNest->FidelityList[controllerID] = newFidelity; //qilu 09/08/2016
ClosestNest->DensityOnFidelity[controllerID]= ResourceDensity; //09/11/2016
	/* Add the robot's new fidelity position to the global fidelity list. */
	//LoopFunctions->FidelityList.push_back(newFidelity);
 

	/* Update the local fidelity position for this robot. */
	SiteFidelityPosition = newFidelity;
 
  updateFidelity = true;
}

/*****
 * Update the global site fidelity list for graphics display and remove the old fidelity position.
 *****/
void MPFA_controller::SetFidelityList() {
	std::vector<argos::CVector2> newFidelityList;

	/* Remove this robot's old fidelity position from the fidelity list. */
	/* Update the global fidelity list. */
	//LoopFunctions->FidelityList = newFidelityList;
 ClosestNest->FidelityList.erase(controllerID); //qilu 09/08/2016
    ClosestNest->DensityOnFidelity.erase(controllerID); //09/11/2016
 SiteFidelityPosition = CVector2(10000, 10000);
 updateFidelity = true; 
}

/*****
 * Update the pheromone list and set the target to a pheromone position.
 * return TRUE:  pheromone was successfully targeted
 *        FALSE: pheromones don't exist or are all inactive
 *****/
bool MPFA_controller::SetTargetPheromone() {
	argos::Real maxStrength = 0.0, randomWeight = 0.0;
	bool isPheromoneSet = false;

 if(ClosestNest->PheromoneList.size()==0) return isPheromoneSet; //qilu 09/08/2016 the case of no pheromone.
	/* update the pheromone list and remove inactive pheromones */
	// LoopFunctions->UpdatePheromoneList();

	/* default target = nest; in case we have 0 active pheromones */
	//SetIsHeadingToNest(true);
	//SetTarget(LoopFunctions->NestPosition);
 //SetTarget(ClosestNest->GetLocation()); //qilu 07/26/2016

	/* Calculate a maximum strength based on active pheromone weights. */
	for(size_t i = 0; i < ClosestNest->PheromoneList.size(); i++) {
		if(ClosestNest->PheromoneList[i].IsActive()) {
			maxStrength += ClosestNest->PheromoneList[i].GetWeight();
		}
	}

	/* Calculate a random weight. */
	randomWeight = RNG->Uniform(argos::CRange<argos::Real>(0.0, maxStrength));

	/* Randomly select an active pheromone to follow. */
	for(size_t i = 0; i < ClosestNest->PheromoneList.size(); i++) {
		   if(randomWeight < ClosestNest->PheromoneList[i].GetWeight()) {
			       /* We've chosen a pheromone! */
			       SetIsHeadingToNest(false);
                   //argos::LOG<<"heading to false in set pheromone..."<<endl;
          SetTarget(ClosestNest->PheromoneList[i].GetLocation());
          //TrailToFollow = ClosestNest->PheromoneList[i].GetTrail();
          isPheromoneSet = true;
          /* If we pick a pheromone, break out of this loop. */
          break;
     }

     /* We didn't pick a pheromone! Remove its weight from randomWeight. */
     randomWeight -= ClosestNest->PheromoneList[i].GetWeight();
	}

	//ofstream log_output_stream;
	//log_output_stream.open("MPFA_log.txt", ios::app);
	//log_output_stream << "Found: " << LoopFunctions->PheromoneList.size()  << " waypoints." << endl;
	//log_output_stream << "Follow waypoint?: " << isPheromoneSet << endl;
	//log_output_stream.close();

	return isPheromoneSet;
}

/*****
 * Calculate and return the exponential decay of "value."
 *****/
argos::Real MPFA_controller::GetExponentialDecay(argos::Real w, argos::Real time, argos::Real lambda) {
	/* convert time into units of haLoopFunctions-seconds from simulation frames */
	//time = time / (LoopFunctions->TicksPerSecond / 2.0);

	//LOG << "time: " << time << endl;
	//LOG << "correlation: " << (value * exp(-lambda * time)) << endl << endl;

	//return (value * std::exp(-lambda * time));
    Real     twoPi       = (CRadians::TWO_PI).GetValue();
    return w + (twoPi-w)* exp(-lambda * time);
}

/*****
 * Provides a bound on the value by rolling over a la modulo.
 *****/
argos::Real MPFA_controller::GetBound(argos::Real value, argos::Real min, argos::Real max) {
	/* Calculate an offset. */
	argos::Real offset = std::abs(min) + std::abs(max);

	/* Increment value by the offset while it's less than min. */
	while (value < min) {
			value += offset;
	}

	/* Decrement value by the offset while it's greater than max. */
	while (value > max) {
			value -= offset;
	}

	/* Return the bounded value. */
	return value;
}

size_t MPFA_controller::GetSearchingTime(){//qilu 10/22
    return searchingTime;
}
size_t MPFA_controller::GetTravelingTime(){//qilu 10/22
    return travelingTime;
}

string MPFA_controller::GetStatus(){//qilu 10/22
    //DEPARTING, SEARCHING, RETURNING
    if (MPFA_state == DEPARTING) return "DEPARTING";
    else if (MPFA_state ==SEARCHING)return "SEARCHING";
    else if (MPFA_state == RETURNING)return "RETURNING";
    else if (MPFA_state == SURVEYING) return "SURVEYING";
    //else if (MPFA_state == INACTIVE) return "INACTIVE";
    else return "SHUTDOWN";
    
}

/*void MPFA_controller::AddTravelingTime(size_t remainderTime){//qilu 10/22
    travelingTime += remainderTime;
}
void MPFA_controller::AddSearchingTime(size_t remainderTime){//qilu 10/22
    searchingTime += remainderTime;
}*/

/*****
 * Return the Poisson cumulative probability at a given k and lambda.
 *****/
argos::Real MPFA_controller::GetPoissonCDF(argos::Real k, argos::Real lambda) {
	argos::Real sumAccumulator       = 1.0;
	argos::Real factorialAccumulator = 1.0;

	for (size_t i = 1; i <= floor(k); i++) {
		factorialAccumulator *= i;
		sumAccumulator += pow(lambda, i) / factorialAccumulator;
	}

	return (exp(-lambda) * sumAccumulator);
}

void MPFA_controller::UpdateTargetRayList() {
	if(SimulationTick() % LoopFunctions->DrawDensityRate == 0 && LoopFunctions->DrawTargetRays == 1) {
		/* Get position values required to construct a new ray */
		argos::CVector2 t(GetTarget());
		argos::CVector2 p(GetPosition());
		argos::CVector3 position3d(p.GetX(), p.GetY(), 0.02);
		argos::CVector3 target3d(t.GetX(), t.GetY(), 0.02);

		/* scale the target ray to be <= searchStepSize */
		argos::Real length = std::abs(t.Length() - p.Length());

		if(length > SearchStepSize) {
			MyTrail.clear();
		} else {
			/* add the ray to the robot's target trail */
			argos::CRay3 targetRay(target3d, position3d);
			MyTrail.push_back(targetRay);

			/* delete the oldest ray from the trail */
			if(MyTrail.size() > MaxTrailSize) {
				MyTrail.erase(MyTrail.begin());
			}

			LoopFunctions->TargetRayList.insert(LoopFunctions->TargetRayList.end(), MyTrail.begin(), MyTrail.end());
			// loopFunctions.TargetRayList.push_back(myTrail);
		}
	}
}

void MPFA_controller::SetClosestNest(){//qilu 07/26/2016
    Nest* NewClosestNest;
    CVector2 robotPos = GetPosition();
    Real minSquaredLen = (LoopFunctions->Nests[0].GetLocation()-robotPos).SquareLength();
    size_t minIdx =0;
    Real squaredLen;
    for(map<int, Nest>:: iterator it= LoopFunctions->Nests.begin(); it!= LoopFunctions->Nests.end(); it++){
        squaredLen = (it->second.GetLocation()-robotPos).SquareLength();
        if (squaredLen < minSquaredLen) {
            minSquaredLen = squaredLen;
            minIdx = it->first;
        }
    }
    //ClosestNest->FidelityList.erase(controllerID); //qilu 09/07/2016
		  //SiteFidelityPosition=CVector2(10000,10000);
		
    NewClosestNest = &LoopFunctions->Nests[minIdx];
    if(ClosestNest != NULL){
        if(ClosestNest->GetNestIdx() != NewClosestNest->GetNestIdx())
            SetFidelityList();
            TrailToShare.clear();//qilu 09/29/2016
            ClosestNest = NewClosestNest;
    }
    else ClosestNest = NewClosestNest;
}
REGISTER_CONTROLLER(MPFA_controller, "MPFA_controller")
