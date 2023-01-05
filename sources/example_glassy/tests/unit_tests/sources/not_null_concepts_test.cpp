// SPDX-FileCopyrightText: © 2023 Melg Eight <public.melg8@gmail.com>
//
// SPDX-License-Identifier: MIT

#include <not_null_concepts.h>

#include <asn_1_int.h>

namespace glassy::test {

// Any type wrapped in gsl::not_null<T> should pass this.
static_assert(is_not_null<Asn1IntOwner>);
static_assert(is_not_null<Asn1IntegerNotNull>);
static_assert(is_not_null<Asn1IntegerConstNotNull>);

// If type is not wrapped in gsl::not_null it should not pass.
static_assert(!is_not_null<Asn1IntHolder>);

static_assert(is_not_null_of_raw_pointer<Asn1IntegerNotNull>);

// not_null_provider_of concept mark a type which:
// 1. Wrapped in gsl::not_null<T>
// 2. Is pointer of second argument type
// 3. Or is smart pointer with get() method providing pointer same as second
// argument type.
static_assert(not_null_provider_of<Asn1IntOwner, const ASN1_INTEGER*>);
static_assert(!not_null_provider_of<Asn1IntHolder, const ASN1_INTEGER*>);

}  // namespace glassy::test
