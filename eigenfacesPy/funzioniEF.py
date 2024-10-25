#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr  8 20:21:14 2023

@author: enricogreppi
"""
import numpy as np

def fmedio(trainingset,k,L):
    f=np.zeros(k)
    for i in range (L):
        f=f+trainingset[:,i]
    f=f/L
    # calcolo la faccia media sommando i vettori del training set componente a componente
    # e dividendo poi per il numero di campioni
    return f