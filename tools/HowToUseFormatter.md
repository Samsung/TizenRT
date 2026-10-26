# How to use the formatter

This document explains how to format C sources with the coding rules of
[CodingStyleGuide](../docs/CodingStyleGuide.md).

## Prerequisites

The C formatter requires **python3 only** (standard library, no pip
packages). There is no longer any dependency on GNU `indent`, `astyle`
or `perl`.

The C++ formatter (`cxxformatter.sh`) still requires `clang-format`.

## C formatter

### Usage

```bash
./tools/cformatter.sh os/kernel/sched/sched_addreadytorun.c   # one file
./tools/cformatter.sh os/kernel                               # a directory (recursive)
./tools/cformatter.sh --check os/kernel                        # report only (CI-friendly, exit 1 on violations)
./tools/cformatter.sh --diff os/kernel                         # print a unified diff, do not write
./tools/cformatter.sh --backup os/kernel                        # keep a *.orig copy before rewriting
./tools/cformatter.sh -v os/kernel                             # verbose per-file status
```

`cformatter.sh` is a thin wrapper; the implementation lives in
`tools/cformatter.py` and can be invoked directly:

```bash
python3 tools/cformatter.py [--check] [--diff] [--backup] [-v] TARGET...
```

### Applied rules

The formatter rewrites only whitespace between tokens. It applies the
mechanical rules of the C coding style guide:

| Rule | Description |
| ---- | ----------- |
| [M01] | tab-only indentation, tab width 4 |
| [M02] | `case`/`default` at the same indent level as `switch` |
| [M03] | keyword spacing (`if (`, `sizeof(`) |
| [M04] | operator spacing, no space after casts and unary operators (`~`, `!`, `&`, `*`, `-`, prefix/postfix `++`/`--`), right-bound `*` declarators |
| [M05] | separator spacing (`,`, `;`, parentheses, brackets) |
| [M06] | function opening brace starts the next line |
| [M07] | control opening brace at end of line, closing brace on its own line, `} else` / `} while` attached |
| [M08] | `struct`/`enum`/`union` opening brace on the same line |
| [R03] | goto labels at column 0 |
| [R05] | no trailing whitespace |
| [R06] | newline at end of file |
| [R09] | `#` of preprocessor directives at the first column |

### Layout policy

- The author's line breaks are kept. Lines are only split or joined
  where a brace rule requires it ([M06], [M07], [M08]).
- Continuation lines (wrapped arguments, conditions, initializers) keep
  their alignment relative to the first line of the statement; only the
  tab indentation of that first line is corrected ([M01]).
- Trailing comments stay on their line together with their alignment,
  and column alignment of declarations, assignments and table entries
  (two or more blanks) is kept.
- When the role of a token cannot be decided without type information,
  the original spacing is kept, e.g. `a * b` versus `type *name`, or
  `(x) - 1` versus `(type)-1`.
- `extern "C" {` blocks are not indented.
- `case` blocks (`case X: {` or `case X:` followed by `{` on the next
  line) keep the author's position of the `break;` after the block.
- A comment between `}` and `else`/`while`, or between a control header
  and its `{`, cannot be moved by the formatter. Such [M07] violations
  are reported as `file:line: warning: ...` by `--check`, `--diff` and
  `-v` (and make `--check` fail); move the comment by hand.
- Regions between `/* *INDENT-OFF* */` and `/* *INDENT-ON* */` (or
  `// clang-format off` / `// clang-format on`) and regions guarded by
  `__ASSEMBLY__`/`__ASSEMBLER__` are left verbatim.
- C++ sources (`class`, `namespace`, `template`, access specifiers) are
  skipped; use `cxxformatter.sh` for them.

### Safety guarantees

Before a file is rewritten, the formatter verifies that:

1. the token stream (identifiers, literals, comments, operators) is
   unchanged, so the code semantics cannot be altered;
2. formatting is idempotent (running it twice produces no further change).

If either check fails the file is left untouched and an error is
reported.

Regression tests for the formatter: `python3 tools/cformatter_test.py`.

### Limitations (rules not automated)

Rules that need semantic understanding are intentionally out of scope:
comment style [R01], code organization [R07], header split [M09],
`inline` placement [R02], prototype parameter names [M11], `do-while`
wrapping of macros [R08], macro hygiene [R09]/[R10], tag/variable split
[R11], one declaration per line [R12], naming [M12]/[R13]-[R16].
Please fix those manually.

## C++ formatter

`cxxformatter.sh` runs `clang-format` with the repository
[.clang-format](../.clang-format), which follows the C++ section of the
coding style guide:

```bash
./tools/cxxformatter.sh framework/src/media
```
