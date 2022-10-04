import numpy as np
import platform
import tempfile
import os
#import matplotlib.pyplot as plt
from scipy import ndimage as ndi
# necessite scikit-image 
from skimage import io as skio
import scipy
import scipy.io
import skimage
import pickle
import scipy.signal as signal
import matplotlib.pyplot as plt 
import skimage.transform
# y=scipy.io.loadmat('aeiou.mat')['s'].reshape(-1)
def affiche(im,normalise=True,MINI=0.0, MAXI=255.0,titre='',printname=False):
    """ Cette fonction fait afficher l'image EN NIVEAUX DE GRIS 
        dans gimp. Si un gimp est deja ouvert il est utilise.
        Par defaut normalise=True. Et dans ce cas l'image est normalisee 
        entre 0 et 255 avant d'être sauvegardee.
        Si normalise=False MINI et MAXI seront mis a 0 et 255 dans l'image resultat
        
    """
    imt=np.float32(im.copy())
    if platform.system()=='Darwin': #on est sous mac
        prephrase='open -a GIMP '
        endphrase=' ' 
    else: #SINON ON SUPPOSE LINUX (si vous avez un windows je ne sais comment faire. Si vous savez dites-moi.)
        prephrase='gimp '
        endphrase= ' &'
    
    if normalise:
        m=imt.min()
        imt=imt-m
        M=imt.max()
        if M>0:
            imt=imt/M

    else:
        imt=(imt-MINI)/(MAXI-MINI)
        imt[imt<0]=0
        imt[imt>1]=1
    
    if titre!='':
        titre='_'+titre+'_'
    titre=titre.replace(' ','')
    nomfichier=tempfile.mktemp('TPIMA'+titre+'.png')
    commande=prephrase +nomfichier+endphrase
    skio.imsave(nomfichier,imt)
    os.system(commande)
    if printname:
        print(nomfichier)
        
sin=np.sin
cos=np.cos
fft=np.fft.fft
ifft=np.fft.ifft
fft2=np.fft.fft2
ifft2=np.fft.ifft2
fftshift=np.fft.fftshift
ifftshift=np.fft.ifftshift
pi=np.pi
log=np.log
real=np.real
conj=np.conj
randn=np.random.randn 
def charge_image(fich,repimages=''):
    a=np.float32(skio.imread(repimages+fich))
    return a

def conv2(im,h,option='same'):
    """ Renvoie la convolution de l'image im avec le noyau h 
     
    par défaut option='same' signifie que l'image renvoyée est de la même taille que im.
    L'option 'full' donne toute la convolution (taille N+M-1 si N et M sont les tailles des entrées)."""
    return signal.convolve2d(im,h,option)
i=np.complex(0,1)
exp=np.exp
angle=np.angle

def norme2(X):
    return ((X**2).sum())**0.5
#%%

#%% SECTION ANALYSE DES VISAGE
visages=np.float32(scipy.io.loadmat('Visages.mat')['fea'])

nb,N=visages.shape #nb = nmbre de visages

# on normalise les visages pour que le max de chaque image soit 1
for k in range(nb):
    visages[k,:]-=visages[k,:].min()
    visages[k,:]/=visages[k,:].max()

Vm=visages.mean(axis=0)
affiche(Vm.reshape((32,32)).T,titre='le_visage_moyen')
#%%

normalise=visages-Vm.reshape((1,-1)) #soustraire le visage moyen
A=normalise.T@normalise # matrice de covariance 
# calcul des vecteurs propres
vals,vecs=np.linalg.eig(A); #vecs= vecteurs propres; vals sont ordonnees de la plus grande a la plus petite

plt.subplot(4,5,1)

for k in range(20):
    plt.subplot(4,5,k+1);
    plt.imshow(vecs[:,k].reshape((32,32)).T,cmap='gray')


plt.show()

#%% On prend 20 visages au hasard et on garde leurs 100 plus grandes composantes 
plt.close('all')
nbc=100 #on garde 100 composantes
K=20 #on prend 20 visages au hasard
idxs=np.random.randint(0,nb,size=(K,))
originaux=normalise[idxs,:]
#on affiche les originaux
#CERTAINES PHOTOS SONT tres sombres, c'est normal
plt.figure()
plt.subplot(4,5,1) #4*5=20, si vous changez K changer 4 et 5
for k in range(K):
    plt.subplot(4,5,k+1)
    plt.imshow((originaux[k,:]+Vm).reshape((32,32)).T,cmap='gray')
plt.subplot(4,5,1)
plt.title('originaux')
plt.show()

recomposes=np.zeros((K,N))
for k in range(K):
    tmp=originaux[k,:]
    decomp=vecs.T@tmp
    decomp[nbc:]=0 #troncature
    tmp=vecs@decomp
    recomposes[k,:]=tmp+Vm

plt.figure()
plt.subplot(4,5,1) #4*5=20, si vous changez K changez 4 et 5
for k in range(K):
    plt.subplot(4,5,k+1)
    plt.imshow(recomposes[k,:].reshape((32,32)).T,cmap='gray')
plt.subplot(4,5,1)
plt.title('recomposes avec '+str(nbc)+' composantes')
plt.show()

#%% LA MEME CHOSE AVEC DU BRUIT
plt.close('all')
nbc=100 #on garde 100 composantes
K=20 #on prend 20 visages au hasard
idxs=np.random.randint(0,nb,size=(K,))
originaux=normalise[idxs,:]
puissance=((originaux**2).sum()/N/K)**0.5
sig=puissance/2 # on va rajouter un bruit comparable a la puissance du signal
# on peut aussi essayer avec nbc=10 et sig=puissance
originaux=originaux+sig*np.random.randn(*originaux.shape)
#on affiche les originaux
#CERTAINES PHOTOS SONT tres sombres, c'est normal
plt.figure()
plt.subplot(4,5,1) #4*5=20, si vous changez K changer 4 et 5
for k in range(K):
    plt.subplot(4,5,k+1)
    plt.imshow((originaux[k,:]+Vm).reshape((32,32)).T,cmap='gray')
plt.subplot(4,5,1)
plt.title('originaux bruités')
plt.show()

recomposes=np.zeros((K,N))
for k in range(K):
    tmp=originaux[k,:]
    decomp=vecs.T@tmp
    decomp[nbc:]=0 #troncature
    tmp=vecs@decomp
    recomposes[k,:]=tmp+Vm

plt.figure()
plt.subplot(4,5,1) #4*5=20, si vous changez K changer 4 et 5
for k in range(K):
    plt.subplot(4,5,k+1)
    plt.imshow(recomposes[k,:].reshape((32,32)).T,cmap='gray')
plt.subplot(4,5,1)
plt.title('recomposes a partir avec '+str(nbc)+' composantes')
plt.show()

#%% FIN SECTIONS ANALYSE DES VISAGE
#%% definition des DCT
def madct(v):
    """ calcule la DCT de toutes les colonnes de v, (axis=0)"""
    n,m=v.shape 
    sym=np.flipud(v)
    vv=np.concatenate((v,sym),axis=0) #symetrisation de toutes les colonnes
    fvv=fft(vv,axis=0)
    # on ne garde que la moitie
    fvv=fvv[:n,:]
    # on construitun vecteur par lequel multiplier la sortie de la TFD
    w=...( ... np.arange(n)/(2*n)) #COMPLETER
    w=w/((2*n)**0.5)
    w[0]=w[0]/... #COMPLETER
    w=w.reshape((-1,1)) #une colonne
    
    #on multilie (une colonne est multipliee par toutes les colonnes de fvv)
    
    fvv=real(fvv*w)
    return fvv

def imadct(v):
    """ calcule la dct inverse des colonnes de v"""
    vnew=v+0*i #passr en complexe
    n,m=vnew.shape
    w=exp(-i*pi*np.arange(n)/(2*n)) 
    w=w/((2*n)**0.5)
    w[0]=w[0]/2**0.5 
    w=w.reshape((-1,1)) #une colonne
    fvv=vnew
    fvv=fvv/w
    fvv=np.concatenate((fvv,np.zeros((1,m))*i,np.conj(np.flipud(fvv[1:,:]))))
    fvv=ifft(fvv,axis=0)
    return real(fvv[:n,:])
    
def madct2(I):
    """ calcule la dct2d d'une image"""
    interm=madct(I)
    return madct(interm.T).T #  .T fait la transposition 
def imadct2(I):
    """ calcule la dct2d inverse"""
    interm=imadct(I)
    return imadct(interm.T).T #  .T fait la transposition 
#%% decroissance dct/fft
plt.close('all')
n=np.arange(1024).reshape((-1,1)) #colonne
dc=abs(madct(n))
dc/=dc.max() #normalisation
plt.plot(dc)
plt.show()
plt.title('DCT')
plt.figure()

ff=abs(fft(n,axis=0))
ff/=ff.max() #normalisation
plt.plot(ff)

plt.title('FFT')
plt.show()

#%%   
def dct2_par_blocks(I,N):
    """calcule la dct par blocks d'une image. La taille de l'image doit etre multiple de N"""
    y,x=I.shape
    out=np.zeros((y,x))
    assert (y%N ==0) and (x % N ==0), "attention la taille de l'image n'est pas multiple de N dans dct_par_blocks"
    for k in range(x//N):
        for l in range(y//N):
            tmp=I[l*N:l*N+N,k*N:k*N+N]
            tmp=madct2(tmp) # on calcule la DCT2 de chaque block
            out[l*N:l*N+N,k*N:k*N+N]=tmp
    return out
   
def fft2_par_blocks(I,N): #UTILE POUR LA QUESTION 4.3
    """calcule la dct par blocks d'une image. La taille de l'image doit etre multiple de N"""
    y,x=I.shape
    out=np.zeros((y,x))+0*i
    assert (y%N ==0) and (x % N ==0), "attention la taille de l'image n'est pas multiple de N dans dct_par_blocks"
    for k in range(x//N):
        for l in range(y//N):
            tmp=I[l*N:l*N+N,k*N:k*N+N]
            tmp=fft2(tmp) # on calcule la DCT2 de chaque block
            out[l*N:l*N+N,k*N:k*N+N]=tmp
    return out

#%%
def affiche_DCT(d,titre=''): # affiche une dct en utilisant le log
    affiche(log(abs(d)+0.0001),titre=titre)
#%%
lena=charge_image('lena.png')
affiche_DCT(dct2_par_blocks(lena,32),titre='dct16x16')
affiche(lena)
#%% 
def compression_lineaire(I,N,m):
    """ Calcule la compression linaire: On garde m coefficient de DCT par block NxN
    ATTENTION LE NOMBRE TOTAL DE COEFFCIENTS GARDES est m*(taille(I)(en pixels)/N^2)
    CELA N'EST PAS LA MEME CHOSE QUE le m de compression_adapatative(I,N,m)
    """
    y,x=I.shape
    assert (y%N ==0) and (x % N ==0), "attention la taille de l'image n'est pas multiple de N dans compression_lineaire"

    #CREATION DU MASK (SOUPE PYTHON)
    X,Y=np.meshgrid(np.arange(N),np.arange(N))
    no=X**2+Y**2
    idxs=np.argsort(no.reshape(-1))
    mask=np.zeros(N*N)
    mask[idxs[:m]]=1
    mask=mask.reshape((N,N))
    
    out=np.zeros((y,x))
    # parcours des blocks 
    for k in range(x//N):
        for l in range(y//N):
            tmp=madct2(I[l*N:l*N+N,k*N:k*N+N])
            tmp=...  # COMPLETER
            tmp=imadct2(tmp)
            out[l*N:l*N+N,k*N:k*N+N]=tmp
    print('Attention nombre total de coefficients gardes = ', m*(x//N)*(y//N))
    return out

#%%
def compression_adaptative(I,N,m):
    """ Calcule la compression adaptative: On garde m coefficient les plus gros 
    de DCT par block NxN
    ATTENTION LE NOMBRE TOTAL DE COEFFCIENTS GARDES est m
    CELA N'EST PAS LA MEME CHOSE QUE le m de compression_lienaire(I,N,m)
    """
    y,x=I.shape
    assert (y%N ==0) and (x % N ==0), "attention la taille de l'image n'est pas multiple de N dans compression_lineaire"

    
    out=np.zeros((y,x))
    d=dct2_par_blocks(I,N) #pour 4.3 modifier 
    s=abs(d).reshape(-1)
    s=np.sort(-s) #ordre croissant
    seuil=-s[...] # COMPLETER (le m-ieme coefficient le plus gros)
    d=d*(abs(d) ... seuil) #COMPLETER (mettre à zéro les plus petits)
    
    # parcours des blocks pour la recomposition de l'image 
    for k in range(x//N):
        for l in range(y//N):
            tmp=d[l*N:l*N+N,k*N:k*N+N]
            tmp=imadct2(tmp)
            out[l*N:l*N+N,k*N:k*N+N]=tmp
    print('Attention nombre total de coefficients gardes = ', m)
    return out

def compression_adaptative_FFT(I,N,m): #UTILE POUR question 4.3
    """ Calcule la compression adaptative: On garde m coefficient les plus gros 
    de DCT par block NxN
    ATTENTION LE NOMBRE TOTAL DE COEFFCIENTS GARDES est m
    CELA N'EST PAS LA MEME CHOSE QUE le m de compression_lineaire(I,N,m)
    """
    y,x=I.shape
    assert (y%N ==0) and (x % N ==0), "attention la taille de l'image n'est pas multiple de N dans compression_lineaire"

    
    out=np.zeros((y,x))
    d=fft2_par_blocks(I,N) 
    s=abs(d).reshape(-1)
    s=np.sort(-s) #ordre croissant
    seuil=-s[...] # COMPLETER le m-ieme coefficient le plus gros
    d=d*(abs(d)...seuil) #COMPLETER
    
    # parcours des blocks pour la recomposition de l'image 
    for k in range(x//N):
        for l in range(y//N):
            tmp=d[l*N:l*N+N,k*N:k*N+N]
            tmp=ifft2(tmp) 
            out[l*N:l*N+N,k*N:k*N+N]=tmp
    print('Attention nombre total de coefficients gardes = ', m)
    return out



#%% atteindre une precision egale a jpg avec une compression donnee
lenajpg=charge_image('lena.jpg')
erreurjpg=norme2(lena-lenajpg)
for m in range(20000,30000,1000):
    ca=compression_lineaire(lena,512,m)
    erreurca=norme2(lena-ca)
    if erreurca>erreurjpg:
        print('pour ',m,' coefficients gardes, l\'erreur est encore trop grande (erreur compressee=',erreurca,' erreurjpg=',erreurjpg,')')
    else:
        print('pour ',m,' coefficients gardes, l\'erreur est plus faible que le jpg (erreur compressee=',erreurca,' erreurjpg=',erreurjpg,')')
    
#%%
cl=compression_lineaire(lena,16,5)
ca=compression_adaptative(lena,16,5120)

affiche(cl,titre='LINEAIRE')
affiche(ca,titre='ADAPTATIVE')
#%%
affiche_DCT(dct2_par_blocks(cl,16))
affiche_DCT(dct2_par_blocks(ca,16))


#%%
cafft=compression_adaptative_FFT(lena,16,5120)
affiche(cafft,titre='ADAPTATIVEFFT')
#%% PARTIE SUR LE BRUIT et la COMPRESSIBILITE
plt.close('all')
bruit=randn(512,512)
bdct=dct2_par_blocks(bruit,8)
s=np.sort(-abs(bdct).reshape(-1))
plt.plot(np.arange(bruit.shape[0]*bruit.shape[1]), np.cumsum(s**2)/(s**2).sum()) # augmentation du carré de l'énergie
plt.title('augmentation de l\'énergie dans une image de bruit')
plt.show()

lenadct8=dct2_par_blocks(lena,8)
s=np.sort(-abs(lenadct8).reshape(-1))
plt.figure()
plt.plot(np.arange(bruit.shape[0]*bruit.shape[1]),np.cumsum(s**2)/(s**2).sum()) # augmentation du carré de l'énergie
plt.title('augmentation de l\'énergie dans une image naturelle')
plt.show()

#%% LA COMPRESSION COMME DEBRUITAGE
lenabruit=lena+20*randn(512,512)
affiche(lenabruit)
affiche(compression_adaptative(lenabruit,16,3000))


ms=np.arange(2000,10000,500)
erreurs=np.zeros(len(ms))

for k,m in enumerate(ms):
    erreurs[k]=norme2(lena-compression_adaptative(lenabruit,16,m)) 
plt.plot(ms,erreurs)
plt.show()

#%% Revers engineering de JPEG
plt.close('all')
lena=charge_image('lena.png')
lenajpg=charge_image('lena.jpg')
tr=dct2_par_blocks(lena,8)
trjpg=dct2_par_blocks(lenajpg,8)
plt.plot(abs(tr.reshape(-1)),abs(trjpg.reshape(-1)))
plt.show()
print('nombre de composantes nulles dans trjpg',(abs(trjpg)<0.0001).sum())
#%% quantification des differents canaux de la dct dans une image compressee en JPG
plt.close('all')
for canaly in range(3):
    for canalx in range(3):
        sous_canal_jpg=trjpg[canaly::8,canalx::8]
        sous_canal_original=tr[canaly::8,canalx::8]
        plt.figure()
        plt.plot(abs(sous_canal_original).reshape(-1),abs(sous_canal_jpg).reshape(-1),'+')
        plt.title('quantification du canal '+str((canaly,canalx)))
        plt.show()
        
