/* -*- c++ -*- */
/*
 * Copyright 2026 Jeff Long
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <uhd/usrp/multi_usrp.hpp>

#include <memory>
#include <utility>

/*| role: a non-null device handle an offline case gives a block for one call. It passes the
        block's checks for a missing device. The call then reaches the paths that refuse or
        hold a tune before any device call. No device stands behind it, and a call through it
        is undefined behavior.
*/
namespace capture::uhd::test {

/*| contract: run call with the placeholder handle in block._usrp and return what call
        returns. The handle is reset when call returns or throws. The block holds it for that
        call alone.
*/
template <typename Block, typename Call>
decltype(auto) withPlaceholderHandle(Block& block, Call&& call) {
    auto device = std::make_shared<int>(0);
    block._usrp = ::uhd::usrp::multi_usrp::sptr(device, reinterpret_cast<::uhd::usrp::multi_usrp*>(device.get()));
    struct Reset {
        Block& held;
        ~Reset() { held._usrp.reset(); }
    } reset{block};
    return std::forward<Call>(call)();
}

} // namespace capture::uhd::test
