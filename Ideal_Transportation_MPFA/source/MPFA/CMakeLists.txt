
###############################################
# define shared object files
###############################################

add_library(MPFA_loop_functions SHARED MPFA_loop_functions.h
                                       MPFA_loop_functions.cpp
                                       MPFA_qt_user_functions.h
                                       MPFA_qt_user_functions.cpp)

add_library(MPFA_controller     SHARED MPFA_controller.h
                                       MPFA_controller.cpp
                                       MPFA_loop_functions.h
                                       MPFA_loop_functions.cpp)

###############################################
# link shared object files to dependencies
###############################################

target_link_libraries(MPFA_controller
                      BaseController
                      Pheromone
                      Nest)

target_link_libraries(MPFA_loop_functions
                      MPFA_controller
                      argos3core_simulator
                      argos3plugin_simulator_entities
                      argos3plugin_simulator_footbot
                      argos3plugin_simulator_genericrobot
                      argos3plugin_simulator_qtopengl
                      ${ARGOS_QTOPENGL_LIBRARIES})
