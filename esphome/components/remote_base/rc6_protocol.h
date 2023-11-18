#pragma once

#include "remote_base.h"

namespace esphome {
namespace remote_base {

struct RC6Data {
  uint8_t mode : 3;
  uint8_t toggle : 1;
  uint8_t address;
  uint8_t command;
  uint8_t oem1;
  uint8_t oem2;
  bool operator==(const RC6Data &rhs) const {
    return mode == rhs.mode &&
      (mode != 6 || (oem1 == rhs.oem1 && oem2 == rhs.oem2)) &&
      address == rhs.address && command == rhs.command;
  }
};

class RC6Protocol : public RemoteProtocol<RC6Data> {
 public:
  void encode(RemoteTransmitData *dst, const RC6Data &data) override;
  optional<RC6Data> decode(RemoteReceiveData src) override;
  void dump(const RC6Data &data) override;
};

DECLARE_REMOTE_PROTOCOL(RC6)

template<typename... Ts> class RC6Action : public RemoteTransmitterActionBase<Ts...> {
 public:
  TEMPLATABLE_VALUE(uint8_t, mode)
  TEMPLATABLE_VALUE(uint8_t, oem1)
  TEMPLATABLE_VALUE(uint8_t, oem2)
  TEMPLATABLE_VALUE(uint8_t, address)
  TEMPLATABLE_VALUE(uint8_t, command)

  void encode(RemoteTransmitData *dst, Ts... x) {
    RC6Data data{};
    data.mode = this->mode_.value(x...);
    data.toggle = this->toggle_;
    data.oem1 = this->oem1_.value(x...);
    data.oem2 = this->oem2_.value(x...);
    data.address = this->address_.value(x...);
    data.command = this->command_.value(x...);
    RC6Protocol().encode(dst, data);
    this->toggle_ = !this->toggle_;
  }

 protected:
  uint8_t toggle_{0};
};

}  // namespace remote_base
}  // namespace esphome
