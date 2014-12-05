from __future__ import division
import os
from scipy.signal import detrend
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

import numpy as np

def remove_distortion(X):
    X -= np.mean(X, axis=0)
    X /= np.std(X, axis=0)
    X = detrend(X, axis = 0, type = "linear")
    return X


def uniform_size(X):
    X -= np.mean(X, axis=0)
    X /= np.std(X, axis=0)
    # X = detrend(X, axis = 0, type = "linear")

    return X



# timestamp,
# x,y,z,
# v_x,v_y,v_z,
# a_x,a_y,a_z,r
# roll,pitch,yaw,
# theta,phi,psi


t_i     = 0
x_i     = 1
y_i     = 2
z_i     = 3
v_x_i   = 4
v_y_i   = 5
v_z_i   = 6
a_x_i   = 7
a_y_i   = 8
a_z_i   = 9
roll_i  = 10
pitch_i = 11
yaw_i   = 12
theta_i = 13
phi_i   = 14
psi_i   = 15

def plot_all_signals(fname, detrend=False):
    X = np.loadtxt(fname, delimiter=',', skiprows=1)
    time = X[:, t_i].copy() - X[0, t_i]
    if not detrend:
        X = uniform_size(X)
    else:
        X = remove_distortion(X)

    f = plt.figure(0)
    fig, subs = plt.subplots(5,3)
    subs[0, 0].plot(time, X[:,x_i])
    subs[0, 1].plot(time, X[:,y_i])
    subs[0, 2].plot(time, X[:,z_i])
    subs[1, 0].plot(time, X[:,v_x_i])
    subs[1, 1].plot(time, X[:,v_y_i])
    subs[1, 2].plot(time, X[:,v_z_i])
    subs[2, 0].plot(time, X[:,a_x_i])
    subs[2, 1].plot(time, X[:,a_y_i])
    subs[2, 2].plot(time, X[:,a_z_i])
    subs[3, 0].plot(time, X[:,roll_i])
    subs[3, 1].plot(time, X[:,pitch_i])
    subs[3, 2].plot(time, X[:,yaw_i])
    subs[4, 0].plot(time, X[:,theta_i])
    subs[4, 1].plot(time, X[:,phi_i])
    subs[4, 2].plot(time, X[:,psi_i])

    f2 = plt.figure(0)
    fig = plt.plot(X[:,x_i],X[:,y_i])

    plt.show()


def plot_all_ownint_signals(fname, detrend=False):
    X = np.loadtxt(fname, delimiter=',', skiprows=1)
    time = X[:, t_i].copy() - X[0, t_i]
    if not detrend:
        norm_fn = uniform_size
    else:
        norm_fn = remove_distortion

    X = norm_fn(X)
    v_x = norm_fn(np.cumsum(X[:,a_x_i]))
    x   = norm_fn(np.cumsum(v_x))
    v_y = norm_fn(np.cumsum(X[:,a_y_i]))
    y   = norm_fn(np.cumsum(v_y))
    v_z = norm_fn(np.cumsum(X[:,a_z_i]))
    z   = norm_fn(np.cumsum(v_z))

    fig0 = plt.figure(0)

    fig, subs = plt.subplots(5,3)
    subs[0, 0].plot(time, X[:,x_i], 'b')
    subs[0, 0].plot(time, x, 'k')

    subs[0, 1].plot(time, X[:,y_i], 'b')
    subs[0, 1].plot(time, y, 'k')

    subs[0, 2].plot(time, X[:,z_i], 'b')
    subs[0, 2].plot(time, z, 'k')

    subs[1, 0].plot(time, X[:,v_x_i], 'b')
    subs[1, 0].plot(time, v_x, 'k')

    subs[1, 1].plot(time, X[:,v_y_i], 'b')
    subs[1, 1].plot(time, v_y, 'k')

    subs[1, 2].plot(time, X[:,v_z_i], 'b')
    subs[1, 2].plot(time, v_z, 'k')

    subs[2, 0].plot(time, X[:,a_x_i], 'b')

    subs[2, 1].plot(time, X[:,a_y_i], 'b')

    subs[2, 2].plot(time, X[:,a_z_i], 'b')

    subs[3, 0].plot(time, X[:,roll_i], 'b')
    # subs[3, 0].plot(time, roll, 'k')

    subs[3, 1].plot(time, X[:,pitch_i], 'b')
    # subs[3, 1].plot(time, pitch, 'k')

    subs[3, 2].plot(time, X[:,yaw_i], 'b')
    # subs[3, 2].plot(time, yaw, 'k')

    subs[4, 0].plot(time, X[:,theta_i], 'b')

    subs[4, 1].plot(time, X[:,phi_i], 'b')

    subs[4, 2].plot(time, X[:,psi_i], 'b')


    fig1 = plt.figure(0)
    plt.plot(X[:,x_i],X[:,y_i])
    plt.plot(x,y)

    plt.show()

def plot_all(directory, prefix, suffix, detrend=False):
    fnames = []
    for f in os.listdir(directory):
        if f.startswith(prefix) and f.endswith(suffix):
            fnames.append(f)

    if not detrend:
        norm_fn = uniform_size
    else:
        norm_fn = remove_distortion

    # fig, subs = plt.subplots(len(fnames), sharex=True, sharey=True)

    for i, fname in enumerate(fnames):
        X = np.loadtxt(
            directory +'/'+fname,
            delimiter=',',
            skiprows=1
        )
        plt.figure(i)
        X = norm_fn(X)
        plt.plot(X[:,x_i],X[:,y_i])

    plt.show()


def plot_csv_all(directory, prefix, suffix):
    fnames = []
    for f in os.listdir(directory):
        if f.startswith(prefix) and f.endswith(suffix):
            fnames.append(f)


    for f in fnames:
        d = np.loadtxt(directory +'/'+f, delimiter=',', skiprows=1)

        d -= np.mean(d, axis=0)
        d /= np.std(d, axis=0)

        plt.subplot(3,3,1)
        plt.plot(d[:,0],d[:,1], "b")

        plt.subplot(3,3,2)
        plt.plot(d[:,0],d[:,2], "b")

        plt.subplot(3,3,3)
        plt.plot(d[:,1],d[:,2], "b")



        plt.subplot(3,3,4)
        plt.plot(d[:,3],d[:,4], "k")

        plt.subplot(3,3,5)
        plt.plot(d[:,3],d[:,5], "k")

        plt.subplot(3,3,6)
        plt.plot(d[:,4],d[:,5], "k")


        plt.subplot(3,3,7)
        plt.plot(d[:,6],d[:,7], "g")

        plt.subplot(3,3,8)
        plt.plot(d[:,6],d[:,8], "g")

        plt.subplot(3,3,9)
        plt.plot(d[:,7],d[:,8], "g")


        plt.show()


def plot_avx_all(directory, prefix, suffix):
    fnames = []
    for f in os.listdir(directory):
        if f.startswith(prefix) and f.endswith(suffix):
            fnames.append(f)


    for f in fnames:
        print f
        d = np.loadtxt(directory +'/'+f, delimiter=',', skiprows=1)

        derivative = (d[1:,:2] - d[:-1,:2])**2
        max_ij = np.argmax(derivative, axis=0)

        if max_ij[0]==max_ij[1]:
            max_i = max_ij[0] + 1
        elif derivative[max_ij[0], 0] > derivative[max_ij[1], 1]:
            max_i = max_ij[0] + 1
        else:
            max_i = max_ij[1] + 1

        # cut out mystery dead zone
        d = d[max_i:,:]

        d = -uniform_size(d)

        plt.subplot(3,2,1)
        plt.plot(d[:,6],d[:,7], "k")

        ax = d[:,6]
        ay = d[:,7]
        plt.subplot(3,2,2)
        plt.plot(ax,ay, "b")

        plt.subplot(3,2,3)
        plt.plot(d[:,3],d[:,4], "k")

        vx = uniform_size(np.cumsum(ax))
        vy = uniform_size(np.cumsum(ay))

        plt.subplot(3,2,4)
        plt.plot(vx,vy, "b")


        plt.subplot(3,2,5)
        plt.plot(d[:,0],d[:,1], "k")
        x = uniform_size(np.cumsum(vx))
        y = uniform_size(np.cumsum(vy))
        plt.subplot(3,2,6)
        plt.plot(x, y, "b")
        plt.show()


def plot_avx_all_show_dead(directory, prefix, suffix):
    fnames = []
    for f in os.listdir(directory):
        if f.startswith(prefix) and f.endswith(suffix):
            fnames.append(f)


    for f in fnames:
        print f
        d = np.loadtxt(directory +'/'+f, delimiter=',', skiprows=1)
        d = -uniform_size(d)

        derivative = (d[1:,:2] - d[:-1,:2])**2
        max_ij = np.argmax(derivative, axis=0)

        if max_ij[0]==max_ij[1]:
            max_i = max_ij[0] + 1
        elif derivative[max_ij[0], 0] > derivative[max_ij[1], 1]:
            max_i = max_ij[0] + 1
        else:
            max_i = max_ij[1] + 1

        d_bad = d[:max_i+1,:]
        d_good = uniform_size(d[max_i+1:,:])


        plt.subplot(3,3,1)
        # plt.plot(d[:,6],d[:,7], "k")
        plt.plot(d_good[:,6],d_good[:,7], "g")
        plt.plot(d_bad[:,6],d_bad[:,7], "r.")

        ax = d_good[:,6]
        ay = d_good[:,7]
        plt.subplot(3,3,2)
        plt.plot(ax,ay, "b")

        ax_u = remove_distortion(d_good[:,6])
        ay_u = remove_distortion(d_good[:,7])
        plt.subplot(3,3,3)
        plt.plot(ax_u,ay_u, "k")

        plt.subplot(3,3,4)
        # plt.plot(d[:,3],d[:,4], "k")
        plt.plot(d_good[:,3],d_good[:,4], "g")
        plt.plot(d_bad[:,3],d_bad[:,4], "r.")

        vx = uniform_size(np.cumsum(ax))
        vy = uniform_size(np.cumsum(ay))

        plt.subplot(3,3,5)
        plt.plot(vx,vy, "b")


        vx_u = remove_distortion(vx)
        vy_u = remove_distortion(vy)

        plt.subplot(3,3,6)
        plt.plot(vx_u,vy_u, "k")


        plt.subplot(3,3,7)
        # plt.plot(d[:,0],d[:,1], "g")
        # plt.plot(d[:,0],d[:,1], "r.")
        plt.plot(d_good[:,0],d_good[:,1], "g")
        plt.plot(d_bad[:,0],d_bad[:,1], "r.")

        x = uniform_size(np.cumsum(vx))
        y = uniform_size(np.cumsum(vy))
        plt.subplot(3,3,8)
        plt.plot(x, y, "b")

        x_u = remove_distortion(x)
        y_u = remove_distortion(y)

        plt.subplot(3,3,9)
        plt.plot(x_u,y_u, "k")


        plt.show()

