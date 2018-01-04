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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDECODER_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDECODER_H_

#include "libimejyutping_export.h"
#include <libime/core/decoder.h>
#include <libime/jyutping/jyutpingdictionary.h>

namespace libime {
namespace jyutping {

class JyutpingLatticeNodePrivate;

class LIBIMEJYUTPING_EXPORT JyutpingLatticeNode : public LatticeNode {
public:
    JyutpingLatticeNode(boost::string_view word, WordIndex idx,
                        SegmentGraphPath path, const State &state, float cost,
                        std::unique_ptr<JyutpingLatticeNodePrivate> data);
    virtual ~JyutpingLatticeNode();

    const std::string &encodedJyutping() const;

private:
    std::unique_ptr<JyutpingLatticeNodePrivate> d_ptr;
};

class LIBIMEJYUTPING_EXPORT JyutpingDecoder : public Decoder {
public:
    JyutpingDecoder(const JyutpingDictionary *dict,
                    const LanguageModelBase *model)
        : Decoder(dict, model) {}

protected:
    LatticeNode *createLatticeNodeImpl(const SegmentGraphBase &graph,
                                       const LanguageModelBase *model,
                                       boost::string_view word, WordIndex idx,
                                       SegmentGraphPath path,
                                       const State &state, float cost,
                                       std::unique_ptr<LatticeNodeData> data,
                                       bool onlyPath) const override;
};
} // namespace jyutping
}

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDECODER_H_
