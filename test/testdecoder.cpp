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
/*
 * Copyright (C) 2017~2017 by CSSlayer
 * wengxt@gmail.com
 *
 * This library is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1 of the
 * License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; see the file COPYING. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#include "libime/core/languagemodel.h"
#include "libime/jyutping/jyutpingdecoder.h"
#include "libime/jyutping/jyutpingdictionary.h"
#include "libime/jyutping/jyutpingencoder.h"
#include "testdir.h"
#include <fcitx-utils/log.h>

using namespace libime;
using namespace libime::jyutping;

void testTime(JyutpingDictionary &, Decoder &decoder, const char *jyutping,
              int nbest = 1) {
    auto graph = JyutpingEncoder::parseUserJyutping(jyutping, true);
    Lattice lattice;
    decoder.decode(lattice, graph, nbest, decoder.model()->nullState(),
                   std::numeric_limits<float>::max(),
                   -std::numeric_limits<float>::max(), Decoder::beamSizeDefault,
                   Decoder::frameSizeDefault, nullptr);
    for (size_t i = 0, e = lattice.sentenceSize(); i < e; i++) {
        auto &sentence = lattice.sentence(i);
        for (auto &p : sentence.sentence()) {
            std::cout << p->word() << " ";
        }
        std::cout << sentence.score() << std::endl;
    }
}

int main() {
    JyutpingDictionary dict;
    dict.load(JyutpingDictionary::SystemDict,
              LIBIME_BINARY_DIR "/data/jyutping.dict",
              JyutpingDictFormat::Binary);
    LanguageModel model(LIBIME_BINARY_DIR "/data/zh_HK.lm");
    JyutpingDecoder decoder(&dict, &model);
    testTime(dict, decoder, "jinhau");

    return 0;
}
