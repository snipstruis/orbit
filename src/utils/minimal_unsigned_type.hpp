#pragma once

#include <cstdint>
#include <type_traits>

template<uint64_t M>
using minimal_unsigned_type =
 typename std::conditional< (M <= UINT8_MAX),  uint8_t,
  typename std::conditional< (M <= UINT16_MAX), uint16_t,
   typename std::conditional< (M <= UINT32_MAX), uint32_t, uint64_t>::type
  >::type
>::type;
