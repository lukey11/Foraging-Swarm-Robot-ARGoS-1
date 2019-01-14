#include "MPFA_qt_user_functions.h"

/*****
 * Constructor: In order for drawing functions in this class to be used by
 * ARGoS it must be registered using the RegisterUserFunction function.
 *****/
MPFA_qt_user_functions::MPFA_qt_user_functions() :
	loopFunctions(dynamic_cast<MPFA_loop_functions&>(CSimulator::GetInstance().GetLoopFunctions()))
{
	RegisterUserFunction<MPFA_qt_user_functions, CFootBotEntity>(&MPFA_qt_user_functions::DrawOnRobot);
	RegisterUserFunction<MPFA_qt_user_functions, CFloorEntity>(&MPFA_qt_user_functions::DrawOnArena);
}

void MPFA_qt_user_functions::DrawOnRobot(CFootBotEntity& entity) {
	MPFA_controller& c = dynamic_cast<MPFA_controller&>(entity.GetControllableEntity().GetController());

	if(c.IsHoldingFood()) {
		for(size_t i=0; i < c.GetNumHeldFood(); i++){
		    //DrawCylinder(CVector3(0.0, 0.0, 0.3), CQuaternion(), loopFunctions.FoodRadius, 0.025, CColor::BLACK);
		    DrawCylinder(CVector3(0.0, 0.0, 0.3+i*0.04), CQuaternion(), loopFunctions.FoodRadius, 0.025, CColor::BLACK);
	    }
	}

	if(loopFunctions.DrawIDs == 1) {
		/* Disable lighting, so it does not interfere with the chosen text color */
		glDisable(GL_LIGHTING);
		/* Disable face culling to be sure the text is visible from anywhere */
		glDisable(GL_CULL_FACE);
		/* Set the text color */
		CColor cColor(CColor::BLACK);
		glColor3ub(cColor.GetRed(), cColor.GetGreen(), cColor.GetBlue());

		/* The position of the text is expressed wrt the reference point of the footbot
		 * For a foot-bot, the reference point is the center of its base.
		 * See also the description in
		 * $ argos3 -q foot-bot
		 */
		
		// Disable for now
		//GetOpenGLWidget().renderText(0.0, 0.0, 0.5,             // position
		//			     entity.GetId().c_str()); // text
		
			DrawText(CVector3(0.0, 0.0, 0.3),   // position
            entity.GetId().c_str()); // text
		/* Restore face culling */
		glEnable(GL_CULL_FACE);
		/* Restore lighting */
		glEnable(GL_LIGHTING);
	}
}
 
void MPFA_qt_user_functions::DrawOnArena(CFloorEntity& entity) {
	DrawFood();
	DrawFidelity();
	DrawPheromones();
	DrawNest();
    DrawBranches();
	if(loopFunctions.DrawTargetRays == 1) DrawTargetRays();
}

/*****
 * This function is called by the DrawOnArena(...) function. If the iAnt_data
 * object is not initialized this function should not be called.
 *****/
void MPFA_qt_user_functions::DrawNest() {
	/* 2d cartesian coordinates of the nest */
//	Real x_coordinate = loopFunctions.NestPosition.GetX();
//	Real y_coordinate = loopFunctions.NestPosition.GetX();
Real x_coordinate, y_coordinate;
for(map<int, Nest>:: iterator it= loopFunctions.Nests.begin(); it!= loopFunctions.Nests.end(); it++){     
     x_coordinate = it->second.GetLocation().GetX(); //qilu 07/05
     y_coordinate = it->second.GetLocation().GetY();
    //DrawCircle(nest_3d, CQuaternion(), loopFunctions.NestRadius, CColor::GRAY50);
	    Real elevation = loopFunctions.NestElevation;

	    // 3d cartesian coordinates of the nest
	    CVector3 nest_3d(x_coordinate, y_coordinate, elevation);

    	// Draw the nest on the arena

	    //DrawCircle(nest_3d, CQuaternion(), loopFunctions.NestRadius, CColor::GREEN);
     //DrawCylinder(nest_3d, CQuaternion(), loopFunctions.NestRadius, 0.008, CColor::GREEN);
     DrawCylinder(nest_3d, CQuaternion(), it->second.GetNestRadius(), 0.008, CColor::GREEN);
    }
}

void MPFA_qt_user_functions::DrawFood() {

	Real x, y;

	for(size_t i = 0; i < loopFunctions.FoodList.size(); i++) {
		x = loopFunctions.FoodList[i].GetX();
		y = loopFunctions.FoodList[i].GetY();
		DrawCylinder(CVector3(x, y, 0.002), CQuaternion(), loopFunctions.FoodRadius, 0.025, loopFunctions.FoodColoringList[i]);
	}
 
 //draw food in nests
 for(map<int, Nest>:: iterator it= loopFunctions.Nests.begin(); it!= loopFunctions.Nests.end(); it++){
       for (size_t j=0; j< it->second.FoodList.size(); j++){
        x = it->second.FoodList[j].GetX();
        y = it->second.FoodList[j].GetY();
        DrawCylinder(CVector3(x, y, 0.002), CQuaternion(), loopFunctions.FoodRadius, 0.025, CColor::BLACK);
     }
  } 
  
}

void MPFA_qt_user_functions::DrawFidelity() {

	   Real x, y;
    for(map<int, Nest>:: iterator iter= loopFunctions.Nests.begin(); iter!= loopFunctions.Nests.end(); iter++){
        for(map<string, CVector2>::iterator it= iter->second.FidelityList.begin(); it!=iter->second.FidelityList.end(); ++it) {
            x = it->second.GetX()+0.001;
            y = it->second.GetY()+0.001;
            DrawCylinder(CVector3(x, y, 0.0), CQuaternion(), loopFunctions.FoodRadius, 0.025, CColor::YELLOW);
        }
   }
}

void MPFA_qt_user_functions::DrawPheromones() {

	Real x, y, weight;
	vector<CVector2> trail;
	CColor trailColor = CColor::GREEN, pColor = CColor::GREEN;
    for(map<int, Nest>:: iterator it= loopFunctions.Nests.begin(); it!= loopFunctions.Nests.end(); it++){
        for(size_t i = 0; i < it->second.PheromoneList.size(); i++) {
            x = it->second.PheromoneList[i].GetLocation().GetX();
            y = it->second.PheromoneList[i].GetLocation().GetY();
    
            if(loopFunctions.DrawTrails == 1) {
                trail  = it->second.PheromoneList[i].GetTrail();
                weight = it->second.PheromoneList[i].GetWeight();
                
                if(weight > 0.25 && weight <= 1.0)        // [ 100.0% , 25.0% )
                    pColor = trailColor = CColor::GREEN;
                else if(weight > 0.05 && weight <= 0.25)  // [  25.0% ,  5.0% )
                    pColor = trailColor = CColor::YELLOW;
                else                                      // [   5.0% ,  0.0% ]
                    pColor = trailColor = CColor::RED;
      
                 CRay3 ray;
                 size_t j = 0;
      
                 for(j = 1; j < trail.size(); j++) {
                     ray = CRay3(CVector3(trail[j - 1].GetX(), trail[j - 1].GetY(), 0.01),
                     CVector3(it->second.GetLocation().GetX(), it->second.GetLocation().GetY(), 0.01));
                     DrawRay(ray, trailColor, 1.0);
                 }
                 DrawCylinder(CVector3(x, y, 0.0), CQuaternion(), loopFunctions.FoodRadius, 0.025, pColor);
            } 
            else {
                weight = it->second.PheromoneList[i].GetWeight();
    
                if(weight > 0.25 && weight <= 1.0)        // [ 100.0% , 25.0% )
                    pColor = CColor::GREEN;
                else if(weight > 0.05 && weight <= 0.25)  // [  25.0% ,  5.0% )
                    pColor = CColor::YELLOW;
                else                                      // [   5.0% ,  0.0% ]
                    pColor = CColor::RED;
      
                DrawCylinder(CVector3(x, y, 0.0), CQuaternion(), loopFunctions.FoodRadius, 0.025, pColor);
            }
        }
    }
}

void MPFA_qt_user_functions::DrawTargetRays() {
	for(size_t j = 0; j < loopFunctions.TargetRayList.size(); j++) 
    {
        DrawRay(loopFunctions.TargetRayList[j], loopFunctions.TargetRayColorList[j]);
    }
    loopFunctions.TargetRayList.clear();
}
void MPFA_qt_user_functions::DrawBranches() {
	std::vector<vector<argos::CRay3> >    RayList;
    std::vector<argos::CRay3>    tempList;
    CRay3 targetRay;
    size_t count=0, prev_level=1, revLevel=0, lineWidth=15;
    size_t level, pidx; 
    Real x, y, px, py;
    
    
    std::vector<argos::CColor> TrailColor;
    TrailColor.push_back(CColor::GREEN);
    TrailColor.push_back(CColor::RED);
    TrailColor.push_back(CColor::BLUE);
    TrailColor.push_back(CColor::ORANGE);
    
    for(map<int, Nest>::iterator it = loopFunctions.Nests.begin(); it != loopFunctions.Nests.end(); it++)
    {
        x = it->second.GetLocation().GetX();
        y = it->second.GetLocation().GetY();
        
        level = it->second.GetLevel();
        pidx = it->second.GetParentNestIdx();
        px = loopFunctions.Nests[pidx].GetLocation().GetX();
        py = loopFunctions.Nests[pidx].GetLocation().GetY();
        targetRay = CRay3(CVector3(x, y, 0.01), CVector3(px, py, 0.01));
        DrawRay(targetRay, TrailColor[level], lineWidth/(level+1));
    }
}

REGISTER_QTOPENGL_USER_FUNCTIONS(MPFA_qt_user_functions, "MPFA_qt_user_functions")
