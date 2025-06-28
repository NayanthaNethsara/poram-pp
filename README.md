
# 🚀 Poram++ DSL

**Poram++** is a domain-specific language (DSL) built in C using Lex and Yacc (Flex/Bison) for defining structured form UIs and generating static HTML output.

It is modular, offline-ready, and comes with a command-line interface to control output formatting (theme, shape, and style). It powers the **[Poram++ GUI](https://github.com/NayanthaNethsara/poram-pp-gui)** — an Electron-based graphical frontend for previewing forms live.

---

## 🧠 Features

- ✅ Built from scratch using Lex/Yacc and C
- ✅ Converts DSL input into responsive HTML forms
- ✅ Supports validation logic in DSL (`validate { if age < 18 ... }`)
- ✅ CLI flags for:
  - Theme: `-t dark` / `-t light`
  - Shape: `-s rounded` / `-s square`
  - Style: `-m minimal` / `-m fancy`
- ✅ Works offline (no Tailwind CDN)
- ✅ Clean memory management

---

## 📦 Build Instructions

### Prerequisites

- gcc or clang
- Flex and Bison
- Make

### To Build

```bash
make
```

### To Run

```bash
./poram -o output/form.html -t dark -s rounded -m fancy < examples/registration.poram
```

- If `-o` is not provided, output goes to `stdout`.

---

## 🛠 CLI Flags

| Flag     | Description                       | Example             |
|----------|-----------------------------------|---------------------|
| `-o`     | Output HTML file path             | `-o form.html`      |
| `-t`     | Theme: `dark` or `light`          | `-t light`          |
| `-s`     | Shape: `rounded` or `square`      | `-s square`         |
| `-m`     | Style: `minimal` or `fancy`       | `-m fancy`          |

---

## 📂 Project Structure

```
poram-pp/
├── src/            # All C source code (parser, lexer, model)
├── examples/       # DSL input examples
├── output/         # (gitignored) generated HTML
├── Makefile        # Build system
├── README.md
├── GRAMMAR.md      # Full DSL syntax
├── LICENSE
```

---

## 🔍 DSL Syntax

For the full DSL syntax, see [`GRAMMAR.md`](./GRAMMAR.md).

Example snippet:

```poram
form Registration {
  section Account {
    field username: text required;
    field password: password required;
  }

  validate {
    if password == "1234" {
      error "Password too weak.";
    }
  }
}
```

---

## 💻 GUI Companion

Want to use this with a graphical interface?

👉 Try the Electron GUI: **[Poram++ GUI](https://github.com/NayanthaNethsara/poram-pp-gui)**  
It connects to this DSL compiler, renders live preview, and supports exporting.

---

## 🧑‍💻 Author

Developed by [Nayantha Nethsara](https://nayantha.me)

---

## 📄 License

This project is licensed under the [MIT License](./LICENSE).  
Use it freely, with no warranty or liability.
