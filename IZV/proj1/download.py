#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# Author: David Hudak
# Login: xhudak03
# Subject: IZV
# School: BUT FIT
# Short description: Script downloads and parses data from server.

from bs4 import BeautifulSoup
import numpy as np
import zipfile
import requests, os, io
import re
import pickle
import gzip
import csv


from requests.api import request

# Kromě vestavěných knihoven (os, sys, re, requests …) byste si měli vystačit s: gzip, pickle, csv, zipfile, numpy, matplotlib, BeautifulSoup.
# Další knihovny je možné použít po schválení opravujícím (např ve fóru WIS).


class DataDownloader:
    """ TODO: dokumentacni retezce 

    Attributes:
        headers    Nazvy hlavicek jednotlivych CSV souboru, tyto nazvy nemente!  
        regions     Dictionary s nazvy kraju : nazev csv souboru
    """

    headers = ["p1", "p36", "p37", "p2a", "weekday(p2a)", "p2b", "p6", "p7", "p8", "p9", "p10", "p11", "p12", "p13a",
               "p13b", "p13c", "p14", "p15", "p16", "p17", "p18", "p19", "p20", "p21", "p22", "p23", "p24", "p27", "p28",
               "p34", "p35", "p39", "p44", "p45a", "p47", "p48a", "p49", "p50a", "p50b", "p51", "p52", "p53", "p55a",
               "p57", "p58", "a", "b", "d", "e", "f", "g", "h", "i", "j", "k", "l", "n", "o", "p", "q", "r", "s", "t", "p5a"]
    types = {"p1":  str, 
        "p36": np.uint8, 
        "p37": str, 
        "p2a": str, 
        "weekday(p2a)": np.uint8, 
        "p2b": str, 
        "p6":  np.uint8, 
        "p7":  np.uint8, 
        "p8":  np.uint8,
        "p9":  np.uint8, 
        "p10": np.uint8, 
        "p11": np.uint8, 
        "p12": np.uint16, 
        "p13a":np.uint8,
        "p13b":np.uint8, 
        "p13c":np.uint8, 
        "p14": np.uint64, 
        "p15": np.uint8, 
        "p16": np.uint8, 
        "p17": np.uint8, 
        "p18": np.uint8, 
        "p19": np.uint8, 
        "p20": np.uint8,
        "p21": np.uint8, 
        "p22": np.uint8, 
        "p23": np.uint8, 
        "p24": np.uint8, 
        "p27": np.uint8, 
        "p28": np.uint8,
        "p34": np.uint8, 
        "p35": np.uint8, 
        "p39": np.uint8, 
        "p44": np.uint8, 
        "p45a":np.uint8, 
        "p47": str,
        "p48a":np.uint8, 
        "p49": np.uint8, 
        "p50a":np.uint8, 
        "p50b":np.uint8, 
        "p51": np.uint8, 
        "p52": np.uint8, 
        "p53": np.uint32, 
        "p55a":np.uint8,
        "p57": np.uint8,
        "p58": np.uint8, 
        "a":   np.float64, 
        "b":   np.float64, 
        "d":   np.float64, 
        "e":   np.float64, 
        "f":   np.float64,
        "g":   np.float64, 
        "h":   str, 
        "i":   str, 
        "j":   str, 
        "k":   str, 
        "l":   str, 
        "n":   str,
        "o":   str, 
        "p":   str, 
        "q":   str, 
        "r":   str, 
        "s":   str, 
        "t":   str, 
        "p5a": np.uint8}

    regions = {
        "PHA": "00",
        "STC": "01",
        "JHC": "02",
        "PLK": "03",
        "ULK": "04",
        "HKK": "05",
        "JHM": "06",
        "MSK": "07",
        "OLK": "14",
        "ZLK": "15",
        "VYS": "16",
        "PAK": "17",
        "LBK": "18",
        "KVK": "19",
    }

    def __init__(self, url="https://ehw.fit.vutbr.cz/izv/", folder="data", cache_filename="data_{}.pkl.gz"):
        self.url = url
        self.folder = folder
        self.cache_filename = cache_filename
        path = os.path.split(cache_filename)
        if path[0] != '':
            if os.path.exists(path[0]) == False:
                os.makedirs(path[0])
        self.downloaded = False
        
        self.regionArray = {} # Contains all downloaded data merged into one (actually doesnt make sense and not much used in my project)
    

    def download_data(self):
        """ Downloads data from webpage
            Keyword arguments:
                self -- object itself
        """
        self.downloaded = True
        request = requests.get(self.url)
        datas = request.text
        soup = BeautifulSoup(datas, features="html.parser") # Parse HTML to some understandable data
        if not (self.folder in os.listdir(".")):
            os.mkdir(self.folder)
        for buttons in soup.find_all("button"): 
            on_click = buttons.get("onclick")
            paths = on_click.split("'")
            for path in paths:
                if ".zip" in path:
                    url = self.url + path
                    zipFile = requests.get(url)
                    name = re.findall("/.*", string=path)
                    realname = name[-1]
                    realname = realname[1:]
                    filename = open("./"+str(self.folder)+"/"+realname, "wb")
                    filename.write(zipFile.content) 

    def clear_from_duplicities(self, regionArray):
        """ Clears regionArray from duplicities -- unfinished
            Keyword arguments:
                self -- object itself, type DataDownloader
                regionArray -- complete region dictionary
        """      
        uniques, counts = np.unique(regionArray["p1"], return_counts = True)
        duplicitiesWhere = np.where(counts > 1)[0]
        indicesDelete = []
        for i in uniques[duplicitiesWhere]:
            indicesDelete.append(np.where(regionArray["p1"]==i)[0][1:])
        if indicesDelete:
            indicesDelete = np.concatenate(indicesDelete)
        for header in self.headers:
            regionArray[header] = np.delete(regionArray[header], indicesDelete)
        regionArray["region"] = np.delete(regionArray["region"], indicesDelete)
        return regionArray


    def parse_region_data(self, region):
        """ Returns parsed regions from all zip files in default folder.
            Very slow, but I cannot find how to optimize it without cheating.
            Keyword arguments:
                self -- object itself
                region -- region to parse (for example JHM)

        """
        if region == None:
            return
        regionArray = {}
        for header in self.headers:
            regionArray[header]=[] # Create memory for dictionary
        regionArray["region"]=[]
        number = self.regions[region] # Find number for csv file
        if os.path.exists(self.folder) == False:
            os.makedirs(self.folder)
            DataDownloader().download_data()
        if os.listdir("./" + self.folder) == []:
            DataDownloader().download_data()
        for zipFile in os.listdir("./" + self.folder):
            if ".zip" in zipFile:
                zipF = zipfile.ZipFile("./" + self.folder + "/" + zipFile, "r")
                if str(number)+".csv" in zipF.namelist(): # find file in zip
                    with zipF.open(str(number)+".csv", mode="r") as csvfile: # read and decompress file from zip
                        dialect = csv.Sniffer().sniff(csvfile.peek(n=128).decode('cp1250'))
                        
                        lines = csv.reader(io.TextIOWrapper(csvfile, 'cp1250'), dialect)
                        for line in lines: # parse every line from zip
                            if len(line) == 64:
                                regionArray["region"].append(region)
                            for header, segm in zip(self.headers, line): # append all segments to dictionary of np.ndarrays
                                regionArray[header].append(segm)
                else: # file missing, download again
                    print("Chybí data")
                    self.download_data()
                    print("Staženo")
                    return None
        regionArrayNumpy = {}
        for header in self.headers:
            if self.types[header] in [np.uint8, np.uint16, np.uint32, np.uint64]:
                pointerArray = np.array(regionArray[header])
                pointerArray[pointerArray == ''] = "0"
                regionArrayNumpy[header]=np.array(pointerArray, dtype = self.types[header])
            elif self.types[header] in [np.float64]:
                arrayMy = []
                for i in regionArray[header]:
                    i = i.replace(",", ".")
                    if i == "":
                        i = "0.0"
                    if i in ["A:", "B:", "C:", "D:", "E:", "F:", "G:"]:
                        i = "0.0"
                    arrayMy.append(i)
                regionArrayNumpy[header]=np.array(arrayMy, dtype=np.float64)
            else:
                regionArrayNumpy[header]=np.asarray(regionArray[header], dtype = self.types[header]) # Create memory for dictionary
        regionArrayNumpy["region"]= np.array(regionArray["region"], dtype = str)
        #print(counts)
        return self.clear_from_duplicities(regionArrayNumpy)




    def get_dict(self, regions=None):
        """ Functions loads full dictionary to self.regionArray (and returns it).
            Keyword arguments:
                self -- object itself
                regions -- list of regions, which should be parsed
        """
        if regions == None:
            regions = self.regions
    
        for header in self.headers: # initialize correct dictionary for storing data
            self.regionArray[header] = np.ndarray((0,))
        self.regionArray["region"] = np.ndarray((0,))  

        for region in regions: # Iterate through input regions ("JHM", "HKK" etc.)
            myPickleRick = self.cache_filename.replace("{}", region)
            if myPickleRick in os.listdir("./"): # region have been already parsed, so you can just read it from cache pickle file
                
                with gzip.open(myPickleRick,'rb') as f:
                    data = ""
                    data = pickle.load(f)
                    data = pickle.loads(data)
                    for header in self.headers: # Append downloaded data to every header in dictionary
                       self.regionArray[header] = np.append(self.regionArray[header], data[header]) 
                    self.regionArray["region"] = np.append(self.regionArray["region"], data["region"])

            else: # region havent been parsed yet.

                pom = self.parse_region_data(region) # parse region from all zip files
                if pom == None:
                    print("Some data were missing. Data downloaded. If problem sustains, call method DataDownloader().download_data()")
                    return None
                with gzip.open(myPickleRick,'wb') as f:
                    data = pickle.dumps(pom) # serialize and store data in cache
                    pickle.dump(data, f)
                for header in self.headers:
                    self.regionArray[header]=np.append(self.regionArray[header], pom[header])
                self.regionArray["region"]=np.append(self.regionArray["region"], pom["region"])
        
        return self.regionArray

downloader = DataDownloader()
#downloader.download_data()
#downloader.parse_region_data("JHM")

# TODO vypsat zakladni informace pri spusteni python3 download.py (ne pri importu modulu)

def main():
    """ Function does some default stuff by zadání
    """
    downloader = DataDownloader()
    regions = downloader.get_dict(["JHM", "OLK", "VYS"])
    print("Regions used: JHM, OLK, VYS")
    indices, counts = np.unique(regions["region"], return_counts = True)
    sums = 0
    for i, c in zip(indices, counts):
        print("Region", i, "has", c, "records")
        sums = sums + c
    print("Overall number of records: ", sums)
    print("Number of columns: ", len(downloader.headers))
    print("Number of regions: ", len(indices))


if __name__ == "__main__":
    main()