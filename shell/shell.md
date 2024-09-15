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

---

### Flujo estándar

---

### Tuberías múltiples

---

### Variables de entorno temporarias

---

### Pseudo-variables

---

### Comandos built-in

---

### Historial

---
