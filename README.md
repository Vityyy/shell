# 🐚 Unix Shell Implementation

<div align="center">
  <img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white" />
    <img src="https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white" />
  <img src="https://img.shields.io/badge/Unix-000000?style=for-the-badge&logo=linux&logoColor=white" />
  <img src="https://img.shields.io/badge/Shell-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white" />
  <img src="https://img.shields.io/badge/Operating_Systems-2E8B57?style=for-the-badge&logo=linux&logoColor=white" />
</div>

<div align="center">
  <img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif">
</div>

> **📚 Academic Project Notice**  
> This project was developed during our **third year** of the Computer Engineering program at the University of Buenos Aires. _This repository **does not** reflect our current programming level or professional skills. It is kept here as an academic and knowledge record._

---

## 🤖 About

**shell** is a fully functional **command-line shell** implemented in **C**, supporting essential Unix commands, pipeline operations, and environment variable handling. This project provides deep insight into system-level programming and process management fundamentals taught in operating systems courses.

### 🎯 Key Features
- **🔧 Basic Unix Commands** — Full support for standard shell operations
- **🔀 Pipeline Support** — Chain commands with pipe operators
- **🌍 Environment Variables** — Complete variable substitution and handling
- **📁 Directory Navigation** — Advanced `cd` command implementation
- **⚡ Process Management** — Fork, exec, and wait system calls

### 🔧 Technical Implementation
- **Low-level C programming** for maximum performance
- **System call interface** for process and file operations
- **Memory management** for dynamic command parsing
- **Signal handling** for proper process control

---

## 📋 Prerequisites

- **Linux Environment** with standard development tools
- **GCC Compiler** for C compilation
- **Make** build system
- **Standard Unix utilities** for full functionality testing

---

## 🛠️ Quick Start

### Compilation
```bash
make
```

### Launch Shell
Start your custom shell:
```bash
./sh
```

### Try It Out
Test basic commands in your shell:
```bash
# Directory navigation
$ cd /home
$ pwd

# Pipeline operations
$ ls -la | grep shell
$ cat file.txt | wc -l

# Environment variables
$ echo $HOME
$ export MY_VAR=hello
$ echo $MY_VAR
```

---

## 🧪 Testing & Validation

### Run All Tests
Execute the complete test suite:
```bash
make test
```

### Run Individual Tests
Target specific functionality:
```bash
make test-TEST_NAME
```

**Example**:
```bash
make test-env_empty_variable
```

### Test Output Format
Each test displays its identifier for easy debugging:
```
=== Temporary files will be stored in: /tmp/tmp0l10br1k-shell-test ===
PASS 1/26: cd . and cd .. work correctly by checking pwd (no prompt) (cd_back)
PASS 2/26: cd works correctly by checking pwd (no prompt) (cd_basic)
PASS 3/26: cd with no arguments takes you home (/proc/sys :D) (cd_home)
PASS 4/26: empty variables are not substituted (env_empty_variable)
...
```

---

### 📖 Theoretical Component
Comprehensive theoretical answers and implementation details are available in the `shell.md` file included in this repository.

---

## 🔧 Development Tools

### Code Formatting
Apply consistent code style:
```bash
make format
```

Commit formatting changes:
```bash
git add .
git commit -m "format: apply code formatter"
```

---

## 💡 Learning Outcomes

Mastery of:
- **System Programming** in C
- **Process Management** and Inter-Process Communication
- **File Descriptor Manipulation** and I/O redirection
- **Memory Management** for dynamic parsing
- **Unix System Calls** and their practical applications

---

<div align="center">
  <img src="https://user-images.githubusercontent.com/73097560/115834477-dbab4500-a447-11eb-908a-139a6edaec5c.gif">
  
  **Built with 🔧 C, 🐍 Python & 🐧 Unix at Universidad de Buenos Aires**
</div>
