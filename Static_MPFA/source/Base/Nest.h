#ifndef NEST_H_
#define NEST_H_

#include <map> //qilu 09/11/2016
#include <argos3/core/utility/math/vector2.h>
//#include <argos3/core/utility/math/ray3.h>
#include <argos3/core/utility/logging/argos_log.h>
#include "Pheromone.h"
using namespace argos;
using namespace std;

/*****
 * Implementation of the iAnt nest object used by the iAnt MPFA. iAnts
 * build and maintain a list of these nest objects.
 *****/
class Nest {

	public:
		      Nest();
		      Nest(CVector2 location);
		       
		      vector<Pheromone> PheromoneList;
        map<string, argos::CVector2> FidelityList; //qilu 09/10/2016
        map<string, size_t> DensityOnFidelity; //qilu 09/11/2016
        vector<CVector2> FoodList;
        size_t num_collected_tags;
        size_t visited_time_point_in_minute;
        argos::Real NestRadius;
        argos::Real NestRadiusSquared; //qilu 12/2018
        /* constructor function */
		
		      /* public helper functions */
        CVector2	GetLocation();
        void		SetLocation();
        void		SetLocation(CVector2 newLocation); //qilu 09/11/2016
        void        SetNestRadius(int level, Real radius, float regionWidth, size_t numNest);
        argos::Real GetNestRadius();
        argos::Real GetNestRadiusSquared();
        void  SetNestIdx(size_t idx);
        void SetParentNestIdx_with_backtrack(size_t idx);
        void SetDeliveryCapacity(size_t c);
        size_t GetDeliveryCapacity();
        size_t  GetNestIdx();
        size_t  GetParentNestIdx();
        size_t GetCollectedTagNum();
        
	private:
        CVector2 nestLocation;
        size_t  nest_idx;
        size_t capacity;
        
};

#endif /* IANT_NEST_H_ */
