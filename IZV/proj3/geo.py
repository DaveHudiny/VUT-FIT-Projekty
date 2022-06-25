#!/usr/bin/python3.8
# coding=utf-8
"""This is my solution of geo part for IZV project 3.

Keyword arguments:
author -- David Hudák
login -- xhudak03
year -- 2021
"""
import pandas as pd
import geopandas as gp
import matplotlib.pyplot as plt
import contextily as cx
import sklearn.cluster
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable
# muzete pridat vlastni knihovny


def make_geo(df: pd.DataFrame) -> gp.GeoDataFrame:
    """Konvertovani dataframe do geopandas.GeoDataFrame se spravnym kodovanim.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    """
    gdf = gp.GeoDataFrame(df, geometry=gp.points_from_xy(df["d"], df["e"]),
                          crs="EPSG:5514")

    return gdf[gdf['geometry'].is_valid]


def plot_geo(gdf: gp.GeoDataFrame, fig_location: str = None,
             show_figure: bool = False):
    """Plot 6 subplots for accidents in JHM.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    fig_location -- string leading to file where print figures
    show_figure -- if True, figure is showed
    """
    gdf["date"] = gdf["p2a"].values
    gdf = gdf.astype({"date": "datetime64"})
    gdfJHM = gdf[gdf["region"] == "JHM"].to_crs("epsg:3857")
    gdfDalnice = gdfJHM[gdfJHM["p36"] == 0]
    gdfSilnice = gdfJHM[gdfJHM["p36"] == 1]
    fig, axes = plt.subplots(3, 2, sharex=True, sharey=True, figsize=(7.5, 10))

    gdfDalnice[gdfDalnice["date"].dt.year == 2018].plot(ax=axes[0][0],
                                                        markersize=0.5,
                                                        color="green")
    axes[0][0].set_title("JHM kraj: dálnice (2018)", fontsize=8)

    gdfDalnice[gdfDalnice["date"].dt.year == 2019].plot(ax=axes[1][0],
                                                        markersize=0.5,
                                                        color="green")
    axes[1][0].set_title("JHM kraj: dálnice (2019)", fontsize=8)

    gdfDalnice[gdfDalnice["date"].dt.year == 2020].plot(ax=axes[2][0],
                                                        markersize=0.5,
                                                        color="green")
    axes[2][0].set_title("JHM kraj: dálnice (2020)", fontsize=8)

    gdfSilnice[gdfSilnice["date"].dt.year == 2018].plot(ax=axes[0][1],
                                                        markersize=0.5,
                                                        color="red")
    axes[0][1].set_title("JHM kraj: silnice první třídy (2018)", fontsize=8)

    gdfSilnice[gdfSilnice["date"].dt.year == 2019].plot(ax=axes[1][1],
                                                        markersize=0.5,
                                                        color="red")
    axes[1][1].set_title("JHM kraj: silnice první třídy (2019)", fontsize=8)

    gdfSilnice[gdfSilnice["date"].dt.year == 2020].plot(ax=axes[2][1],
                                                        markersize=0.5,
                                                        color="red")
    axes[2][1].set_title("JHM kraj: silnice první třídy (2020)", fontsize=8)

    for ax in axes:
        for axx in ax:
            cx.add_basemap(axx, crs=gdfJHM.crs,
                           source=cx.providers.Stamen.TonerLite, alpha=0.9)
            axx.set_xticks([])
            axx.set_yticks([])
            axx.axis('off')

    if fig_location is not None:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def plot_cluster(gdf: gp.GeoDataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """Figure map with clusters for accidents.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    fig_location -- string leading to file where print figures
    show_figure -- if True, figure is showed
    """
    gdfJHM = gdf[(gdf["region"] == "JHM") &
                 (gdf["p36"] == 1)].to_crs("epsg:3857")

    coords = np.dstack([gdfJHM.geometry.x, gdfJHM.geometry.y]).reshape(-1, 2)
    db = sklearn.cluster.MiniBatchKMeans(n_clusters=23).fit(coords)
    # I've tried classical KMeans and Gaussian, but I liked this most.
    # Maybe not same as original output from zadani, but the best I can do.
    # Amount of cluster was chosen clearly experimentally.
    # Also best I can do.

    gdfJHM["cluster"] = db.labels_
    gdfPOM = gdfJHM.dissolve(by="cluster", aggfunc={"p1": "count"})

    gdfResult = gdfPOM.explode()
    fig, ax = plt.subplots(1, 1, figsize=(10, 10))
    divider = make_axes_locatable(ax)
    cax = divider.append_axes("bottom", size="5%", pad=0.1)
    gdfResult.plot(ax=ax, markersize=2, column="p1", legend=True,
                   legend_kwds={"label": "Počet nehod v úseku",
                                "orientation": "horizontal",
                                "shrink": 0.7}, vmin=0, cax=cax)

    cx.add_basemap(ax, crs=gdfJHM.crs,
                   source=cx.providers.Stamen.TonerLite, alpha=0.9)
    ax.axis('off')
    ax.set_title("Nehody v JHM kraji na silnicích 1. třídy", fontsize=9)
    if fig_location is not None:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    # zde muzete delat libovolne modifikace
    gdf = make_geo(pd.read_pickle("accidents.pkl.gz"))
    plot_geo(gdf, "geo1.png", True)
    plot_cluster(gdf, "geo2.png", True)
