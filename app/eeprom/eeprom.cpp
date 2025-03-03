#include "eeprom.hpp"

namespace EEPROM {

    std::uint8_t EEPROM::read_byte(std::uint8_t const reg_address) const noexcept
    {
        return std::visit([reg_address](auto const& device) { return device.template read_byte(reg_address); },
                          this->serial_device);
    }

    void EEPROM::write_byte(std::uint8_t const reg_address, std::uint8_t const byte) const noexcept
    {
        std::visit([reg_address, byte](auto const& device) { device.template write_byte(reg_address, byte); },
                   this->serial_device);
    }

}; // namespace EEPROM