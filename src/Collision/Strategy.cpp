#include "react/Collision/Strategy.h"

#include "react/Collision/reBSPTree.h"
#include "react/Entities/Entity.h"

using namespace re;

/**
 * Tests if the parent node should merge the child nodes
 * 
 * @param node The parent node
 * @return True if the condition for merging child nodes are met
 */

bool Strategy::shouldMerge(const reBSPNode& node) {
  return (node.hasChildren() && !node.child(0).hasChildren() &&
            (node.placements() +
             node.child(0).placements() +
             node.child(1).placements() < 15));
}

/**
 * Tests if the current node should split
 * 
 * @param node The current node
 */

bool Strategy::shouldSplit(const reBSPNode& node) {
  return node.placements() > 10 && node.depth() < 5;
}

/**
 * Determines the optimal split plane for the node by sampling the contained
 * entities
 * 
 * @param parentDir The parent split direction
 * @param sample A sample of the entities contained in the parent
 */

Plane Strategy::computeSplitPlane(const re::vec3& axis, const reLinkedList<Entity*>& sample) {
  re::vec3 split(0.0, 0.0, 0.0);

  const re::vec3 guess[3] = {
    re::normalize(re::cross(axis, re::vec3::rand())),
    re::normalize(re::cross(axis, re::vec3::rand())),
    re::normalize(re::cross(axis, re::vec3::rand()))
  };
  
  const reUInt NUM_GUESSES = sizeof(guess) / sizeof(re::vec3);
  
  reFloat score[NUM_GUESSES] = { 0.0 };
  reUInt index = 0;

  for (const Entity* entity : sample) {
    split += entity->center();
  }
  split /= sample.size();
  
  for (const Entity* entity : sample) {
    for (reUInt i = 0; i < NUM_GUESSES; i++) {
      score[i] += re::dot(guess[i], - split + entity->center());
    }
  }
  
  for (reUInt i = 1; i < NUM_GUESSES; i++) {
    if (re::abs(score[i]) < re::abs(score[index])) {
      index = i;
    }
  }
  
  return re::Plane(guess[index], split);
}
