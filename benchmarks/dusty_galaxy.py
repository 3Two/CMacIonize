#! /usr/bin/python

################################################################################
# This file is part of CMacIonize
# Copyright (C) 2017 Bert Vandenbroucke (bert.vandenbroucke@gmail.com)
#
# CMacIonize is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# CMacIonize is distributed in the hope that it will be useful,
# but WITOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with CMacIonize. If not, see <http://www.gnu.org/licenses/>.
################################################################################

##
# @file dusty_galaxy.py
#
# @brief Script used to plot the binary output file generated by the dusty
# galaxy test.
#
# @author Bert Vandenbroucke (bv7@st-andrews.ac.uk)
##

## load necessary modules
# for reading in the binary file
import numpy as np
# for plotting
import pylab as pl

# load the binary file
image = np.fromfile("galaxy_image.dat", dtype = np.float64)

# convert to a 200x200 image array
image = image.reshape((200, 200))
# due to different ordering in CMacIonize and Python, we need to transpose the
# image before we can plot it
image = image.T

# make the plot
fig, ax = pl.subplots(1, 1)

cax = ax.imshow(image)
fig.colorbar(cax)
pl.savefig("galaxy_image.png")
