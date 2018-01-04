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

#include "libime/core/historybigram.h"
#include "libime/core/lattice.h"
#include "libime/core/userlanguagemodel.h"
#include "libime/jyutping/jyutpingcontext.h"
#include "libime/jyutping/jyutpingdecoder.h"
#include "libime/jyutping/jyutpingdictionary.h"
#include "libime/jyutping/jyutpingime.h"
#include "testdir.h"
#include <boost/algorithm/string.hpp>
#include <boost/iostreams/device/null.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <fcitx-utils/log.h>
#include <fcitx-utils/stringutils.h>
#include <fstream>
#include <functional>
#include <sstream>

using namespace libime;
using namespace libime::jyutping;

int main(int argc, char *argv[]) {
    fcitx::Log::setLogRule("libime=5");
    JyutpingIME ime(std::make_unique<JyutpingDictionary>(),
                    std::make_unique<UserLanguageModel>(LIBIME_BINARY_DIR
                                                        "/data/zh_HK.lm"));
    ime.setNBest(2);
    ime.dict()->load(JyutpingDictionary::SystemDict,
                     LIBIME_BINARY_DIR "/data/jyutping.dict",
                     JyutpingDictFormat::Binary);
    if (argc >= 2) {
        ime.dict()->load(JyutpingDictionary::UserDict, argv[1],
                         JyutpingDictFormat::Binary);
    }
    if (argc >= 3) {
        std::fstream fin(argv[2], std::ios::in | std::ios::binary);
        ime.model()->history().load(fin);
    }
    ime.setScoreFilter(1.0f);
    JyutpingContext c(&ime);

    std::string word;
    while (std::cin >> word) {
        bool printAll = false;
        if (word == "back") {
            c.backspace();
        } else if (word == "reset") {
            c.clear();
        } else if (word.size() == 1 &&
                   (('a' <= word[0] && word[0] <= 'z') ||
                    (!c.userInput().empty() && word[0] == '\''))) {
            c.type(word);
        } else if (word.size() == 1 && ('0' <= word[0] && word[0] <= '9')) {
            size_t idx;
            if (word[0] == '0') {
                idx = 9;
            } else {
                idx = word[0] - '1';
            }
            if (c.candidates().size() >= idx) {
                c.select(idx);
            }
        } else if (word == "all") {
            printAll = true;
        }
        if (c.selected()) {
            std::cout << "COMMIT:   " << c.preedit() << std::endl;
            c.learn();
            c.clear();
            continue;
        }
        std::cout << "PREEDIT:  " << c.preedit() << std::endl;
        std::cout << "SENTENCE: " << c.sentence() << std::endl;
        size_t count = 1;
        for (auto &candidate : c.candidates()) {
            std::cout << (count % 10) << ": ";
            for (auto node : candidate.sentence()) {
                auto &jyutping = static_cast<const JyutpingLatticeNode *>(node)
                                     ->encodedJyutping();
                std::cout << node->word();
                if (!jyutping.empty()) {
                    std::cout << " "
                              << JyutpingEncoder::decodeFullJyutping(jyutping);
                }
            }
            std::cout << " " << candidate.score() << std::endl;
            count++;
            if (!printAll && count > 10) {
                break;
            }
        }
    }

    boost::iostreams::stream<boost::iostreams::null_sink> nullOstream(
        (boost::iostreams::null_sink()));
    ime.dict()->save(JyutpingDictionary::UserDict, nullOstream,
                     JyutpingDictFormat::Binary);
    ime.model()->history().dump(nullOstream);

    return 0;
}
