#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Apr  8 18:29:49 2023

@author: enrico greppi, jacopo mensa
"""

import numpy as np
import matplotlib.pylab as plt
from funzioniEF import fmedio
from scipy import linalg as lg

m=92
n=112
nSoggetti=40
# 40 soggetti in questione
nIm=10
# ogni soggetto dei 40 ha 10 immagini
nTr=6
# uso 6 immagini delle 10 per il training
#nTe=4
# uso le restanti 4 per il test
L=nSoggetti*nTr
i=1
j=1
k=0
trainingset=np.zeros((m*n,L))
# la matrice training set ha su ogni colonna un elemento (una foto delle 6 per ognuno dri 40 soggetti
# sulle varie righe ho le info legate ai pixel di ogni immagine  


#lettura e definizione training set
while i<=40:
    while j<=nTr:
        im=np.array(plt.imread('archive/s'+str(i)+'/'+str(j)+'.pgm'),dtype='float64')
        f=np.reshape(im,m*n)
        trainingset[:,k]=f
        k=k+1
        j=j+1
    i=i+1
    j=1
    
#faccia media
fm=fmedio(trainingset,m*n,L)

#training set centrato
for i in range (L):
    trainingset[:,i]=trainingset[:,i]-fm
    # ora trainingset è la phi ovvero il trainingset centrato rispetto alla media
    

#matrice di covarianza
Covarianza=np.dot(trainingset,trainingset.T) /L



#autovalori autovettori di C e ordinamento decrescente

autovaloriAppoggio, autovettoriAppoggio = lg.eig(Covarianza)

autovettori=np.zeros((n*m,L))
autovalori=np.zeros(L)
for i in range(L):
    av=np.max(autovaloriAppoggio)
    indice=np.argmax(autovaloriAppoggio)
    autovalori[i]=av
    autovettori[:,i]=autovettoriAppoggio[indice]
    autovaloriAppoggio[indice]=0
    
    
#scelta autovettori
vtot=np.sum(autovalori)
sommaAA=0
Lprimo=0
filtro=0
varianzaTrattenuta=0.80
while filtro<=varianzaTrattenuta:
    sommaAA=sommaAA+autovalori[Lprimo]
    Lprimo=Lprimo+1
    filtro=sommaAA/vtot
# sommo gli autovalori finchè non raggiungo l'80% del valore totale di somma ovvero l'80-esimo percentile 
# seleziono i primi Lprimo valori perchè autovalori è vettore ordinato decrescente
    
#proiezione nel sottospazio
sottospazio=autovettori[:,:Lprimo]
eigenface=np.dot(sottospazio.T, trainingset)
# è matrice Lprimo x L che ha sulle righe le componenti della L esima immagine rispetto agli
# L' elementi del sottospazio
print('training eseguito')
 
import shelve
#questa è la mia matrice
d= shelve.open('trainingmatrix.dat')   #crea il file matrice.dat che conterrà il dizionario
d['sottospazio']= sottospazio
d['eigenface']= eigenface #alla chiave assegno il valore della matrice
d['facciamedia'] = fm
d['lprimo'] = Lprimo
d.close()  #chiudo il file dizionario
print('copia su file eseguita')


#fase di test

#scelgo una soglia
# bound = 0.5
# fnew = np.ones(m*n)
# # trasformo nuova faccia in vettore colonna
# i=1
# while i<=40:
#     j=1
#     for j in np.linspace(nTe,nIm,1):
#         im=np.array(plt.imread('archive/s'+str(i)+'/'+str(j)+'.pgm'),dtype='float64')
#         fnew = np.reshape(im,m*n)
#         print('Test su immagine archive/s'+str(i)+'/'+str(j)+'.pgm')
        
# #centro l'immagine
#         fnew = fnew - fm

# #proietto fnew nel sottospazio
#         fprz=np.dot(sottospazio.T, fnew)

# #ora ho un vettore in cui sono raccolte le N componenti di fnew nella base generatrice dell'autospazio 

# # la minima distanza euclidea coincide con la distanza tra fnew e la sua proiezione nel sottospazio
#         k=0
#         dist=np.zeros((Lprimo))
#         for k in range(Lprimo):
#             dist[k] = np.linalg.norm(eigenface[:,k] - fprz, ord = 2)

#         epsilon = min(dist)

# # controllo soglia di riconoscimento
#         if epsilon < bound:
#             print('Riconoscimento avvenuto: elemento archive/s'+str(i)+'/'+str(j)+'.pgm è una faccia presente nel database')
#             index = np.argmin(dist)
#             sogg = index/nTr
#             if sogg == 0:
#                 sogg = 1
#             elif sogg - int(sogg/nTr) != 0:
#                 sogg = int(sogg/nTr) + 1
#             elif sogg - int(sogg/nTr) < 1e-10:
#                 sogg = int(sogg/nTr)
#                 print('il soggetto riconosciuto è il numero', sogg , 'con immagine più simile tra quelle del training set numero', index - (sogg-1)*nTr +1 )
#         else:
#             print('Riconoscimento errato: elemento archive/s'+str(i)+'/'+str(j)+'.pgm è una faccia non presente nel database')
#     i = i+1