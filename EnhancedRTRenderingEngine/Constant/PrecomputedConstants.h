#pragma once

#include <cmath>
#include "SpiralLibrary/Common/Math.hpp"
#include "SpiralLibrary/Container/Array.hpp"

namespace PrecomputedConstants {
    namespace detail {
        template <int Base, int Level>
        constexpr libspiral::Array<std::size_t, Level + 1> CaluclatePows() {
            libspiral::Array<std::size_t, Level + 1> arr = {};
            for (int i = 0; i < Level + 1; i++) {
                arr[i] = libspiral::pow(Base, i);
            }

            return arr;
        }
    }

    template <int Base, int Level>
    struct PowNumbers {
        static constexpr std::size_t Get(int level) {
            return _pows[level];
        }

        static constexpr libspiral::Array<std::size_t, Level + 1> _pows = detail::CaluclatePows<Base, Level>();
    };
}
