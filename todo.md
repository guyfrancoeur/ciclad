# ciclad v3


### notation

- [ ] Quel est le nom du premier index (item vers concept) ?
     + R: index inversé ou liste transversale


## TODO

### Fonctionnalités
- [ ] détecteur de transactions identique
- [ ] détecteur de concept associé à une transaction
     + on hash la transaction pour ajouter à l'index (limité);
     + on hash seulement les transactions donc nous avons __tous__ les items dans l'index inversé (principal);
     + l'index hash pointe vers le concept (la trx) et les sous-concepts de la transaction;
     + de cette façon il est assez facile de faire le `+1` ou `-1` en parallèle;
- [ ] multi-treillis (parallele)
     + merge;
     + split;
- [ ] option de la ligne de commande pour la fenêtre glissante
- [ ] gestionnaire de transactions externe

### Langage C++
- [ ] module et fonction de lecture de transaction (unique);
- [ ] Compléter les templates C++;
- [ ] Rendre le code compatible aux directives `#pragma` de OpenMP;

### Productivité
- [ ] Création de CMakeLists.txt
- [ ] Utiliser yaml (ci et test de regression)
- [ ] Utiliser CTest dans CMake
- [ ] Utiliser GNUplot pour  graphiser les résultats

### Expérimentations:
 - [ ] Utiliser [splay tree](https://en.wikipedia.org/wiki/Splay_tree)
 - [ ] Utiliser [van Emde Boas tree](https://en.wikipedia.org/wiki/Van_Emde_Boas_tree)
 - [ ] Utiliser Judy array [wiki](https://en.wikipedia.org/wiki/Judy_array) [official](http://judy.sourceforge.net/) [resultat](http://www.nothings.org/computer/judy/)

## Détails

### Options pour la fenêtre glissante (sliding window)

| option | description | default | mode |
|:--------- |:-------- | :----| :------ |
| -t durée | la durée avant une supression en heure :wink: | nsp  | autonome |
| -c 1000  | nombre de transactions (count) | nsp | autonome | 
| -r 80    | utilsation de la mémoire (ram) en % |  -r 60 | non | drop |
| -l 1000  | temps limite en ms pour gèrer un ajout | -l 5000 | drop |

#### mode
  + autonome : En mode autonome seulement;
  + drop : les trx seront perdus si l'option n'est pas respectée;

### Gestionnaire de transactions externe

Nous ne voulons peut-être pas gérer les transactions dans ciclad. Enfin ne pas prendre de la mémoire pour maintenir des
milliers de transactions pour longtemps avant d'avoir le premier effacement.

Donc nous allons laisser le soin de cette tache à un processus externe de nous fournir la trx à effacer.

Ceci implique deux choses : que nous soyons capables de recevoir les trx à ajouter et celles à effacer. Nous allons donc 
formaliser la signature des transactions add et delete.

+ `stream.in`
```
add 8c8647fd 1 3 4 59 90
add cbf6d2f0 4 5 56 58 89
add e8103724 3 4 56 59 89
del 8c8647fd 1 3 4 59 90
...
fin
```

Avec une option `-e` il serait possible de dire au programme de sortir les transactions reçues sur la sortie `stderr` pour
quelles soient redirigé vers l'OS.  Nous pouvons donc rediriger le canal `stderr` vers un fichier.
 + Exemple `rm -f strean.in ; touch ./stream.in ; tail -f ./file.in | ./ciclad 2> trx-in-window.txt`.

L'important ici c'est de ne pas avoir de <kbd>CTRL</kbd>-<kbd>D</kbd> dans le fichier `stream.in` pour éviter la fin du stream.

### mkfifo

Voici un autre moyen de faire des `Named pipes` natif pour Unix.  Ceci reprend l'exemple plus haut mais
sans la gestion des lignes à détruire.  

```bash
# Pour créer le pipe
$ mkfifo pipe2
# Liste les fichiers (le pipe aura un p comme attribut de départ)
$ ls -lash
```

```bash
# On lance des lignes dans le pipe nommé pipe2
$ echo -e "add 8c8647fd 1 3 4 59 90" > pipe2 &
```

```bash
# Maintenant on veut récupérer les données dans pipe2
Tout ce qui a été ajouté sera sortie du pipe. Nul besoin de faire le ménage.
$ cat < pipe2

# En terminant, il serait surement possible de prendre la commande tail -f afin de recevoir les données
tail -f pipe2
```

```bash
#!/bin/bash

pipe=MyPipe

rm -f $pipe

if [[ ! -p $pipe ]]; then
    mkfifo $pipe
fi

while true
do
  if read line <$pipe; then
    if [[ "$line" == 'fin' ]]; then
      break
    fi
    echo $line
  fi
done

echo "Reader exiting, fin."
```
