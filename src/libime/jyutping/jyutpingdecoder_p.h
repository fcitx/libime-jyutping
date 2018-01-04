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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDECODER_P_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDECODER_P_H_

#include "jyutpingdecoder.h"
#include <libime/core/lattice.h>

namespace libime {
namespace jyutping {

class JyutpingLatticeNodePrivate : public LatticeNodeData {
public:
    JyutpingLatticeNodePrivate(boost::string_view encodedJyutping)
        : encodedJyutping_(encodedJyutping.to_string()) {}

    std::string encodedJyutping_;
};

} // namespace jyutping
}

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDECODER_P_H_
