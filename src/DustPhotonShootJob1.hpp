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
 * @file DustPhotonShootJob.hpp
 *
 * @brief Job implementation that shoots photons through a dusty DensityGrid.
 *
 * @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
 */
#ifndef DUSTPHOTONSHOOTJOB1_HPP
#define DUSTPHOTONSHOOTJOB1_HPP

#include "CCDImage.hpp"
#include "DensityGrid.hpp"
#include "DustScattering.hpp"
#include "Photon.hpp"
#include "PhotonSource.hpp"
#include "RandomGenerator.hpp"

/**
 * @brief Job implementation that shoots photons through a dusty DensityGrid.
 */
class DustPhotonShootJob1 {
private:
  /*! @brief PhotonSource that emits photons. */
  const PhotonSource &_photon_source;

  /*! @brief DustScattering object that scatters photons off dust. */
  const DustScattering &_dust_scattering;

  /*! @brief RandomGenerator used to generate random uniform numbers. */
  RandomGenerator _random_generator;

  /*! @brief DensityGrid through which photons are propagated. */
  DensityGrid &_density_grid;

  /*! @brief Number of photons to propagate through the DensityGrid. */
  uint_fast64_t _numphoton;

public:
  /**
   * @brief Constructor.
   *
   * @param photon_source PhotonSource that emits photons.
   * @param dust_scattering DustScattering object that scatters photons off
   * dust.
   * @param random_seed Seed for the RandomGenerator used by this specific
   * thread.
   * @param density_grid DensityGrid through which photons are propagated.
   * @param image CCDImage to construct.
   */
  inline DustPhotonShootJob1(PhotonSource &photon_source,
                            const DustScattering &dust_scattering,
                            int_fast32_t random_seed, DensityGrid &density_grid)
      : _photon_source(photon_source), _dust_scattering(dust_scattering),
        _random_generator(random_seed), _density_grid(density_grid), _numphoton(0) {}

  /**
   * @brief Set the number of photons for the next execution of the job.
   *
   * @param numphoton New number of photons.
   */
  inline void set_numphoton(uint_fast64_t numphoton) { _numphoton = numphoton; }



  /**
   * @brief Should the Job be deleted by the Worker when it is finished?
   *
   * @return False, since the Job is reused and managed by the
   * DustPhotonShootJobMarket.
   */
  inline bool do_cleanup() const { return false; }

  /**
   * @brief Shoot _numphoton photons from _photon_source through _density_grid.
   */
  inline void execute() {
    // parameter
    const double band_albedo = _dust_scattering.get_albedo();

    for (uint_fast64_t i = 0; i < _numphoton; ++i) {
      Photon photon = _photon_source.get_random_photon(_random_generator);
      // overwrite direction: we need the direction components to speed things
      // up in other parts of the algorithm
      //double cost = 2. * _random_generator.get_uniform_random_double() - 1.;
      //double sint = std::sqrt(std::max(1. - cost * cost, 0.));
      //double phi = 2. * M_PI * _random_generator.get_uniform_random_double();
      //double cosp = std::cos(phi);
      //double sinp = std::sin(phi);
      //const CoordinateVector<> direction(sint * cosp, sint * sinp, cost);
      //photon.set_direction(direction);
      //photon.set_direction_parameters(sint, cost, phi, sinp, cosp);
      // overwrite cross section: we want it to be the dust attenuation
      photon.set_cross_section(ION_H_n, _dust_scattering.get_kappa());
	  photon.set_weight(1.);


      const double tau_max = _density_grid.integrate_optical_depth(photon);
      double tau = -std::log(_random_generator.get_uniform_random_double());
      DensityGrid::iterator it = _density_grid.interact(photon, tau);

  //    while (it != _density_grid.end()) {

  //      // after every scattering event, the accumulated albedo is reduced
 
  //     /* _dust_scattering.scatter(photon, _random_generator);*/
  //      tau = -std::log(_random_generator.get_uniform_random_double());
  //      it = _density_grid.interact(photon, tau);
  //    }
  }
  }

  /**
   * @brief Get a name tag for this job.
   *
   * @return "dustphotonshootjob".
   */
  inline std::string get_tag() const { return "dustphotonshootjob1"; }
};

#endif // DUSTPHOTONSHOOTJOB1_HPP
