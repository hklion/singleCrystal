#!/usr/bin/python

from root_numpy import root2rec
import numpy as np
from pylab import *
from scipy import interpolate
import random
from math import exp, pi, sqrt
from probfit import BinnedChi2, UnbinnedLH, BinnedLH, Extended, Normalized
from probfit import gaussian
from iminuit import Minuit

# Setup
# Seed random number generator
random.seed(0)
# Define quantum efficiency and an interpolation function
e_ph = [1.378, 1.459, 1.550, 1.653, 1.771, 1.908, 2.067, 2.254, 2.480, 2.755,
        3.100, 3.543]
q_eff = [0.69, 0.78, 0.82, 0.85, 0.85, 0.86, 0.85, 0.84, 0.81, 0.75, 0.65, 0.50]
# Convert eV to MeV
for i in range(len(e_ph)):
    e_ph[i] /= 1000000
# Determine the maximum and minimum energies - this determines the
# interpolation range.
interp_min = e_ph[0]
interp_max = e_ph[-1]
# Interpolate to get a function for quantum efficiency in terms of photon
# energy.
q_eff_fn = interpolate.UnivariateSpline(e_ph, q_eff)

# Get data
data = root2rec("../output/length_study/lengthStudy100small.root", treename="ntp1")
print "got data"

# Get output data file
f = open("processed.dat", "w")

# Number of events processed
iEvent_proc = 0
# Arrays of processed number of hits and energy deposit
n_hits_proc = []
energy_proc = []
n_hits_proc_uncut = []
energy_proc_uncut = []
# Arrays of the processed data arrays for each run
n_hits_all = []
energy_all = []
n_hits_all_uncut = []
energy_all_uncut = []
# Loop through the energy deposit arrays for each event
for i in range(len(data.eventID)):
    eventID = data.eventID[i]
    event_energy = data.energy[i]
    # Number of hits and energy detected in a single event
    n_hits_registered = 0
    energy_registered = 0
    # Loop through the energy deposit of each hit
    for hit_energy in event_energy:
        # Get a random float from 0.0 to 1.0. If it is less than the quantum
        # efficiency for that energy, consider the hit as accepted, and add it
        # to the hit and energy counters.
        if hit_energy > interp_max:
            print hit_energy
        elif hit_energy < interp_min:
            print hit_energy
        if (True):
        #if (random.random() < q_eff_fn(hit_energy)):
            n_hits_registered += 1 
            energy_registered += hit_energy
    # Once all hits in an event have been processed, add the hit and energy
    # counters to the respective lists, restricting the range if needed
    if energy_registered < 0.002:
        n_hits_proc.append(n_hits_registered)
        energy_proc.append(energy_registered * 1000)
    # Add all hits to the uncut histogram
    n_hits_proc_uncut.append(n_hits_registered)
    energy_proc_uncut.append(energy_registered * 1000)
    f.write('%7d %7d %7.6f\n' % (eventID, n_hits_registered, energy_registered))
    # Print current progress
    iEvent_proc += 1
    if iEvent_proc % 1000 == 0:
        print iEvent_proc
    # If the next event ID is zero or doesn't exist (because we are at the last
    # event, we are at the end of a run, and should add our processed data to
    # the 'all' arrays. The working processed data arrays should also be set
    # to be empty.
    if i == len(data.eventID) - 1 or data.eventID[i + 1] == 0: 
        n_hits_all.append(n_hits_proc)
        energy_all.append(energy_proc)
        n_hits_all_uncut.append(n_hits_proc_uncut)
        energy_all_uncut.append(energy_proc_uncut)
        n_hits_proc = []
        energy_proc = []
        n_hits_proc_uncut = []
        energy_proc_uncut = []

dist = [-4.9, -4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0]
for i in range(len(dist)):
    dist[i] += 5.5
for i in range(len(n_hits_all)):
    run = n_hits_all[i]
    run_np = np.zeros(len(run))
    for j in range(len(run)):
        run_np[j] = run[j]
    n_bins = max(run) - min(run) + 1
    hist(run, bins=n_bins, histtype='step')
    title("Number of photons incident on APD, 511 keV gamma " + str(dist[i]) + "cm away from APD")
    xlabel("Number of photons")
    ylabel("Number of events")
    savefig("n_hits" + str(i) + ".pdf")
    figure()
#    norm_const = len(run)
#    pdf_nh = Normalized(gaussian, (min(run_np), max(run_np)))
#    pdf_nh = Extended(pdf_nh)
#    blh_nh = BinnedLH(pdf_nh, run_np, bins=n_bins, extended=True)
#    m_nh = Minuit(blh_nh, mean=250., sigma= 10, N=norm_const, error_mean=10.,
#    error_sigma=1.,limit_sigma=(0.,1000.), error_N=100.)
#    m_nh.set_up(0.5)
#    m_nh.migrad()
#    blh_nh.show(m_nh)
    savefig("n_hits" + str(i) + ".pdf")


for i in range(len(energy_all)):
    run = energy_all[i]
    hist(run, bins=50, histtype='step')
    title("Energy deposited on APD, 511 keV gamma " + str(dist[i]) + " cm away from APD")
    xlabel("Energy (keV)")
    ylabel("Number of events")
    savefig("edep" + str(i) + ".pdf")
    figure()
'''
# Copy regular arrays to appropriately sized numpy arrays
n_hits_proc_np = np.zeros(len(n_hits_proc))
energy_proc_np = np.zeros(len(energy_proc))
for i in range(len(n_hits_proc)):
    n_hits_proc_np[i] = n_hits_proc[i]
    # Rescale the energy deposit to be in keV
    energy_proc_np[i] = energy_proc[i] * 1000
# Determine the number of bins for the number of hits for the fit-- need to be
# careful because the distribution is discrete
n_hits_bins = max(n_hits_proc_np) - min(n_hits_proc_np) + 1
# Do the same for the full histogram
n_hits_bins_uncut = max(n_hits_proc_uncut) - min(n_hits_proc_uncut) - 1
# Make histogram
hist(n_hits_proc_uncut, bins=n_hits_bins_uncut, histtype='step')
# Fit for number of photons
norm_const = len(n_hits_proc_np)
pdf_nh = Normalized(gaussian, (min(n_hits_proc_np), max(n_hits_proc_np)))
pdf_nh = Extended(pdf_nh)
blh_nh = BinnedLH(pdf_nh, n_hits_proc_np, bins=n_hits_bins, extended=True)
m_nh = Minuit(blh_nh, mean=170., sigma= 10., N=norm_const, error_mean=10.,
    error_sigma=1.,limit_sigma=(0.,1000.), error_N=100.)
m_nh.set_up(0.5)
m_nh.migrad()
blh_nh.show(m_nh)

# Generate and save figure for the number of photons
title("Number of photons detected by APD")
xlabel("Number of photons")
ylabel("Number of events")
savefig("n_ph.pdf")

# Create a new figure
figure()
# Make histogram for the energy deposit
hist(energy_proc_uncut, bins=40, histtype='step')
# Fit for the energy deposit
pdf_en = Normalized(gaussian, (min(energy_proc_np), max(energy_proc_np)))
pdf_en = Extended(pdf_en)
blh_en = BinnedLH(pdf_en, energy_proc_np, extended=True)
m_en = Minuit(blh_en, mean=0.5, sigma=0.05, N = norm_const, error_mean=0.1, 
    limit_mean = (0, 100), error_sigma=0.005, limit_sigma = (0, 1000),
    error_N = 100)
m_en.set_up(0.5)
m_en.migrad()
blh_en.show(m_en)

# Generate and save figure for the energy deposit
title("Energy detected by APD")
xlabel("Energy (keV)")
ylabel("Number of events")
savefig("edep.pdf")'''
