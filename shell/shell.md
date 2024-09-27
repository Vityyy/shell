# shell

### Búsqueda en $PATH

* Pregunta 1:
  *execve(2)* es una syscall de más bajo nivel que interactúa directamente con el kernel, mientras que los wrappers
  provistos por la librería estándar de C *(libc) exec(3)* proporcionan un nivel de abstracción mayor, generando un
  entorno de mayor simpleza, por ejemplo *execvp* recibe un archivo y lo busca en **\$PATH**, mientras que en *execv(
  2)*, el
  path tiene que ser pasado como argumento.
  En resumen; *execve(2)* pide al programador que se le pasen explícitamente tanto el **\$PATH**, argumentos y variables
  de
  entorno, mientras que *exec(3)* es más conveniente y sencillo de usar, con argumentos variables.
* Pregunta 2:
  Sí, puede fallar. La shell imprime un mensaje de error utilizando **perror** y mata al proceso hijo con *exit(-1)*. En
  este caso, se lanza el mensaje de error: *"No such file or directory."*

### Procesos en segundo plano

* Pregunta 1:
  El uso de señales es necesario para que los procesos se comuniquen. Las señales son lanzadas por los procesos y se
  atrapan desde los handlers de otros procesos. Por ejemplo, la señal SIGCHILD se genera cada vez que un proceso hijo
  termina.

---

### Flujo estándar

El significado de 2>&1 es redirigir el flujo de error estándar al flujo de salida estándar.

- El símbolo > representa una redirección, que por default se hará de la salida estandar (stdout) a un archivo.
- Si un número precede al símbolo de redicción, entonces se redirigirá ese file descriptor.
- Si el archivo al que se redirige es el símbolo & sucedido por un número, entonces se redirigirá el file descriptor
  dado al file descriptor con el &
- Por lo tanto, 2>&1 significa redirigir el file descriptor 2 (stderr) a el file descriptor 1 (stdout)


- En el ejemplo la salida de cat out.txt sería:

  ls: cannot access '/noexiste': No such file or directory

  /home:

  user


- Si se invierte el orden de las redirecciones el resultado es el mismo. Al compararlo con bash, en bash el stderr
  (fd = 2) se imprime por la salida estandar (terminal) y sólamente el stdout (fd = 1) se redirige a un archivo

### Tuberías múltiples

El valor de status luego de llamar tanto a una tubería simple como múltiple no se modifica.

Si alguno de los comandos falla, la shell imprime "No such file or directory". En caso de Bash imprime: \<command>:
command not found

- Ejemplos (en bash):

    - Con el comando: seq 10 | no_existe

      La consola imprime: no_existe: command not found

    - Con el comando: seq 10 | xargs -n 4 | no_existe

      La consola imprime: no_existe: command not found

### Variables de entorno temporarias

* Pregunta 1:
  Para que --en el caso de que se modifique el entorno de proceso hijo luego del *fork()*-- se asegure todo de que
  solo el proceso hijo tenga las variables de entorno temporales, sin afectar al resto de procesos hijos y/o al proceso
  padre.


* Pregunta 2:
  a) El comportamiento es el mismo; las variables de entorno pasadas reemplazarán las del proceso hijo. Es igual a usar
  *setenv(3)* para cada una de las variables antes de llamar a execvp, solo que evitando modificar el entorno del
  proceso padre.

  b) Lo dicho anteriormente: Crear un arreglo de strings que contenga las variables de entorno con el formato
  **VAR=VALUE**. Luego se puede llamar a *execve* para ejecutar el comando deseado, pasando el arreglo como tercer
  argumento.

---

### Pseudo-variables

* **a)**  '$$' almacena el PID del proceso shell actual.
* Por ejemplo: echo $$


* ** b)**  '$!' almacena el PID del último proceso ejecutado en segundo plano (background)
* Por ejemplo:

sleep 10 &

echo $!

* **c)** '$?' almacena el código de salida del último comando ejecutado
* Por ejemplo:

ls

echo $?

---

### Comandos built-in

Solamente *pwd* podría implementarse sin necesidad de ser built-in. Esto es posible porque pwd solo necesita
leer/mostrar
el directorio de trabajo actual sin modificar nada más en el proceso de la shell, mientras que *cd* tiene la opción de
cambiarlo.
Este último caso, si *cd* **no** fuera built_in solamente modificaría el directorio del child process y no de la
shell.

---

### Historial

Porque ayudan a la comunicación clara entre procesos, el manejo correcto de eventos de finalización/administración de
hijos y, gracias a esto, controlar la ejecución de procesos en segundo plano.

---
