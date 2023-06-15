# FT_IRC: Serveur IRC en C++ (Projet 42)
![image](https://github.com/BenJ3D/FT_IRC-42/assets/49345674/543ae8cf-bd58-4929-9f43-809300e38a80)
Ce projet est un serveur IRC développé en C++98 dans le cadre du cursus de l'école 42. Le serveur est capable de gérer plusieurs clients simultanément sans bloquer et utilise des opérations d'entrée/sortie non bloquantes.

## Fonctionnalités

- Connexion et communication en temps réel avec des clients IRC.
- Authentification avec mot de passe.
- Gestion des utilisateurs, des canaux et des messages privés.
- Transmission des messages dans les canaux aux utilisateurs concernés.
- Prise en charge des opérateurs et des commandes spécifiques aux opérateurs.
- Prise en charge de l'envoi de fichiers (bonus).
- Intégration d'un bot (bonus).

## Prérequis

- Compilateur C++ compatible avec la norme C++98.
- Makefile pour compiler les fichiers sources.

## Installation et utilisation

1. Cloner le repo GitHub et compiler le projet avec la commande make


2. Lancer le serveur IRC en spécifiant le port d'écoute et le mot de passe
      ```./ircserv <port> <password>```

3. Utiliser un le client IRC WEECHAT pour se connecter au serveur. 
Assurez-vous de spécifier l'adresse IP du serveur et le port d'écoute, ainsi que le password.
Par exemple pour un serveur lancé en local sur le port 4242 et password 4242, ajouter dans weechat le serveur suivant :
     ```/server add host 127.0.0.1/4242 password=4242```

Puis s'y connecter à l'aide de la commande : ```/connect host```

Voici les commandes de bases utilisées dans weechat pour irc :

1. **`/connect <server>`** : Se connecter à un serveur IRC.
2. **`/disconnect`** : Se déconnecter du serveur IRC actuel.
3. **`/join <#channel>`** : Rejoindre un canal.
4. **`/part <#channel>`** : Quitter un canal.
5. **`/msg <nickname> <message>`** : Envoyer un message privé à un utilisateur.
6. **`/query <nickname>`** : Ouvrir un nouvel onglet pour discuter en privé avec un utilisateur.
7. **`/nick <new_nickname>`** : Changer de pseudo.
10. **`/back`** : Revenir en ligne après avoir été absent.
11. **`/topic <new_topic>`** : Changer le sujet du canal.
12. `/names` : Lister les utilisateurs présents sur le canal.
13. `/list` : Lister les canaux disponibles sur le serveur.
14. **`/buffer close`** : Fermer l'onglet actuel.
15. **`/set <option> <value>`** : Modifier les options de configuration de WeeChat.
20. **`/help`** : Afficher de l'aide sur les commandes disponibles.

Voici les commandes operators de notre serveur:

1. **`+o`** / **`o`** : Ajouter ou supprimer le statut d'opérateur d'un utilisateur.
Exemple : **`MODE #channel +o nickname`** ou **`MODE #channel -o nickname`**
3. **`+k`** / **`k`** : Ajouter ou supprimer un mot de passe (clé) pour rejoindre le canal.
Exemple : **`MODE #channel +k password`** ou **`MODE #channel -k password`**
5. **`+b`** / **`b`** : Ajouter ou supprimer un bannissement pour un utilisateur ou un groupe d'utilisateurs, empêchant les utilisateurs bannis de rejoindre le canal.
Exemple : **`MODE #channel +b usermask`** ou **`MODE #channel -b usermask`**
6. **`+i`** / **`i`** : Rendre le canal accessible uniquement sur invitation, empêchant les utilisateurs de le rejoindre sans y être invités.
Exemple : **`MODE #channel +i`** ou **`MODE #channel -i`**

Vous pouvez aussi changer le topic d'un channel via la commande TOPIC :
```/topic NouveauTopicChannel```

Et afficher le topic avec la commande TOPIC sans arguments :
```/topic```

BONUS :
Vous pouvez compiler avec make et lancer le bot en allant dans le dossier Bot, il prend comme argument obligatoire le Mdp du serveur et se connectera sur l'ip `127.0.0.1` avec le port `6667`, mais vous pouvez les rajouter dans cet ordre pour les modifier à volonté
$\hspace{5pt}$```./Bob <password> [<ip_address> [port]]```

Pour l'envoi de fichier il suffit d'utiliser la commande send de weechat :
```/dcc send <nom_utilisateur_destinataire> <chemin_du_fichier>```
