import numpy as np
import platform
import tempfile
import os
#import matplotlib.pyplot as plt
from scipy import ndimage as ndi
# necessite scikit-image 
from skimage import io as skio

import skimage
import pickle
import scipy.signal as signal

import skimage.transform

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
fft2=np.fft.fft2
ifft2=np.fft.ifft2
fftshift=np.fft.fftshift
ifftshift=np.fft.ifftshift
pi=np.pi
log=np.log
real=np.real
conj=np.conj 
def conv2(im,h,option='same'):
    """ Renvoie la convolution de l'image im avec le noyau h 
     
    par défaut option='same' signifie que l'image renvoyée est de la même taille que im.
    L'option 'full' donne toute la convolution (taille N+M-1 si N et M sont les tailles des entrées)."""
    return signal.convolve2d(im,h,option)
i=np.complex(0,1)
exp=np.exp
angle=np.angle
#%%
        
repimages='' # A ADAPTER
a=np.float32(skio.imread(repimages+'maison.tif'))
affiche(a)

#%% Poru faire la conversion float automatiquement
def charge_image(fich):
    a=np.float32(skio.imread(repimages+fich))
    return a


#%%
x= np.arange(0,256).reshape((-1,1)); y= 2*pi*(8/256)*np.ones((1,256));
a= sin(x*y) 

#%%
def affiche_spectre(a):
    A=abs(fft2(a))
    A=ifftshift(A)
    affiche(A)

affiche_spectre(a)

#%%
xx= 2*pi*(24/256)*np.ones((256,1)); yy= np.arange(0,256).reshape((1,-1));
a= sin(xx*yy);
affiche_spectre(a)

#%%
a=sin(x*y+xx*yy)
affiche_spectre(a)

#%%
a=np.zeros((256,256));a[62:191,62:191]=1;
affiche_spectre(a)

#%%

a=a*sin(x*y);
affiche_spectre(a)

#%% affichage spectre en log/ sans log
a=charge_image('maison.tif')
A=fft2(a); A=fftshift(A);
affiche_spectre(a)
affiche(log(0.1+abs(A)))
#%%

def affiche_spectre_log(a):
    A=abs(fft2(a))
    A=ifftshift(A)
    affiche(log(0.1+A))

#%%
a=charge_image('carte.gif')
affiche(a)
#%%
b=a[::2,::2]
affiche(b)
#%%
affiche_spectre_log(a)
#%%
affiche_spectre_log(b)
#%%
c=conv2(a,np.ones((3,3))/9,'same'); #conv2 effectue la convolution bidimensionnelle 
affiche(c[::2,::2])

#%%
a=charge_image('chapeau.tif')
b=charge_image('maison.tif')
A=fft2(a); B=fft2(b);
Ap=exp(i*angle(A))*abs(B); #module de la TFD de b et phase de A
Bp=exp(i*angle(B))*abs(A);
ap=real(ifft2(Ap)); affiche(ap,titre='module_b_phase_a');
#%%
bp=real(ifft2(Bp)); affiche(bp,titre='module_a_phase_b');

#%% Synthèse de teture
a=charge_image('bois.tif')
(m,n)=a.shape
b=np.random.randn(m,n)
A=fft2(a)
B=fft2(b)
C=abs(A)*exp(i*angle(B))
c=real(ifft2(C))
affiche(c, titre='synthese')
#%%
affiche(a,titre='originale')

#%% Filtrage passe-haut
a=charge_image('maison.tif')
fx=np.asarray([[0 ,0, 0],[-1 ,1, 0],[ 0 ,0, 0]])
fy=fx.T
gx=conv2(a,fx) # conv2 effectue une convolution bi-dimensionnelle 
gy=conv2(a,fy)
g=(gx**2+gy**2)**0.5
affiche(g)

#%%
conv2(fm,fx,option='full') #Q4.1
#%% UNsharp mask
a=charge_image('flou.jpg');
laplace=np.ones((3,3)); laplace[::2,::2]=0; laplace[1,1]=-4; #filtre laplacien discret
dirac=np.zeros((3,3)); dirac[1,1]=1; #un dirac sur un support 3x3
alpha=3#taux de mélange
b=conv2(a,dirac-alpha*laplace); 
affiche(a,titre='orig',normalise=False);
#%%
affiche(b,titre='net',normalise=False); #certaines valeurs vont dépasser 255 car la RI n'est pas positive 
#%%
def appareil_photo(im,dy,dx):
    """Une fonction qui simule un appareil photo"""
    sousech=2
    h=np.ones((3,3))/9
    (nl,nc)=im.shape
    flou=real(ifft2(fft2(h,s=(nl,nc))*fft2(im)))
    out=flou[dy::sousech,dx::sousech]
    return out

def zoomimage(im):
    """ zoom une image d'un facteur 2"""
    (m,n)=im.shape
    out=np.zeros((2*m,2*n))
    out[::2,::2]=im
    out[1:-1:2,::2]=1/2*(out[:-2:2,::2]+out[2::2,::2]) #moyenne en y pour trouver les pixels du type (2k+1,2l)
    out[::2,1:-1:2]=1/2*(out[::2,:-2:2]+out[::2,2::2])
    out[1:-1:2,1:-1:2]=1/4*(out[:-2:2,2::2]+out[2::2,:-2:2]+out[2::2,2::2]+out[:-2:2,:-2:2]) # pixels (2k+1,2l+1)
    return out
def trouve_decalage(i1,i2):
    """ fonction qui trouve le decalage entre deux images sous-echantillnnees"""
    im1=zoomimage(i1)
    im2=zoomimage(i2)
    fim1=fft2(im1);
    fim2=fft2(im2);
    corr=real(ifft2(fim1*conj(fim2)))  # que serait le résultat sans conj( )?
    dx=corr.max(axis=0).argmax()
    dy=corr.max(axis=1).argmax()
    return (dy,dx)

def recalage(capte1,capte2,capte3,capte4):
    (m,n)=capte1.shape
    sortie=np.zeros((2*m,2*n))
    sortie[::2,::2]=capte1
    (dy2,dx2)=trouve_decalage(capte1,capte2)
    print("decalage 2:",dy2,dx2)
    sortie[dy2::2,dx2::2]=capte2
    (dy3,dx3)=trouve_decalage(capte1,capte3)
    print("decalage 3:",dy3,dx3)
    sortie[dy3::2,dx3::2]=capte3
    (dy4,dx4)=trouve_decalage(capte1,capte4)
    print("decalage 4:",dy4,dx4)
    sortie[dy4::2,dx4::2]=capte4
    return sortie
def defloute(re):
    """defloute la sortie de recalage"""
    h=1/9*np.ones((3,3)) #ATTENTION C'EST FAUX (voir question Q5.1)
    fh=fft2(h,s=(re.shape))
    fre=fft2(re)
    fdefloute=fre/fh
    return real(ifft2(fdefloute))
#%% Lecture des images qui ont été prise grace a appareil photo (se mettre dans le repertoire des donnees)
f=open('donnees_super.pickle','rb')
(capte1,capte2,capte3,capte4)=pickle.load(f)
f.close()

affiche (capte1,titre='image_captee') # une image parmi celles captees
reconstruction=recalage(capte1,capte2,capte3,capte4)
affiche(reconstruction,titre='reconstruction_brute')
affiche(defloute(reconstruction),titre='defloutee')
