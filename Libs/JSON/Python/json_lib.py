#!/usr/bin/env python3

from json import (dump, load)

def readJsonFile(fileName):
    with open(fileName) as f:
        data = load(f)
    print(data)
    return data

def createADCVJsonObject(): 
    jsonObject = {"name": "Bob",
    "languages": ["English", "Fench"],
    "married": True,
    "age": 32
    }
    return jsonObject

def writeJsonFile(fileName, jsonObject):
    with open(fileName, 'w') as f:
        dump(jsonObject, f, indent=4)  

def main():
    print("\n\n\tThis file must be used as library!\n\n")

if __name__ == "__main__":
    main()