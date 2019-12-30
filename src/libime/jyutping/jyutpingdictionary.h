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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDICTIONARY_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDICTIONARY_H_

#include "libimejyutping_export.h"
#include <libime/core/triedictionary.h>

namespace libime {
namespace jyutping {

enum class JyutpingDictFormat { Text, Binary };

class JyutpingDictionaryPrivate;

typedef std::function<bool(std::string_view encodedJyutping,
                           std::string_view hanzi, float cost)>
    JyutpingMatchCallback;
class JyutpingDictionary;

using JyutpingTrie = typename TrieDictionary::TrieType;

class LIBIMEJYUTPING_EXPORT JyutpingDictionary : public TrieDictionary {
public:
    static const size_t SystemDict = 0;
    static const size_t UserDict = 1;
    explicit JyutpingDictionary();
    ~JyutpingDictionary();

    // Load dicitonary for a specific dict.
    void load(size_t idx, std::istream &in, JyutpingDictFormat format);
    void load(size_t idx, const char *filename, JyutpingDictFormat format);

    // Match the word by encoded jyutping.
    void matchWords(const char *data, size_t size,
                    JyutpingMatchCallback callback) const;

    void save(size_t idx, const char *filename, JyutpingDictFormat format);
    void save(size_t idx, std::ostream &out, JyutpingDictFormat format);

    void addWord(size_t idx, std::string_view fullJyutping,
                 std::string_view hanzi, float cost = 0.0f);

    using dictionaryChanged = TrieDictionary::dictionaryChanged;

protected:
    void
    matchPrefixImpl(const SegmentGraph &graph,
                    const GraphMatchCallback &callback,
                    const std::unordered_set<const SegmentGraphNode *> &ignore,
                    void *helper) const override;

private:
    void loadText(size_t idx, std::istream &in);
    void loadBinary(size_t idx, std::istream &in);
    void saveText(size_t idx, std::ostream &out);

    std::unique_ptr<JyutpingDictionaryPrivate> d_ptr;
    FCITX_DECLARE_PRIVATE(JyutpingDictionary);
};

} // namespace jyutping
} // namespace libime

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGDICTIONARY_H_
