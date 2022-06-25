#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Author: David Hudak
# Login: xhudak03
# Subject: IZV
# School: BUT FIT
# Short description: Script vizualizes data from download.py.

import numpy as np
import matplotlib.pyplot as plt
import matplotlib
from download import * 
import argparse
import os

# povolene jsou pouze zakladni knihovny (os, sys) a knihovny numpy, matplotlib a argparse

def divideByAverage(x):
    """ Function creates lines usable for second figure. Line should contain values from >0 to 100.
        Keywords:
            x -- line of values. 
    """
    average = np.sum(x)
    y = (x/average)*100
    y[y == 0.0] = np.nan # denaturate some values
    return y


from download import DataDownloader

def get_data(data_source):
    """ Function 
        Keywords:
            data_source -- big dictionary of headers
        Return:
            regionDict -- big dictionary separated to regions
    """
    regions = np.unique(data_source["region"])
    regionDict = {}
    for region in regions:
        regionDict[region] = np.array(data_source["p24"])[np.where(data_source["region"] == region)[0]]
        # returns array from indices, where region value is the one we were looking for
    return regionDict

def plot_stat(data_source,
              fig_location=None,
              show_figure=False):
    """Function creates  two figures
        Keywords:
            data_source -- dictionary of dictionaries which contains all usable data
            fig_location -- path to file, where save figure.
            show_figure -- bool, show or not show figure
    """
    if data_source == None:
        print("Run script again, some data were missing.")
        return
    betterData = get_data(data_source)


    #    regions = np.array((,))
    #for header in DataDownloader().headers:
    #    regions = np.ndarray((0,))
    #regions = np.ndarray((0,))

    poleNp = np.array([])
    xedges = []
    i = np.double(0)
    ticksX = [x for x in range(len(betterData))]
    for region in betterData: # Maybe there is better way to do this, but I dont know any
        xedges.append(region)
        pole = np.zeros((6,))
        unique, counts = np.unique(betterData[region], return_counts=True)
        for i, c in zip(unique, counts):
            if i == 0:
                pole[5] = c
            else:
                pole[int(i)-1] = c # To make same order of values like in zadani
            
        poleNp = np.append(poleNp, pole)

    poleNp = poleNp.reshape(len(betterData), 6).T # Transform it to some better format
    #plt.plot(poleNp)
    #print(poleNp)
    cm = 1/2.54 # Definition for cm,... pretty weird though
    fig, ax = plt.subplots(2, 1, figsize=(18*cm, 18*cm))
    ax[0].set_title("Absolutně")
    img1 = ax[0].matshow(poleNp, origin='upper', norm=matplotlib.colors.LogNorm(vmin=10**0, vmax=10**5)) # Figuring and printing some useful values
    ax[0].set_xticks(ticksX)
    ax[0].set_yticks([5,4,3,2,1,0]) # Working with default values
    
    ax[0].set_xticklabels(xedges)
    ax[0].set_yticklabels(["Žádná úprava", "Nevyznačená", "Přenosné dopravní značky", 
        "Dopravní značky", "Semafor mimo provoz", "Přerušovaná žlutá"]) # Naming it by zadání
    ax[0].xaxis.set_ticks_position("bottom")
    #fig.colors.LogNorm(10**0, 10**5)
    cbar = plt.colorbar(img1, ax=ax[0], shrink = 0.6) # creating colorbar
    cbar.ax.set_ylabel("Počet nehod")
    plt.tight_layout()            #

    cmap = matplotlib.cm.plasma # Setting colors to plasma
    cmap = matplotlib.cm.get_cmap("plasma").copy()
    cmap.set_bad(color='white')


    ax[1].set_title("Relativně vůči příčině")
    poleNp = np.apply_along_axis(divideByAverage, axis=1, arr=poleNp) # Normalizing values to some better format.
    img2 = ax[1].matshow(poleNp, origin='upper', cmap=cmap)
    ax[1].set_xticks(ticksX)
    ax[1].set_yticks([5,4,3,2,1,0])

    ax[1].set_xticklabels(xedges)
    ax[1].set_yticklabels(["Žádná úprava", "Nevyznačená", "Přenosné dopravní značky", 
        "Dopravní značky", "Semafor mimo provoz", "Přerušovaná žlutá"])
    ax[1].xaxis.set_ticks_position("bottom")

    cbar2 = plt.colorbar(img2, ax=ax[1], shrink = 0.6)
    cbar2.ax.set_ylabel("Podíl nehod pro danou příčinu [%]")
    plt.tight_layout()



    #plt.plot(DataDownloader().headers, indices, counts)
    if show_figure == True:
        plt.show()

    if fig_location != None: # Creating file with figure
        path = os.path.split(fig_location)
        if os.path.exists(path[0]) == False:
            os.makedirs(path[0])
        plt.savefig(fig_location)
    
        
    


# TODO pri spusteni zpracovat argumenty

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--fig_location",  help = "Path to file for saving result")
    parser.add_argument("--show_figure", action = "store_true", help = "Show figure in new window")
    #plot_stat()
    args = parser.parse_args()
    #DataDownloader().download_data()
    
    plot_stat(DataDownloader().get_dict(), args.fig_location, args.show_figure)
    pass

if __name__ == "__main__":
    main()