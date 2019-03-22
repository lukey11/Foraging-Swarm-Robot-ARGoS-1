#ifndef MPFA_CONTROLLER_H
#define MPFA_CONTROLLER_H

#include <source/Base/BaseController.h>
#include <source/Base/Pheromone.h>
#include <source/Base/Nest.h> //qilu 09/06
#include <source/MPFA/MPFA_loop_functions.h>
#include <argos3/core/simulator/entity/floor_entity.h>
/* Definition of the LEDs actuator */
#include <argos3/plugins/robots/generic/control_interface/ci_leds_actuator.h>

using namespace std;
using namespace argos;

static unsigned int total_targets_collected = 0;

class MPFA_loop_functions;

class MPFA_controller : public BaseController {

	public:

		MPFA_controller();

		// CCI_Controller inheritence functions
		void Init(argos::TConfigurationNode &node);
		void ControlStep();
		void Reset();

		bool IsHoldingFood();
		bool IsUsingSiteFidelity();
		bool IsInTheNest();
        bool IsInTargetNest();
		Real FoodDistanceTolerance;
        //size_t QuardArena;

		void SetLoopFunctions(MPFA_loop_functions* lf);
  void SetClosestNest();//qilu 07/26/2016
  
  size_t     GetSearchingTime();//qilu 09/26/2016
  size_t     GetTravelingTime();//qilu 09/26/2016
  string     GetStatus();//qilu 09/26/2016
  size_t     GetNumHeldFood();
  //void        AddTravelingTime(size_t remainderTime);//qilu 09/26/2016
  //void        AddSearchingTime(size_t remainderTime);//qilu 09/26/2016
  size_t      startTime;//qilu 09/26/2016
        

	private:
  Nest* ClosestNest; //qilu 07/26/2016
  Nest* TargetNest;
  string 			controllerID;//qilu 07/26/2016

		MPFA_loop_functions* LoopFunctions;
		argos::CRandom::CRNG* RNG;

		/* pheromone trail variables */
		std::vector<argos::CVector2> TrailToShare;
		std::vector<argos::CVector2> TrailToFollow;
		std::vector<argos::CRay3>    MyTrail;

		/* robot position variables */
		argos::CVector2 SiteFidelityPosition;
  bool			 updateFidelity; //qilu 09/07/2016
  
		vector<CRay3> myTrail;
		CColor        TrailColor;

		bool isInformed;
		bool isHoldingFood;
		size_t numHeldFood;
		bool isUsingSiteFidelity;
		bool isGivingUpSearch;
  
		size_t ResourceDensity;
		size_t MaxTrailSize;
		size_t SearchTime;//for informed search
  
  size_t           searchingTime; //qilu 09/26
  size_t           travelingTime;//qilu 09/26
        
  
		/* iAnt MPFA state variable */
		enum MPFA_state {
			DEPARTING = 0,
			SEARCHING = 1,
			RETURNING = 2,
			SURVEYING = 3,
			DEPOT_IDLING = 4,
			DEPOT_DELIVERING = 5,
			DEPOT_RETURNING = 6
		} MPFA_state;

		/* iAnt MPFA state functions */
		void MPFA();
		void Departing();
		void Searching();
		void Returning();
		void Surveying();
		void Delivering();
		//void Idling();
		//void DepotReturning();
		

		/* MPFA helper functions */
		void SetRandomSearchLocation();
		void SetHoldingFood();
		void SetLocalResourceDensity();
		void SetFidelityList(argos::CVector2 newFidelity);
		void SetFidelityList();
		bool SetTargetPheromone();

		argos::Real GetExponentialDecay(argos::Real value, argos::Real time, argos::Real lambda);
		argos::Real GetBound(argos::Real value, argos::Real min, argos::Real max);
		argos::Real GetPoissonCDF(argos::Real k, argos::Real lambda);

		void UpdateTargetRayList();
  
		CVector2 previous_position;

		string results_path;
		string results_full_path;
		bool isUsingPheromone;

		unsigned int survey_count;
		/* Pointer to the LEDs actuator */
        CCI_LEDsActuator* m_pcLEDs;
};

#endif /* MPFA_CONTROLLER_H */
