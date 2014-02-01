#include "react/Collision/reTreeBalanceStrategy.h"

#include "react/Collision/reBSPTree.h"

namespace {
  const reUInt GUESSES = 3;
  const reUInt SAMPLES = 8;
}

/**
 * Tests if the parent node should merge the child nodes
 * 
 * @param node The parent node
 * @return True if the condition for merging child nodes are met
 */

bool reTreeBalanceStrategy::shouldMerge(const reBSPTreeNode& node) {
  if (!node.hasChildren() || node.child(0).hasChildren() || node.child(1).hasChildren()) return false;
  return node.child(0).size() + node.child(1).size() < 10;
}

/**
 * Tests if the current node should split
 * 
 * @param node The current node
 */

bool reTreeBalanceStrategy::shouldSplit(const reBSPTreeNode& node) {
  return node.size() > 10 && node.depth < 5;
}

/**
 * Determines the optimal split plane for the node by sampling the contained
 * entities
 * 
 * @param parentDir The parent split direction
 * @param entities A list of entities contained in the parent
 * @param anchor This field is set to the optimal anchor point
 * @param dir This field is set to the optimal split plane direction
 */

void reTreeBalanceStrategy::computeSplitPlane(const re::vec3& parentDir, const reEntList& entities, re::vec3& anchor, re::vec3& dir) {
  anchor.set(0.0, 0.0, 0.0);
  
  re::vec3 dirs[GUESSES] = {
    re::cross(parentDir, re::vec3::random()),
    re::cross(parentDir, re::vec3::random()),
    re::cross(parentDir, re::vec3::random())
  };
  
  reFloat vals[GUESSES] = { 0.0 };
  reUInt index = 0;
  
  if (entities.size() > SAMPLES) {
    const reLinkedList<reEnt*> list = entities.sample(SAMPLES);
    
    for (const reEnt* ent : list) {
      anchor += ent->center();
    }
    
    anchor /= SAMPLES;
    
    const reLinkedList<reEnt*> list2 = entities.sample(SAMPLES);
    for (const reEnt* ent : list2) {
      for (reUInt i = 0; i < GUESSES; i++) {
        vals[i] += re::dot(dirs[i], anchor - ent->center());
      }
    }
    
    for (reUInt i = 1; i < GUESSES; i++) {
      if (reAbs(vals[i]) < reAbs(vals[index])) {
        index = i;
      }
    }
  } else {
    reUInt num = 0;
    reUInt n = reMin(SAMPLES, entities.size());
    
    auto end = entities.qEnd();
    for (auto iter = entities.qBegin(); iter != end; ++iter) {
      const reQueryable& q = *iter;
      anchor += q.ent.center();
      if (++num >= n) {
        break;
      }
    }
    anchor /= n;
    for (reUInt i = 0; i < GUESSES; i++) {
      num = 0;
      auto end = entities.qEnd();
      for (auto iter = entities.qBegin(); iter != end; ++iter) {
        const reQueryable& q = *iter;
        vals[i] += re::dot(dirs[i], anchor - q.ent.center());
        if (++num >= n) {
          break;
        }
      }
      if (i > 0 && reAbs(vals[i]) < reAbs(vals[index])) {
        index = i;
      }
    }
  }
  
  dir = dirs[index];
}
