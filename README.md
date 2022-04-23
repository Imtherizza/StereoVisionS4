Il y a plusieurs programmes fonctionnnant sur plusieurs plateformes : le PC, la raspberry compute module 3 

Compilation : 

--> Si 1ere utilisation d'un programme, se placer dans le répertoire release et suppprimer tous les fichiers puis click droit -> ouvrir dans terminale  (sur raspberry se placer dans le dossier et F4)
--> taper la commande $ cmake .. puis build le programme avec $ make à chaque changement du code

Possibile erreur concernant la bibliothèque boost qui sera peut-être nécessaire de télécharger
Pour le make, il peut y avoir des erreurs un peu  étranges, liés au fait que le programme ne trouve pas les images. Il faudra vérifier les chemins vers les doissiers ou le nom des images définis dans le code pour l'ensemble des programmes

-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

SUR PC : 3 programmes

***stereo_pics*** : permet de calibrer les caméra, prendre des photos et réaliser la stéréovision sur une scène fixe

1)Compiler
2)En se placant dans le répertoire "release" lancer le programme en tapant $ ./stereo_pics

Prise de photo :
// taper 'y' pour sauter l'étape de calibration, sinon taper 'n'
// taper 's' pour prendre les images à traiter de l'image de droite et de gauche dans le dossier /data/Image sous le nom "imageD.jpg" et "imageG.jpg"
// taper 'c' pour prendre une photo pour la calibration, sauve avec incrémentation l'image de droite et de gauche dans les dossier calib_image_droite et calib_image_gauche et met à jour stereo_calib.xml
(prendre au  minimum 9 images sous différents angles) 
//taper 'q' pour passer à la suite

Calibration :
//le programme prends les photos de calibration faites en suivant la liste générée du fichier stereo_calib.xml
//taper 'q' pour passer à la suite
//il est possible de changer les parametres de l'échiquier à detecter dans le code
//génere les fichiers de calibration intrinsecs.yml et extrinsecs.yml

Stereo matching :
//L'algo est de la SGBM mais possibilité de changer en BM ou autre dans le code
//Changer les parametre de la sgbm dans le code si résultats non satisfaisant: blocksize, uniquenessRatio, minDisparity etc.


***stereo_match_vid***: permet de lire une video .mp4 et de réaliser une stéréovision

1)Compiler
2)En se placant dans le répertoire "release" lancer le programme en tapant $ ./stereo_match

//lit 
//changer la video a lire dans le code en changeant le chemin 
//taper 'echap' pour quitter


***stereo_flux*** : permet de calibrer les caméras, prendre des photos et réaliser la stéréovision sur une scène en temps réel

1)Compiler
2)En se placant dans le répertoire "release" lancer le programme en tapant $ ./stereo_pics    <--dailleurs dans le future, changer le nom en stereo_flux dans le CMakeList

Si la calibration a déjà été effectuée avec stereo_pics, remplacer les fichiers intrinsecs.yml et extrinsecs.yml par ceux de stereo_pics et skip la calib

Prise de photo :
// taper 'y' pour sauter l'étape de calibration, sinon taper 'n'
// taper 's' pour prendre les images à traiter de l'image de droite et de gauche dans le dossier /data/Image sous le nom "imageD.jpg" et "imageG.jpg"
// taper 'c' pour prendre une photo pour la calibration, sauve avec incrémentation l'image de droite et de gauche dans les dossier calib_image_droite et calib_image_gauche et met à jour stereo_calib.xml
(prendre au  minimum 9 images sous différents angles) 
//taper 'q' pour passer à la suite

Calibration :
//le programme prends les photos de calibrations faites en suivant la liste générée du fichier stereo_calib.xml
//taper 'q' pour passer à la suite
//il est possible de changer les parametres de l'échiquier à detecter dans le code
//génere les fichiers de calibration intrinsecs.yml et extrinsecs.yml

Stereo matching :
//Ici l'algo est de la BM mais possibilité de changer en SGBM en remplacant entièrement le contenu de my_functions.cpp par le contenu de my_functions.sgbm
//Changer les parametre de la bm u sgbm dans le code si résultats non satisfaisant
//taper 'echap' pour quitter


***stereo_calib*** : permet de régler les paramètre de la sgbm en temps réel

Pour plus d'information sur ce programme voir les liens suivants :
https://learnopencv.com/camera-calibration-using-opencv/
https://learnopencv.com/depth-perception-using-stereo-camera-python-c/

Il faudra fournir a ce programme les images de calibration prises avec stereo_pics


1)Compiler
2)En se placant dans le répertoire "release" lancer le programme en tapant $ ./stereo_calib


//A la fin du programme, le fichier improved_params2_cpp.xml est généré

***disparity_ajust*** : permet de régler les paramètre de la sgbm en temps réel

Pour plus d'information sur ce programme voir les liens suivants :
https://learnopencv.com/camera-calibration-using-opencv/
https://learnopencv.com/depth-perception-using-stereo-camera-python-c/


1)Compiler
2)En se placant dans le répertoire "release" lancer le programme en tapant $ ./ajust 

Ce programme a besoin du fichier improved_params2_cpp.xml du programme stereo_calib

//remplacer le contenu du main.cpp par le contenu du fichier main_flux.HOLD pour régler les parametre en temps réel
//sinon remplacer par le contenu du fichier main_pic.HOLD pour ajuster les paramètres sur une scène fixe. (les photos se trouvent dans le dossier /Image




----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

Sur Raspberry :

Utilisation similaire des programmes


----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


FIN

Bonne chance ;)
