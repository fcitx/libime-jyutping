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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGMATCHSTATE_P_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGMATCHSTATE_P_H_

#include <fcitx-utils/macros.h>
#include <libime/core/lattice.h>
#include <libime/core/lrucache.h>
#include <libime/jyutping/jyutpingdictionary.h>
#include <libime/jyutping/jyutpingmatchstate.h>
#include <memory>
#include <unordered_map>

namespace libime {

namespace jyutping {

using JyutpingTriePosition = std::pair<uint64_t, size_t>;
using JyutpingTriePositions = std::vector<JyutpingTriePosition>;

// Matching result for a specific JyutpingTrie.
struct MatchedJyutpingTrieNodes {
    MatchedJyutpingTrieNodes(const JyutpingTrie *trie, size_t size)
        : trie_(trie), size_(size) {}
    FCITX_INLINE_DEFINE_DEFAULT_DTOR_COPY_AND_MOVE(MatchedJyutpingTrieNodes)

    const JyutpingTrie *trie_;
    JyutpingTriePositions triePositions_;

    // Size of syllables.
    size_t size_;
};

// A cache to store the matched word, encoded Full Jyutping for this word and
// the
// adjustment score.
struct JyutpingMatchResult {
    JyutpingMatchResult(boost::string_view s, float value,
                        boost::string_view encodedJyutping)
        : word_(s, InvalidWordIndex), value_(value),
          encodedJyutping_(encodedJyutping.to_string()) {}
    WordNode word_;
    float value_;
    std::string encodedJyutping_;
};

// class to store current SegmentGraphPath leads to this match and the match
// reuslt.
struct MatchedJyutpingPath {
    MatchedJyutpingPath(const JyutpingTrie *trie, size_t size,
                        SegmentGraphPath path)
        : result_(std::make_shared<MatchedJyutpingTrieNodes>(trie, size)),
          path_(std::move(path)) {}

    MatchedJyutpingPath(std::shared_ptr<MatchedJyutpingTrieNodes> result,
                        SegmentGraphPath path)
        : result_(result), path_(std::move(path)) {}

    FCITX_INLINE_DEFINE_DEFAULT_DTOR_COPY_AND_MOVE(MatchedJyutpingPath)

    auto &triePositions() { return result_->triePositions_; }
    const auto &triePositions() const { return result_->triePositions_; }
    const JyutpingTrie *trie() const { return result_->trie_; }

    // Size of syllables. not necessarily equal to size of path_, because there
    // may be separators.
    auto size() const { return result_->size_; }

    std::shared_ptr<MatchedJyutpingTrieNodes> result_;
    SegmentGraphPath path_;
};

// A list of all search paths
typedef std::vector<MatchedJyutpingPath> MatchedJyutpingPaths;

// Map from SegmentGraphNode to Search Paths.
typedef std::unordered_map<const SegmentGraphNode *, MatchedJyutpingPaths>
    NodeToMatchedJyutpingPathsMap;

// A cache for all JyutpingTries. From a jyutping string to its matched
// JyutpingTrieNode
typedef std::unordered_map<
    const JyutpingTrie *,
    LRUCache<std::string, std::shared_ptr<MatchedJyutpingTrieNodes>>>
    JyutpingTrieNodeCache;

// A cache for JyutpingMatchResult.
typedef std::unordered_map<
    const JyutpingTrie *,
    LRUCache<std::string, std::vector<JyutpingMatchResult>>>
    JyutpingMatchResultCache;

class JyutpingMatchStatePrivate {
public:
    JyutpingMatchStatePrivate(JyutpingContext *context) : context_(context) {}

    JyutpingContext *context_;
    NodeToMatchedJyutpingPathsMap matchedPaths_;
    JyutpingTrieNodeCache nodeCacheMap_;
    JyutpingMatchResultCache matchCacheMap_;
};

} // namespace jyutping
}

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGMATCHSTATE_P_H_
