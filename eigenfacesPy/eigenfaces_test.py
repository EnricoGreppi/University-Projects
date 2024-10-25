# -*- coding: utf-8 -*-
"""
Created on Wed Apr 12 10:56:37 2023

@author: jacop
"""
import numpy as np
import matplotlib.pylab as plt
# from funzioniEF import fmedio
from scipy import linalg as lg

m=92
n=112
nSoggetti=40
# 40 soggetti in questione
nIm=10
# ogni soggetto dei 40 ha 10 immagini
nTr=6
# uso 6 immagini delle 10 per il training
nTe= 4
# uso le restanti 4 per il test
L=nSoggetti*nTr

#fase di test


import shelve
d= shelve.open('trainingmatrix.dat')  #riapro il file dizionario
sottospazio= d['sottospazio']  #assegno ad una variabile il valore della matrice contenuto nella chiave matrice
eigenface= d['eigenface']
fm= d['facciamedia']
Lprimo= d['lprimo']
d.close()  #chiudo il file dizionario


fnew = np.ones(m*n)
# trasformo nuova faccia in vettore colonna
i=1
ric =0
counter =0
while i<=40:
    j=1
    flag = 0
    io = 0
    for j in np.arange(nTr+1, nIm+1, 1, dtype=int):
        im=np.array(plt.imread('archive/s'+str(i)+'/'+str(j)+'.pgm'),dtype='float64')
        fnew = np.reshape(im,m*n)
        print('Test su immagine archive/s'+str(i)+'/'+str(j)+'.pgm')
        
#centro l'immagine
        fnew = fnew - fm

#proietto fnew nel sottospazio
        fprz=np.dot(sottospazio.T, fnew)

#ora ho un vettore in cui sono raccolte le N componenti di fnew nella base generatrice dell'autospazio 

# la minima distanza euclidea coincide con la distanza tra fnew e la sua proiezione nel sottospazio
        k=0
        dist=np.zeros((L))
        for k in range(L):
            dist[k] = lg.norm(eigenface[:,k] - fprz, ord = 2)

        epsilon = min(dist)
#scelgo una soglia
        bound = 36
# controllo soglia di riconoscimento
        if epsilon < bound:
            print('\n Riconoscimento avvenuto: elemento archive/s'+str(i)+'/'+str(j)+'.pgm è una faccia presente nel database')
            flag = flag +1
            ric = ric +1
            index = np.argmin(dist)
            sogg = index/nTr
            if sogg == 0:
                sogg = 1
            else:
                sogg = int(index/nTr) +1
            #print('\n il soggetto riconosciuto è il numero', sogg , 'con immagine più simile tra quelle del training set numero', index - (sogg-1)*nTr +1 )
            if sogg == i :
                counter = counter + 1
                io = io +1
                print('Il soggetto è riconosciuto correttamente come il numero', sogg) 
            else:
               print(' \n      $$$$$$$$$$   ATTENZIONE    $$$$$$$$$ ')
               print('\n Il soggetto ',i, ' - ', j , 'è riconosciuto ERRONEAMENTE come il numero', sogg) 
        else:
            print('Riconoscimento errato: elemento archive/s'+str(i)+'/'+str(j)+'.pgm è una faccia non presente nel database')
        #print('\n ')
    print(' \n scegliendo la soglia',bound,'per individuo archive/s'+str(i)+ ' sono state riconosciute' , flag, 'facce di cui CORRETTAMENTE', io , ' elementi sul totale di ', nTe, 'test effettuati' )
    i = i+1
    #print('\n \n ')
print('\n Scelta la soglia =',bound,'la dimensionalita dello spazio = ', Lprimo ,'numero di immagini per individuo come test = ',nTe,' in totale la soglia di riconoscimento è del', (ric/(40*nTe))*100)
print('\n Scelta la soglia =',bound,' la dimensionalita dello spazio = ', Lprimo, ' numero di immagini per individuo come test = ',nTe,' in totale la soglia di riconoscimento CORRETTA è del', (counter/(40*nTe))*100)
print(' \n in totale la soglia di riconoscimento corretta pesata sulle riconosciute è del', (counter/ric)*100)