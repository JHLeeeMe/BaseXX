/// ============================================================================
/// BaseXX.h
/// ----------------------------------------------------------------------------
/// Header-only library for various Base Encoding & Decoding algorithms.
/// includes Base64, Base64-URLsafe, Base32, Base32-Hex, and Base16.
/// Based on RFC 4648 (https://datatracker.ietf.org/doc/html/rfc4648)
///
/// ----------------------------------------------------------------------------
/// Code Structure
/// ----------------------------------------------------------------------------
/// namespace BaseXX
/// {
///     // Common functions, enums, and type definitions
///     namespace _64_
///     {
///         // Base64[-URLsafe] encoding & decoding implementations
///         // Helper functions
///     }
///     namespace _32_
///     {
///         // Base32[-Hex] encoding & decoding implementations
///         // Helper functions
///     }
///     namespace _16_
///     {
///         // Base16 encoding & decoding implementations
///         // Helper functions
///     }
/// }
/// using base64 = ::BaseXX::_64_;
/// using base32 = ::BaseXX::_32_;
/// using base16 = ::BaseXX::_16_;
///
/// ----------------------------------------------------------------------------
/// License: The Unlicense <http://unlicense.org/>
/// ============================================================================

#ifndef BASEXX_H
#define BASEXX_H


#include <cstdint>  // uint8_t
#include <stdexcept>  // std::runtime_error
#include <string>
#include <vector>
#if __cplusplus >= 201703L
    #include <string_view>
#endif  // __cplusplus >= 201703L

#define FALLTHROUGH do{} while (false)

namespace BaseXX
{
    /// ========================================================================
    /// Common Type Definitions & Enums & Functions
    /// ========================================================================

#if __cplusplus >= 201703L
    using StringType = std::string_view;
#else  // __cplusplus >= 201703L
    using StringType = const std::string&;
#endif  // __cplusplus >= 201703L

    enum class eResultCode
    {
        Success = 0,
        
        InvalidBase         = 10,
        InvalidLength       = InvalidBase + 1,  // 11
        InvalidCharacter    = InvalidBase + 2,  // 12
        InvalidEncodedType  = InvalidBase + 3,  // 13
        InvalidPaddingCount = InvalidBase + 4,  // 14
    };

    [[noreturn]]
    inline void throwRuntimeError(
        eResultCode code, StringType caller_info, StringType msg = "")
    {
        std::string error_message{ "Error occurred in " };
        error_message += caller_info;
        error_message += ":\n\t";

        if (!msg.empty())
        {
            error_message += msg;
        }
        else
        {
            switch (code)
            {
            case eResultCode::InvalidLength:
                error_message += "Invalid encoded text length.";
                break;
            case eResultCode::InvalidPaddingCount:
                error_message += "Invalid encoded padding count.";
                break;
            case eResultCode::InvalidCharacter:
                error_message += "Invalid encoded character.";
                break;
            case eResultCode::InvalidEncodedType:
                error_message += "Invalid encoded type.";
                break;
            default:
                error_message += "Invalid encoded text.";
                break;
            }
        }

        throw std::runtime_error(error_message);
    }

namespace _64_
{
    /// ========================================================================
    /// Base64[-URLsafe] Encoding/Decoding Implementation
    /// ========================================================================

    /// The Base 64 Alphabet Table
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-4
    ///
    static constexpr uint8_t encoding_table[64] = {
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
    static constexpr uint8_t urlsafe_encoding_table[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',  // 24 ~ 31
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',  // 32 ~ 39
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',  // 40 ~ 47
        'w', 'x', 'y', 'z', '0', '1', '2', '3',  // 48 ~ 55
        '4', '5', '6', '7', '8', '9', '-', '_',  // 56 ~ 63
    };

    inline eResultCode check_format(
        const char* encoded_text, const size_t text_len)
    {
        if (text_len % 4 != 0)
        {
            return eResultCode::InvalidLength;
        }

        size_t idx = text_len - 1;
        size_t padding_cnt = 0;
        while (true)
        {
            if (padding_cnt > 2)
            {
                return eResultCode::InvalidPaddingCount;
            }

            if (encoded_text[idx] != '=')
            {
                break;
            }

            idx--;
            padding_cnt++;
        }

        return eResultCode::Success;
    }

    inline const uint8_t decode_char(const char c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - 'A';
        }
        else if (c >= 'a' && c <= 'z')
        {
            return 26 + (c - 'a');
        }
        else if (c >= '0' && c <= '9')
        {
            return 52 + (c - '0');
        }
        else if (c == '+')
        {
            return 62;
        }
        else if (c == '/')
        {
            return 63;
        }

        throwRuntimeError(eResultCode::InvalidCharacter, __FUNCTION__);
    }

    inline const uint8_t urlsafe_decode_char(const char c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - 'A';
        }
        else if (c >= 'a' && c <= 'z')
        {
            return 26 + (c - 'a');
        }
        else if (c >= '0' && c <= '9')
        {
            return 52 + (c - '0');
        }
        else if (c == '-')
        {
            return 62;
        }
        else if (c == '_')
        {
            return 63;
        }

        throwRuntimeError(eResultCode::InvalidCharacter, __FUNCTION__);
    }
    
    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* table = encoding_table)
    {
        std::string encoded{};
        encoded.reserve(data_len * 4 / 3);

        uint8_t decoded_data_3[3] = {0,};
        uint8_t encoded_data_4[4] = {0,};

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            decoded_data_3[i++] = data[pos];
            if (i == 3)
            {
                encoded_data_4[0] = (decoded_data_3[0] & 0xFC) >> 2;

                encoded_data_4[1] = ((decoded_data_3[0] & 0x03) << 4) |
                                    ((decoded_data_3[1] & 0xF0) >> 4);

                encoded_data_4[2] = ((decoded_data_3[1] & 0x0F) << 2) |
                                    ((decoded_data_3[2] & 0xC0) >> 6);

                encoded_data_4[3] = decoded_data_3[2] & 0x3F;

                for (const auto& c : encoded_data_4)
                {
                    encoded.push_back(table[c]);
                }

                i = 0;
            }
        }

        if (i)  // i == 1 or i == 2
        {
            memset(decoded_data_3 + i, 0x00, 3 - i);

            switch (i)
            {
            case 2:
                encoded_data_4[2] = (decoded_data_3[1] & 0x0F) << 2;
                FALLTHROUGH;
            case 1:
                encoded_data_4[1] = ((decoded_data_3[0] & 0x03) << 4) |
                                    ((decoded_data_3[1] & 0xF0) >> 4);
                encoded_data_4[0] = (decoded_data_3[0] & 0xFC) >> 2;
                FALLTHROUGH;
            default:
                break;
            }

            for (size_t j = 0; j < i + 1; j++)
            {
                encoded.push_back(table[encoded_data_4[j]]);
            }

            size_t padding_cnt = 4 - (encoded.length() % 4);
            while (padding_cnt-- != 0)
            {
                encoded.push_back('=');
            }
        }

        return encoded;
    }

    inline std::string decode_base(const char* data,
        const size_t data_len,
        const uint8_t (*decode_char_func)(const char) = &decode_char)
    {
        eResultCode code = check_format(data, data_len);
        if (code != eResultCode::Success)
        {
             throwRuntimeError(code, __FUNCTION__);
        }

        std::string decoded{};
        decoded.reserve(data_len * 3 / 4);

        uint8_t encoded_data_4[4] = { 0, };
        uint8_t decoded_data_3[3] = { 0, };

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            if (data[pos] == '=')
            {
                break;
            }

            encoded_data_4[i++] = decode_char_func(data[pos]);

            if (i == 4)
            {
                decoded_data_3[0] = ((encoded_data_4[0] & 0x3F) << 2) |
                                    ((encoded_data_4[1] & 0x30) >> 4);

                decoded_data_3[1] = ((encoded_data_4[1] & 0x0F) << 4) |
                                    ((encoded_data_4[2] & 0x3C) >> 2);

                decoded_data_3[2] = ((encoded_data_4[2] & 0x03) << 6) |
                                    encoded_data_4[3];

                for (const auto& c : decoded_data_3)
                {
                    decoded.push_back(c);
                }

                i = 0;
            }
        }

        if (i)  // i == (2 or 3)
        {
            memset(encoded_data_4 + i, 0x00, 4 - i);

            switch (i)
            {
            case 3:
                decoded_data_3[1] = ((encoded_data_4[1] & 0x0F) << 4) |
                                    ((encoded_data_4[2] & 0x3C) >> 2);
                FALLTHROUGH;
            case 2:
                decoded_data_3[0] = ((encoded_data_4[0] & 0x3F) << 2) |
                                    ((encoded_data_4[1] & 0x30) >> 4);
                FALLTHROUGH;
            default:
                break;
            }

            for (int j = 0; j < (i - 1); ++j)
            {
                decoded.push_back(decoded_data_3[j]);
            }
        }

        return decoded;
    }


    /// ========================================================================
    /// Helper Functions
    /// ========================================================================

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
            : encode_base(str.data(), str.size(), urlsafe_encoding_table);
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
                list.size(), urlsafe_encoding_table);
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
                vec.size(), urlsafe_encoding_table);
    }

    inline std::string decode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size());
    }

    inline std::string decode_urlsafe(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size(), &urlsafe_decode_char);
    }

    inline std::string decode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string decode_urlsafe(
        const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), &urlsafe_decode_char);
    }

    inline std::string decode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string decode_urlsafe(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), &urlsafe_decode_char);
    }
}  // namespace BaseXX::_64_

namespace _32_
{
    /// ========================================================================
    /// Base32[-Hex] Encoding/Decoding Implementation
    /// ========================================================================

    /// The Base 32 Alphabet Table
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-6
    ///
    static constexpr uint8_t encoding_table[32] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',  // 0 ~ 7
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',  // 8 ~ 15
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',  // 16 ~ 23
        'Y', 'Z', '2', '3', '4', '5', '6', '7',  // 24 ~ 31
    };

    /// The Base 32 Alphabet Table (Extended Hex)
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-7
    ///
    static constexpr uint8_t hex_encoding_table[32] = {
        '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
        'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',  // 16 ~ 23
        'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',  // 24 ~ 31
    };

    inline eResultCode check_format(
        const char* encoded_text, const size_t text_len)
    {
        if (text_len % 8 != 0)
        {
            return eResultCode::InvalidLength;
        }

        size_t idx = text_len - 1;
        size_t padding_cnt = 0;
        while (true)
        {
            if (padding_cnt > 6)
            {
                return eResultCode::InvalidPaddingCount;
            }

            if (encoded_text[idx] != '=')
            {
                break;
            }

            idx--;
            padding_cnt++;
        }

        return eResultCode::Success;
    }

    inline const uint8_t decode_char(const char c)
    {
        if (c >= 'A' && c <= 'Z')
        {
            return c - 'A';
        }
        else if (c >= '2' && c <= '7')
        {
            return 26 + (c - '2');
        }

        throwRuntimeError(eResultCode::InvalidCharacter, __FUNCTION__);
    }

    inline const uint8_t hex_decode_char(const char c)
    {
        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'V')
        {
            return 10 + (c - 'A');
        }

        throwRuntimeError(eResultCode::InvalidCharacter, __FUNCTION__);
    }

    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* table = encoding_table)
    {
        std::string encoded{};
        encoded.reserve(data_len * 8 / 5);

        uint8_t decoded_data_5[5] = { 0, };
        uint8_t encoded_data_8[8] = { 0, };

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            decoded_data_5[i++] = data[pos];
            if (i == 5)
            {
                encoded_data_8[0] = (decoded_data_5[0] & 0xF8) >> 3;

                encoded_data_8[1] = ((decoded_data_5[0] & 0x07) << 2) |
                                    ((decoded_data_5[1] & 0xC0) >> 6);

                encoded_data_8[2] = (decoded_data_5[1] & 0x3E) >> 1;

                encoded_data_8[3] = ((decoded_data_5[1] & 0x01) << 4) |
                                    ((decoded_data_5[2] & 0xF0) >> 4);

                encoded_data_8[4] = ((decoded_data_5[2] & 0x0F) << 1) |
                                    ((decoded_data_5[3] & 0x80) >> 7);

                encoded_data_8[5] = (decoded_data_5[3] & 0x7C) >> 2;

                encoded_data_8[6] = ((decoded_data_5[3] & 0x03) << 3) |
                                    ((decoded_data_5[4] & 0xE0) >> 5);

                encoded_data_8[7] = decoded_data_5[4] & 0x1F;

                for (const auto& c : encoded_data_8)
                {
                    encoded.push_back(table[c]);
                }
                
                i = 0;
            }
        }

        if (i)  // i == (1 ~ 4)
        {
            memset(decoded_data_5 + i, 0x00, 5 - i);

            size_t remaining_bytes = 0;
            switch (i)
            {
            case 4:
                encoded_data_8[6] = (decoded_data_5[3] & 0x03) << 3;
                encoded_data_8[5] = (decoded_data_5[3] & 0x7C) >> 2;
                remaining_bytes += 2;
                FALLTHROUGH;
            case 3:
                encoded_data_8[4] = ((decoded_data_5[2] & 0x0F) << 1) |
                                    ((decoded_data_5[3] & 0x80) >> 7);
                remaining_bytes++;
                FALLTHROUGH;
            case 2:
                encoded_data_8[3] = ((decoded_data_5[1] & 0x01) << 4) |
                                    ((decoded_data_5[2] & 0xF0) >> 4);
                encoded_data_8[2] = (decoded_data_5[1] & 0x3E) >> 1;
                remaining_bytes += 2;
                FALLTHROUGH;
            case 1:
                encoded_data_8[1] = ((decoded_data_5[0] & 0x07) << 2) |
                                    ((decoded_data_5[1] & 0xC0) >> 6);
                encoded_data_8[0] = (decoded_data_5[0] & 0xF8) >> 3;
                remaining_bytes += 2;
                FALLTHROUGH;
            default:
                break;
            }

            for (size_t idx = 0; idx < remaining_bytes; idx++)
            {
                encoded.push_back(table[encoded_data_8[idx]]);
            }

            size_t padding_cnt = 8 - (encoded.length() % 8);
            while (padding_cnt-- != 0)
            {
                encoded.push_back('=');
            }
        }

        return encoded;
    }

    inline std::string decode_base(const char* data,
        const size_t data_len,
        const uint8_t (*decode_char_func)(const char) = &decode_char)
    {
        eResultCode code = check_format(data, data_len);
        if (code != eResultCode::Success)
        {
            throwRuntimeError(code, __FUNCTION__);
        }

        std::string decoded{};
        decoded.reserve(data_len * 5 / 8);

        uint8_t encoded_data_8[8] = { 0, };
        uint8_t decoded_data_5[5] = { 0, };

        size_t i = 0;
        for (size_t pos = 0; pos < data_len; pos++)
        {
            if (data[pos] == '=')
            {
                break;
            }

            encoded_data_8[i++] = decode_char_func(data[pos]);

            if (i == 8)
            {
                decoded_data_5[0] = ((encoded_data_8[0] & 0x1F) << 3) |
                                    ((encoded_data_8[1] & 0x1C) >> 2);

                decoded_data_5[1] = ((encoded_data_8[1] & 0x03) << 6) |
                                    ((encoded_data_8[2] & 0x1F) << 1) |
                                    ((encoded_data_8[3] & 0x10) >> 4);

                decoded_data_5[2] = ((encoded_data_8[3] & 0x0F) << 4) |
                                    ((encoded_data_8[4] & 0x1E) >> 1);

                decoded_data_5[3] = ((encoded_data_8[4] & 0x01) << 7) |
                                    ((encoded_data_8[5] & 0x1F) << 2) |
                                    ((encoded_data_8[6] & 0x18) >> 3);

                decoded_data_5[4] = ((encoded_data_8[6] & 0x07) << 5) |
                                    encoded_data_8[7];

                for(const auto& c : decoded_data_5)
                {
                    decoded.push_back(c);
                }

                i = 0;
            }
        }

        if (i)  // i == (2 || 4 || 5 || 7)
        {
            size_t remaining_bytes = 0;
            switch (i)
            {
            case 7:
                decoded_data_5[3] = ((encoded_data_8[4] & 0x01) << 7) |
                                    ((encoded_data_8[5] & 0x1F) << 2) |
                                    ((encoded_data_8[6] & 0x18) >> 3);
                remaining_bytes++;
                FALLTHROUGH;
            case 5:
                decoded_data_5[2] = ((encoded_data_8[3] & 0x0F) << 4) |
                                    ((encoded_data_8[4] & 0x1E) >> 1);
                remaining_bytes++;
                FALLTHROUGH;
            case 4:
                decoded_data_5[1] = ((encoded_data_8[1] & 0x03) << 6) |
                                    ((encoded_data_8[2] & 0x1F) << 1) |
                                    ((encoded_data_8[3] & 0x10) >> 4);
                remaining_bytes++;
                FALLTHROUGH;
            case 2:
                decoded_data_5[0] = ((encoded_data_8[0] & 0x1F) << 3) |
                                    ((encoded_data_8[1] & 0x1C) >> 2);
                remaining_bytes++;
                FALLTHROUGH;
            default:
                break;
            }

            for (size_t idx = 0; idx < remaining_bytes; idx++)
            {
                decoded.push_back(decoded_data_5[idx]);
            }
        }

        return decoded;
    }


    /// ========================================================================
    /// Helper Functions
    /// ========================================================================

    inline std::string encode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.length());
    }

    inline std::string encode_hex(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.length(), hex_encoding_table);
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string encode_hex(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), hex_encoding_table);
    }

    inline std::string encode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string encode_hex(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), hex_encoding_table);
    }

    inline std::string decode(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size());
    }

    inline std::string decode_hex(StringType str = "")
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size(), &hex_decode_char);
    }

    inline std::string decode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string decode_hex(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(list.begin()),
                list.size(), &hex_decode_char);
    }

    inline std::string decode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string decode_hex(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(reinterpret_cast<const char*>(vec.data()),
                vec.size(), &hex_decode_char);
    }
}  // namespace BaseXX::_32_

namespace _16_
{
    /// ========================================================================
    /// Base16 Encoding/Decoding Implementation
    /// ========================================================================

    /// The Base 16 Alphabet Table
    /// https://datatracker.ietf.org/doc/html/rfc4648#section-8
    ///
    static constexpr uint8_t encoding_table[16] = {
        '0', '1', '2', '3', '4', '5', '6', '7',  // 0 ~ 7
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',  // 8 ~ 15
    };

    inline const uint8_t decode_char(const char c)
    {
        if (c >= '0' && c <= '9')
        {
            return c - '0';
        }
        else if (c >= 'A' && c <= 'F')
        {
            return 10 + (c - 'A');
        }

        throwRuntimeError(eResultCode::InvalidCharacter, __FUNCTION__);
    }

    inline std::string encode_base(const char* data,
        const size_t data_len, const uint8_t* table = encoding_table)
    {
        std::string encoded{};
        encoded.reserve(data_len * 2);

        uint8_t encoded_data_2[2] = { 0, };

        for (size_t pos = 0; pos < data_len; pos++)
        {
            encoded_data_2[0] = (data[pos] & 0xF0) >> 4;
            encoded_data_2[1] = data[pos] & 0x0F;

            for (const auto& c : encoded_data_2)
            {
                encoded.push_back(table[c]);
            }
        }

        return encoded;
    }

    inline std::string decode_base(const char* data, const size_t data_len)
    {
        if (data_len % 2 != 0)
        {
            throwRuntimeError(eResultCode::InvalidLength, __FUNCTION__);
        }

        std::string decoded{};
        decoded.reserve(data_len / 2);

        for (size_t i = 0; i < data_len; i += 2)
        {
            decoded += ((decode_char(data[i]) & 0x0F) << 4) |
                       decode_char(data[i + 1]);
        }

        return decoded;
    }


    /// ========================================================================
    /// Helper Functions
    /// ========================================================================

    inline std::string encode(StringType str)
    {
        return (str.empty())
            ? std::string("")
            : encode_base(str.data(), str.size());
    }

    inline std::string encode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string encode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : encode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }

    inline std::string decode(StringType str)
    {
        return (str.empty())
            ? std::string("")
            : decode_base(str.data(), str.size());
    }

    inline std::string decode(const std::initializer_list<uint8_t>& list)
    {
        return (list.size() == 0)
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(list.begin()), list.size());
    }

    inline std::string decode(const std::vector<uint8_t>& vec)
    {
        return (vec.empty())
            ? std::string("")
            : decode_base(
                reinterpret_cast<const char*>(vec.data()), vec.size());
    }
}  // namespace BaseXX::_16_
}  // namespace BaseXX

namespace base64 = ::BaseXX::_64_;
namespace base32 = ::BaseXX::_32_;
namespace base16 = ::BaseXX::_16_;


#endif  // BASEXX_H

