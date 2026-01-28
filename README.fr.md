# Minishell

[![en](https://img.shields.io/badge/lang-en-blue.svg)](README.md)
[![fr](https://img.shields.io/badge/lang-fr-red.svg)](README.fr.md)

Une implementation legere d'un shell UNIX en C, reproduisant les fonctionnalites principales de bash. Ce projet fait partie du cursus de l'ecole 42 et demontre des concepts de programmation systeme incluant la gestion des processus, les descripteurs de fichiers et la gestion des signaux.

---

## Table des Matieres

- [Apercu](#apercu)
- [Fonctionnalites](#fonctionnalites)
- [Structure du Projet](#structure-du-projet)
- [Architecture](#architecture)
- [Composants Principaux](#composants-principaux)
  - [1. Lexer (Tokenisation)](#1-lexer-tokenisation)
  - [2. Parser](#2-parser)
  - [3. Gestion des Here-Documents](#3-gestion-des-here-documents)
  - [4. Execution](#4-execution)
  - [5. Commandes Integrees (Built-in)](#5-commandes-integrees-built-in)
  - [6. Gestion des Signaux](#6-gestion-des-signaux)
- [Structures de Donnees](#structures-de-donnees)
- [Quotes et Expansion de Variables](#quotes-et-expansion-de-variables)
- [Pipes et Redirections](#pipes-et-redirections)
- [Compilation](#compilation)
- [Utilisation](#utilisation)

---

## Apercu

Minishell lit l'entree utilisateur, la tokenise, parse les tokens en commandes, et les execute. Le shell supporte :
- Execution de commandes avec arguments
- Pipes (`|`) pour enchainer les commandes
- Redirections d'entree/sortie (`<`, `>`, `>>`)
- Here-documents (`<<`)
- Expansion des variables d'environnement (`$VAR`, `$?`)
- Gestion des quotes (simples et doubles)
- Commandes integrees (cd, echo, env, exit, export, pwd, unset)

---

## Fonctionnalites

| Fonctionnalite | Description |
|----------------|-------------|
| **Prompt** | Prompt interactif utilisant GNU readline |
| **Historique** | Navigation dans l'historique avec les fleches |
| **Pipes** | Enchainer plusieurs commandes : `cmd1 \| cmd2 \| cmd3` |
| **Redirections** | `<` entree, `>` sortie, `>>` ajout |
| **Here-doc** | `<< DELIMITEUR` entree multi-ligne |
| **Variables** | Expansion `$VAR`, `$?` pour le code de retour |
| **Quotes** | `'simple'` (litteral) et `"double"` (avec expansion) |
| **Signaux** | Gestion de Ctrl+C, Ctrl+D, Ctrl+\ |
| **Built-ins** | cd, echo, env, exit, export, pwd, unset |

---

## Structure du Projet

```
minishell/
├── include/
│   ├── minishell.h      # Header principal - toutes les declarations de fonctions
│   ├── struct.h         # Definitions des structures de donnees
│   ├── macros.h         # Constantes et enum des types de signaux
│   ├── library.h        # Includes des bibliotheques systeme
│   └── my_lib/          # Bibliotheque libft personnalisee
│
├── src/
│   ├── main.c           # Point d'entree et boucle principale
│   ├── header.c         # Animation ASCII au demarrage
│   ├── signals.c        # Fonctions de configuration des signaux
│   ├── signals_execution.c  # Gestionnaires de signaux
│   │
│   ├── lexer/           # Module de tokenisation
│   │   ├── lexer.c              # Point d'entree du lexer
│   │   ├── identify_tokens.c    # Classification des tokens
│   │   ├── calc_token_len.c     # Calcul de la longueur apres expansion
│   │   ├── copy_token.c         # Copie avec expansion
│   │   ├── token_parser.c       # Suppression des quotes
│   │   └── utils.c              # Utilitaires du lexer
│   │
│   ├── parser/          # Creation de la structure de commandes
│   │   ├── parser.c             # Point d'entree du parser
│   │   ├── check_token_syntax.c # Validation de la syntaxe
│   │   ├── counting.c           # Comptage des commandes/arguments
│   │   ├── prepar_cmd.c         # Construction de la liste de commandes
│   │   └── handle_redirection.c # Parsing des redirections
│   │
│   ├── execution/       # Execution des commandes
│   │   ├── execute.c            # Logique principale d'execution
│   │   ├── execute_build_in.c   # Dispatcheur des built-ins
│   │   ├── execute_external.c   # Execution des commandes externes
│   │   ├── get_path.c           # Resolution du PATH
│   │   ├── check_the_acces.c    # Validation des acces fichiers
│   │   └── utilities.c          # Helpers pour pipes/wait
│   │
│   ├── here_doc/        # Gestion des here-documents
│   │   ├── here_doc.c           # Logique principale here-doc
│   │   ├── here_doc_utils.c     # Gestion des fichiers temporaires
│   │   └── expand.c             # Expansion des variables
│   │
│   ├── build_in/        # Commandes integrees
│   │   ├── cd.c, echo.c, env.c, pwd.c, exit.c, unset.c
│   │   ├── export.c             # Commande export
│   │   ├── export_parser.c      # Parsing des arguments d'export
│   │   └── export_utils.c       # Helpers pour export
│   │
│   ├── init/            # Initialisation
│   │   ├── init_data.c          # Init des donnees principales
│   │   ├── init_envp.c          # Configuration de l'environnement
│   │   └── init_cmd.c           # Init de la structure de commande
│   │
│   └── utils/           # Utilitaires
│       ├── cleanup.c            # Nettoyage apres commande
│       ├── free_all.c           # Liberation de memoire
│       ├── error.c              # Messages d'erreur
│       ├── exit_all.c           # Sortie avec nettoyage
│       ├── get_char.c           # Utilitaires pour strings
│       ├── get_int.c            # Utilitaires pour entiers
│       └── utilities_.c         # Helpers d'execution
│
└── Makefile
```

---

## Architecture

Le shell suit un pipeline d'interpreteur classique :

```
┌─────────────────────────────────────────────────────────────────────────┐
│                           BOUCLE PRINCIPALE                              │
│                           (src/main.c)                                   │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  1. ENTREE                                                               │
│     readline("minishell$ ") → chaine d'entree brute                     │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  2. LEXER (Tokenisation)                        src/lexer/              │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ "echo hello | cat -e" → [echo][hello][|][cat][-e]           │    │
│     │                                                               │    │
│     │ • Decoupage sur espaces et operateurs (|, <, >, <<, >>)      │    │
│     │ • Preservation des sections entre quotes                      │    │
│     │ • Expansion des $VARIABLES (sauf entre quotes simples)       │    │
│     │ • Attribution des types de tokens (COMMAND, STRING, etc.)    │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  3. PARSER                                      src/parser/             │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ Tokens → Structures de Commandes (liste chainee t_cmd)       │    │
│     │                                                               │    │
│     │ • Validation de la syntaxe (pas de pipes vides, etc.)        │    │
│     │ • Regroupement des tokens en commandes (separation par |)    │    │
│     │ • Extraction des redirections et noms de fichiers            │    │
│     │ • Construction des tableaux d'arguments pour chaque commande │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  4. TRAITEMENT HERE-DOC                         src/here_doc/           │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ Avant l'execution, lire tout le contenu here-doc             │    │
│     │                                                               │    │
│     │ • Fork d'un processus enfant pour lire l'entree              │    │
│     │ • Stockage dans un fichier temporaire (/tmp/here_doc_N)      │    │
│     │ • Expansion des variables si le delimiteur n'est pas quote   │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  5. EXECUTION                                   src/execution/          │
│     ┌──────────────────────────────────────────────────────────────┐    │
│     │ Execution du pipeline de commandes                           │    │
│     │                                                               │    │
│     │ Built-in seul (sans pipe) :                                  │    │
│     │   → Execution directe dans le processus parent               │    │
│     │                                                               │    │
│     │ Pipeline ou commande externe :                               │    │
│     │   → Creation des pipes entre commandes                       │    │
│     │   → Fork d'un enfant pour chaque commande                    │    │
│     │   → Configuration des redirections (dup2)                    │    │
│     │   → Execution (built-in ou execve)                           │    │
│     │   → Attente de tous les enfants                              │    │
│     └──────────────────────────────────────────────────────────────┘    │
└─────────────────────────────────────────────────────────────────────────┘
                                    │
                                    ▼
┌─────────────────────────────────────────────────────────────────────────┐
│  6. NETTOYAGE                                   src/utils/              │
│     • Liberation des tokens et structures de commandes                  │
│     • Suppression des fichiers temporaires                              │
│     • Reinitialisation pour la prochaine commande                       │
└─────────────────────────────────────────────────────────────────────────┘
```

---

## Composants Principaux

### 1. Lexer (Tokenisation)

**Emplacement :** [src/lexer/](src/lexer/)

Le lexer transforme l'entree brute en une liste chainee de tokens. C'est la premiere etape de traitement.

#### Point d'entree : `lexer_main()` dans [lexer.c](src/lexer/lexer.c)

```c
void lexer_main(t_data *data)
{
    cut_lexeur(data);        // Etape 1 : Decoupage en tokens bruts
    identify_tokens(data);   // Etape 2 : Attribution des types
    token_parser(data);      // Etape 3 : Traitement des quotes et expansions
}
```

#### Etape 1 : Decoupage des Tokens (`cut_lexeur`)

**Fichier :** [identify_tokens.c](src/lexer/identify_tokens.c)

Decoupe la chaine d'entree en tokens selon :
- **Les espaces** (espaces, tabulations) - separateurs de tokens
- **Les operateurs** (`|`, `<`, `>`, `<<`, `>>`) - deviennent leurs propres tokens
- **Les quotes** - le contenu entre quotes reste ensemble, meme avec des espaces

```
Entree :  echo "hello world" | cat
Tokens :  [echo] ["hello world"] [|] [cat]
```

Fonctions cles :
- `handle_specials()` - Identifie les tokens operateurs (gere `<<` et `>>` comme tokens uniques)
- `handle_token()` - Extrait les tokens reguliers, sautant par-dessus les sections entre quotes

#### Etape 2 : Classification des Tokens (`identify_tokens`)

**Fichier :** [identify_tokens.c](src/lexer/identify_tokens.c)

Attribue un type a chaque token :

| Type | Description | Exemple |
|------|-------------|---------|
| `COMMAND` | Premier token d'une commande | `echo`, `cat`, `ls` |
| `STRING` | Arguments | `hello`, `-la`, `file.txt` |
| `PIPE` | Operateur pipe | `\|` |
| `REDIRECT_IN` | Redirection d'entree | `<` |
| `REDIRECT_OUT` | Redirection de sortie | `>` |
| `APPEND` | Redirection en ajout | `>>` |
| `HEREDOC` | Here-document | `<<` |
| `STDIN` | Nom de fichier apres `<` ou delimiteur apres `<<` | |
| `STDOUT` | Nom de fichier apres `>` ou `>>` | |

#### Etape 3 : Traitement des Quotes et Variables

**Fichiers :** [calc_token_len.c](src/lexer/calc_token_len.c), [copy_token.c](src/lexer/copy_token.c), [token_parser.c](src/lexer/token_parser.c)

Cette etape :
1. **Calcule la longueur apres expansion** - Pre-calcule la taille du token apres expansion des variables
2. **Copie avec expansion** - Cree une nouvelle valeur de token avec `$VAR` remplace par les vraies valeurs
3. **Supprime les quotes** - Enleve les caracteres de quote tout en preservant le contenu

```
Token d'entree :  "Hello $USER"
Apres expansion : "Hello john"
Apres suppression des quotes : Hello john
```

---

### 2. Parser

**Emplacement :** [src/parser/](src/parser/)

Le parser convertit la liste de tokens en structures de commandes executables.

#### Point d'entree : `parsing()` dans [parser.c](src/parser/parser.c)

```c
int parsing(t_data *data)
{
    if (!check_tokens_syntax(data))   // Valider la syntaxe
        return (0);
    cut_to_cmds(data);                // Construire la liste de commandes
    if (!process_here_docs(data))     // Gerer les here-docs
        return (0);
    return (1);
}
```

#### Validation de la Syntaxe

**Fichier :** [check_token_syntax.c](src/parser/check_token_syntax.c)

Detecte les erreurs comme :
- Pipes vides : `| cmd` ou `cmd |`
- Operateurs consecutifs : `> >`, `| |`
- Cibles de redirection manquantes : `cmd >` (rien apres)
- Sequences d'operateurs invalides : `> |`

#### Construction des Commandes

**Fichier :** [prepar_cmd.c](src/parser/prepar_cmd.c)

Cree une liste chainee de structures `t_cmd`, une par commande dans le pipeline :

```
Entree : echo hello | cat -e | wc -l

Cree :
cmd1 → cmd2 → cmd3
 │      │      │
 ├─args: [echo, hello]
 │      ├─args: [cat, -e]
 │      │      └─args: [wc, -l]
```

#### Extraction des Redirections

**Fichier :** [handle_redirection.c](src/parser/handle_redirection.c)

Parse les tokens de redirection et les stocke dans la structure de commande :

```c
// Pour : cat < input.txt > output.txt
cmd->input_file = "input.txt"
cmd->output_file = "output.txt"
cmd->append = 0  // (serait 1 pour >>)
```

Pour les here-docs :
```c
// Pour : cat << EOF
cmd->here_doc = 1
cmd->delimiter = "EOF"
cmd->heredoc_expand = 1  // (0 si le delimiteur etait quote : << 'EOF')
```

---

### 3. Gestion des Here-Documents

**Emplacement :** [src/here_doc/](src/here_doc/)

Les here-documents permettent une entree multi-ligne jusqu'a ce qu'un delimiteur soit rencontre.

#### Comment ca Fonctionne

**Fichier :** [here_doc.c](src/here_doc/here_doc.c)

```
$ cat << END
> Hello
> World
> END
Hello
World
```

Implementation :
1. **Fork d'un processus enfant** - Environnement isole pour lire l'entree
2. **Creation d'un fichier temporaire** - `/tmp/here_doc_0`, `/tmp/here_doc_1`, etc.
3. **Lecture des lignes** - Avec `readline("> ")` jusqu'a ce que le delimiteur soit trouve
4. **Ecriture dans le fichier temporaire** - Avec expansion optionnelle des variables
5. **Le parent ouvre le fichier temporaire** - Comme entree de la commande
6. **Unlink du fichier temporaire** - Fichier supprime mais reste accessible via le FD

#### Expansion des Variables dans les Here-Docs

**Fichier :** [expand.c](src/here_doc/expand.c)

```bash
# Expansion activee (delimiteur non quote)
$ cat << EOF
Hello $USER
EOF
# Sortie : Hello john

# Expansion desactivee (delimiteur quote)
$ cat << 'EOF'
Hello $USER
EOF
# Sortie : Hello $USER
```

Le flag `heredoc_expand` controle ce comportement :
- Delimiteur non quote (`<< EOF`) → `heredoc_expand = 1`
- Delimiteur quote (`<< 'EOF'` ou `<< "EOF"`) → `heredoc_expand = 0`

---

### 4. Execution

**Emplacement :** [src/execution/](src/execution/)

Le module d'execution lance les commandes parsees.

#### Point d'entree : `execution()` dans [execute.c](src/execution/execute.c)

```c
void execution(t_data *data, t_envp **envp)
{
    // Cas special : built-in seul sans pipes
    // Execute dans le parent pour affecter l'etat du shell (cd, export, etc.)
    if (data->num_cmds == 1 && cmd->is_built_in)
    {
        solo_build_in_execution_and_handling_of_fd(data, cmd, envp);
        return;
    }

    // Cas general : fork pour chaque commande
    process(data, envp);
}
```

#### Execution de Pipeline

**Fichier :** [execute.c](src/execution/execute.c)

Pour `cmd1 | cmd2 | cmd3` :

```
          pipe[0]      pipe[1]
             │            │
    ┌────────┼────────────┼────────┐
    │        ▼            ▼        │
    │   ┌─────────┐  ┌─────────┐   │
    │   │ ecriture│  │ ecriture│   │
cmd1 ──►│ end [1] │  │ end [1] │◄── cmd2
    │   └────┬────┘  └────┬────┘   │
    │        │            │        │
    │   ┌────▼────┐  ┌────▼────┐   │
    │   │ lecture │  │ lecture │   │
    │   │ end [0] │──►  end [0]│──► cmd3
    │   └─────────┘  └─────────┘   │
    └──────────────────────────────┘
```

Processus :
1. Creer `n-1` pipes pour `n` commandes
2. Fork d'un enfant pour chaque commande
3. Enfant : configurer stdin/stdout avec `dup2()`, fermer les pipes inutilises, executer
4. Parent : fermer tous les pipes, attendre tous les enfants

#### Execution de Commandes Externes

**Fichier :** [execute_external.c](src/execution/execute_external.c)

```c
void execute_external_function(t_cmd *cmd, t_envp **envp)
{
    char *cmd_path = find_command_path(cmd, *envp);
    char **envp_strs = convert_envp_to_strs(*envp);

    execve(cmd_path, cmd->args, envp_strs);
    // Si on arrive ici, execve a echoue
}
```

#### Resolution du PATH

**Fichier :** [get_path.c](src/execution/get_path.c)

1. **Chemin absolu** (`/bin/ls`) - Verifier l'existence et les permissions directement
2. **Chemin relatif** (`./script.sh`) - Resoudre depuis le repertoire courant
3. **Nom de commande** (`ls`) - Chercher dans les repertoires du PATH

```c
// PATH=/usr/bin:/bin:/usr/local/bin
// Commande : ls
// Recherche : /usr/bin/ls, /bin/ls, /usr/local/bin/ls
// Retourne le premier qui existe et est executable
```

---

### 5. Commandes Integrees (Built-in)

**Emplacement :** [src/build_in/](src/build_in/)

Les built-ins sont executes directement par le shell, pas via `execve()`.

| Commande | Fichier | Description |
|----------|---------|-------------|
| `cd` | [cd.c](src/build_in/cd.c) | Change de repertoire, met a jour PWD/OLDPWD |
| `echo` | [echo.c](src/build_in/echo.c) | Affiche les arguments, supporte le flag `-n` |
| `env` | [env.c](src/build_in/env.c) | Affiche les variables d'environnement |
| `exit` | [exit.c](src/build_in/exit.c) | Quitte le shell avec un code de retour |
| `export` | [export.c](src/build_in/export.c) | Definit des variables d'environnement |
| `pwd` | [pwd.c](src/build_in/pwd.c) | Affiche le repertoire de travail |
| `unset` | [unset.c](src/build_in/unset.c) | Supprime des variables d'environnement |

#### Pourquoi les Built-ins s'Executent dans le Parent

Les built-ins comme `cd` et `export` doivent modifier l'etat du shell. S'ils s'executaient dans un enfant forke, les changements seraient perdus quand l'enfant se termine.

```c
// Dans execution() :
if (data->num_cmds == 1 && cmd->is_built_in)
{
    // Execute dans le parent - les changements persistent !
    solo_build_in_execution_and_handling_of_fd(data, cmd, envp);
}
```

Cependant, dans un pipeline (`export VAR=x | cat`), chaque commande s'execute dans un enfant, donc `export` n'affectera pas le shell parent.

---

### 6. Gestion des Signaux

**Emplacement :** [signals.c](src/signals.c), [signals_execution.c](src/signals_execution.c)

Le shell gere les signaux differemment selon le contexte :

#### Modes de Signaux

| Mode | Contexte | Ctrl+C (SIGINT) | Ctrl+\ (SIGQUIT) |
|------|----------|-----------------|-------------------|
| `MAIN_SIGNAL` | Attente d'entree | Reaffiche le prompt | Ignore |
| `CHILD_SIGNAL` | Pendant l'execution | Newline + exit 130 | "Quit" + exit 131 |
| `HEREDOC_CHILD` | Lecture here-doc | Exit 130 | Ignore |
| `HEREDOC_PARENT` | Attente du here-doc | Definit exit code 130 | Ignore |

#### Implementation

```c
void set_signal(int signal_type)
{
    if (signal_type == MAIN_SIGNAL)
    {
        signal(SIGINT, reset_prompt);   // Ctrl+C : nouveau prompt
        signal(SIGQUIT, SIG_IGN);       // Ctrl+\ : ignore
    }
    else if (signal_type == CHILD_SIGNAL)
    {
        signal(SIGINT, ctrl_c);         // Ctrl+C : exit 130
        signal(SIGQUIT, ctrl_backslash); // Ctrl+\ : exit 131
    }
    // ... autres modes
}
```

---

## Structures de Donnees

**Emplacement :** [include/struct.h](include/struct.h)

### Structure Principale : `t_data`

```c
typedef struct s_data
{
    char    *input;              // Entree brute de readline
    int     num_cmds;            // Nombre de commandes dans le pipeline
    int     exit_code;           // Code de retour de la derniere commande

    t_tokens    **tokens;        // Tableau de listes de tokens
    t_envp      *env_var;        // Environnement en liste chainee
    t_cmd       *cmds;           // Liste chainee de commandes

    char    **here_doc_temp_files;  // Suivi des fichiers temporaires
    // ... champs de redirection
} t_data;
```

### Structure de Token : `t_tokens`

```c
typedef struct s_tokens
{
    int         position;        // Index du token
    char        *value;          // Chaine du token
    t_tok_type  type;            // COMMAND, STRING, PIPE, etc.
    bool        simple_quote;    // Etait entre quotes simples
    bool        was_quoted;      // Etait quote

    struct s_tokens *next;
    struct s_tokens *prev;
} t_tokens;
```

### Structure de Commande : `t_cmd`

```c
typedef struct s_cmd
{
    char    **args;              // Tableau d'arguments pour execve
    int     args_count;
    int     is_built_in;         // 1 si commande built-in

    // Redirections
    char    *input_file;         // < fichier
    char    *output_file;        // > ou >> fichier
    int     input_fd;
    int     output_fd;
    int     append;              // 1 pour >>, 0 pour >

    // Here-doc
    int     here_doc;            // 1 si a un here-doc
    char    *delimiter;
    int     heredoc_expand;      // 1 pour expansion des variables

    struct s_cmd *next;
    struct s_cmd *prev;
} t_cmd;
```

### Structure d'Environnement : `t_envp`

```c
typedef struct s_envp
{
    char    *name;               // Nom de la variable
    char    *value;              // Valeur de la variable
    struct s_envp *next;
    struct s_envp *prev;
} t_envp;
```

---

## Quotes et Expansion de Variables

### Regles des Quotes

| Type de Quote | Expansion de Variables | Caracteres Speciaux |
|---------------|------------------------|---------------------|
| Sans quotes | Oui (`$VAR` → valeur) | Les espaces separent les tokens |
| Simple `'` | Non (`$VAR` → litteral) | Tout est litteral |
| Double `"` | Oui (`$VAR` → valeur) | Les espaces sont preserves |

### Processus d'Expansion

**Emplacement :** [src/lexer/calc_token_len.c](src/lexer/calc_token_len.c), [src/lexer/copy_token.c](src/lexer/copy_token.c)

```
Token : "Hello $USER"

1. Calcul de la longueur :
   - "Hello " = 6
   - $USER → recherche "john" = 4
   Total : 10 + terminateur null

2. Copie avec expansion :
   Resultat : "Hello john"

3. Suppression des quotes :
   Final : Hello john
```

### Variables Speciales

| Variable | Signification |
|----------|---------------|
| `$VAR` | Valeur de la variable d'environnement |
| `$?` | Code de retour de la derniere commande |

---

## Pipes et Redirections

### Implementation des Pipes

Pour `cmd1 | cmd2` :

1. Creer le pipe : `pipe(pipefd)` → `pipefd[0]` (lecture), `pipefd[1]` (ecriture)
2. Fork cmd1 : stdout → `pipefd[1]`
3. Fork cmd2 : stdin → `pipefd[0]`
4. Fermer les extremites du pipe dans le parent
5. Attendre les deux enfants

### Implementation des Redirections

**Fichier :** [src/execution/check_the_acces.c](src/execution/check_the_acces.c)

```c
// Redirection d'entree : < fichier
open(file, O_RDONLY);
dup2(fd, STDIN_FILENO);

// Redirection de sortie : > fichier
open(file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
dup2(fd, STDOUT_FILENO);

// Redirection en ajout : >> fichier
open(file, O_CREAT | O_WRONLY | O_APPEND, 0644);
dup2(fd, STDOUT_FILENO);
```

### Priorite des Redirections

- Les redirections ulterieures ecrasent les precedentes
- Seule la derniere redirection de chaque type prend effet
- Exemple : `< a < b` → lit depuis `b` uniquement

---

## Compilation

```bash
make        # Compiler minishell
make clean  # Supprimer les fichiers objets
make fclean # Supprimer les objets et l'executable
make re     # Recompiler depuis zero
```

Prerequis :
- Compilateur GCC
- Bibliotheque GNU Readline (`-lreadline`)
- Make

---

## Utilisation

```bash
./minishell
```

### Exemples

```bash
# Commande simple
minishell$ ls -la

# Pipeline
minishell$ cat file.txt | grep "pattern" | wc -l

# Redirections
minishell$ echo "Hello" > output.txt
minishell$ cat < input.txt >> output.txt

# Here-document
minishell$ cat << EOF
> Ligne 1
> Ligne 2
> EOF

# Expansion de variables
minishell$ echo "Utilisateur : $USER, Statut : $?"

# Commandes integrees
minishell$ cd /tmp
minishell$ export MA_VAR="bonjour"
minishell$ echo $MA_VAR
minishell$ unset MA_VAR
minishell$ exit 0
```

---

## Licence

Ce projet fait partie du cursus de l'ecole 42.
