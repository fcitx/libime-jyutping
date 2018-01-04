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

#include "jyutpingime.h"
#include "jyutpingdecoder.h"
#include "libime/core/userlanguagemodel.h"

namespace libime {
namespace jyutping {

class JyutpingIMEPrivate : fcitx::QPtrHolder<JyutpingIME> {
public:
    JyutpingIMEPrivate(JyutpingIME *q, std::unique_ptr<JyutpingDictionary> dict,
                       std::unique_ptr<UserLanguageModel> model)
        : fcitx::QPtrHolder<JyutpingIME>(q), dict_(std::move(dict)),
          model_(std::move(model)), decoder_(std::make_unique<JyutpingDecoder>(
                                        dict_.get(), model_.get())) {}

    FCITX_DEFINE_SIGNAL_PRIVATE(JyutpingIME, optionChanged);

    std::unique_ptr<JyutpingDictionary> dict_;
    std::unique_ptr<UserLanguageModel> model_;
    std::unique_ptr<JyutpingDecoder> decoder_;
    bool innerSegment_ = true;
    size_t nbest_ = 1;
    size_t beamSize_ = Decoder::beamSizeDefault;
    size_t frameSize_ = Decoder::frameSizeDefault;
    float maxDistance_ = std::numeric_limits<float>::max();
    float minPath_ = -std::numeric_limits<float>::max();
};

JyutpingIME::JyutpingIME(std::unique_ptr<JyutpingDictionary> dict,
                         std::unique_ptr<UserLanguageModel> model)
    : d_ptr(std::make_unique<JyutpingIMEPrivate>(this, std::move(dict),
                                                 std::move(model))) {}

JyutpingIME::~JyutpingIME() {}

bool JyutpingIME::innerSegment() const {
    FCITX_D();
    return d->innerSegment_;
}

void JyutpingIME::setInnerSegment(bool inner) {
    FCITX_D();
    d->innerSegment_ = inner;
    emit<JyutpingIME::optionChanged>();
}

JyutpingDictionary *JyutpingIME::dict() {
    FCITX_D();
    return d->dict_.get();
}

const JyutpingDictionary *JyutpingIME::dict() const {
    FCITX_D();
    return d->dict_.get();
}

const JyutpingDecoder *JyutpingIME::decoder() const {
    FCITX_D();
    return d->decoder_.get();
}

UserLanguageModel *JyutpingIME::model() {
    FCITX_D();
    return d->model_.get();
}

const UserLanguageModel *JyutpingIME::model() const {
    FCITX_D();
    return d->model_.get();
}

size_t JyutpingIME::nbest() const {
    FCITX_D();
    return d->nbest_;
}

void JyutpingIME::setNBest(size_t n) {
    FCITX_D();
    if (d->nbest_ != n) {
        d->nbest_ = n;
        emit<JyutpingIME::optionChanged>();
    }
}

size_t JyutpingIME::beamSize() const {
    FCITX_D();
    return d->beamSize_;
}

void JyutpingIME::setBeamSize(size_t n) {
    FCITX_D();
    if (d->beamSize_ != n) {
        d->beamSize_ = n;
        emit<JyutpingIME::optionChanged>();
    }
}

size_t JyutpingIME::frameSize() const {
    FCITX_D();
    return d->frameSize_;
}

void JyutpingIME::setFrameSize(size_t n) {
    FCITX_D();
    if (d->frameSize_ != n) {
        d->frameSize_ = n;
        emit<JyutpingIME::optionChanged>();
    }
}

void JyutpingIME::setScoreFilter(float maxDistance, float minPath) {
    FCITX_D();
    if (d->maxDistance_ != maxDistance || d->minPath_ != minPath) {
        d->maxDistance_ = maxDistance;
        d->minPath_ = minPath;
        emit<JyutpingIME::optionChanged>();
    }
}

float JyutpingIME::maxDistance() const {
    FCITX_D();
    return d->maxDistance_;
}

float JyutpingIME::minPath() const {
    FCITX_D();
    return d->minPath_;
}
}
}
