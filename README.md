# 📊 TP_DataFrame — C++17

## 🧠 Description

**TP_DataFrame** est une implémentation légère d’un *dataframe* en **C++17**, inspirée des structures de données tabulaires (type pandas).  
Le projet permet de manipuler des **colonnes typées**, d’insérer/supprimer des lignes, d’effectuer des **recherches, tris, statistiques simples**, et de gérer l’import/export **CSV**.

---

## 📁 Structure du projet

```

TP_DataFrame/
├── Column/
│   ├── Column.h
│   └── Column.cpp
├── CDataframe/
│   ├── CDataframe.h
│   └── CDataframe.cpp
├── main.cpp
├── Makefile
└── README.md

````

---

## ⚙️ Compilation

Le projet utilise **g++** avec la norme **C++17**.

### Compiler
Depuis la racine du projet :

```bash
make
````

Cela génère l’exécutable :

```
dataframe.o
```

---

## ▶️ Exécution

```bash
make run
```

Ou directement :

```bash
./dataframe.o
```

---

## 🧹 Nettoyage

Supprimer les fichiers générés :

```bash
make clean
```

---

## 🧩 Fonctionnalités principales

### 📌 Colonnes (`Column`)

* Stockage de valeurs typées via `std::variant` (`ColumnValue`)
* Valeurs nulles (`std::monostate`)
* Tri ascendant / descendant
* Index interne pour recherche dichotomique
* Comptage et comparaisons
* Support des types :

  * entiers signés / non signés
  * flottants
  * chaînes (`std::string`)
  * objets génériques (`std::any`)

### 📌 DataFrame (`CDataframe`)

* Gestion dynamique des colonnes (`std::shared_ptr`)
* Insertion / suppression de lignes et colonnes
* Affichage complet, `head`, `tail`
* Statistiques simples :

  * nombre de lignes / colonnes
  * comptage de cellules (égal, supérieur, inférieur)
* Import / export CSV
* Recherche de valeurs dans l’ensemble du tableau

---

## 🧪 Exemple d’utilisation (extrait)

```cpp
Column col("Age", ColumnType::INT);
col.insertValue(25);
col.insertValue(30);

CDataframe df({col});
df.display();
```

---

## 📚 Génération de la documentation

Le projet est compatible **Doxygen**.

### Générer la documentation

```bash
doxygen -g
doxygen
```

La documentation HTML est générée dans :

```
docs/html/index.html
```

---

## 🛠️ Prérequis

* g++ compatible **C++17**
* GNU Make
* (optionnel) Doxygen pour la documentation

---

## 👨‍💻 Auteur

Projet réalisé dans le cadre d’un **TP C++**
Implémentation orientée **modularité, lisibilité et sécurité mémoire**.

- Thomas Guibert
- Romeo Froger

---

## 📄 Licence

Projet académique — usage pédagogique.

