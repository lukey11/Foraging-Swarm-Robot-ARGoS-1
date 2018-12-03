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

void Nest::SetNestRadius(Real radius){
    
    if(nest_idx == 0)
    {
       NestRadius = radius;
    }
    else
    {
    NestRadius = 0.12;
    }
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
  
void Nest::SetDeliveryCapacity(size_t c)
{
    capacity = c;
	
	}
     
size_t Nest::GetDeliveryCapacity()
{
    return capacity;
}
	   
size_t Nest:: GetNestIdx(){
     return nest_idx;
 } 


size_t Nest:: GetCollectedTagNum(){
     return FoodList.size();
 } 
 
