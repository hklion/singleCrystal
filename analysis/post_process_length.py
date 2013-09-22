# post_process_length.py
# Script for generating histograms and plots of average number of hits and
# energy deposits for a run that consists of events started at different
# positions.

# -*- coding: utf-8 -*-
# <nbformat>3.0</nbformat>

# <codecell>

from root_numpy import root2rec
import numpy as np
from pylab import *
from scipy import interpolate
import random
from math import exp, pi, sqrt
from probfit import BinnedChi2, UnbinnedLH, BinnedLH, Extended, Normalized
from probfit import gaussian
from iminuit import Minuit

# <codecell>

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

# <codecell>

# Get data
data = root2rec("/home/pythontutorial/mountpoint/output/length_study/lengthStudy1000.root", treename="ntp1")
print "got data"

# Get output data file
f = open("processed.dat", "w")

# <codecell>

# Cuts for the plotting of the data
n_hits_plot_cut_low = 0
n_hits_plot_cut_high = 500
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
        if (random.random() < q_eff_fn(hit_energy)):
            n_hits_registered += 1
            energy_registered += hit_energy
    # Add hits within plotting range
    if n_hits_plot_cut_low < n_hits_registered < n_hits_plot_cut_high:
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

# <codecell>


# <codecell>

# Distances to APD face of crystal
dist = [0.6, 1.5, 2.5, 3.5, 4.5, 5.5, 6.5, 7.5, 8.5, 9.5, 10.5]
# Bounds for fits
low_fit_bounds = [90, 100, 125, 115, 120, 120, 120, 120, 120, 120, 120]
high_fit_bounds = [130, 250, 175, 165, 160, 170, 165, 165, 160, 160, 165]
en_low_fit_bounds = [0.25, 0.3, 0.375, 0.325, 0.33, 0.35, 0.35, 0.325, 0.325, 0.33, 0.325]
en_high_fit_bounds = [0.38, 0.7, 0.5, 0.5, 0.467, 0.475, 0.475, 0.475, 0.475, 0.475, 0.475]
nh_means = []
en_means = []
# Loop through each of the runs at a different distance
for i in range(len(n_hits_all)):
    # Get run data
    run = n_hits_all[i]
    run_uncut = n_hits_all_uncut[i]
    # Convert to numpy array
    run_np = np.zeros(len(run))
    for j in range(len(run)):
        run_np[j] = run[j]
    # Determine number of bins
    n_hits_bins_plot = max(run_uncut) - min(run_uncut) + 1
    n_hits_bins_fit = max(run) - min(run) + 1
    # Make histogram
    hist(run_uncut, bins=n_hits_bins_plot, histtype='step')
    norm_const = len(run) # Normalization constant
    # Get PDF
    pdf_nh = Normalized(gaussian, (min(run), max(run)))
    pdf_nh = Extended(pdf_nh)
    # Do fit
    blh_nh = BinnedLH(pdf_nh, run_np, bound=(low_fit_bounds[i], high_fit_bounds[i]), bins=(high_fit_bounds[i] - low_fit_bounds[i] + 1), extended=True)
    m_nh = Minuit(blh_nh, mean=150., sigma= 12., N=400, error_mean=10., error_sigma=1.,limit_sigma=(0.,10000.), error_N=10., limit_mean=(0,800))
    m_nh.set_up(0.5)
    m_nh.migrad()
    nh_means.append(m_nh.values["mean"])
    blh_nh.show(m_nh);
    # Output plot
    title("Number of photons incident on APD, 511 keV gamma " + str(dist[i]) + "cm away from APD")
    xlabel("Number of photons")
    ylabel("Number of events")
    savefig("n_hits" + str(i) + ".pdf")
    figure()
    # Energy. Same procedure as for the number of hits.
    run = energy_all[i]
    run_uncut = energy_all_uncut[i]
    run_np = np.zeros(len(run))
    for j in range(len(run)):
        run_np[j] = run[j]
    energy_bins_plot = 15/(en_high_fit_bounds[i] - en_low_fit_bounds[i]) * (max(run) - min(run))
    hist(run_uncut, bins=energy_bins_plot, histtype='step')
    norm_const = len(run)
    pdf_nh = Normalized(gaussian, (min(run), max(run)))
    pdf_nh = Extended(pdf_nh)
    blh_nh = BinnedLH(pdf_nh, run_np, bound=(en_low_fit_bounds[i], en_high_fit_bounds[i]), bins=15, extended=True)
    m_nh = Minuit(blh_nh, mean=0.5, sigma= 0.1, N=400, error_mean=0.1, error_sigma=0.01,limit_sigma=(0.,10000.), error_N=10., limit_mean=(0,800), limit_N=(0,10000))
    m_nh.set_up(0.5)
    m_nh.migrad()
    en_means.append(m_nh.values["mean"])
    blh_nh.show(m_nh);
    title("Energy deposited on APD, 511 keV gamma " + str(dist[i]) + "cm away from APD")
    xlabel("Energy (keV)")
    ylabel("Number of events")
    savefig("energy" + str(i) + ".pdf")
    figure()

# <codecell>

plot(dist, en_means, 'bo');
plot(dist, en_means, 'b-');
xlabel("Distance of incident gamma from APD crystal face(cm)")
ylabel("Mean deposited energy (keV)");
savefig("length_energy.pdf")
figure()
plot(dist, nh_means, 'bo');
plot(dist, nh_means, 'b-');
xlabel("Distance of incident gamma from APD crystal face (cm)")
ylabel("Mean photons deposited")
axis([0, 12, 100, 190])
savefig("length_nph.pdf")

# <codecell>


