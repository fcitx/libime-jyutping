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
#ifndef _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGCONTEXT_H_
#define _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGCONTEXT_H_

#include "libimejyutping_export.h"
#include <libime/core/inputbuffer.h>
#include <libime/core/lattice.h>
#include <libime/jyutping/jyutpingime.h>
#include <vector>

namespace libime {
namespace jyutping {

class JyutpingContextPrivate;

class LIBIMEJYUTPING_EXPORT JyutpingContext : public InputBuffer {
public:
  JyutpingContext(JyutpingIME *ime);
  virtual ~JyutpingContext();

  void erase(size_t from, size_t to) override;
  void setCursor(size_t pos) override;

  const std::vector<SentenceResult> &candidates() const;
  void select(size_t idx);
  void cancel();
  bool cancelTill(size_t pos);

  bool selected() const;
  std::string sentence() const {
    auto &c = candidates();
    if (c.size()) {
      return selectedSentence() + c[0].toString();
    } else {
      return selectedSentence();
    }
  }

  std::string preedit() const;
  std::pair<std::string, size_t> preeditWithCursor() const;
  std::string selectedSentence() const;
  size_t selectedLength() const;

  std::vector<std::string> selectedWords() const;

  std::string selectedFullJyutping() const;
  std::string candidateFullJyutping(size_t i) const;

  void learn();

  int jyutpingBeforeCursor() const;
  int jyutpingAfterCursor() const;

  JyutpingIME *ime() const;

  State state() const;

protected:
  void typeImpl(const char *s, size_t length) override;

private:
  void update();
  bool learnWord();
  std::unique_ptr<JyutpingContextPrivate> d_ptr;
  FCITX_DECLARE_PRIVATE(JyutpingContext);
};

} // namespace jyutping
} // namespace libime

#endif // _LIBIME_JYUTPING_LIBIME_JYUTPING_JYUTPINGCONTEXT_H_
