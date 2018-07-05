/*******************************************************************************
 * This file is part of CMacIonize
 * Copyright (C) 2016 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
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
 * @file HomogeneousDensityFunction.hpp
 *
 * @brief Homogeneous DensityFunction implementation.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef HOMOGENEOUSDENSITYFUNCTION_HPP
#define HOMOGENEOUSDENSITYFUNCTION_HPP

#include "DensityFunction.hpp"
#include "Log.hpp"
#include "ParameterFile.hpp"

/**
 * @brief DensityFunction that returns a constant value for all coordinates,
 * corresponding to a homogeneous density field.
 */
class HomogeneousDensityFunction : public DensityFunction {
private:
  /*! @brief Single density value for the entire box (in m^-3). */
  const double _density;

  /*! @brief Single temperature value for the entire box (in K). */
  const double _temperature;

  /*! @brief Dust density */
  double _dustdensity;

  /*! @brief Albedo for dust scattering */
  double _albedo;

  /*! @brief h-g parameter for dust scattering*/
  double _gval;

  /*! @brief opacity for dust scattering*/
  double _opacity;

  /*! @brief Initial hydrogen neutral fraction for the entire box. */
  const double _neutral_fraction_H;

public:
  /**
   * @brief Constructor.
   *
   * @param density Single density value for the entire box (in m^-3).
   * @param temperature Single temperature value for the entire box (in K).
   * @param neutral_fraction_H Single hydrogen neutral fraction value for the
   * entire box.
   * @param log Log to write logging information to.
   */
	HomogeneousDensityFunction(double density = 1., double temperature = 8000., double dustdensity = 1e-16,
		                     double albedo = 0., double gval = 0.9, double opacity = 3.24,
                             double neutral_fraction_H = 1.e-6,
                             Log *log = nullptr)
      : _density(density), _temperature(temperature), _dustdensity(dustdensity), _albedo(albedo), _gval(gval), _opacity(opacity),
        _neutral_fraction_H(neutral_fraction_H) {

    if (log) {
      log->write_status(
          "Created HomogeneousDensityFunction with constant density ", _density,
          " m^-3 and constant temperature ", _temperature, " K.","and dust density", _dustdensity, "unit");
    }
  }

  /**
   * @brief ParameterFile constructor.
   *
   * Parameters are:
   *  - density: Constant number density value (default: 100. cm^-3)
   *  - temperature: Constant initial temperature value (default: 8000. K)
   *  - neutral fraction H: Contant initial neutral fraction value
   *    (default: 1.e-6)
   *
   * @param params ParameterFile to read from.
   * @param log Log to write logging information to.
   */
  HomogeneousDensityFunction(ParameterFile &params, Log *log = nullptr)
      : HomogeneousDensityFunction(
            params.get_physical_value< QUANTITY_NUMBER_DENSITY >(
                "DensityFunction:density", "100. cm^-3"),
            params.get_physical_value< QUANTITY_TEMPERATURE >(
                "DensityFunction:temperature", "8000. K"),
		    params.get_value< double >(
			  "DensityFunction:dustdensity", 1e-16),
		    params.get_value< double >(
			  "DensityFunction:albedo", 0.),
		    params.get_value< double >(
			  "DensityFunction:gval", 0.9),
		  params.get_value< double >(
			  "DensityFunction:opacity", 3.24),
            params.get_value< double >("DensityFunction:neutral fraction H",
                                       1.e-6),
            log) {}

  /**
   * @brief Function that gives the density for a given cell.
   *
   * @param cell Geometrical information about the cell.
   * @return Initial physical field values for that cell.
   */
  virtual DensityValues operator()(const Cell &cell) const {
    DensityValues values;
    values.set_number_density(_density);
    values.set_temperature(_temperature);
	values.set_dustdensity(_dustdensity);
	values.set_albedo(_albedo);
	values.set_gval(_gval);
	values.set_opacity(_opacity);
    values.set_ionic_fraction(ION_H_n, _neutral_fraction_H);
    values.set_ionic_fraction(ION_He_n, 1.e-6);
    return values;
  }
};

#endif // HOMOGENEOUSDENSITYFUNCTION_HPP
