#include "KaleidoForce.h"


KaleidoForce::KaleidoForce(mc_rbdyn::RobotModulePtr rm, double dt, const mc_rtc::Configuration & config)
: mc_control::fsm::Controller(rm, dt, config)
{

  config_.load(config);

  // Activedof_ = config("active_dof");

  Activedof_ = {"Root", "L_CROTCH_Y", "L_CROTCH_R", "L_CROTCH_P", "L_KNEE_P", "L_ANKLE_R", "L_ANKLE_P", 
                      "R_CROTCH_Y", "R_CROTCH_R", "R_CROTCH_P", "R_KNEE_P", "R_ANKLE_R", "R_ANKLE_P"};

  // Setting the anchor frame for the Kinematic Inertial estimator

  // datastore().make_call("KinematicAnchorFrame::" + robot().name(),
  //                         [this](const mc_rbdyn::Robot & robot)
  //                         {
  //                           return sva::interpolate(robot.surfacePose("LeftFoot"),
  //                                                   robot.surfacePose("RightFoot"),
  //                                                   leftFootRatio);
  //                         });


  comTask_ = std::make_shared<mc_tasks::CoMTask>(robots(), robots().robot().robotIndex(), 2, 1e7);
  baseTask_ = std::make_shared<mc_tasks::OrientationTask>("BODY", robots(), robots().robot().robotIndex(), 2, 1e7);
  chestTask_ = std::make_shared<mc_tasks::OrientationTask>("CHEST_P_LINK", robots(), robots().robot().robotIndex(), 0, 1e7);

  rightFoot_CoPTask_ = std::make_shared<mc_tasks::force::CoPTask>("RightFoot", robots(), robots().robot().robotIndex(), 5, 1e7);
  leftFoot_PosTask_ = std::make_shared<mc_tasks::PositionTask>("L_ANKLE_P_LINK", robots(), robots().robot().robotIndex(), 2, 1e9);


  logger().addLogEntry("Kaleido_CoM", [this]() { return realRobots().robot().com(); });

  mc_rtc::log::success("KaleidoForce init done ");
}

bool KaleidoForce::run()
{

  if(CoM && !Foot && !click) 

  {
  
  click = true;

    gui()->removeElement({"Controller"}, "MoveCoM");

    gui()->addElement({"Controller"}, mc_rtc::gui::Button("LiftFoot", [this]() {
    solver().addTask(chestTask_);
    solver().addTask(rightFoot_CoPTask_);
    solver().addTask(leftFoot_PosTask_);


    removeContact({robot().name(), "ground", "LeftFoot", "AllGround"});
    leftFoot_PosTask_->position(Eigen::Vector3d{0, 0.1, 0.2});

    mc_rtc::log::info("Lift the Left Foot\n");

    Foot = true;

    }));

  }


  return mc_control::fsm::Controller::run();
}

void KaleidoForce::reset(const mc_control::ControllerResetData & reset_data)
{
  comTask_->reset();
  baseTask_->reset();
  chestTask_->reset();

  rightFoot_CoPTask_->reset();
  leftFoot_PosTask_->reset();

  // Add the CoM and Base Tasks permanently to not cause the drifting of the base of the robot when the controller is disabled

  comTask_->selectActiveJoints(Activedof_);
  baseTask_->selectActiveJoints(Activedof_);

  solver().addTask(comTask_);
  solver().addTask(baseTask_);

  // const auto & observerp = observerPipeline(observerPipelineName_);
  
  // if(observerp.success())
  // {
  //   mc_rtc::log::info("Pipeline \"{}\" for real robot observation loaded!", observerPipelineName_);
  // }

  if (!CoM) {
    gui()->addElement({"Controller"}, mc_rtc::gui::Button("MoveCoM", [this]() {


      comTask_->com(comTask_->com() - Eigen::Vector3d{0, 0.09, 0});

      mc_rtc::log::info("Moved the CoM above the Right Foot\n");

      CoM = true;

   }));
  }

  mc_control::fsm::Controller::reset(reset_data);
}


