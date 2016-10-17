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
 * @file CMacIonize.cpp
 *
 * @brief Entrance point of the CMacIonize program
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#include "Box.hpp"
#include "CommandLineOption.hpp"
#include "CommandLineParser.hpp"
#include "CoordinateVector.hpp"
#include "DensityFunctionFactory.hpp"
#include "DensityGrid.hpp"
#include "LineCoolingData.hpp"
#include "ParameterFile.hpp"
#include "PhotonSource.hpp"
#include "PlanckPhotonSourceSpectrum.hpp"
#include "SingleStarPhotonSourceDistribution.hpp"
#include "VernerCrossSections.hpp"
#include "VernerRecombinationRates.hpp"
#include <iostream>
#include <string>
using namespace std;

/**
 * @brief Entrance point of the program
 *
 * @param argc Number of command line arguments passed on to the program.
 * @param argv Array containing the command line arguments.
 */
int main(int argc, char **argv) {
  // first thing we should do: parse the command line arguments
  // we need to define a CommandLineParser object that does this and acts as a
  // dictionary that can be queried
  CommandLineParser parser("CMacIonize");
  parser.add_required_option< string >(
      "params", 'p',
      "Name of the parameter file containing the simulation parameters.");
  parser.parse_arguments(argc, argv);

  // second: initialize the parameters that are read in from static files
  // these files should be configured by CMake and put in a location that is
  // stored in a CMake configured header
  LineCoolingData line_cooling_data;

  // third: read in the parameters of the run from a parameter file. This file
  // should be read by a ParameterFileParser object that acts as a dictionary
  ParameterFile params(parser.get_value< string >("params"));

  // fourth: construct the density grid. This should be stored in a separate
  // DensityGrid object with geometrical and physical properties
  Box box(CoordinateVector<>(), CoordinateVector<>(1.));
  CoordinateVector< int > ncell(64);
  DensityFunction *density_function =
      DensityFunctionFactory::generate("GadgetSnapshot", params);
  VernerCrossSections cross_sections;
  VernerRecombinationRates recombination_rates;
  DensityGrid grid(params, box, ncell, *density_function, recombination_rates);

  // fifth: construct the stellar sources. These should be stored in a
  // separate
  // StellarSources object with geometrical and physical properties.
  SingleStarPhotonSourceDistribution sourcedistribution(
      CoordinateVector<>(0.5));
  PlanckPhotonSourceSpectrum spectrum;
  PhotonSource source(sourcedistribution, spectrum, cross_sections);

  // finally: the actual program loop whereby the density grid is ray traced
  // using photon packets generated by the stellar sources
  // it would be nice to have some interactor classes that can do this

  // idea: the photons themselves should be a class, storing a current
  // position/cell, current direction, and the photon package energy
  // photons are then emitted by the StellarSources object
  // there is another object, a PhotonInteractor, that selects a random
  // optical depth for a photon and then moves it to a new position until
  // it leaves the system

  delete density_function;

  return 0;
}
