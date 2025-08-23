**README** available in English and Spanish | **README** disponible en Inglés y en Español

# shell — Operating Systems (7508) Shell Project

Repository for the solution of the [TP: shell](https://fisop.github.io/website/tps/shell) from the Mendez-Fresia course of **Operating Systems (7508) - FIUBA**.

## Theoretical answers

Use the `shell.md` file provided in the repository.

## Compile

```bash
make
```

## Tests

- Run all tests:

```bash
make test
```

- Run a single test:

```bash
make test-TEST_NAME
```

For example:

```bash
make test-env_empty_variable
```

Each test identifier is shown in parentheses `(TEST_NAME)` next to each test when running all tests:

```
=== Temporary files will be stored in: /tmp/tmp0l10br1k-shell-test ===

PASS 1/26: cd . and cd .. work correctly by checking pwd (no prompt) (cd_back)
PASS 2/26: cd works correctly by checking pwd (no prompt) (cd_basic)
PASS 3/26: cd with no arguments takes you home (/proc/sys :D) (cd_home)
PASS 4/26: empty variables are not substituted (env_empty_variable)
...
```

## Run

```bash
./sh
```

## Linter

```bash
make format
```

To commit the formatting changes:

```bash
$ git add .
$ git commit -m "format: apply code formatter"
```



------------------------------------------------------------------------------------



# shell - Sistemas Operativos (7508) Proyecto Shell

Repositorio para la solución del [TP: shell](https://fisop.github.io/website/tps/shell) del curso Mendez-Fresia de **Sistemas Operativos (7508) - FIUBA**

## Respuestas teóricas

Utilizar el archivo `shell.md` provisto en el repositorio

## Compilar

```bash
make
```

## Pruebas

- Ejecutar todas las pruebas

```bash
make test
```

- Ejecutar una **única** prueba

```bash
make test-TEST_NAME
```

Por ejemplo:

```bash
make test-env_empty_variable
```

Cada identificador de una prueba se muestra entre paréntesis `(TEST_NAME)` al lado de cada _test_ cuando se ejecutan todas las pruebas.

```
=== Temporary files will be stored in: /tmp/tmp0l10br1k-shell-test ===

PASS 1/26: cd . and cd .. work correctly by checking pwd (no prompt) (cd_back)
PASS 2/26: cd works correctly by checking pwd (no prompt) (cd_basic)
PASS 3/26: cd with no arguments takes you home (/proc/sys :D) (cd_home)
PASS 4/26: empty variables are not substituted (env_empty_variable)
...
```

## Ejecutar

```bash
./sh
```

## Linter

```bash
make format
```

Para efectivamente subir los cambios producidos por `make format`, hay que hacer `git add .` y `git commit`.
