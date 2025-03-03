#ifndef EEPROM_HPP
#define EEPROM_HPP

#include "i2c_device.hpp"
#include "spi_device.hpp"
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace EEPROM {

    struct EEPROM {
    public:
        template <typename T>
        void write_memory(std::uint8_t const reg_address, T const& value) const noexcept;

        template <typename T>
        T read_memory(std::uint8_t const reg_address) const noexcept;

        template <typename T>
        void write_memory(std::string_view const& name, T const& value) noexcept;

        template <typename T>
        T read_memory(std::string_view const& name) noexcept;

        std::variant<Utility::I2CDevice, Utility::SPIDevice> serial_device{};

        std::uint8_t memory_size{};
        std::uint8_t memory_pointer{0U};

        struct Memory {
            std::uint8_t address{};
            std::size_t bytes{};
        };

        std::unordered_map<std::string_view, Memory> memory_map{};

    private:
        std::uint8_t read_byte(std::uint8_t const reg_address) const noexcept;

        template <std::size_t SIZE>
        std::array<std::uint8_t, SIZE> read_bytes(std::uint8_t const reg_address) const noexcept;

        void write_byte(std::uint8_t const reg_address, std::uint8_t const byte) const noexcept;

        template <std::size_t SIZE>
        void write_bytes(std::uint8_t const reg_address, std::array<std::uint8_t, SIZE> const& bytes) const noexcept;
    };

    template <typename T>
    void EEPROM::write_memory(std::uint8_t const reg_address, T const& value) const noexcept
    {
        this->write_bytes(reg_address, Utility::value_to_bytes(value));
    }

    template <typename T>
    T EEPROM::read_memory(std::uint8_t const reg_address) const noexcept
    {
        return Utility::bytes_to_value<T>(this->read_bytes<sizeof(T)>(reg_address));
    }

    template <typename T>
    void EEPROM::write_memory(std::string_view const& name, T const& value) noexcept
    {
        if (this->memory_pointer + sizeof(T) <= this->memory_size) {
            this->write_bytes(this->memory_pointer, Utility::value_to_bytes(value));
            this->memory_map.try_emplace(this->memory_map.end(), name, this->memory_pointer, sizeof(T));
            this->memory_pointer += sizeof(T);
        }
    }

    template <typename T>
    T EEPROM::read_memory(std::string_view const& name) noexcept
    {
        if (this->memory_pointer + sizeof(T) <= this->memory_size) {
            auto memory = this->memory_map.at(name);
            if (sizeof(T) == memory.bytes) {
                return this->read_bytes<sizeof(T)>(memory.address);
            }
        }
        return T{};
    }

    template <std::size_t SIZE>
    inline std::array<std::uint8_t, SIZE> EEPROM::read_bytes(std::uint8_t const reg_address) const noexcept
    {
        return std::visit([reg_address](auto const& device) { device.template read_bytes<SIZE>(reg_address); },
                          this->serial_device);
    }

    template <std::size_t SIZE>
    inline void EEPROM::write_bytes(std::uint8_t const reg_address,
                                    std::array<std::uint8_t, SIZE> const& bytes) const noexcept
    {
        std::visit([reg_address, &bytes](auto const& device) { device.template write_bytes(reg_address, bytes); },
                   this->serial_device);
    }

} // namespace EEPROM

#endif // EEPROM_HPP