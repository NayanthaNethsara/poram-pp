
# 📘 Poram++ DSL Grammar

> This document describes the grammar and structure of the Poram++ DSL for form definition.

---

## 🧾 Top-Level Structure

```ebnf
form ::= "form" IDENTIFIER "{" form_body "}";
form_body ::= { form_item };
form_item ::= section | meta_block | validate_block;
```

- A form begins with the `form` keyword and an identifier (its name).
- The form body may contain sections, metadata, or validation rules.

---

## 🔹 Metadata

```ebnf
meta_block ::= "meta" IDENTIFIER "=" STRING ";"
```

- Used to provide metadata like `author`, e.g.:
  ```poram
  meta author = "Nayantha Nethsara";
  ```

---

## 🔹 Sections and Fields

```ebnf
section ::= "section" IDENTIFIER "{" field_list "}";
field_list ::= { field };
field ::= "field" IDENTIFIER ":" field_type [ attributes_opt ] ";";
```

- Sections group related fields.
- Each field has a name, type, and optional attributes.

---

## 🔹 Field Types

```ebnf
field_type ::= "text"
             | "textarea"
             | "number_type"
             | "email"
             | "date"
             | "checkbox"
             | "radio"
             | "dropdown"
             | "password"
             | "filetype";
```

---

## 🔹 Field Attributes (Optional)

```ebnf
attributes_opt ::= attributes | /* empty */;
attributes ::= attribute { attribute };
```

```ebnf
attribute ::= "required"
            | "default" "=" STRING
            | "default" "=" "true"
            | "default" "=" "false"
            | "min" "=" value
            | "max" "=" value
            | "[" option_list "]"
            | "accept" "=" STRING
            | "pattern" "=" STRING
            | "placeholder" "=" STRING
            | "rows" "=" NUMBER
            | "cols" "=" NUMBER;
```

- Fields can have zero or more attributes such as default values, constraints, or UI hints.

---

## 🔹 Values & Options

```ebnf
value ::= NUMBER | STRING;
option_list ::= STRING { "," STRING };
```

- Used for dropdown/radio options:
  ```poram
  ["Yes", "No", "Maybe"]
  ```

---

## 🔹 Validation Rules

```ebnf
validate_block ::= "validate" "{" validation_rules "}";
validation_rules ::= { validation_rule };
```

```ebnf
validation_rule ::= "if" IDENTIFIER comparator value "{" "error" STRING ";" "}";
```

```ebnf
comparator ::= "<" | ">" | "==" | "!=" | "<=" | ">=";
```

- Example:
  ```poram
  validate {
    if age < 18 {
      error "You must be 18 or older.";
    }
  }
  ```

---

## 🧪 Example DSL Snippet

```poram
form Registration {
  meta author = "John";

  section PersonalInfo {
    field fullName: text required;
    field email: email required;
    field age: number_type min = 18 max = 99;
  }

  section Preferences {
    field gender: radio ["Male", "Female", "Other"];
    field subscribe: checkbox default = true;
  }

  validate {
    if age < 18 {
      error "You must be 18 or older.";
    }
  }
}
```
