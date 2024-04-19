#pragma once

#include <mc_control/mc_controller.h>
#include <mc_control/fsm/Controller.h>
#include <mc_tasks/CoMTask.h>
#include <mc_tasks/PositionTask.h>
#include <mc_tasks/OrientationTask.h>
#include <mc_tasks/AdmittanceTask.h>
#include <mc_tasks/CoPTask.h>

#include "api.h"

struct KaleidoForce_DLLAPI KaleidoForce : public mc_control::fsm::Controller
{
  KaleidoForce(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config);

  bool run() override;

  void reset(const mc_control::ControllerResetData & reset_data) override;

  private:

    // GUI Related

    bool click = false;
    bool CoM = false;
    bool Foot = false;

    // Tasks

    std::shared_ptr<mc_tasks::CoMTask> comTask_;
    std::shared_ptr<mc_tasks::OrientationTask> baseTask_;
    std::shared_ptr<mc_tasks::OrientationTask> chestTask_;

    std::shared_ptr<mc_tasks::force::CoPTask> rightFoot_CoPTask_;
    
    std::shared_ptr<mc_tasks::PositionTask> leftFoot_PosTask_;

    // Active Joints for the CoM and Base tasks

    std::vector<std::string> Activedof_;

  // Ratio for the kinematic inertial
    
  double leftFootRatio = 0.5;

  // Observer Pipeline
    
  std::string observerPipelineName_ = "KaleidoForceObserverPipeline";
  
};