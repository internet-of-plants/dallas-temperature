#ifndef IOP_LIB_SENSORS_DALLAS_TEMP_CPP17_MOCK_HPP
#define IOP_LIB_SENSORS_DALLAS_TEMP_CPP17_MOCK_HPP

#include <dallas_temperature.hpp>

#include <random>

namespace dallas {
auto randomTemperature() -> float {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(150, 250);
    return static_cast<float>(dist(rng)) / 10.;
}

auto randomTemperatureVariation(float current) -> float {
    if (current <= 16.) return current + 3.;
    if (current >= 24.) return current - 3.;

    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, 60);
    return std::max(std::min(current + static_cast<float>(dist(rng)) / 10. - 3., 25.), 15.);
}

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