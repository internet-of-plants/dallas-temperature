#ifndef IOP_LIB_SENSORS_DALLAS_TEMP_ARDUINO_HPP
#define IOP_LIB_SENSORS_DALLAS_TEMP_ARDUINO_HPP

#include <dallas_temperature.hpp>
#include <iop-hal/panic.hpp>

#include <OneWire.h>
#include <DallasTemperature.h>

namespace dallas {
#define SENSOR(self) static_cast<OneWireBus*>((self).sensor)

/// Be ware, self-referential struct, it should _never_ move addresses.
class OneWireBus {
    OneWire oneWire;
    DallasTemperature temp;

public:
    OneWireBus(iop_hal::PinRaw pin) noexcept: oneWire(pin), temp(&oneWire) {}

    OneWireBus(const OneWireBus &other) noexcept = delete;
    OneWireBus(OneWireBus &&other) noexcept = delete;
    auto operator=(const OneWireBus &other) noexcept -> OneWireBus & = delete;
    auto operator=(OneWireBus &&other) noexcept -> OneWireBus & = delete;

    friend TemperatureCollection;
};

TemperatureCollection::TemperatureCollection(iop_hal::PinRaw pin) noexcept: sensor(new (std::nothrow) OneWireBus(pin)) {
    iop_assert(this->sensor, IOP_STR("Unable to allocate OneWireBus"));
}

auto TemperatureCollection::begin() noexcept -> void {
    IOP_TRACE();
    iop_assert(this->sensor, IOP_STR("Sensor is nullptr"));
    SENSOR(*this)->temp.begin();
}
auto TemperatureCollection::measure() noexcept -> float {
    IOP_TRACE();
    iop_assert(this->sensor, IOP_STR("Sensor is nullptr"));
    // Blocks until reading is done
    SENSOR(*this)->temp.requestTemperatures();
    // Accessing by index is bad. It's slow, we should store the sensor's address
    // and use it
    return SENSOR(*this)->temp.getTempCByIndex(0);
}

auto TemperatureCollection::operator=(TemperatureCollection && other) noexcept -> TemperatureCollection & {
    delete SENSOR(*this);
    this->sensor = other.sensor;
    other.sensor = nullptr;
    return *this;
}
TemperatureCollection::~TemperatureCollection() noexcept {
    delete SENSOR(*this);
}
}

#endif
