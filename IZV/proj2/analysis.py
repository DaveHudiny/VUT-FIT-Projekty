#!/usr/bin/env python3.9
"""This is my solution of IZV project 2.

Keyword arguments:
author -- David Hudák
login -- xhudak03
year -- 2021
"""
# coding=utf-8
from matplotlib import pyplot as plt
import matplotlib.dates as matdates
import pandas as pd
import seaborn as sns
import numpy as np
import os
import matplotlib.colors as mcolors

pd.options.mode.chained_assignment = None


def get_dataframe(filename: str, verbose: bool = False) -> pd.DataFrame:
    """Create pandas dataframe from pickle file.

    Keyword arguments:
    filename -- path to a pickle file
    verbose -- if True, show old and new size of pandas DataFrame
    """
    df = pd.read_pickle(filename)

    if verbose:
        usage = round(df.memory_usage(deep=True).sum()/(1_048_576), 1)
        print("orig_size=" + str(usage) + " MB")

    df["date"] = df["p2a"].values

    df = df.astype(
        {"p36": "category", "p37": "category", "p47": "category",
         "weekday(p2a)": "category", "p6": "category",  # "p2b": "int16",
         "p7": "category", "p8": "category",
         "p9": "category", "p10": "category", "p11": "category",
         # "p12": "int16", "p13a": "int8", "p13b": "int8", "p13c": "int8",
         # "p14": "int16", "p15": "int8", "p16": "int8", "p17": "int8",
         # "p18": "int8", "p19": "int8", "p20": "int8", "p21": "int8",
         # "p22": "int8", "p23": "int8", "p24": "int8", "p27": "int8",
         # "p28": "int8", "p34": "int8", "p35": "int8", "p39": "int8",
         # "p44": "int8", "p45a": "int8", "p48a": "int8", "p49": "int8",
         # "p50a": "int8", "p50b": "int8", "p51": "int8", "p52": "int8",
         # "p53": "int32", "p55a": "int8", "p57": "int8", "p58": "int8",
         "h": "category", "i": "category", "k": "category", "l": "category",
         "p": "category", "q": "category", "t": "category",  # "p5a": "int8",
         "date": "datetime64"})
    # Some conversions

    if verbose:
        usage = round(df.memory_usage(deep=True).sum()/(1_048_576), 1)
        print("new_size=" + str(usage) + " MB")
    return df


# Ukol 2: počty nehod v jednotlivých regionech podle druhu silnic

def _convert_to_roads(row):
    """Convert value from pandas DataFrame by other row value (road type).

    Keyword arguments:
    row -- row from pandas DataFrame
    """
    if row["p21"] == 0:
        return "Jiné"
    elif row["p21"] == 1:
        return "Dvoupruhová komunikace"
    elif row["p21"] == 2:
        return "Třípruhová komunikace"
    elif row["p21"] in [3, 4]:
        return "Čtyřpruhová komunikace"
    elif row["p21"] == 5:
        return "Vícepruhová komunikace"
    elif row["p21"] == 6:
        return "Rychlostní komunikace"


def plot_roadtype(df: pd.DataFrame, fig_location: str = None,
                  show_figure: bool = False):
    """Function plots count of accidents by type of road.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    fig_location -- string leading to file where print figures
    show_figure -- if True, figure is showed
    """
    regions_only = df[df["region"].isin(["JHM", "VYS", "OLK", "ZLK"])]
    # Filtrate data by region

    regions_only["typ"] = (
        regions_only.apply(lambda row: _convert_to_roads(row), axis=1))
    # Create new column typ where should be names of road types.

    g = sns.catplot(
        data=regions_only, x="region", kind="count", col="typ", col_wrap=3,
        col_order=["Dvoupruhová komunikace", "Třípruhová komunikace",
                   "Čtyřpruhová komunikace", "Vícepruhová komunikace",
                   "Rychlostní komunikace", "Jiné"],
        ci=None, legend=False, sharex=True, sharey=False)
    # Plot it all!

    for ax in g.axes:
        ax.set_xlabel("Kraj")
        ax.set_ylabel("Počet nehod")
        ax.tick_params(labelbottom=True)

    g.set_titles("{col_name}", size=15)
    g.tight_layout(pad=5)
    g.fig.suptitle("Počet nehod dle druhu silnice", size=24)

    if fig_location is not None:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def _convert_to_causes(row):
    """Convert value from pandas DataFrame by other row value (cause type).

    Keyword arguments:
    row -- row from pandas DataFrame
    """
    if row["p10"] in [1, 2]:
        return "řidičem"
    elif row["p10"] == 4:
        return "zvěří"
    else:
        return "jiné"


# Ukol3: zavinění zvěří
def plot_animals(df: pd.DataFrame, fig_location: str = None,
                 show_figure: bool = False):
    """Function plots count of accidents by animal cause.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    fig_location -- string leading to file where print figures
    show_figure -- if True, figure is showed
    """
    without_year = df.loc[df["date"].dt.year < 2021]
    without_year = without_year.loc[without_year["p58"] == 5]
    regions_only = without_year[
        without_year["region"].isin(["JHM", "VYS", "OLK", "ZLK"])]
    # Filter "p58" which are not 5, too young datas and include only
    # regions I want.

    regions_only["p10"] = regions_only.apply(
        lambda row: _convert_to_causes(row), axis=1)
    # Convert "p10" to Isla... strings

    regions_only["month"] = regions_only["date"].dt.month

    dater = regions_only.groupby(["region", "month", "p10"]).size()
    dater = dater.reset_index(drop=False)

    hue_order = ['zvěří', 'řidičem', 'jiné']

    g = sns.catplot(
        data=dater, x="month", col="region", hue="p10", y=0,
        kind="bar", col_wrap=2, sharex=False, sharey=False,
        hue_order=hue_order, legend=True)
    # Plot it all!

    for ax in g.axes:
        ax.set_xlabel("Měsíc")
        ax.set_ylabel("Počet nehod")
        ax.tick_params(labelbottom=True)

    g.set_titles("Kraj: {col_name}", size=15)

    g.tight_layout(pad=5)
    g.fig.suptitle("Počet nehod dle způsobu zavinění", size=24)

    g._legend.set_title("Zavinění")

    if fig_location is not None:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def _convert_to_wind(row):
    """Convert value from pandas DataFrame by other row value (weather type).

    Keyword arguments:
    row -- row from pandas DataFrame
    """
    if row["p18"] == 1:
        return "neztížené"
    elif row["p18"] == 2:
        return "mlha"
    elif row["p18"] == 3:
        return "na počátku deště"
    elif row["p18"] == 4:
        return "déšť"
    elif row["p18"] == 5:
        return "sněžení"
    elif row["p18"] == 6:
        return "náledí"
    elif row["p18"] == 7:
        return "silný vítr"


# Ukol 4: Povětrnostní podmínky
def plot_conditions(df: pd.DataFrame, fig_location: str = None,
                    show_figure: bool = False):
    """Function plots count of accidents by weather conditions.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    fig_location -- string leading to file where print figures
    show_figure -- if True, figure is showed
    """
    regions_only = df[df["region"].isin(["JHM", "VYS", "OLK", "ZLK"])]
    the_others = regions_only[regions_only["p18"] != 0]

    the_others["p18r"] = the_others.apply(
        lambda row: _convert_to_wind(row), axis=1)

    the_others["help"] = 1

    table = pd.pivot_table(
        the_others, index=["region", "date", "p18r"],
        values="help", aggfunc="count", fill_value=0)

    table = table.unstack(level=[0, 2])
    table = table.resample("M").sum()
    # Undersample datas -- 30 (roughly) days -> 1 month

    table = table.stack(level=[1, 2]).swaplevel(1, 0)
    facet_kws = {
        "sharey": False,
        "sharex": False
    }
    table = table.reset_index()
    g = sns.relplot(
        data=table, x="date", y="help", hue="p18r", ci=None, kind="line",
        col="region", col_wrap=2, facet_kws=facet_kws)
    # Plot it all!

    for ax in g.axes:
        ax.set_xlabel("")
        ax.set_ylabel("Počet nehod")
        ax.tick_params(labelbottom=True)

    g.set_titles("Kraj: {col_name}", size=15)

    g.tight_layout(pad=5)
    g.fig.suptitle("Povětrnostní podmínky", size=24)

    xformatter = matdates.DateFormatter("%m/%y")
    for ax in g.axes:
        ax.xaxis.set_major_formatter(xformatter)

    g.set(xlim=(pd.to_datetime('20160101', format='%Y%m%d'),
                pd.to_datetime('20210101', format='%Y%m%d')))
    # Set data limitation -- year 2021 is incomplete

    g._legend.set_title("Podmínky")

    if fig_location is not None:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


if __name__ == "__main__":
    # zde je ukazka pouziti, tuto cast muzete modifikovat podle libosti
    # skript nebude pri testovani pousten primo, ale budou volany konkreni ¨
    # funkce.
    df = get_dataframe("accidents.pkl.gz", verbose=False)
    plot_roadtype(df, fig_location="01_roadtype.png", show_figure=True)
    plot_animals(df, "02_animals.png", True)
    plot_conditions(df, "03_conditions.png", True)
