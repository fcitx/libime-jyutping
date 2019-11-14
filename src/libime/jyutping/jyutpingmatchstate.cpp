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

#include "jyutpingcontext.h"
#include "jyutpingime.h"
#include "jyutpingmatchstate_p.h"

namespace libime {
namespace jyutping {

JyutpingMatchState::JyutpingMatchState(JyutpingContext *context)
    : d_ptr(std::make_unique<JyutpingMatchStatePrivate>(context)) {}
JyutpingMatchState::~JyutpingMatchState() {}

void JyutpingMatchState::clear() {
    FCITX_D();
    d->matchedPaths_.clear();
    d->nodeCacheMap_.clear();
    d->matchCacheMap_.clear();
}

void JyutpingMatchState::discardNode(
    const std::unordered_set<const SegmentGraphNode *> &nodes) {
    FCITX_D();
    for (auto node : nodes) {
        d->matchedPaths_.erase(node);
    }
    for (auto &p : d->matchedPaths_) {
        auto &l = p.second;
        auto iter = l.begin();
        while (iter != l.end()) {
            if (nodes.count(iter->path_.front())) {
                iter = l.erase(iter);
            } else {
                iter++;
            }
        }
    }
}

void JyutpingMatchState::discardDictionary(size_t idx) {
    FCITX_D();
    d->matchCacheMap_.erase(d->context_->ime()->dict()->trie(idx));
    d->nodeCacheMap_.erase(d->context_->ime()->dict()->trie(idx));
}

} // namespace jyutping
} // namespace libime
