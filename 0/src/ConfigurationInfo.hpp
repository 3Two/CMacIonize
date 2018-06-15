/*******************************************************************************
 * This file is part of CMacIonize
 * Copyright (C) 2017 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
 *
 * CMacIonize is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CMacIonize is distributed in the hope that it will be useful,
 * but WITOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with CMacIonize. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

/**
 * @file ConfigurationInfo.hpp
 *
 * @brief Class containing info about the configuration options that were
 * enabled when the code was compiled.
 *
 * Do not edit this file directly! Instead edit ConfigurationInfo.hpp.in.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef CONFIGURATIONINFO_HPP
#define CONFIGURATIONINFO_HPP

// disable clang-format for these lines to keep the final @ attached to the
// variable name
// clang-format off
#define CONFIGURATION_OPTIONS_NUMBER (0+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1)
// clang-format on

#include <string>

/**
 * @brief Class containing info about the configuration options that were
 * enabled when the code was compiled.
 */
class ConfigurationInfo {
private:
  /*! @brief Configuration option names. */
  static const std::string _configuration_option_keys[];

  /*! @brief Configuration option values. */
  static const std::string _configuration_option_values[];

public:
  /**
   * @brief Iterator to iterate over the configuration options.
   */
  class iterator {
  private:
    /*! @brief Option the iterator points to. */
    uint_fast8_t _index;

  public:
    /**
     * @brief Constructor.
     *
     * @param index Option the iterator points to.
     */
    inline iterator(uint_fast8_t index) : _index(index) {}

    /**
     * @brief Increment operator.
     *
     * @return Reference to the incremented operator.
     */
    inline iterator &operator++() {
      ++_index;
      return *this;
    }

    /**
     * @brief Comparison operator.
     *
     * @param it iterator to compare with.
     * @return True if both operators point to the same field.
     */
    inline bool operator==(iterator it) const { return _index == it._index; }

    /**
     * @brief Comparison operator.
     *
     * @param it iterator to compare with.
     * @return True if both operators point to a different field.
     */
    inline bool operator!=(iterator it) const { return !(*this == it); }

    /**
     * @brief Get the key this iterator points to.
     *
     * @return Key.
     */
    inline std::string get_key() const {
      return std::string(ConfigurationInfo::_configuration_option_keys[_index]);
    }

    /**
     * @brief Get the value this iterator points to.
     *
     * @return Value.
     */
    inline std::string get_value() const {
      return std::string(
          ConfigurationInfo::_configuration_option_values[_index]);
    }
  };

  /**
   * @brief iterator to the first option that should be written out.
   *
   * @return iterator to first option.
   */
  inline static iterator begin() { return iterator(0); }

  /**
   * @brief iterator to the beyond last option that should be written out.
   *
   * @return iterator to the beyond last option.
   */
  inline static iterator end() {
    return iterator(CONFIGURATION_OPTIONS_NUMBER);
  }
};

#endif // CONFIGURATIONINFO_HPP