#pragma once

#include <variant>

#include <base/date.hpp>
#include <base/defs.hpp>
#include <base/timestamp.hpp>

namespace ctptrader::base {

struct alignas(8) Bar {
  Date trading_day_;      // +8 bytes
  Timestamp update_time_; // +16 bytes
  ID instrument_id_;      // +4 bytes
  Price open_;            // +8 bytes
  Price high_;            // +8 bytes
  Price low_;             // +8 bytes
  Price close_;           // +8 bytes
  Volume volume_;         // +4 bytes
  Money turnover_;        // +8 bytes
};
static_assert(sizeof(Bar) == 80);

struct alignas(8) Static {
  Date trading_day_;  // +8 bytes
  ID instrument_id_;  // +4 bytes
  Price prev_close_;  // +8 bytes
  Price upper_limit_; // +8 bytes
  Price lower_limit_; // +8 bytes
};
static_assert(sizeof(Static) == 40);

struct alignas(8) Depth {
  Timestamp update_time_; // +16 bytes
  ID instrument_id_;      // +4 bytes
  Price open_;            // +8 bytes
  Price high_;            // +8 bytes
  Price low_;             // +8 bytes
  Price last_;            // +8 bytes
  Volume volume_;         // +4 bytes
  Money turnover_;        // +8 bytes
  Price ask_price_[5];    // +40 bytes
  Price bid_price_[5];    // +40 bytes
  Volume ask_volume_[5];  // +20 bytes
  Volume bid_volume_[5];  // +20 bytes
};
static_assert(sizeof(Depth) == 192);

using Msg = std::variant<Bar, Static, Depth>;

} // namespace ctptrader::base