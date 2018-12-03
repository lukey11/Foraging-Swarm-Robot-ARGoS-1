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
    //num_collected_tags=0;
    visited_time_point_in_minute=0;
    nest_idx=-1;
    parent_nest_idx = -1;
    level = -1;
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
    //NestRadius = pow(2, level) * radius;
    NestRadius = (level+1) * radius;
    //argos::LOG<<"nest id="<<GetNestIdx()<<", radius="<< NestRadius <<endl;
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
 
void Nest::SetParentNestIdx_with_backtrack(size_t idx)
{
    if (idx == 0) {
        parent_nest_idx = 0;
    }
    else{
	    parent_nest_idx = (idx-1)/4;
    }
}
 
void Nest::SetParentNestIdx_no_backtrack(vector<Nest*> parents)
{
    Nest* curr_parent = parents[0];
    Real curr_squared_distance = (GetLocation() - curr_parent->GetLocation()).SquareLength();
    Real distance;
    for(int i =1; i < parents.size(); i++){
        distance = (GetLocation() - parents[i]->GetLocation()).SquareLength(); 
        if (distance - curr_squared_distance <= 25){
            curr_squared_distance = distance;
            parent_nest_idx = parents[i]->GetNestIdx();
        } 
    }
}
 
void Nest::SetDeliveryCapacity(size_t c)
{
    capacity = c;
	
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
 
