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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUPINGIME_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUPINGIME_H_

#include "libimejyutping_export.h"
#include <fcitx-utils/connectableobject.h>
#include <fcitx-utils/macros.h>
#include <libime/jyutping/jyutpingencoder.h>
#include <limits>
#include <memory>

namespace libime {

class UserLanguageModel;

namespace jyutping {

class JyutpingIMEPrivate;
class JyutpingDecoder;
class JyutpingDictionary;

/// \brief Provides shared data for JyutpingContext.
class LIBIMEJYUTPING_EXPORT JyutpingIME : public fcitx::ConnectableObject {
public:
    JyutpingIME(std::unique_ptr<JyutpingDictionary> dict,
                std::unique_ptr<UserLanguageModel> model);
    virtual ~JyutpingIME();

    bool innerSegment() const;
    void setInnerSegment(bool inner);
    size_t nbest() const;
    void setNBest(size_t n);
    size_t beamSize() const;
    void setBeamSize(size_t n);
    size_t frameSize() const;
    void setFrameSize(size_t n);
    void setScoreFilter(float maxDistance = std::numeric_limits<float>::max(),
                        float minPath = -std::numeric_limits<float>::max());

    float maxDistance() const;
    float minPath() const;

    JyutpingDictionary *dict();
    const JyutpingDictionary *dict() const;
    const JyutpingDecoder *decoder() const;
    UserLanguageModel *model();
    const UserLanguageModel *model() const;

    FCITX_DECLARE_SIGNAL(JyutpingIME, optionChanged, void());

private:
    std::unique_ptr<JyutpingIMEPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(JyutpingIME);
};

} // namespace jyutping
} // namespace libime

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUPINGIME_H_
