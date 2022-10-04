#%% SECTION de definition et d'imports a lancer au debut
import numpy as np
import soundfile as sf

import matplotlib.pyplot as plt
import scipy 
import scipy.io
import platform
import time

import tempfile
import os

if platform.system()=='Darwin': #MAC 
    import sounddevice as sd
    
def play(y,Fe=44100):
    z=np.real(y)/(abs(np.real(y)).max())
    if platform.system()=='Darwin': #MAC (sous linux sounddevice a un comportement erratique)
        sd.play(z,Fe)
        return
    fichier=tempfile.mktemp()+'SONTPOASIS.wav'
    sec=len(y)/Fe
    if sec<=20:
        rep=True
    if sec>20:
        print ('Vous allez creer un fichier son de plus de 20 secondes.')
        rep=None
        while rep is None:
            x=input('Voulez-vous continuer? (o/n)')
            if x=='o':
                rep=True
            if x=='n':
                rep=False
            if rep is None:
                print ('Repondre par o ou n, merci. ')
    if rep:
        sf.write(fichier,z,Fe)
        os.system('/usr/bin/play '+fichier+' &')



#%% quelques simplifications de fonctions usuelles
exp=np.exp
cos=np.cos

fft=np.fft.fft
ifft=np.fft.ifft
real=np.real

plot=plt.plot
stem=plt.stem
show=plt.show # force l'affichage du graphique courant
i=np.complex(0,1)
pi=np.pi 

#%% UNE ONDE ET SA FFT

nu=0.123
n=np.arange(0,100)
onde=np.exp(2*i*pi*nu*n) # est-ce vraiment une onde de Fourier de la TFD?
stem(n,abs(fft(onde)))
show()
stem(n,abs(fft(real(onde))))

#%% DEUXIEME ONDE ET SA FFT

nu=0.1
n=np.arange(0,100)
onde=np.exp(2*i*pi*nu*n) # est-ce vraiment une onde de Fourier de la TFD?
stem(n,abs(fft(onde)))
show()
stem(n,abs(fft(real(onde))))
#%%
a=np.asarray([1,2,3])
b=np.asarray([1,1,0])
c=np.asarray([4,3,5]) #COMPLETER convolution des deux a calculer a la main (convolution circulaire taille 3)
n=np.arange(0,3)
stem(n,abs(fft(c)))
show()
fft(a)*fft(b)-fft(c)
#%%
a=np.array([1,2,3,4])
b=np.array([1,-2,3,-4])
n=np.arange(0,4)
stem(n,abs(fft(a)))
show()
stem(n,abs(fft(b)))
#%%
ifft(fft([1,1,1]))
ifft(fft([1,1,1,0]))
ifft(fft([1,2,3])*fft([1,1,0]))-[4,3,5]
#%%
h=np.asarray([1,2,3])
g=np.asarray([1,2,1])
321*121
N=max(len(h),len(g))
hz=np.concatenate((h, np.zeros(N))) #COMPLETER
gz=np.concatenate((g, np.zeros(N))) #COMPLETER
out=real(ifft(fft(hz)*fft(gz)))
out=out[0:len(h)+len(g)-1]

def convolution_rapide(g,h):
    N=max(len(g),len(h)) 
    hz=np.concatenate((h,np.zeros(N))) # COMPLETER
    gz=np.concatenate((g,np.zeros(N))) #COMPLETER
    result=real(ifft(fft(hz)*fft(gz)))
    return result

h=np.random.randn(10000)
g=np.random.randn(10000)
t0=time.time()         
res=convolution_rapide(h,g)
print ('temps rapide=',time.time()-t0)

t0=time.time()         
res=scipy.convolve(h,g)
print ('temps normal=',time.time()-t0)

#%%
Fe=44000 ;Te=1/Fe ; #echantillonnage a 1000 Hz 
t=np.arange(0,1500)*Te# t represente le temps en secondes de 300 echantillons

xi=440; # frequence de l'onde 
h=cos(2*pi*xi*t) # equiv. cos(2*pi*(xi/Fe)*np.arange(0,300))
fh=fft(h)
fh=fh[0:1500]
plot(np.arange(0,1500)/1500*Fe,abs(fh))

#%% lecture de données sauvegardées au format matlab LECTURE AEIOU.MAT
y=scipy.io.loadmat('aeiou.mat')['s'].reshape(-1)
Fe=22050

#%%

def une_colonne_spectrogramme(u,M,N,n):
    """ 
    Renvoie une colonne de spectrogramme c'est a dire la TFD de taille M
    d'une morceau de u debutant en n et de taille N multiplie par une fenetre
     de hamming """
    uu=u.copy().reshape(-1)  # on recopie sinon on risque d'ecraser 
    # construction de la fnetre
    idx=np.arange(0,N)
    w=0.54-0.46*cos(2*pi*idx/(N-1))
    # les index tels que u(m)w(n-m) non nul
    m=np.arange(n,n+M).astype(np.int)#COMPLETER (jusqu'au ou?)
    morceauu=uu[m]  #mrceau utile de u
    fenu=morceauu*w  #COMPLETER (quelle opération?)
    Uc=fft(fenu,M) # on calcule la TFD 
    Uc=abs(Uc) # on s'interesse seulement au module
    Uc=Uc[0:M//2+1] # pour un signal reel il suffit de garder la moitie
    return Uc 
#%%   
#%%
def affiche_spectrogramme(u,N,M=None,nb=None,Fe=8192):
    """Affiche le specrogramme du signal u
     La taille des fenetres est N
     Si M n'est pas fourni il est pris egal à N
     nb est le pas entre deux fenetres dont on calcule la TFD 
     si nb n'est pas fourni, nb est pris egal a N/2"""
    
    if M is None:
        M=N
    if nb is None:
        nb=N/2
    # On cmmence par creer un tableau de la bonne taille don les colonnes seront
    # calculees par une_colonne_spectrogramme
    uu=u.copy().reshape(-1)
    L=len(u)
    nombre_fen=int((L-N)//nb+1)
    spectro=np.zeros((M//2+1,nombre_fen))
    for k in range(nombre_fen):
        spectro[:,k]=une_colonne_spectrogramme(u,M,N,k*nb)
    temps_debut=0
    temps_fin=nombre_fen/Fe #COMPLETER
    freq_debut=0
    freq_fin=Fe/2 # COMPLETER
    
    # ci-dessous de la cuisine python pour un affichage d'une image deux fois 
    # pus large que haute
    fig, ax = plt.subplots(figsize=(6,6))
    ax.imshow(np.log(np.flipud(spectro)), interpolation='none', # cmap=plt.cm.Reds
              extent=[temps_debut,temps_fin,freq_debut,freq_fin])
    ax.set_aspect(1/2*(temps_fin/freq_fin))
    return spectro
    
#%%
affiche_spectrogramme(y,2048,M=2048,nb=1024,Fe=Fe)       
#%%
Fe=8000
t=np.arange(0,2*Fe)/Fe # le temps en secondes
s=cos(2*pi* (t**2)* ???) #COMPLETER
affiche_spectrogramme(s,2048,nb=128,Fe=Fe)
#%%
affiche_spectrogramme(s[::2],2048,nb=128,Fe=Fe/2)
