from __future__ import division
import os
from scipy.signal import detrend
import matplotlib.pyplot as plt
import numpy as np

def remove_distortion(X):
    X -= np.mean(X, axis=0)
    X /= np.std(X, axis=0)
    X = detrend(X, axis = 0, type = "linear")
    return X

def plot_all(directory, prefix, suffix):
    fnames = []
    for f in os.listdir(directory):
        if f.startswith(prefix) and f.endswith(suffix):
            fnames.append(f)


    for f in fnames:
        d = np.loadtxt(directory +'/'+f, delimiter=',', skiprows=1)

        vx = np.cumsum(d[:,0])
        vy = np.cumsum(d[:,1])

        x = np.cumsum(vx)
        y = np.cumsum(vy)

        x = remove_distortion(x)
        y = remove_distortion(y)

        plt.subplot(2,2,2)
        plt.plot(x,y, "g")

        d -= np.mean(d, axis=0)
        d /= np.std(d, axis=0)

        plt.subplot(2,2,1)
        plt.plot(d[:,0],d[:,1], "b")

        d = detrend(d, axis = 0, type = "linear")

        plt.subplot(2,2,3)
        plt.plot(d[:,6],d[:,7],"k")

        vx = np.cumsum(d[:,0])
        vy = np.cumsum(d[:,1])
        x = np.cumsum(vx)
        y = np.cumsum(vy)
        x = remove_distortion(x)
        y = remove_distortion(y)

        plt.subplot(2,2,4)
        plt.plot(x,y, "m")
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

def uniform_size(X):
    X -= np.mean(X, axis=0)
    X /= np.std(X, axis=0)
    # X = detrend(X, axis = 0, type = "linear")

    return X



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

