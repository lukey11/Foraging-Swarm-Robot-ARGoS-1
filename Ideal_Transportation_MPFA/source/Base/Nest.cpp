#include "Nest.h"

/*****
 * The iAnt nest needs to keep track of four things:
 *
 * [1] location
 * [2] nest id 
 * [3] site fidelity
 * [4] pheromone trails
 *
 *****/
	Nest::Nest(){}
	Nest::Nest(CVector2   location)
{
    /* required initializations */
	   nestLocation    = location;
	   PheromoneList.clear();
    FidelityList.clear();
    DensityOnFidelity.clear(); //qilu 09/11/2016
    FoodList.clear(); //qilu 09/07/2016
    FoodDeliveryStartTime.clear();
    RobotReturnStartTime.clear();
    //num_collected_tags=0;
    visited_time_point_in_minute=0;
    nest_idx=-1;
    parent_nest_idx = -1;
    level = -1;
    numDeliveryRobot=0;
    DeliveringTime=0;
}

/*****
 *****/

/*****
 * Return the nest's location.
 *****/
 
 
CVector2 Nest::GetLocation() {
    return nestLocation;
}


void Nest::SetLocation() {
    nestLocation=CVector2(0.0, 0.0);
}

void Nest::SetLocation(CVector2 newLocation) {
    nestLocation = newLocation;
}

void Nest::SetNestRadius(int level, Real radius){
    NestRadius = sqrt(level+1) * radius;
    NestRadiusSquared = NestRadius*NestRadius;
}

argos::Real Nest::GetNestRadiusSquared(){
    
    return NestRadiusSquared;
    }

argos::Real Nest::GetNestRadius(){
    
    return NestRadius;
    }

        
void Nest:: SetNestIdx(size_t idx){
     nest_idx = idx;
 }
 
void Nest:: SetLevel(size_t l){
	level = l; 
}
 
void Nest::SetParentNestIdx_with_backtrack(size_t idx, size_t NumOfBranch, map<int, Nest> *Nests)
{
    if (idx == 0) {
        parent_nest_idx = 0;
    }
    else{
	    parent_nest_idx = (idx-1)/NumOfBranch;
        map<int, Nest>::iterator it = Nests->find(parent_nest_idx);
        while(it == Nests->end()){//the nest id does not exist
           parent_nest_idx = parent_nest_idx/NumOfBranch;
           it = Nests->find(parent_nest_idx);
        }
    }
}
 
void Nest::SetParentNestIdx_no_backtrack(vector<Nest*> parents)
{
    CVector2 curr_direction, other_direction;
    Real x1, y1, x2, y2, product, detrminant, angle;
    
    curr_direction = GetLocation() - parents[0]->GetLocation();
    parent_nest_idx = parents[0]->GetNestIdx();
       
    x1 = curr_direction.GetX();
    y1 = curr_direction.GetY();
        
    for(int i =1; i < parents.size(); i++){
        other_direction = GetLocation() - parents[i]->GetLocation();
        x2 = other_direction.GetX();
        y2 = other_direction.GetY();
        
        product = x1*x2 + y1*y2;
        detrminant = x1*y2 - y1*x2;  
        if (abs(atan2(detrminant, product)) >= 1.8){//degree, remove backtracking 
            parent_nest_idx = parents[i]->GetNestIdx();
        } 
    }
}
 
argos::Real Nest::GetDeliveringTime()
{
    return DeliveringTime;
}

void Nest::SetDeliveringTime(argos::Real t)
{
    DeliveringTime = t;
}

void Nest::SetDeliveryRobot(size_t n)
{
    numDeliveryRobot = n;
}

size_t Nest::GetDeliveryRobot()
{
    return numDeliveryRobot;
}

void Nest::SetDeliveryCapacity(size_t c)
{
    capacity = c;
	//argos::LOG<<"nest id="<<GetNestIdx()<<", capacity="<< capacity <<endl;
	}
     
size_t Nest::GetDeliveryCapacity()
{
    return capacity;
}
	   
size_t Nest:: GetLevel()
{
	return level;
	}
        
        
size_t Nest:: GetNestIdx(){
     return nest_idx;
 } 

size_t Nest:: GetParentNestIdx()
{
	return parent_nest_idx;
	}

/*size_t Nest:: GetCollectedTagNum(){
     return FoodList.size();
 }*/ 
 
