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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGMATCHSTATE_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGMATCHSTATE_H_

#include "libimejyutping_export.h"
#include <fcitx-utils/macros.h>
#include <libime/jyutping/jyutpingencoder.h>
#include <unordered_set>

namespace libime {

class SegmentGraphNode;

namespace jyutping {

class JyutpingMatchStatePrivate;
class JyutpingContext;

// Provides caching mechanism used by JyutpingContext.
class LIBIMEJYUTPING_EXPORT JyutpingMatchState {
    friend class JyutpingMatchContext;

public:
    JyutpingMatchState(JyutpingContext *context);
    ~JyutpingMatchState();

    // Invalidate everything in the state.
    void clear();

    // Invalidate a set of node, usually caused by the change of user input.
    void discardNode(const std::unordered_set<const SegmentGraphNode *> &node);

    // Invalidate a whole dictionary, usually caused by the change to the
    // dictionary.
    void discardDictionary(size_t idx);

private:
    std::unique_ptr<JyutpingMatchStatePrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(JyutpingMatchState);
};

} // namespace jyutping
} // namespace libime

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGMATCHSTATE_H_
