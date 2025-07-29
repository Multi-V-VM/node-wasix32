// Copyright 2011 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_STRINGS_CHAR_PREDICATES_H_
#define V8_STRINGS_CHAR_PREDICATES_H_

#include "src/base/strings.h"
#include "src/strings/unicode.h"

namespace v8 {
namespace internal {

// Unicode character predicates as defined by ECMA-262, 3rd,
// used for lexical analysis.

inline constexpr int AsciiAlphaToLower(::v8::base::uc32 c);
inline constexpr bool IsCarriageReturn(::v8::base::uc32 c);
inline constexpr bool IsLineFeed(::v8::base::uc32 c);
inline constexpr bool IsAsciiIdentifier(::v8::base::uc32 c);
inline constexpr bool IsAlphaNumeric(::v8::base::uc32 c);
inline constexpr bool IsDecimalDigit(::v8::base::uc32 c);
inline constexpr bool IsHexDigit(::v8::base::uc32 c);
inline constexpr bool IsOctalDigit(::v8::base::uc32 c);
inline constexpr bool IsBinaryDigit(::v8::base::uc32 c);
inline constexpr bool IsRegExpWord(::v8::base::uc32 c);

inline constexpr bool IsAsciiLower(::v8::base::uc32 ch);
inline constexpr bool IsAsciiUpper(::v8::base::uc32 ch);

inline constexpr ::v8::base::uc32 ToAsciiUpper(::v8::base::uc32 ch);
inline constexpr ::v8::base::uc32 ToAsciiLower(::v8::base::uc32 ch);

// ES#sec-names-and-keywords
// This includes '_', '$' and '\', and ID_Start according to
// http://www.unicode.org/reports/tr31/, which consists of categories
// 'Lu', 'Ll', 'Lt', 'Lm', 'Lo', 'Nl', but excluding properties
// 'Pattern_Syntax' or 'Pattern_White_Space'.
inline bool IsIdentifierStart(::v8::base::uc32 c);
#ifdef V8_INTL_SUPPORT
V8_EXPORT_PRIVATE bool IsIdentifierStartSlow(::v8::base::uc32 c);
#else
inline bool IsIdentifierStartSlow(::v8::base::uc32 c) {
  // Non-BMP characters are not supported without I18N.
  return (c <= 0xFFFF) ? unibrow::ID_Start::Is(c) : false;
}
#endif

// ES#sec-names-and-keywords
// This includes \u200c and \u200d, and ID_Continue according to
// http://www.unicode.org/reports/tr31/, which consists of ID_Start,
// the categories 'Mn', 'Mc', 'Nd', 'Pc', but excluding properties
// 'Pattern_Syntax' or 'Pattern_White_Space'.
inline bool IsIdentifierPart(::v8::base::uc32 c);
#ifdef V8_INTL_SUPPORT
V8_EXPORT_PRIVATE bool IsIdentifierPartSlow(::v8::base::uc32 c);
#else
inline bool IsIdentifierPartSlow(::v8::base::uc32 c) {
  // Non-BMP charaacters are not supported without I18N.
  if (c <= 0xFFFF) {
    return unibrow::ID_Start::Is(c) || unibrow::ID_Continue::Is(c);
  }
  return false;
}
#endif

// ES6 draft section 11.2
// This includes all code points of Unicode category 'Zs'.
// Further included are \u0009, \u000b, \u000c, and \ufeff.
inline bool IsWhiteSpace(::v8::base::uc32 c);
#ifdef V8_INTL_SUPPORT
V8_EXPORT_PRIVATE bool IsWhiteSpaceSlow(::v8::base::uc32 c);
#else
inline bool IsWhiteSpaceSlow(::v8::base::uc32 c) {
  return unibrow::WhiteSpace::Is(c);
}
#endif

// WhiteSpace and LineTerminator according to ES6 draft section 11.2 and 11.3
// This includes all the characters with Unicode category 'Z' (= Zs+Zl+Zp)
// as well as \u0009 - \u000d and \ufeff.
inline bool IsWhiteSpaceOrLineTerminator(::v8::base::uc32 c);
inline bool IsWhiteSpaceOrLineTerminatorSlow(::v8::base::uc32 c) {
  return IsWhiteSpaceSlow(c) || unibrow::IsLineTerminator(c);
}

inline bool IsLineTerminatorSequence(::v8::base::uc32 c, ::v8::base::uc32 next);

}  // namespace internal
}  // namespace v8

#endif  // V8_STRINGS_CHAR_PREDICATES_H_
