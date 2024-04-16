#include "KaleidoForce_Initial.h"

#include "../KaleidoForce.h"

void KaleidoForce_Initial::configure(const mc_rtc::Configuration & config)
{
}

void KaleidoForce_Initial::start(mc_control::fsm::Controller & ctl_)
{
  auto & ctl = static_cast<KaleidoForce &>(ctl_);
}

bool KaleidoForce_Initial::run(mc_control::fsm::Controller & ctl_)
{
  auto & ctl = static_cast<KaleidoForce &>(ctl_);
  output("OK");
  return true;
}

void KaleidoForce_Initial::teardown(mc_control::fsm::Controller & ctl_)
{
  auto & ctl = static_cast<KaleidoForce &>(ctl_);
}

EXPORT_SINGLE_STATE("KaleidoForce_Initial", KaleidoForce_Initial)
