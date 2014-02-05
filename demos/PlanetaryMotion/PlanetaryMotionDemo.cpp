#include "demos/PlanetaryMotion/PlanetaryMotionDemo.h"

#include "react/reWorld.h"
#include "react/Collision/reBSPTree.h"
#include "react/Entities/reRigidBody.h"
#include "react/Dynamics/reGravAction.h"
#include "react/Collision/Shapes/shapes.h"

#include "demos/Common/Sphere.h"
#include "demos/Common/StaticObject.h"

using namespace re::demo;

PlanetaryMotionDemo::PlanetaryMotionDemo() : _world(), _cam(), _canvas() {
  // do nothing
}

PlanetaryMotionDemo::~PlanetaryMotionDemo() {
  release();
}

void PlanetaryMotionDemo::init() {
  _cam.init(ViewCam::Mode::FIRST_PERSON);
  
  glClearColor(0.3f, 0.3f, 0.3f, 0.5f);
  glLineWidth(1);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0f);
  glPolygonMode(GL_BACK, GL_FILL);
  glPolygonMode(GL_FRONT, GL_LINE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  
  _canvas.init();
  
  prepareWorld();
  
  _canvas.prepareScene();
  onResize(width(), height());
}

void PlanetaryMotionDemo::release() {
  _canvas.release();
  _world.clear();
}

void PlanetaryMotionDemo::draw() {
  _world.advance(0.1);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  _cam.update();
  _canvas.setViewMat(_cam.viewMat());
  _canvas.renderScene();
}

void PlanetaryMotionDemo::onResize(unsigned int w, unsigned int h) {
  double aspectRatio = w / double(h);
  _canvas.frustum(-aspectRatio, aspectRatio, -1.0, 1.0, 1.0, 35.0);
}

void PlanetaryMotionDemo::restart() {
  release();
  init();
}

void PlanetaryMotionDemo::prepareWorld() {
  reSphere sphere(1.0);
  
  reBuilder build = _world.build();
  
  reRigidBody& body = build.RigidBody(sphere).withMass(5.0).at(0, 0, -2).rotatingWith(0.0, 0.00, 0.01);
  
  for (int i = 0; i < 15; i++) {
    reRigidBody& b = build.RigidBody(sphere.withRadius(1))
      .withMass(5.0)
      .at(2.5*i - 3 , 1, -5)
      .facing(re::vec3(0.0, -1.0, 1.0), re::vec3(0.0, 1.0, 0.0))
      .rotatingWith(0.0, 0.01, 0.0)
      .movingAt(re::vec3(0.01, 0.0, 0.0));
    build.GravAction(body, b);
  }
  
  reRigidBody& A = build.RigidBody(sphere).withMass(2.0).at(-3, -1, -5).movingAt(re::vec3(0.01, 0.0, 0.0));
  reRigidBody& B = build.RigidBody(sphere).withMass(2.0).at(3, 3, -5);
  
  build.GravAction(A, B);
  build.GravAction(body, B);
  build.GravAction(body, A);
  
  for (reEnt* ent : _world.entities()) {
    _canvas.bind(*ent).withColor(re::normalize(re::vec3::rand(0.0, 1.0))).withAlpha(0.8f);
  }
  
  _canvas.addPlane(re::vec3(0, -1, 0), re::vec3(0, 0, 1), re::vec3(0, -1, 0), 10.0f).material.diffuse = re::vec3(1.0, 0.3, 0.5);
  
  _world.broadPhase().rebalance();
  ((reBSPTree&)_world.broadPhase()).execute([](reBSPNode& node) {
    printf("DEPTH: %d\n", node.depth());
    return false;
  });
}

