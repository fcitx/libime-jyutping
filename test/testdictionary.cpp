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

#include "libime/jyutping/jyutpingdictionary.h"
#include "libime/jyutping/jyutpingencoder.h"
#include "testdir.h"
#include <fcitx-utils/log.h>

int main() {
    using namespace libime;
    using namespace libime::jyutping;
    JyutpingDictionary dict;
    dict.load(JyutpingDictionary::SystemDict,
              LIBIME_BINARY_DIR "/data/jyutping.dict",
              JyutpingDictFormat::Binary);

    char c[] = {static_cast<char>(JyutpingInitial::J),
                static_cast<char>(JyutpingFinal::IN),
                static_cast<char>(JyutpingInitial::H),
                static_cast<char>(JyutpingFinal::AU)};
    dict.matchWords(c, 2,
                    [c](boost::string_view encodedJyutping,
                        boost::string_view hanzi, float cost) {
                        std::cout << JyutpingEncoder::decodeFullJyutping(
                                         encodedJyutping)
                                  << " " << hanzi << " " << cost << std::endl;
                        return true;
                    });
    dict.matchWords(c + 2, 2,
                    [c](boost::string_view encodedJyutping,
                        boost::string_view hanzi, float cost) {
                        std::cout << JyutpingEncoder::decodeFullJyutping(
                                         encodedJyutping)
                                  << " " << hanzi << " " << cost << std::endl;
                        return true;
                    });

    auto graph = JyutpingEncoder::parseUserJyutping("jinhau", true);
    dict.matchPrefix(graph,
                     [&graph](const SegmentGraphPath &path, WordNode &node,
                              float, std::unique_ptr<LatticeNodeData>) {
                         for (auto &step : path) {
                             std::cout << step->index() << " ";
                         }
                         std::cout << node.word() << std::endl;
                         return true;
                     });

    dict.save(0, LIBIME_BINARY_DIR "/test/testjyutpingdictionary.dict",
              JyutpingDictFormat::Binary);
    // dict.save(0, std::cout, JyutpingDictFormat::Text);
    return 0;
}
