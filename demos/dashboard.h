#ifndef DEMO_DASHBOARD_H
#define DEMO_DASHBOARD_H

enum DemoID {
  RAY_TRACING = 0,
  PLANETARY_MOTION
};

int startDemo(int argc, char** argv, DemoID id);

#endif
