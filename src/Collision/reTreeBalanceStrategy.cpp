#include "react/Collision/reTreeBalanceStrategy.h"

#include "react/Collision/reBSPTree.h"
#include "react/Entities/reEnt.h"

/**
 * Tests if the parent node should merge the child nodes
 * 
 * @param node The parent node
 * @return True if the condition for merging child nodes are met
 */

bool reTreeBalanceStrategy::shouldMerge(const reBSPNode& node) {
  return (node.hasChildren() && 
            (node.placements() +
             node.child(0).placements() +
             node.child(1).placements() < 15));
}

/**
 * Tests if the current node should split
 * 
 * @param node The current node
 */

bool reTreeBalanceStrategy::shouldSplit(const reBSPNode& node) {
  return node.placements() > 10 && node.depth() < 5;
}

/**
 * Determines the optimal split plane for the node by sampling the contained
 * entities
 * 
 * @param parentDir The parent split direction
 * @param sample A sample of the entities contained in the parent
 */

re::SplitPlane reTreeBalanceStrategy::computeSplitPlane(const re::vec3& parentDir, const reLinkedList<const reEnt*>& sample) {
  re::SplitPlane split;
  split.center.set(0.0, 0.0, 0.0);
  
  re::vec3 guess[3] = {
    re::normalize(re::cross(parentDir, re::vec3::rand())),
    re::normalize(re::cross(parentDir, re::vec3::rand())),
    re::normalize(re::cross(parentDir, re::vec3::rand()))
  };
  
  const reUInt NUM_GUESSES = sizeof(guess) / sizeof(re::vec3);
  
  reFloat score[NUM_GUESSES] = { 0.0 };
  reUInt index = 0;
  
  // TODO merge the two loops? (Principle of Superposition)
  for (const reEnt* entity : sample) {
    split.center += entity->center();
  }
  split.center /= sample.size();
  
  for (const reEnt* entity : sample) {
    for (reUInt i = 0; i < NUM_GUESSES; i++) {
      score[i] += re::dot(guess[i], split.center - entity->center());
    }
  }
  
  for (reUInt i = 1; i < NUM_GUESSES; i++) {
    if (re::abs(score[i]) < re::abs(score[index])) {
      index = i;
    }
  }
  
  split.normal = guess[index];
  return split;
}
