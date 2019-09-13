# ciclad v3


## notation

- [ ] Quel est le nom du premier index (item vers concept) ?
     +

## TODO

- [ ] détecteur de transactions multiple
- [ ] détecteur de concept associé à une transaction
     + on hash la transaction pour ajouter à l'index (limité);
     + on hash seulement les trasactions donc nous avons les items dans l'index principal;
     + l'index hash pointe vers les concepts ont les items;
     + de cette façon il est assez facile de faire le `+1` ou `-1` en parallel;
- [ ] multi-treillis
     + merge;
     + split;
     
## Langage C++
- [ ] module et fonction de lecture de transaction (unique);
- [ ] Compléter les templates C++;
- [ ] Rendre le code compatible aux directives `#pragma` de OpenMP;


## Idées

+ [van Emde Boas tree](https://en.wikipedia.org/wiki/Van_Emde_Boas_tree)
+ [splay tree](https://en.wikipedia.org/wiki/Splay_tree)
