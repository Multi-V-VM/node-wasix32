#ifdef __wasi__
// WASI compatibility - must be included before any other headers
#include <stdint.h>
#include <stddef.h>
#include <vector>

namespace v8 {
namespace internal {

// Missing sandbox constants
constexpr size_t kExternalPointerTableReservationSize = 1024 * 1024;
constexpr size_t kCodePointerTableReservationSize = 1024 * 1024;
constexpr uint32_t kMaxExternalPointers = 65536;
constexpr uint32_t kMaxCodePointers = 65536;
constexpr uint32_t kMaxCapacity = 65536;
constexpr size_t kCodePointerTableEntrySize = 8;
constexpr int kExternalPointerTagShift = 48;

using ExternalPointerHandle = uint32_t;
using CodePointerHandle = uint32_t;
constexpr ExternalPointerHandle kNullExternalPointerHandle = 0;
constexpr CodePointerHandle kNullCodePointerHandle = 0;

} // namespace internal
} // namespace v8

namespace v8 {
// LocalVector template
template<typename T> class Local;
template<typename T>
class LocalVector : public std::vector<Local<T>> {
 public:
  explicit LocalVector(Isolate* isolate) {}
};
} // namespace v8
#endif // __wasi__

// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/compiler/turbofan-graph.h"

#include <algorithm>

#include "src/compiler/node.h"
#include "src/compiler/turbofan-graph-visualizer.h"
#include "src/compiler/verifier.h"

namespace v8 {
namespace internal {
namespace compiler {

TFGraph::TFGraph(Zone* zone)
    : zone_(zone),
      start_(nullptr),
      end_(nullptr),
      mark_max_(0),
      next_node_id_(0),
      decorators_(zone),
      has_simd_(false),
      simd_stores_(zone) {
  // Nodes use compressed pointers, so zone must support pointer compression.
  // If the check fails, ensure the zone is created with kCompressGraphZone
  // flag.
  CHECK_IMPLIES(kCompressGraphZone, zone->supports_compression());
}

void TFGraph::Decorate(Node* node) {
  for (GraphDecorator* const decorator : decorators_) {
    decorator->Decorate(node);
  }
}

void TFGraph::AddDecorator(GraphDecorator* decorator) {
  decorators_.push_back(decorator);
}

void TFGraph::RemoveDecorator(GraphDecorator* decorator) {
  auto const it = std::find(decorators_.begin(), decorators_.end(), decorator);
  DCHECK(it != decorators_.end());
  decorators_.erase(it);
}

Node* TFGraph::NewNode(const Operator* op, int input_count, Node* const* inputs,
                       bool incomplete) {
  Node* node = NewNodeUnchecked(op, input_count, inputs, incomplete);
  Verifier::VerifyNode(node);
  return node;
}

Node* TFGraph::NewNodeUnchecked(const Operator* op, int input_count,
                                Node* const* inputs, bool incomplete) {
  Node* const node =
      Node::New(zone(), NextNodeId(), op, input_count, inputs, incomplete);
  Decorate(node);
  return node;
}

Node* TFGraph::CloneNode(const Node* node) {
  DCHECK_NOT_NULL(node);
  Node* const clone = Node::Clone(zone(), NextNodeId(), node);
  Decorate(clone);
  return clone;
}

NodeId TFGraph::NextNodeId() {
  // A node's id is internally stored in a bit field using fewer bits than
  // NodeId (see Node::IdField). Hence the addition below won't ever overflow.
  DCHECK_LT(next_node_id_, std::numeric_limits<NodeId>::max());
  return next_node_id_++;
}

void TFGraph::Print() const { StdoutStream{} << AsRPO(*this); }

void TFGraph::RecordSimdStore(Node* store) { simd_stores_.push_back(store); }

ZoneVector<Node*> const& TFGraph::GetSimdStoreNodes() { return simd_stores_; }

}  // namespace compiler
}  // namespace internal
}  // namespace v8
