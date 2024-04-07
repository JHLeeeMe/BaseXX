/// rfc4648
/// https://datatracker.ietf.org/doc/html/rfc4648
///

#ifndef BASEXX_H
#define BASEXX_H


#include <cstdint>  // uint8_t
#include <type_traits>  // std::enable_if_t, std::is_same_v
#include <string>
#include <vector>
#if __cplusplus >= 201703L
#    include <string_view>
#endif  // __cplusplus >= 201703L

#define FALLTHROUGH do{} while (false)

/// The Base 64 Alphabet Table
/// https://datatracker.ietf.org/doc/html/rfc4648#section-4
///
static const uint8_t base64_table[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  // 24 ~ 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  // 32 ~ 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  // 40 ~ 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3',  // 48 ~ 55
    '4', '5', '6', '7', '8', '9', '+', '/',  // 56 ~ 63
};

/// The Base 64 Alphabet Table (URL and Filename safe)
/// https://datatracker.ietf.org/doc/html/rfc4648#section-5
///
static const uint8_t base64_urlsafe_table[64] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
    'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  // 24 ~ 31
    'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  // 32 ~ 39
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  // 40 ~ 47
    'w', 'x', 'y', 'z', '0', '1', '2', '3',  // 48 ~ 55
    '4', '5', '6', '7', '8', '9', '-', '_',  // 56 ~ 63
};

/// The Base 32 Alphabet Table
/// https://datatracker.ietf.org/doc/html/rfc4648#section-6
///
static const uint8_t base32_table[32] = {
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
    'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
    'Y', 'Z', '2', '3', '4', '5', '6', '7',  // 24 ~ 31
};

/// The Base 32 Alphabet Table (Extended Hex)
/// https://datatracker.ietf.org/doc/html/rfc4648#section-7
///
static const uint8_t base32_hex_table[32] = {
    '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
    'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',  // 16 ~ 23
    'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',  // 24 ~ 31
};

/// The Base 16 Alphabet Table
/// https://datatracker.ietf.org/doc/html/rfc4648#section-8
///
static const uint8_t base16_table[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
};

namespace BaseXX
{
namespace _64_
{
    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* encoding_table = base64_table)
    {
        std::string encoded{};
        uint8_t arr_3[3] = {0,};
        uint8_t arr_4[4] = {0,};

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            arr_3[i++] = data[pos];
            if (i == 3)
            {
                arr_4[0] = (arr_3[0] & 0b1111'1100) >> 2;
                arr_4[1] = ((arr_3[0] & 0b0000'0011) << 4) + ((arr_3[1] & 0b1111'0000) >> 4);
                arr_4[2] = ((arr_3[1] & 0b0000'1111) << 2) + ((arr_3[2] & 0b1100'0000) >> 6);
                arr_4[3] = arr_3[2] & 0b0011'1111;

                for (size_t k = 0; k < 4; k++)
                {
                    encoded += encoding_table[arr_4[k]];
                }

                i = 0;
            }
        }

        if (i)  // i == 1 or i == 2
        {
            memset(arr_3 + i, 0x00, 3 - i);

            arr_4[0] = (arr_3[0] & 0b1111'1100) >> 2;
            arr_4[1] = ((arr_3[0] & 0b0000'0011) << 4) + ((arr_3[1] & 0b1111'0000) >> 4);
            arr_4[2] = (arr_3[1] & 0b0000'1111) << 2;

            for (size_t j = 0; j < i + 1; j++)
            {
                encoded += encoding_table[arr_4[j]];
            }

            size_t tmp = encoded.length() % 4;
            while (tmp++ != 4)
            {
                encoded += '=';
            }
        }

        return encoded;
    }

    /***************************************************************************
    * Helper Functions
    *   using StringType = std::string_view || const std::string&
    *
    *   inline std::string encode(StringType)
    *   inline std::string encode_urlsafe(StringType)
    * 
    *   inline std::string encode(const std::initializer_list<uint8_t>&)
    *   inline std::string encode_urlsafe(const std::initializer_list<uint8_t>&)
    * 
    *   inline std::string encode(const std::vector<uint8_t>&)
    *   inline std::string encode_urlsafe(const std::vector<uint8_t>&)
    ***************************************************************************/
#if __cplusplus >= 201703L
    using StringType = std::string_view;
#else
    using StringType = const std::string&;
#endif

    inline std::string encode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.size());
    }

    inline std::string encode_urlsafe(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.size(), base64_urlsafe_table);
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string encode_urlsafe(
        const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), base64_urlsafe_table);
    }

    inline std::string encode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string encode_urlsafe(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), base64_urlsafe_table);
    }
}  // namespace BaseXX::_64_

namespace _32_
{
    template<
        typename T,
        typename = std::enable_if_t<
            std::is_same_v<T, const char*> ||
            std::is_same_v<T, std::string>>
    >
    inline std::string encode_base(
        const T& data, const uint8_t* encoding_table = base32_table)
    {
        std::string encoded{};
        uint8_t arr_5[5] = { 0, };
        uint8_t arr_8[8] = { 0, };

        size_t data_len = 0;
        if constexpr (std::is_same_v<T, const char*>)
        {
            data_len = strlen(data);
        }
        else
        {
            data_len = data.length();
        }

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            arr_5[i++] = data[pos];
            if (i == 5)
            {
                arr_8[0] = (arr_5[0] & 0b1111'1000) >> 3;
                arr_8[1] = ((arr_5[0] & 0b0000'0111) << 2) + ((arr_5[1] & 0b1100'0000) >> 6);
                arr_8[2] = (arr_5[1] & 0b0011'1110) >> 1;
                arr_8[3] = ((arr_5[1] & 0b0000'0001) << 4) + ((arr_5[2] & 0b1111'0000) >> 4);
                arr_8[4] = ((arr_5[2] & 0b0000'1111) << 1) + ((arr_5[3] & 0b1000'0000) >> 7);
                arr_8[5] = (arr_5[3] & 0b0111'1100) >> 2;
                arr_8[6] = ((arr_5[3] & 0b0000'0011) << 3) + ((arr_5[4] & 0b1110'0000) >> 5);
                arr_8[7] = arr_5[4] & 0b0001'1111;

                for (size_t k = 0; k < 8; k++)
                {
                    encoded += encoding_table[arr_8[k]];
                }
                
                i = 0;
            }
        }

        if (i)  // i == (1 ~ 4)
        {
            memset(arr_5 + i, 0x00, 5 - i);

            switch (i)
            {
            case 4:
                arr_8[6] = (arr_5[3] & 0b0000'0011) << 3;
                arr_8[5] = (arr_5[3] & 0b0111'1100) >> 2;
                FALLTHROUGH;
            case 3:
                arr_8[4] = ((arr_5[2] & 0b0000'1111) << 1) + ((arr_5[3] & 0b1000'0000) >> 7);
                FALLTHROUGH;
            case 2:
                arr_8[3] = ((arr_5[1] & 0b0000'0001) << 4) + ((arr_5[2] & 0b1111'0000) >> 4);
                arr_8[2] = (arr_5[1] & 0b0011'1110) >> 1;
                FALLTHROUGH;
            case 1:
                arr_8[1] = ((arr_5[0] & 0b0000'0111) << 2) + ((arr_5[1] & 0b1100'0000) >> 6);
                arr_8[0] = (arr_5[0] & 0b1111'1000) >> 3;
                FALLTHROUGH;
            default:
                break;
            }

            size_t k = 0;
            if (i == 1)
            {
                k = i + 1;
            }
            else if (i == 4)
            {
                k = i + 3;
            }
            else
            {
                k = i + 2;
            }

            for (size_t j = 0; j < k; j++)
            {
                encoded += encoding_table[arr_8[j]];
            }

            size_t tmp = encoded.length() % 8;
            while (tmp++ != 8)
            {
                encoded += '=';
            }
        }

        return encoded;
    }

    inline std::string encode(const char* cstr = "")
    {
        return (cstr == nullptr || *cstr == '\0')
            ? std::string("")
            : encode_base(cstr);
    }

    inline std::string encode(const std::string& str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str);
    }

    inline std::string encode_hex(const char* cstr = "")
    {
        return (cstr == nullptr || *cstr == '\0')
            ? std::string("")
            : encode_base(cstr, base32_hex_table);
    }

    inline std::string encode_hex(const std::string& str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str, base32_hex_table);
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(std::string(list.begin(), list.end()));
    }

    inline std::string encode_hex(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                std::string(list.begin(), list.end()), base32_hex_table);
    }

    template<size_t N>
    inline std::string encode(const std::array<uint8_t, N>& arr)
    {
        return (arr.empty())
            ? std::string("")
            : encode_base(std::string(arr.begin(), arr.end()));
    }

    template<size_t N>
    inline std::string encode_hex(const std::array<uint8_t, N>& arr)
    {
        return (arr.empty())
            ? std::string("")
            : encode_base(
                std::string(arr.begin(), arr.end()), base32_hex_table);
    }
}  // namespace BaseXX::_32_

namespace _16_
{
    template<
        typename T,
        typename = std::enable_if_t<
            std::is_same_v<T, const char*> ||
            std::is_same_v<T, std::string>>
    >
    inline std::string encode_base(
        const T& data, const uint8_t* encoding_table = base16_table)
    {
        std::string encoded{};
        uint8_t arr_2[2] = { 0, };

        size_t data_len = 0;
        if constexpr (std::is_same_v<T, const char*>)
        {
            data_len = strlen(data);
        }
        else
        {
            data_len = data.length();
        }

        for (size_t pos = 0; pos < data_len; pos++)
        {
            arr_2[0] = (data[pos] & 0b1111'0000) >> 4;
            arr_2[1] = data[pos] & 0b0000'1111;

            for (size_t i = 0; i < 2; i++)
            {
                encoded += base16_table[arr_2[i]];
            }
        }

        return encoded;
    }

    inline std::string encode(const char* cstr)
    {
        return (cstr == nullptr || *cstr == '\0')
            ? std::string("")
            : encode_base(cstr);
    }

    inline std::string encode(const std::string& str)
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str);
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(std::string(list.begin(), list.end()));
    }

    template<size_t N>
    inline std::string encode(const std::array<uint8_t, N>& arr)
    {
        return (arr.empty())
            ? std::string("")
            : encode_base(std::string(arr.begin(), arr.end()));
    }
}  // namespace BaseXX::_16_
}  // namespace BaseXX

namespace base64 = ::BaseXX::_64_;
namespace base32 = ::BaseXX::_32_;
namespace base16 = ::BaseXX::_16_;


#endif  // BASEXX_H

