//
// Copyright (C) 2018~2018 by CSSlayer
// wengxt@gmail.com
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as
// published by the Free Software Foundation; either version 2.1 of the
// License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; see the file COPYING. If not,
// see <http://www.gnu.org/licenses/>.
//

#include "jyutpingdecoder_p.h"
#include <cmath>

namespace libime {
namespace jyutping {

JyutpingLatticeNode::JyutpingLatticeNode(
    std::string_view word, WordIndex idx, SegmentGraphPath path,
    const State &state, float cost,
    std::unique_ptr<JyutpingLatticeNodePrivate> data)
    : LatticeNode(word, idx, std::move(path), state, cost),
      d_ptr(std::move(data)) {}

JyutpingLatticeNode::~JyutpingLatticeNode() = default;

const std::string &JyutpingLatticeNode::encodedJyutping() const {
    static const std::string empty;
    if (!d_ptr) {
        return empty;
    }
    return d_ptr->encodedJyutping_;
}

LatticeNode *JyutpingDecoder::createLatticeNodeImpl(
    const SegmentGraphBase &graph, const LanguageModelBase *model,
    std::string_view word, WordIndex idx, SegmentGraphPath path,
    const State &state, float cost, std::unique_ptr<LatticeNodeData> data,
    bool onlyPath) const {
    std::unique_ptr<JyutpingLatticeNodePrivate> jyutpingData(
        static_cast<JyutpingLatticeNodePrivate *>(data.release()));
    if (model->isUnknown(idx, word)) {
        // we don't really care about a lot of unknown single character
        // which is not used for candidates
        if ((jyutpingData && jyutpingData->encodedJyutping_.size() == 2) &&
            path.front() != &graph.start() && !onlyPath) {
            return nullptr;
        }
    }

    return new JyutpingLatticeNode(word, idx, std::move(path), state, cost,
                                   std::move(jyutpingData));
}

} // namespace jyutping
} // namespace libime
