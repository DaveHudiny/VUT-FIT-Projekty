#!/usr/bin/python3.8
# coding=utf-8
"""This is my solution of documentation part for IZV project 3.

Keyword arguments:
author -- David Hudák
login -- xhudak03
year -- 2021
"""
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import seaborn as sns
# muzete pridat vlastni knihovny

pd.options.mode.chained_assignment = None


def create_figure(df: pd.DataFrame, fig_location: str = None,
                  show_figure: bool = False):
    """Function plots count of accidens by driver state.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    fig_location -- string leading to file where print figures
    show_figure -- if True, figure is showed
    """
    dfM = df[(df["p13a"] > 0) & (df["p48a"] == 1)
             & (df["date"].dt.year.isin([2017, 2018, 2019, 2020]))]

    dfM.loc[dfM["p57"].isin([3]), "Stav"] = "Pod vlivem léků či narkotik"
    dfM.loc[dfM["p57"].isin([4, 5]), "Stav"] = "Alkohol"
    dfM.loc[dfM["p57"].isin([2]), "Stav"] = "Únava, mikrospánek"

    dfM.loc[dfM["p57"].isin([6, 7, 8]), "Stav"] = "Zdravotní stav"
    dfM.loc[dfM["p57"].isin([9]), "Stav"] = "Pokus o sebevraždu/sebevražda"
    dfM.loc[dfM["p57"].isin([0]), "Stav"] = "Jiné"

    g = sns.catplot(data=dfM, x="Rok", hue="Stav", kind="count",
                    hue_order=["Alkohol", "Pod vlivem léků či narkotik",
                               "Únava, mikrospánek", "Zdravotní stav",
                               "Pokus o sebevraždu/sebevražda", "Jiné"])

    g.ax.set_ylabel("Počet smrtících nehod")

    if fig_location is not None:
        plt.savefig(fig_location)
    if show_figure:
        plt.show()


def create_table(df: pd.DataFrame, latex: bool = True):
    """Function print table of accidents by cause.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    """
    dfM = df[(df["date"].dt.year.isin([2017, 2018, 2019, 2020]))]
    dfM.loc[dfM["p57"].isin([3]), "Stav"] = "Léky, narkotika"
    dfM.loc[dfM["p57"].isin([4, 5]), "Stav"] = "Alkohol"
    dfM.loc[dfM["p57"].isin([0, 1, 2, 6, 7, 8, 9]), "Stav"] = "Ostatní"
    contingency = pd.crosstab(dfM["Stav"], dfM["Rok"])
    if latex is True:
        print(contingency.to_latex())
        print("Tabulka může být mírně ručně upravena z hlediska designu.")
    else:
        print(contingency)

    print()


def create_values(df: pd.DataFrame):
    """Function print useful values from pandas dataframe.

    Keyword arguments:
    df -- pandas DataFrame, source of datas
    """
    dfM = df[(df["date"].dt.year.isin([2017, 2018, 2019, 2020]))]
    suicides = len(dfM[dfM["p57"].isin([9])])
    print("Počet sebevražd (vč. pokusů) v celém vzorku:", suicides)
    alcoholPrivate = len(dfM[(dfM["p13a"] > 0) &
                             (dfM["p57"].isin([4, 5])) & (dfM["p48a"] == 1)])

    print("Počet smrtících nehod zaviněných alkoholem v soukromých vozidlech:", alcoholPrivate)

    alcoholAll = len(dfM[(dfM["p13a"] > 0) & (dfM["p57"].isin([4, 5]))])

    print("Počet smrtících nehod zaviněných alkoholem ve všech vozidlech:", alcoholAll)

    accidentsPrivate = len(dfM[(dfM["p13a"] > 0) & (dfM["p48a"] == 1)])

    print("Celkový počet smrtících nehod v soukromých vozidlech:", accidentsPrivate)

    accidentsAll = len(dfM[(dfM["p13a"] > 0)])

    print("Celkový počet smrtících nehod:", accidentsAll)
    print("Podíl smrtících nehod zaviněných alkoholem na celkovém počtu nehod (všechna vozidla):",
          round(alcoholPrivate/accidentsPrivate * 100, 2), "%")
    print("Podíl smrtících nehod zaviněných alkoholem na celkovém počtu nehod (všechna vozidla):",
          round(alcoholAll/accidentsAll * 100, 2), "%")
    print("Podíl alkoholu na nehodách obecně:",
          round(len(dfM[dfM["p57"].isin([4, 5])])/len(dfM) * 100, 2), "%")
    print("Celkový počet nehod:", len(dfM))


if __name__ == "__main__":
    df = pd.read_pickle("accidents.pkl.gz")
    df["date"] = df["p2a"].values
    df["Rok"] = pd.DatetimeIndex(df["date"]).year
    df = df.astype({"date": "datetime64"})
    create_figure(df, fig_location="fig.pdf", show_figure=False)
    create_table(df)
    create_values(df)
