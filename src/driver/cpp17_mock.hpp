#ifndef IOP_LIB_SENSORS_DALLAS_TEMP_NOOP_HPP
#define IOP_LIB_SENSORS_DALLAS_TEMP_NOOP_HPP

#include <dallas_temperature.hpp>

auto randomTemperature() -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(250, 350);
    return static_cast<float>(dist(rng)) / 10.;
}

auto randomTemperatureVariation(float current) -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(-3, 3);
    return std::max(std::min(current + static_cast<float>(dist(rng)) / 10., 35.), 25.);
}

namespace dallas {
TemperatureCollection::TemperatureCollection(iop_hal::PinRaw pin) noexcept: sensor(new float(randomTemperature())) { (void) pin; }

auto TemperatureCollection::begin() noexcept -> void {}
auto TemperatureCollection::measure() noexcept -> float {
    auto *ptr = reinterpret_cast<float*>(this->sensor);
    *ptr = randomTemperatureVariation(*ptr);
    return *ptr;
}

auto TemperatureCollection::operator=(TemperatureCollection && other) noexcept -> TemperatureCollection & {
    delete reinterpret_cast<float*>(this->sensor);
    this->sensor = other.sensor;
    other.sensor = nullptr;
    return *this;
}
TemperatureCollection::~TemperatureCollection() noexcept {
    delete reinterpret_cast<float*>(this->sensor);
}
}

#endif