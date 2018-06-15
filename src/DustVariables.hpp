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
 * @file DustVariables.hpp
 *
 * @brief Variables used in the Dust calculation.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef DUSTVARIABLES_HPP
#define DUSTVARIABLES_HPP

#include "Configuration.hpp"
#include "ElementNames.hpp"
#include "CoordinateVector.hpp"

#ifdef USE_LOCKFREE
#include "Atomic.hpp"
#endif



/**
 * @brief Variables used in the dust calculation.
 */
class DustVariables {
private:
	double _DustDensity;
	CoordinateVector<double> _Force;

public:
  /**
   * @brief (Empty) constructor.
   */
  inline DustVariables()
	  : _DustDensity(0.), _Force(0.,0.,0.)  {
  }

  /**
   * @brief Get the dust density.
   *
   * @return Dust density .
   */
  inline double get_dust_density() const { return _DustDensity; }

  /**
   * @brief Set the number density.
   *
   * @param number_density New number density (in m^-3).
   */
  inline void set_dust_density(double _dustdensity) {
    _DustDensity = _dustdensity;
  }

  /**
   * @brief Get the force.
   *
   * @return force.
   */
  inline CoordinateVector<double> get_force() const { return _Force; }

  inline void set_force(CoordinateVector<double> _force) {
	  _Force = _force;
  }

  /**
   * @brief Add the given increment to the mean intensity integral for the ion
   * with the given name.
   *
   * @param ion IonName.
   * @param increment Increment (without normalization factor, in m^3).
   */
  inline void increase_force(CoordinateVector<double> increment) {
#ifdef USE_LOCKFREE
	  Atomic::add(_Force[0], increment.x()); 
	  Atomic::add(_Force[1], increment.y());
	  Atomic::add(_Force[2], increment.z());
#else
    _Force[0] += increment.x();
	_Force[1] += increment.y();
	_Force[2] += increment.z();
#endif
  }


};

#endif // DUSTVARIABLES_HPP
