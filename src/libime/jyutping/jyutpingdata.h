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
#ifndef _FCITX_LIBIME_JYUTPING_JYUTPINGDATA_H_
#define _FCITX_LIBIME_JYUTPING_JYUTPINGDATA_H_

#include "libimejyutping_export.h"
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index_container.hpp>
#include <libime/jyutping/jyutpingencoder.h>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace libime {
namespace jyutping {
struct JyutpingHash : std::unary_function<boost::string_view, std::size_t> {
    std::size_t operator()(boost::string_view const &val) const {
        return boost::hash_range(val.begin(), val.end());
    }
};

class JyutpingEntry {
public:
    JyutpingEntry(const char *jyutping, JyutpingInitial initial,
                  JyutpingFinal final, bool fuzzy = false)
        : jyutping_(jyutping), initial_(initial), final_(final), fuzzy_(fuzzy) {
    }

    boost::string_view jyutping() const { return jyutping_; }
    JyutpingInitial initial() const { return initial_; }
    JyutpingFinal final() const { return final_; }
    bool fuzzy() const { return fuzzy_; }

private:
    std::string jyutping_;
    JyutpingInitial initial_;
    JyutpingFinal final_;
    bool fuzzy_;
};

using JyutpingMap = boost::multi_index_container<
    JyutpingEntry,
    boost::multi_index::indexed_by<boost::multi_index::hashed_non_unique<
        boost::multi_index::const_mem_fun<JyutpingEntry, boost::string_view,
                                          &JyutpingEntry::jyutping>,
        JyutpingHash>>>;

LIBIMEJYUTPING_EXPORT
const JyutpingMap &getJyutpingMap();
LIBIMEJYUTPING_EXPORT const std::vector<bool> &getEncodedInitialFinal();

LIBIMEJYUTPING_EXPORT const
    std::unordered_map<std::string, std::pair<std::string, std::string>> &
    getInnerSegment();

} // namespace jyutping
} // namespace libime

#endif // _FCITX_LIBIME_JYUTPING_JYUTPINGDATA_H_
