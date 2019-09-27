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
- [ ] multi-treillis
     + merge;
     + split;
- [ ] option de la ligne de commande pour la fenêtre glissante
- [ ] gestionnaire de transactions externe

### Langage C++
- [ ] module et fonction de lecture de transaction (unique);
- [ ] Compléter les templates C++;
- [ ] Rendre le code compatible aux directives `#pragma` de OpenMP;

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

`stream.in`
```
add 1 3 4 59 90
add 4 5 56 58 89
add 3 4 56 59 89
del 1 3 4 59 90
```

Avec une option `-e` il serait possible de dire au programme de sortir les transactions reçues sur la sortie `stderr` pour
quelles soient redirigé vers l'OS.  Nous pouvons donc rediriger le canal `stderr` vers un fichier.
 + Exemple `touch ./file.in ; tail -f ./file.in | ./ciclad 2> trx.out`.

L'important ici c'est de ne pas avoir de <kbd>CTRL</kbd>-<kbd>D</kbd> dans le fichier `file.in` pour éviter la fin du stream.

## Idées

+ [van Emde Boas tree](https://en.wikipedia.org/wiki/Van_Emde_Boas_tree)
+ [splay tree](https://en.wikipedia.org/wiki/Splay_tree)
