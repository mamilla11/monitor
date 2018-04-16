#include <string>

#include "NetCommon.h"

namespace net {

IpString ip_to_str(const IpAddress& ip)
{
    IpString ip_str;
    for (IpAddress::value_type field : ip) {
        char field_str[std::numeric_limits<IpAddress::value_type>::digits + 2];
        std::snprintf(field_str, 10, "%d.", field);
        ip_str.add_cstring(field_str);
    }

    ip_str.pop_back();  // remove last dot

    return ip_str;
}

MacString mac_to_str(const MacAddress& ip)
{
    MacString mac_str;
    for (MacAddress::value_type field : ip) {
        char field_str[4];
        std::snprintf(field_str, 5, "%02x:", field);
        mac_str.add_cstring(field_str);
    }

    mac_str.pop_back();  // remove last space

    return mac_str;
}

IpAddress str_to_ip(lang::StringView str)
{
    IpAddress addr = {0, 0, 0, 0};
    uint8_t i_field = 0;
    lang::StringFixed<3> field_str;

    for (char c : str)
    {
        bool is_last_field = (i_field == addr.size() - 1);

        if (i_field >= addr.max_size())
            break;

        if (c != '.') {
            if (c >= 48 && c <= 57) {
                field_str.add(c);
                continue;
            }
            else if (not is_last_field) {
                continue;
            }
        }

        addr[i_field] = std::stoi(field_str.data(), 0, 10);
        field_str.clear();
        ++i_field;
    }

    if (not field_str.empty()) {
        addr[addr.size() - 1] = std::stoi(field_str.data(), 0, 10);
    }

    return addr;
}

MacAddress str_to_mac(lang::StringView str)
{
    MacAddress addr;
    uint8_t i_field = 0;
    lang::StringFixed<2> field_str;

    for (char c : str)
    {
        bool is_last_field = (i_field == addr.size() - 1);

        if (i_field >= addr.max_size())
            break;

        if (c != ':') {
            if ((c >= 48 && c <= 57) ||
                (c >= 65 && c <= 70)) {
                field_str.add(c);
                continue;
            }
            else if (not is_last_field) {
                continue;
            }
        }

        addr[i_field] = std::stoi(field_str.data(), 0, 16);
        field_str.clear();
        ++i_field;
    }

    if (not field_str.empty()) {
        addr[addr.size() - 1] = std::stoi(field_str.data(), 0, 16);
    }

    return addr;
}

} //  namespace net



















