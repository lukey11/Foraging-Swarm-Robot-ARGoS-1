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

void Nest::SetNestRadius(int level, Real radius, float ArenaWidth, size_t nestNum){
    if(GetNestIdx() == 0 && nestNum > 1)
    {
        if(ArenaWidth <= 10){
            NestRadius = radius;
        }
        else
        {
            //NestRadius = radius * (ArenaWidth/10.0);
	    NestRadius = sqrt(level) * radius;
            }
        //NestRadius = radius * (1+log(nestNum-1)/log(4));
        //NestRadius = radius * (regionWidth/4.0) * (1+log(nestNum-1)/log(4));
        //NestRadius = pow(2, 1+log(nestNum-1)/log(4)) * radius;
	//argos::LOG<<"nest id="<<GetNestIdx()<<", radius="<< NestRadius <<endl;
    }
    else
    {
        NestRadius = radius;
        //NestRadius = radius*regionWidth/4.0;
    }
    //argos::LOG<<"nest id="<<GetNestIdx()<<", NestRadius="<<NestRadius<<endl;
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
 
