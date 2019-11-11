//
// Copyright (C) 2017~2017 by CSSlayer
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
#ifndef _LIBIME_JYUTPING_JYUTPINGENCODER_H_
#define _LIBIME_JYUTPING_JYUTPINGENCODER_H_

#include "libimejyutping_export.h"
#include <libime/core/segmentgraph.h>

namespace libime {
namespace jyutping {

enum class JyutpingInitial : char {
  Invalid = 0,
  B = 'A',
  P,
  M,
  F,
  D,
  T,
  N,
  L,
  G,
  K,
  NG,
  H,
  GW,
  KW,
  W,
  Z,
  C,
  S,
  J,
  Zero,
};

enum class JyutpingFinal : char {
  Invalid = 0,
  AA = 'A',
  AAI,
  AAU,
  AAM,
  AAN,
  AANG,
  AAP,
  AAT,
  AAK,
  AI,
  AU,
  AM,
  AN,
  ANG,
  AP,
  AT,
  AK,
  E,
  EI,
  ET,
  EU,
  EM,
  EN,
  ENG,
  EP,
  EK,
  I,
  IU,
  IM,
  IN,
  ING,
  IP,
  IT,
  IK,
  O,
  OI,
  OU,
  ON,
  ONG,
  OT,
  OK,
  OE,
  OENG,
  OEK,
  OM,
  EOI,
  EON,
  EOT,
  U,
  UI,
  UN,
  UNG,
  UT,
  UK,
  YU,
  YUN,
  YUT,
  M,
  NG,
  Zero,
};

struct LIBIMEJYUTPING_EXPORT JyutpingSyllable {
public:
  JyutpingSyllable(JyutpingInitial initial, JyutpingFinal final)
      : initial_(initial), final_(final) {}
  FCITX_INLINE_DEFINE_DEFAULT_DTOR_AND_COPY(JyutpingSyllable)

  JyutpingInitial initial() const { return initial_; }
  JyutpingFinal final() const { return final_; }

  std::string toString() const;

  bool operator==(const JyutpingSyllable &other) const {
    return initial_ == other.initial_ && final_ == other.final_;
  }

  bool operator!=(const JyutpingSyllable &other) const {
    return !(*this == other);
  }
  bool operator<(const JyutpingSyllable &other) const {
    return std::make_pair(initial_, final_) <
           std::make_pair(other.initial_, other.final_);
  }
  bool operator<=(const JyutpingSyllable &other) const {
    return *this < other || *this == other;
  }
  bool operator>(const JyutpingSyllable &other) const {
    return !(*this <= other);
  }
  bool operator>=(const JyutpingSyllable &other) const {
    return !(*this < other);
  }

private:
  JyutpingInitial initial_;
  JyutpingFinal final_;
};

using MatchedJyutpingSyllables = std::vector<
    std::pair<JyutpingInitial, std::vector<std::pair<JyutpingFinal, bool>>>>;

class LIBIMEJYUTPING_EXPORT JyutpingEncoder {
public:
  static SegmentGraph parseUserJyutping(boost::string_view jyutping,
                                        bool inner = true);
  static std::vector<char> encodeOneUserJyutping(boost::string_view jyutping);
  static bool isValidUserJyutping(const char *data, size_t size);

  static std::vector<char> encodeFullJyutping(boost::string_view jyutping);

  static std::string decodeFullJyutping(const char *data, size_t size);
  static std::string decodeFullJyutping(boost::string_view data) {
    return decodeFullJyutping(data.data(), data.size());
  }

  static MatchedJyutpingSyllables
  stringToSyllables(boost::string_view jyutping);

  static const std::string &initialToString(JyutpingInitial initial);
  static JyutpingInitial stringToInitial(const std::string &str);
  static bool isValidInitial(char c) {
    return c >= firstInitial && c <= lastInitial;
  }

  static const std::string &finalToString(JyutpingFinal final);
  static JyutpingFinal stringToFinal(const std::string &str);
  static bool isValidFinal(char c) { return c >= firstFinal && c <= lastFinal; }

  static bool isValidInitialFinal(JyutpingInitial initial, JyutpingFinal final);

  static const char firstInitial = static_cast<char>(JyutpingInitial::B);
  static const char lastInitial = static_cast<char>(JyutpingInitial::Zero);
  static const char firstFinal = static_cast<char>(JyutpingFinal::AA);
  static const char lastFinal = static_cast<char>(JyutpingFinal::Zero);
};

} // namespace jyutping
} // namespace libime

#endif // _LIBIME_JYUTPING_JYUTPINGENCODER_H_
