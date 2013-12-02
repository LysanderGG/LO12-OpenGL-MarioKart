OpenGL et GLUT sont des bibliothèques multi-plateformes, c'est à dire que vos
programmes seront compilables et utilisables sur un grand nombre de systèmes
d'exploitation. Vous trouverez dans ce document une aide à la mise en place de
plateforme de programmation OpenGL pour les systèmes courants.

==============================
=== Toutes les Plateformes ===
==============================

== Utilisation de OpenGL ==

= Dans les fichiers sources = 

Vos fichiers sources *.c ou *.cpp doivent contenir les fichiers d'entête
suivants:

#if defined(__APPLE__) && defined(__MACH__)   
        #include <GLUT/glut.h>
        #include <OpenGL/gl.h>
        #include <OpenGL/glu.h>
#else
        #include <GL/glut.h>
        #include <GL/gl.h>
        #include <GL/glu.h>
#endif

Attention la case (minuscule/majuscule) est importante. 


===============
=== Windows ===
===============

== Dev CPP ==

Dev CPP est un environnement de programmation libre (gratuit et redistribuable)
pour Windows. Il regroupe les outils de la chaîne de programmation GNU à
travers une interface graphique intuitive. Il est léger (60 Mo) et facile à
installer grâce à un système de paquetage intégré.

= Mise en place de la plateforme =

1) Récupérer le fichier d'installation Dev-CPP et le paquet GLUT DevPak.
	Sites Officiels (version à jour):
		http://bloodshed.net/dev/devcpp.html
		http://www.nigels.com/glt/devpak/
	Site UTC (version courante au 24/08/05):
		http://wwwcyg.utc.fr/lo12/Ressources/s
2) Installer Dev CPP
	Choisir une installation complète "Full"
	Ne pas changer le répertoire d'installation: "c:\Dev-CPP"		
3) Installer le paquet GLUT
	Simplement, double-cliquer sur le fichier et l'installateur de paquets de
	Dev-Cpp s'ouvrira.
	A la fin de l'installation, fermer l'installateur de paquets.
4) Configuration lors du premier démarrage
	Choisir sa langue
	Autoriser l'autocomplétion et le navigateur de classes
		"Yes, I want to use this feature"
		"Yes, create the cache now" (ceci prend un peu de temps)
	Ajouter au cache les fichiers d'entête OpenGL
		Sélectionner le menu "Outils" puis "Options de l'éditeur"
		Sélectionner "Explorateur de Classes" en haut dans la boîte de dialogue, puis
		"Complétion"
		Cliquer sur "Ajouter" et Sélectionner les fichiers gl.h glext.h glu.h et
			glut.h 		
		dans le répertoire C:\Dev-Cpp\include\GL
		Cliquer sur "OK"
		
= Utilisation de OpenGL =

Vos fichiers sources *.c ou *.cpp doivent obligatoirement contenir les fichiers
d'entête suivants:
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

1) Créer un nouveau projet:
	Cliquer sur "Fichier" > "Nouveau" > "Projet"
	Sélectionner "Empty Project"
	Donner un nom au projet
	Sélectionner le langage adéquat
	Indiquer un emplacement pour enregistrer le fichier de configuration du projet. 
		Il est conseillé d'enregistrer ce fichier dans le même répertoire que vos 
		sources.
2) Ajouter vos sources au projet:
	Cliquer sur "Projet" > "Ajouter au projet"
	Sélectionner vos fichiers sources, la sélection multiple est possible en 		
		cliquant avec la touche contrôle ou majuscule enfoncée.
3) Configurer l'éditeur de lien pour qu'il prenne en compte les bibliothèques    
   OpenGL et GLUT:
	Cliquer sur "Projet" > "Option du projet"
	Sélectionner "Paramètres" en haut de la boîte de dialogue
	Dans le cadre "Éditeur de lien", ajouter les lignes suivantes, l'ordre est 
	important:
		-lglut32
		-lglu32
		-lopengl32
		-lwinmm
		-lgdi32
4) Compiler votre programme:
	Cliquer sur "Exécuter">"Compiler"
5) Exécuter votre programme:
	Cliquer sur "Exécuter">"Exécuter"

= Compilation et exécution des projets LO12 =

Les projets LO12 sont livrés avec un fichier de configuration. Ouvrir ce
fichier avec Dev-Cpp et passer aux étapes 4 et 5 du paragraphe précédent.


== Microsoft Visual Studio ==

= Mise en place de la plateforme =

= Utilisation de OpenGL =

= Compilation et exécution des projets LO12 =



===============
=== MacOS X ===
===============

== Mise en place de la plateforme ==

Installer le meta paquet Developer.mpkg présent sur le DVD ou sur le dernier CD
d'installation du système d'exploitation. Le programme "Mac OS X Xcode tools"
est l'ensemble des outils nécessaires à la programmation sous ce système. 

== Utilisation de OpenGL ==

= Dans les fichiers sources = 

Vos fichiers sources *.c ou *.cpp doivent obligatoirement contenir les fichiers
d'entête suivants:
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

= Compilation  =

La compilation s'effectue classiquement par:
$ gcc -c -o monfichier.o monfichier.c

= Liaison =

La liaison (création du fichier exécutable) s'effectue par:
$ gcc -o monfichier monfichier.o -framework AGL -framework GLUT -framework
	OpenGL -framework CoreServices -lobjc


== Compilation et exécution des projets LO12 ==

Les projets LO12 sont livrés avec un script de compilation Makefile. Il
s'utilise dans la console.
Placer vous dans votre répertoire de travail à l'aide de la commande cd.

Pour compiler le programme, faire:
$ make -f Makefile.macosx all

Pour éxécuter le programme, taper:
$ ./tp1

Pour nettoyer le projet, exécuter:
$ make -f Makefile.macosx clean

Les scripts Makefile permettent d'automatiser la compilation des projets, si
vous devez programmer dans un environnement POSIX il est conseillé d'apprendre
leur conception. Vous pouvez regarder le fichier Makefile.macosx pour avoir un
exemple de conception.


==================
=== GNU/Linux  ===
==================

== Mise en place de la plateforme ==

Vous devez installer au minimum les programmes gcc et make de la chaîne de
programmation GNU.
Vous devez installer les paquets xlibmesa-gl xlibmesa-gl-dev xlibmesa-glu
xlibmesa-glu-dev libglut3 libglut3-dev.

Sous Debian GNU/Linux, pour se faire, lancer la commande suivante:
$ apt-get install gcc make xlibmesa-gl xlibmesa-gl-dev xlibmesa-glu
	xlibmesa-glu-dev libglut3 libglut3-dev 

== Utilisation de OpenGL ==

= Dans les fichiers sources = 

Vos fichiers sources *.c ou *.cpp doivent obligatoirement contenir les fichiers
d'entête suivants:
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

= Compilation  =

La compilation s'effectue classiquement par:
$ gcc -c -o monfichier.o monfichier.c

= Liaison =

La liaison (création du fichier exécutable) s'effectue par:
$ gcc -o monfichier monfichier.o -lGLU -lGL -lglut


== Compilation et exécution des projets LO12 ==

Les projets LO12 sont livrés avec un script de compilation Makefile. Il
s'utilise dans la console.
Placer vous dans votre répertoire de travail à l'aide de la commande cd.

Pour compiler le programme, faire:
$ make -f Makefile.unix all

Pour éxécuter le programme, taper:
$ ./tp1

Pour nettoyer le projet, exécuter:
$ make -f Makefile.unix clean

Les scripts Makefile permettent d'automatiser la compilation des projets, si
vous devez programmer dans un environnement POSIX il est conseillé d'apprendre
leur conception. Vous pouvez regarder le fichier Makefile.unix pour avoir un
exemple de conception.
