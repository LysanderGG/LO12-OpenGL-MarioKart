OpenGL et GLUT sont des biblioth�ques multi-plateformes, c'est � dire que vos
programmes seront compilables et utilisables sur un grand nombre de syst�mes
d'exploitation. Vous trouverez dans ce document une aide � la mise en place de
plateforme de programmation OpenGL pour les syst�mes courants.

==============================
=== Toutes les Plateformes ===
==============================

== Utilisation de OpenGL ==

= Dans les fichiers sources = 

Vos fichiers sources *.c ou *.cpp doivent contenir les fichiers d'ent�te
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
pour Windows. Il regroupe les outils de la cha�ne de programmation GNU �
travers une interface graphique intuitive. Il est l�ger (60 Mo) et facile �
installer gr�ce � un syst�me de paquetage int�gr�.

= Mise en place de la plateforme =

1) R�cup�rer le fichier d'installation Dev-CPP et le paquet GLUT DevPak.
	Sites Officiels (version � jour):
		http://bloodshed.net/dev/devcpp.html
		http://www.nigels.com/glt/devpak/
	Site UTC (version courante au 24/08/05):
		http://wwwcyg.utc.fr/lo12/Ressources/s
2) Installer Dev CPP
	Choisir une installation compl�te "Full"
	Ne pas changer le r�pertoire d'installation: "c:\Dev-CPP"		
3) Installer le paquet GLUT
	Simplement, double-cliquer sur le fichier et l'installateur de paquets de
	Dev-Cpp s'ouvrira.
	A la fin de l'installation, fermer l'installateur de paquets.
4) Configuration lors du premier d�marrage
	Choisir sa langue
	Autoriser l'autocompl�tion et le navigateur de classes
		"Yes, I want to use this feature"
		"Yes, create the cache now" (ceci prend un peu de temps)
	Ajouter au cache les fichiers d'ent�te OpenGL
		S�lectionner le menu "Outils" puis "Options de l'�diteur"
		S�lectionner "Explorateur de Classes" en haut dans la bo�te de dialogue, puis
		"Compl�tion"
		Cliquer sur "Ajouter" et S�lectionner les fichiers gl.h glext.h glu.h et
			glut.h 		
		dans le r�pertoire C:\Dev-Cpp\include\GL
		Cliquer sur "OK"
		
= Utilisation de OpenGL =

Vos fichiers sources *.c ou *.cpp doivent obligatoirement contenir les fichiers
d'ent�te suivants:
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

1) Cr�er un nouveau projet:
	Cliquer sur "Fichier" > "Nouveau" > "Projet"
	S�lectionner "Empty Project"
	Donner un nom au projet
	S�lectionner le langage ad�quat
	Indiquer un emplacement pour enregistrer le fichier de configuration du projet. 
		Il est conseill� d'enregistrer ce fichier dans le m�me r�pertoire que vos 
		sources.
2) Ajouter vos sources au projet:
	Cliquer sur "Projet" > "Ajouter au projet"
	S�lectionner vos fichiers sources, la s�lection multiple est possible en 		
		cliquant avec la touche contr�le ou majuscule enfonc�e.
3) Configurer l'�diteur de lien pour qu'il prenne en compte les biblioth�ques    
   OpenGL et GLUT:
	Cliquer sur "Projet" > "Option du projet"
	S�lectionner "Param�tres" en haut de la bo�te de dialogue
	Dans le cadre "�diteur de lien", ajouter les lignes suivantes, l'ordre est 
	important:
		-lglut32
		-lglu32
		-lopengl32
		-lwinmm
		-lgdi32
4) Compiler votre programme:
	Cliquer sur "Ex�cuter">"Compiler"
5) Ex�cuter votre programme:
	Cliquer sur "Ex�cuter">"Ex�cuter"

= Compilation et ex�cution des projets LO12 =

Les projets LO12 sont livr�s avec un fichier de configuration. Ouvrir ce
fichier avec Dev-Cpp et passer aux �tapes 4 et 5 du paragraphe pr�c�dent.


== Microsoft Visual Studio ==

= Mise en place de la plateforme =

= Utilisation de OpenGL =

= Compilation et ex�cution des projets LO12 =



===============
=== MacOS X ===
===============

== Mise en place de la plateforme ==

Installer le meta paquet Developer.mpkg pr�sent sur le DVD ou sur le dernier CD
d'installation du syst�me d'exploitation. Le programme "Mac OS X Xcode tools"
est l'ensemble des outils n�cessaires � la programmation sous ce syst�me. 

== Utilisation de OpenGL ==

= Dans les fichiers sources = 

Vos fichiers sources *.c ou *.cpp doivent obligatoirement contenir les fichiers
d'ent�te suivants:
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

= Compilation  =

La compilation s'effectue classiquement par:
$ gcc -c -o monfichier.o monfichier.c

= Liaison =

La liaison (cr�ation du fichier ex�cutable) s'effectue par:
$ gcc -o monfichier monfichier.o -framework AGL -framework GLUT -framework
	OpenGL -framework CoreServices -lobjc


== Compilation et ex�cution des projets LO12 ==

Les projets LO12 sont livr�s avec un script de compilation Makefile. Il
s'utilise dans la console.
Placer vous dans votre r�pertoire de travail � l'aide de la commande cd.

Pour compiler le programme, faire:
$ make -f Makefile.macosx all

Pour �x�cuter le programme, taper:
$ ./tp1

Pour nettoyer le projet, ex�cuter:
$ make -f Makefile.macosx clean

Les scripts Makefile permettent d'automatiser la compilation des projets, si
vous devez programmer dans un environnement POSIX il est conseill� d'apprendre
leur conception. Vous pouvez regarder le fichier Makefile.macosx pour avoir un
exemple de conception.


==================
=== GNU/Linux  ===
==================

== Mise en place de la plateforme ==

Vous devez installer au minimum les programmes gcc et make de la cha�ne de
programmation GNU.
Vous devez installer les paquets xlibmesa-gl xlibmesa-gl-dev xlibmesa-glu
xlibmesa-glu-dev libglut3 libglut3-dev.

Sous Debian GNU/Linux, pour se faire, lancer la commande suivante:
$ apt-get install gcc make xlibmesa-gl xlibmesa-gl-dev xlibmesa-glu
	xlibmesa-glu-dev libglut3 libglut3-dev 

== Utilisation de OpenGL ==

= Dans les fichiers sources = 

Vos fichiers sources *.c ou *.cpp doivent obligatoirement contenir les fichiers
d'ent�te suivants:
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

= Compilation  =

La compilation s'effectue classiquement par:
$ gcc -c -o monfichier.o monfichier.c

= Liaison =

La liaison (cr�ation du fichier ex�cutable) s'effectue par:
$ gcc -o monfichier monfichier.o -lGLU -lGL -lglut


== Compilation et ex�cution des projets LO12 ==

Les projets LO12 sont livr�s avec un script de compilation Makefile. Il
s'utilise dans la console.
Placer vous dans votre r�pertoire de travail � l'aide de la commande cd.

Pour compiler le programme, faire:
$ make -f Makefile.unix all

Pour �x�cuter le programme, taper:
$ ./tp1

Pour nettoyer le projet, ex�cuter:
$ make -f Makefile.unix clean

Les scripts Makefile permettent d'automatiser la compilation des projets, si
vous devez programmer dans un environnement POSIX il est conseill� d'apprendre
leur conception. Vous pouvez regarder le fichier Makefile.unix pour avoir un
exemple de conception.
