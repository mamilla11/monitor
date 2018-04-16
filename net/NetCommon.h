#pragma once

#include <etl/array.h>
#include <lang/StringFixed.hpp>
#include <lang/StringView.h>

namespace net {

enum class Protocol : uint8_t {
    TCP,
    UDP,
    MAC_RAW,
	NONE,
};

using MacAddress = etl::array<uint8_t, 6>;
using IpAddress = etl::array<uint8_t, 4>;
using SubnetMask = etl::array<uint8_t, 4>;
using GatewayIpAdress = etl::array<uint8_t, 4>;

using IpString = lang::StringFixed<IpAddress::SIZE*3+3>;
using MacString = lang::StringFixed<MacAddress::SIZE*2+4>;

using Port = uint16_t;

IpString   ip_to_str(const IpAddress& ip);
MacString  mac_to_str(const MacAddress& ip);

IpAddress str_to_ip(lang::StringView str);
MacAddress str_to_mac(lang::StringView str);


} /* namespace net */
