#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
cformatter.py -- Style formatter for TizenRT C sources.

Automatically applies the mechanical C rules of
docs/CodingStyleGuide.md:

  [M01] tab-only indentation (tab width 4)
  [M02] 'case'/'default' at the same indent level as 'switch'
  [R03] goto labels start at column 0
  [M03] keyword spacing rules
  [M04] operator spacing rules (no space after casts and unary
         operators, right-bound '*')
  [M05] separator spacing rules
  [M06] function opening brace starts the next line
  [M07] control opening brace stays at the end of the line; the
         closing brace stands alone (exceptions: '} else', '} while')
  [M08] struct/enum/union opening brace stays on the same line
  [R05] no trailing whitespace
  [R06] file ends with a newline
  [R09] '#' of a preprocessor directive at the first column

Rules that need semantic understanding are intentionally NOT
automated: [R01], [R07], [M09], [M11], [R08]-[R12], [M12],
[R13]-[R16].

The formatter only rewrites whitespace between tokens.  The author's
line breaks are kept; lines are only split or joined where a brace
rule requires it.  Continuation lines keep their alignment relative
to the first line of the statement.  Where the role of a token cannot
be decided without type information (for example 'a * b' versus
'type *name', or '(x) - 1' versus '(type)-1'), the original spacing
is kept.

Before any output is written, a safety check verifies that the token
stream (identifiers, literals, comments, operators) is unchanged and
that formatting is idempotent.

Usage:
    python3 cformatter.py [--check] [--diff] [--backup] [-v] TARGET...

TARGET is a .c/.h file or a directory (searched recursively).
"""

import argparse
import difflib
import os
import re
import sys

# --------------------------------------------------------------------------
# Rule tables (docs/CodingStyleGuide.md, C section)

# [M03] keywords that take a space after them
KEYWORDS_SPACE_AFTER = {
    'if', 'else', 'for', 'while', 'switch', 'case', 'default', 'do',
    'return', 'goto', 'break', 'continue', 'typedef', 'extern',
    'static', 'register', 'volatile', 'const', 'inline', 'auto',
}

# [M03] function-like keywords: no space after
KEYWORDS_NO_SPACE_AFTER = {
    'sizeof', 'typeof', 'alignof', '_Alignof', '__attribute__',
}

TYPE_KEYWORDS = {
    'void', 'char', 'short', 'int', 'long', 'float', 'double',
    'signed', 'unsigned', 'struct', 'union', 'enum', 'const',
    'volatile', 'register', 'extern', 'static', 'inline', 'auto',
    'restrict', '_Bool', 'bool', '_Atomic', 'FAR', 'NEAR', 'typedef',
}

BINARY_OPS = {
    '=', '+', '-', '*', '/', '%', '|', '&', '^', '<', '>',
    '<<', '>>', '<=', '>=', '==', '!=', '&&', '||',
    '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<=', '>>=',
}

# operators that may be unary or binary depending on the context
UNARY_OR_BINARY = ('*', '&', '+', '-')

OP_TOKENS = [
    '<<=', '>>=', '...', '->', '++', '--', '<<', '>>', '<=', '>=',
    '==', '!=', '&&', '||', '+=', '-=', '*=', '/=', '%=', '&=',
    '|=', '^=', '=', '+', '-', '*', '/', '%', '&', '|', '^', '~',
    '!', '<', '>', '.', '?', ':', ',', ';', '(', ')', '[', ']',
    '{', '}',
]

_OP_RE = re.compile('|'.join(re.escape(op) for op in OP_TOKENS))

C_SUFFIX = ('.c', '.h')

TAB_WIDTH = 4


class Tok(object):
    __slots__ = ('kind', 'text', 'nl_before', 'ws', 'lead', 'line')

    def __init__(self, kind, text, nl_before, ws, lead, line=0):
        self.kind = kind
        self.text = text
        self.nl_before = nl_before      # newlines before the token
        self.ws = ws                    # blanks right before the token
        self.lead = lead                # indentation of its source line
        self.line = line                # source line number (1-based)

    def __repr__(self):
        return 'Tok(%s, %r)' % (self.kind, self.text[:40])


_IDENT_RE = re.compile(r'[A-Za-z_]\w*')
_SPLICE_RE = re.compile(r'\\[ \t\r]*\n')
_NUM_RE = re.compile(r'\.?\d[\w.]*(?:[eEpP][+-][\w.]*)?')


def tokenize(text):
    """Split *text* into tokens.  Every token remembers how many
    newlines preceded it, the blanks right before it and the
    indentation of the source line it is on.  Preprocessor directives
    (including backslash continuations) are kept as one verbatim
    token ([R09], untouched content)."""
    toks = []
    i, n = 0, len(text)
    nl = 0
    ws = ''
    lead = ''
    line_start = True
    line = 1

    while i < n:
        c = text[i]
        if c == '\n':
            nl += 1
            line += 1
            ws = ''
            line_start = True
            i += 1
            continue
        if c in ' \t\r\f\v':
            ws += c if c in ' \t' else ''
            i += 1
            continue
        m = _SPLICE_RE.match(text, i)
        if m:
            # line splice outside a directive: plain line break
            nl += 1
            line += 1
            ws = ''
            line_start = True
            i = m.end()
            continue
        if line_start:
            lead = ws
        if line_start and c == '#':
            j = i
            while True:
                k = text.find('\n', j)
                if k == -1:
                    j = n
                    break
                if text[j:k].rstrip(' \t\r').endswith('\\'):
                    j = k + 1
                else:
                    j = k
                    break
            kind, end = 'PREPROC', j
        elif text.startswith('/*', i):
            j = text.find('*/', i + 2)
            kind, end = 'COMMENT', n if j == -1 else j + 2
        elif text.startswith('//', i):
            j = text.find('\n', i)
            kind, end = 'COMMENT', n if j == -1 else j
        elif c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c and text[j] != '\n':
                if text[j] == '\\':
                    j += 1
                j += 1
            if j < n and text[j] == c:
                j += 1
            kind, end = ('STR' if c == '"' else 'CHAR'), j
        else:
            m = _IDENT_RE.match(text, i) or _NUM_RE.match(text, i)
            if m:
                kind = 'IDENT' if _IDENT_RE.match(m.group(0)) else 'NUM'
                end = m.end()
            else:
                m = _OP_RE.match(text, i)
                kind, end = 'OP', m.end() if m else i + 1
        tok_text = text[i:end]
        if kind in ('PREPROC', 'COMMENT'):
            tok_text = tok_text.rstrip(' \t\r')
        toks.append(Tok(kind, tok_text, nl, ws, lead, line))
        line += text.count('\n', i, end)
        nl = 0
        ws = ''
        line_start = False
        i = end
    return toks


# --------------------------------------------------------------------------
# Analysis passes


def is_op(t, *texts):
    return t is not None and t.kind == 'OP' and t.text in texts


def is_ident(t, *texts):
    return t is not None and t.kind == 'IDENT' and \
        (not texts or t.text in texts)


def pp_kind(t):
    """'if', 'else', 'endif' or None for a PREPROC token."""
    m = re.match(r'#\s*(\w+)', t.text)
    word = m.group(1) if m else ''
    if word in ('if', 'ifdef', 'ifndef'):
        return 'if'
    if word in ('elif', 'else', 'elifdef', 'elifndef'):
        return 'else'
    if word == 'endif':
        return 'endif'
    return None


class PPStack(object):
    """Conditional compilation tracker.  Each #if group is evaluated as
    if its first branch was taken: an #else/#elif branch starts from the
    state of the #if line, and after #endif the state at the end of the
    first branch is restored.  This keeps brace matching and the
    indentation depth consistent when braces are unbalanced inside
    preprocessor branches."""

    def __init__(self):
        self.frames = []

    def handle(self, tok, get, put):
        kind = pp_kind(tok)
        if kind == 'if':
            self.frames.append([get(), None])
        elif kind == 'else' and self.frames:
            frame = self.frames[-1]
            if frame[1] is None:
                frame[1] = get()
            put(frame[0])
        elif kind == 'endif' and self.frames:
            start, first_end = self.frames.pop()
            if first_end is not None:
                put(first_end)


def match_pairs(toks, opening, closing):
    """Map index of each *opening* to its matching *closing* (and back).
    Preprocessor branches are handled like PPStack describes."""
    stack, match = [], {}
    pp = PPStack()

    def get():
        return list(stack)

    def put(s):
        stack[:] = s

    for idx, t in enumerate(toks):
        if t.kind == 'PREPROC':
            pp.handle(t, get, put)
        elif t.kind == 'OP':
            if t.text == opening:
                stack.append(idx)
            elif t.text == closing and stack:
                o = stack.pop()
                match[o] = idx
                match[idx] = o
    return match


def prev_code(toks, i):
    """Index of the nearest token before *i* that is not a comment."""
    j = i - 1
    while j >= 0 and toks[j].kind == 'COMMENT':
        j -= 1
    return j


def prev_sig(toks, i):
    """Index of the nearest token before *i*, skipping comments and
    preprocessor directives."""
    j = i - 1
    while j >= 0 and toks[j].kind in ('COMMENT', 'PREPROC'):
        j -= 1
    return j


def next_sig(toks, i):
    j = i + 1
    while j < len(toks) and toks[j].kind in ('COMMENT', 'PREPROC'):
        j += 1
    return j


def sig_back(toks, i, pmatch):
    """Like prev_sig, but also skips balanced '(...)' groups and
    '__attribute__', so the token before a parenthesized header is
    found ('if (...) {' -> 'if')."""
    j = prev_sig(toks, i)
    while j >= 0:
        t = toks[j]
        if is_ident(t, '__attribute__'):
            j = prev_sig(toks, j)
            continue
        if is_op(t, ')') and j in pmatch:
            j = prev_sig(toks, pmatch[j])
            continue
        return j
    return -1


def find_labels(toks, pmatch):
    """Return (case_head, case_colon, label_head) index sets."""
    n = len(toks)
    case_head, case_colon, label_head = set(), set(), set()
    label_colon = set()
    for idx, t in enumerate(toks):
        if t.kind != 'IDENT':
            continue
        if t.text in ('case', 'default'):
            j = idx + 1
            while j < n:
                tt = toks[j]
                if is_op(tt, '(') and j in pmatch:
                    j = pmatch[j]
                elif is_op(tt, ':'):
                    case_head.add(idx)
                    case_colon.add(j)
                    break
                elif is_op(tt, ';', '{', '}'):
                    break
                j += 1
        else:
            j = idx + 1
            while j < n and toks[j].kind == 'COMMENT':
                j += 1
            if not (j < n and is_op(toks[j], ':')):
                continue
            k = prev_sig(toks, idx)
            pv = toks[k] if k >= 0 else None
            if pv is None or is_op(pv, ';', '{', '}') or \
                    (is_op(pv, ':') and (k in case_colon or k in label_colon)):
                label_head.add(idx)
                label_colon.add(j)
    return case_head, case_colon, label_head


def classify_braces(toks, pmatch, case_colon, label_colon):
    """Classify every '{' token as one of:
       'func'   -- function body definition      -> [M06] next line
       'ctrl'   -- control statement block       -> [M07] end of line
       'type'   -- struct/enum/union definition  -> [M08] same line
       'block'  -- bare compound statement
       'extern' -- extern "C" block (not indented)
       'init'   -- initializer / statement-expr  -> keep layout
    """
    bclass = {}
    stack = []                         # classes of the open braces
    pp = PPStack()

    def get():
        return list(stack)

    def put(s):
        stack[:] = s

    for idx, t in enumerate(toks):
        if t.kind == 'PREPROC':
            pp.handle(t, get, put)
            continue
        if is_op(t, '}'):
            if stack:
                stack.pop()
            continue
        if not is_op(t, '{'):
            continue
        cls = _classify_brace(toks, idx, pmatch, case_colon, label_colon,
                              stack)
        bclass[idx] = cls
        stack.append(cls)
    return bclass


def _classify_brace(toks, idx, pmatch, case_colon, label_colon, stack):
    if 'init' in stack:
        return 'init'
    depth = len([c for c in stack if c != 'extern'])
    j = prev_sig(toks, idx)
    prev = toks[j] if j >= 0 else None
    if prev is None:
        return 'block'
    if prev.kind == 'STR' and prev.text == '"C"' and \
            is_ident(toks[prev_sig(toks, j)] if j > 0 else None, 'extern'):
        return 'extern'
    if prev.kind == 'OP':
        if prev.text == ':':
            if j in case_colon:
                return 'caseblock'
            return 'block' if j in label_colon else 'init'
        if prev.text in (';', '}', '{'):
            return 'block'
        if prev.text == ')':
            k = sig_back(toks, idx, pmatch)
            bt = toks[k] if k >= 0 else None
            if bt is None or bt.kind != 'IDENT':
                return 'init'          # compound literal, '({ ... })'
            if bt.text in ('struct', 'union', 'enum'):
                return 'type'
            if bt.text in ('if', 'while', 'for', 'switch'):
                return 'ctrl'
            if bt.text in KEYWORDS_SPACE_AFTER or \
                    bt.text in KEYWORDS_NO_SPACE_AFTER:
                return 'init'
            # 'name(...) {': a function at file scope, a loop macro
            # inside a function body
            return 'func' if depth == 0 else 'ctrl'
        return 'init'
    if prev.kind == 'IDENT':
        if prev.text in ('do', 'else'):
            return 'ctrl'
        if prev.text in ('struct', 'union', 'enum'):
            return 'type'
        k = prev_sig(toks, j)
        if k >= 0 and is_ident(toks[k], 'struct', 'union', 'enum'):
            return 'type'
        return 'block'
    return 'init'


def find_casts(toks, pmatch):
    """Classify parenthesized groups that may be cast operators.
    Returns (casts, maybe) sets of ')' indices: *casts* certainly close
    a cast ([M04]: no space after it), *maybe* could be a cast or a
    parenthesized expression ('(x) - 1') and keep their spacing."""
    casts, maybe = set(), set()
    for open_, close in pmatch.items():
        if open_ > close or not is_op(toks[open_], '('):
            continue
        inner = [t for t in toks[open_ + 1:close] if t.kind != 'COMMENT']
        if not inner or inner[0].kind != 'IDENT':
            continue
        typed = _is_type_name(inner[0])
        allowed = ('*', '(', ')', '[', ']') if typed else ('*',)
        if any(t.kind not in ('IDENT', 'OP', 'NUM') or
               (t.kind == 'NUM' and not typed) or
               (t.kind == 'OP' and t.text not in allowed) for t in inner):
            continue           # '(int (*)(void))' is a cast as well
        # the token before '(' decides function call vs cast
        j = prev_code(toks, open_)
        bt = toks[j] if j >= 0 else None
        if bt is not None:
            if bt.kind == 'IDENT' and bt.text not in ('return', 'case'):
                continue           # call foo(x), if (x), sizeof (x)
            if is_op(bt, ')', ']') or bt.kind in ('NUM', 'STR', 'CHAR'):
                continue           # call through expression: (*fp)(x)
        idents = [t for t in inner if t.kind == 'IDENT']
        first = idents[0].text
        if first in TYPE_KEYWORDS or first.endswith(('_t', '_s')) \
                or is_op(inner[-1], '*') \
                or (len(idents) > 1 and not any(t.kind == 'OP'
                                                for t in inner)):
            casts.add(close)
            continue
        if len(inner) != 1:
            continue                 # '(a * b)'
        k = close + 1
        while k < len(toks) and toks[k].kind == 'COMMENT':
            k += 1
        nt = toks[k] if k < len(toks) else None
        if nt is None:
            continue
        if nt.kind in ('IDENT', 'NUM', 'STR', 'CHAR') or \
                is_op(nt, '~', '!'):
            casts.add(close)         # '(x) y' can only be a cast
        elif is_op(nt, '*', '&', '-', '+', '(', '++', '--'):
            maybe.add(close)
    return casts, maybe


def _is_type_name(t):
    return t.kind == 'IDENT' and (t.text in TYPE_KEYWORDS or
                                  t.text.endswith(('_t', '_s')))


def decl_pointer_indices(toks):
    """Indices of '*' tokens used in pointer declarations.
    [M04]: the '*' binds to the declarator (right-bound).
    Heuristic: the '*' follows a type keyword, a struct/union/enum
    tag, a typedef-looking name (*_t/*_s) or another declarator '*'."""
    decls = set()
    for idx, t in enumerate(toks):
        if not is_op(t, '*'):
            continue
        j = prev_code(toks, idx)
        prev = toks[j] if j >= 0 else None
        if prev is None:
            continue
        if prev.kind == 'IDENT':
            if _is_type_name(prev):
                decls.add(idx)
            else:
                k = prev_code(toks, j)
                if k >= 0 and is_ident(toks[k], 'struct', 'union', 'enum'):
                    decls.add(idx)
        elif is_op(prev, '*') and j in decls:
            decls.add(idx)
    return decls


def ternary_colons(toks, pmatch):
    """Indices of ':' tokens belonging to a '?:' operator."""
    result = set()
    for idx, t in enumerate(toks):
        if not is_op(t, ':'):
            continue
        balance = 0
        j = idx - 1
        while j >= 0:
            tt = toks[j]
            if tt.kind == 'OP':
                if tt.text in (')', ']') and j in pmatch:
                    j = pmatch[j] - 1
                    continue
                if tt.text in (';', '{', '}', '(', '['):
                    break
                if tt.text == ':':
                    balance -= 1
                elif tt.text == '?':
                    balance += 1
                    if balance > 0:
                        result.add(idx)
                        break
            elif tt.kind == 'PREPROC':
                break
            j -= 1
    return result


class Analysis(object):
    def __init__(self, toks):
        self.toks = toks
        self.pmatch = match_pairs(toks, '(', ')')
        self.smatch = match_pairs(toks, '[', ']')
        self.pmatch.update(self.smatch)
        self.bmatch = match_pairs(toks, '{', '}')
        self.case_head, self.case_colon, self.label_head = \
            find_labels(toks, self.pmatch)
        label_colon = set()
        for h in self.label_head:
            j = h + 1
            while toks[j].kind == 'COMMENT':
                j += 1
            label_colon.add(j)
        self._init_ranges = None
        self.bclass = classify_braces(toks, self.pmatch,
                                      self.case_colon, label_colon)
        # old GNU designators '{ name: value }' are not labels
        for h in list(self.label_head):
            if self._in_init(h):
                self.label_head.discard(h)
                label_colon.discard(h + 1)
        self.label_colon = label_colon
        self.casts, self.maybe_casts = find_casts(toks, self.pmatch)
        self.decls = decl_pointer_indices(toks)
        self.ternary = ternary_colons(toks, self.pmatch)
        self.do_close = set()
        for idx, t in enumerate(toks):
            if is_ident(t, 'do'):
                j = next_sig(toks, idx)
                if j in self.bmatch:
                    self.do_close.add(self.bmatch[j])
        self.switch_brace = set()
        for idx, cls in self.bclass.items():
            if cls == 'ctrl':
                k = sig_back(toks, idx, self.pmatch)
                if k >= 0 and is_ident(toks[k], 'switch'):
                    self.switch_brace.add(idx)
        self.warnings = []          # (line, message) for a manual fix
        self.asm_region = asm_region_indices(toks) | \
            indent_off_indices(toks)

    def _in_init(self, idx):
        if self._init_ranges is None:
            self._init_ranges = [(o, c) for o, c in self.bmatch.items()
                                 if o < c and self.bclass.get(o) == 'init']
        return any(o < idx < c for o, c in self._init_ranges)

    def brace_class(self, i):
        """Class of the '{' or '}' at index *i*."""
        if is_op(self.toks[i], '}'):
            o = self.bmatch.get(i)
            return self.bclass.get(o, 'block') if o is not None \
                else 'block'
        return self.bclass.get(i, 'block')


_ASM_GUARD_RE = re.compile(
    r'#\s*(?:ifdef\s+__ASSEMBL|if\s+.*__ASSEMBL)\w*__')


def asm_region_indices(toks):
    """Token indices inside '#ifdef __ASSEMBLY__'-style blocks.
    Assembly code must never be reformatted as C, so those regions
    are kept verbatim."""
    regions = set()
    n = len(toks)
    for i, t in enumerate(toks):
        if t.kind != 'PREPROC' or not _ASM_GUARD_RE.match(t.text):
            continue
        depth = 0
        j = i + 1
        while j < n:
            tt = toks[j]
            if tt.kind == 'PREPROC':
                kind = pp_kind(tt)
                if kind == 'if':
                    depth += 1
                elif kind == 'endif':
                    if depth == 0:
                        break
                    depth -= 1
                elif kind == 'else' and depth == 0:
                    break
            regions.add(j)
            j += 1
    return regions


_INDENT_OFF_RE = re.compile(r'^(?://|/\*)\s*(\*INDENT-OFF\*|clang-format off)')
_INDENT_ON_RE = re.compile(r'^(?://|/\*)\s*(\*INDENT-ON\*|clang-format on)')


def indent_off_indices(toks):
    """Token indices between '*INDENT-OFF*' / 'clang-format off' and
    '*INDENT-ON*' / 'clang-format on' comments, kept verbatim."""
    regions = set()
    off = False
    for i, t in enumerate(toks):
        if t.kind == 'COMMENT':
            if _INDENT_OFF_RE.match(t.text):
                off = True
                continue
            if _INDENT_ON_RE.match(t.text):
                off = False
        if off:
            regions.add(i)
    return regions


def is_cplusplus(toks):
    """True when the token stream contains C++ only constructs; such
    headers are left to cxxformatter.sh (clang-format)."""
    for i, t in enumerate(toks[:-2]):
        if t.kind != 'IDENT':
            continue
        nt, nnt = toks[i + 1], toks[i + 2]
        if t.text in ('namespace', 'class') and nt.kind == 'IDENT' and \
                is_op(nnt, '{', ':'):
            return True
        if t.text == 'template' and is_op(nt, '<'):
            return True
        if t.text in ('public', 'private', 'protected') and \
                is_op(nt, ':') and not is_op(nnt, ':'):
            return True
    return False


# --------------------------------------------------------------------------
# Whitespace rules ([M03] keywords, [M04] operators, [M05] separators)


def _is_postfix(toks, i):
    """True when the '++'/'--' at *i* is a postfix operator."""
    j = prev_code(toks, i)
    if j < 0:
        return False
    p = toks[j]
    if p.kind == 'IDENT':
        return p.text not in KEYWORDS_SPACE_AFTER and \
            p.text not in KEYWORDS_NO_SPACE_AFTER
    return p.kind in ('NUM', 'STR', 'CHAR') or is_op(p, ')', ']')


def _is_unary_pos(toks, pi, a):
    """True when toks[pi] ('*', '&', '+', '-') is a unary operator."""
    k = prev_code(toks, pi)
    if k < 0:
        return True
    q = toks[k]
    if q.kind == 'IDENT':
        return q.text in KEYWORDS_SPACE_AFTER or \
            q.text in KEYWORDS_NO_SPACE_AFTER or \
            q.text in ('struct', 'union', 'enum')
    if q.kind in ('NUM', 'STR', 'CHAR'):
        return False
    if q.kind == 'PREPROC':
        return True
    if q.text in ('++', '--'):
        return not _is_postfix(toks, k)
    if q.text == ')':
        return k in a.casts
    if q.text == '}':
        return a.brace_class(k) != 'init'   # a new statement follows
    return q.text != ']'


def base_gap(toks, pi, i, a):
    """Separator between the token *pi* and toks[i] on the same line:
    'none', 'space' or 'keep' (the original spacing is kept because
    the rule cannot be decided without type information)."""
    if pi < 0:
        return 'none'
    p, t = toks[pi], toks[i]
    pt = p.text if p.kind == 'OP' else None
    tt = t.text if t.kind == 'OP' else None

    if p.kind == 'COMMENT' or t.kind == 'COMMENT':
        return 'keep'

    # ---- separators ([M05]) ---------------------------------------------
    if tt == '}' and pt == ',' and a.brace_class(i) == 'init':
        return 'keep'                   # '{0,}'
    if pt == ',':
        return 'space'                  # [M05] space after comma
    if tt == ';' and pt == ')' and _ends_header(toks, pi, a):
        return 'keep'                   # empty loop body 'for (...) ;'
    if tt in (',', ';'):
        return 'none'
    if tt == '{':
        if a.brace_class(i) == 'init' and pt in ('(', '['):
            return 'keep'
        return 'space'
    if pt == '{':
        return 'keep' if a.brace_class(pi) == 'init' else 'space'
    if tt == '}':
        return 'keep' if a.brace_class(i) == 'init' else 'space'
    if pt == '}':
        if tt in ('.', ')', ']', '->', '(', '[', '++', '--'):
            return 'none'
        return 'space'                  # [M05] '} else' / '} name'
    if tt in (')', ']'):
        return 'none'                   # [M05] nothing inside parens
    if pt in ('(', '['):
        return 'none'
    if tt in ('.', '->') or pt in ('.', '->'):
        return 'none'                   # [M04] member operators
    if t.kind == 'NUM' and pt == '.':
        return 'space'                  # never merge '. 0' into '.0'

    # ---- '(' and '[' ----------------------------------------------------
    if tt == '[':
        if p.kind in ('IDENT', 'NUM', 'STR', 'CHAR') and \
                p.text not in KEYWORDS_SPACE_AFTER or pt in (')', ']'):
            return 'none'               # [M05] 'array[i]'
        return 'space'
    if tt == '(':
        if p.kind == 'IDENT':
            if p.text in KEYWORDS_SPACE_AFTER:
                return 'space'          # [M03] 'if ('
            if p.text in KEYWORDS_NO_SPACE_AFTER:
                return 'none'           # [M03] 'sizeof('
            k = i + 1
            while k < len(toks) and toks[k].kind == 'COMMENT':
                k += 1
            if k < len(toks) and is_op(toks[k], '*'):
                # 'type (*fp)(...)' declaration or 'call(*ptr)'
                return 'space' if _is_type_name(p) else 'keep'
            return 'none'               # [M05] call: 'name('
        if pt == ')':
            return 'keep' if pi in a.maybe_casts else 'none'
        if pt == ']':
            return 'none'

    # ---- '++' / '--' ([M04]) --------------------------------------------
    if tt in ('++', '--'):
        if _is_postfix(toks, i):
            return 'none'               # 'i++'
        # prefix: spaced like any other operand
    if pt in ('++', '--'):
        if not _is_postfix(toks, pi):
            return 'none'               # '++i'
        return 'space'                  # 'i++ & 1'

    # ---- ':' -------------------------------------------------------------
    if tt == ':':
        if i in a.case_colon or i in a.label_colon:
            return 'none'
        return 'space' if i in a.ternary else 'keep'
    if pt == ':':
        if pi in a.ternary or pi in a.case_colon or pi in a.label_colon:
            return 'space'
        return 'keep'                   # bit-field width
    if tt == '?' or pt == '?':
        return 'space'

    # ---- casts ([M04]) ---------------------------------------------------
    if pt == ')':
        if pi in a.casts:
            return 'none'               # [M04] no space after cast
        if pi in a.maybe_casts and tt in UNARY_OR_BINARY:
            return 'keep'               # '(x) - 1' or '(type)-1'
        return 'space'
    if pt in UNARY_OR_BINARY:
        j = prev_code(toks, pi)
        if j >= 0 and j in a.maybe_casts:
            return 'keep'
        if pt == '*' and pi in a.decls:
            return 'none'               # [M04] right-bound declarator '*'
        if pt == '*' and j >= 0 and toks[j].kind == 'IDENT' and \
                toks[j].text not in KEYWORDS_SPACE_AFTER and \
                t.kind == 'IDENT':
            return 'keep'               # 'a * b' or 'type *name'
        return 'none' if _is_unary_pos(toks, pi, a) else 'space'
    if tt in UNARY_OR_BINARY:
        if p.kind == 'IDENT' and p.text not in KEYWORDS_SPACE_AFTER \
                and tt == '*' and i not in a.decls:
            k = i + 1
            while k < len(toks) and toks[k].kind == 'COMMENT':
                k += 1
            if k < len(toks) and toks[k].kind == 'IDENT':
                return 'keep'           # 'a * b' or 'type *name'
        return 'space'

    # ---- unary and binary operators ([M04]) -----------------------------
    if pt in ('!', '~'):
        return 'none'                   # [M04] unary operators
    if pt in BINARY_OPS or tt in BINARY_OPS:
        return 'space'                  # [M04] binary operators

    # ---- keywords ([M03]) and everything else ----------------------------
    if p.kind == 'IDENT' and p.text in KEYWORDS_NO_SPACE_AFTER:
        return 'space'                  # 'sizeof x': never merge idents
    if pt == ';':
        return 'none' if tt == ';' else 'space'      # 'for (;;)'
    return 'space'


def _would_merge(left, right):
    """True when writing *left* and *right* without a blank between
    them would lex differently."""
    if left.kind in ('IDENT', 'NUM') and right.kind in ('IDENT', 'NUM'):
        return True
    if left.kind == 'OP' and right.kind == 'OP':
        both = left.text + right.text
        return any(both.startswith(op) and len(op) > len(left.text)
                   for op in OP_TOKENS)
    if left.kind == 'OP' and left.text == '.' and right.kind == 'NUM':
        return True
    if left.kind == 'OP' and left.text == '/' and \
            right.text.startswith(('*', '/')):
        return True
    return False


# --------------------------------------------------------------------------
# Layout ([M01], [M02], [M06], [M07], [M08], [R03], [R09])


def _cols(ws):
    col = 0
    for c in ws:
        col = (col // TAB_WIDTH + 1) * TAB_WIDTH if c == '\t' else col + 1
    return col


def _shift_comment(text, old_lead, new_lead):
    """Re-indent the continuation lines of a block comment that moved
    from *old_lead* to *new_lead* indentation."""
    if '\n' not in text or old_lead == new_lead:
        return '\n'.join(ln.rstrip(' \t') for ln in text.split('\n'))
    lines = text.split('\n')
    out = [lines[0].rstrip(' \t')]
    for ln in lines[1:]:
        ln = ln.rstrip(' \t')
        if ln and ln.startswith(old_lead):
            ln = new_lead + ln[len(old_lead):]
        out.append(ln)
    return '\n'.join(out)


class _State(object):
    """Layout state while emitting tokens.  Every open '{' is kept on
    *stack* as (class, outer indent, inner indent): the closing brace
    goes back to the outer indent, the statements inside use the inner
    one."""
    __slots__ = ('paren', 'open', 'body', 'stmt_old', 'stmt_new',
                 'stack', 'after_case', 'last_close')

    def __init__(self):
        self.paren = 0          # '(' nesting
        self.open = False       # inside a statement (continuation)
        self.body = False       # expecting a brace-less control body
        self.stmt_old = ''      # original indent of the statement line
        self.stmt_new = ''      # new indent of the statement line
        self.stack = []
        self.after_case = None  # (indent, source indent) of a closed
                                # 'case X: { ... }' block
        self.last_close = None  # outer indent of the last closed block

    def copy(self):
        s = _State()
        for name in _State.__slots__:
            setattr(s, name, getattr(self, name))
        s.stack = list(self.stack)
        return s

    def block(self):
        """(class, outer, inner) of the innermost non-initializer
        brace, or None at file scope."""
        for entry in reversed(self.stack):
            if entry[0] != 'init':
                return entry
        return None

    def indent(self):
        b = self.block()
        return b[2] if b else ''


def _ends_header(toks, i, a):
    """True when toks[i] ends a control statement header ('if (...)',
    'for (...)', 'while (...)', 'switch (...)', 'else', 'do')."""
    t = toks[i]
    if is_ident(t, 'else', 'do'):
        return True
    if is_op(t, ')') and i in a.pmatch:
        k = prev_code(toks, a.pmatch[i])
        return k >= 0 and is_ident(toks[k], 'if', 'for', 'while', 'switch')
    return False


def layout(toks, a):
    """Produce the formatted text from the token stream."""
    lines = []
    cur = []
    st = _State()
    pp = PPStack()

    def get():
        return st.copy()

    def put(s):
        for name in _State.__slots__:
            setattr(st, name, getattr(s, name))

    line_new = ''
    for i, t in enumerate(toks):
        pi = prev_code(toks, i)
        im = toks[i - 1] if i > 0 else None
        ptok = toks[pi] if pi >= 0 else None
        cls = a.brace_class(i) if is_op(t, '{', '}') else None

        if t.kind == 'PREPROC':
            pp.handle(t, get, put)

        # ---- line break decision -------------------------------------
        nl = t.nl_before > 0 or i == 0
        gap = None
        if i in a.asm_region:
            pass
        elif t.kind == 'PREPROC' or (im is not None and
                                     (im.kind == 'PREPROC' or
                                      (im.kind == 'COMMENT' and
                                       im.text.startswith('//')))):
            nl = True
        elif is_op(t, '{') and cls in ('ctrl', 'switch', 'type'):
            if nl and not (im.kind == 'COMMENT' and im.nl_before > 0):
                nl, gap = False, ' '    # [M07]/[M08] brace ends the header
        elif is_op(t, '{') and cls == 'func':
            nl = True                                   # [M06]
        elif is_op(t, '}') and cls != 'init':
            nl = True                                   # [M07]
        elif (is_ident(t, 'else') and is_op(im, '}')) or \
                (is_ident(t, 'while') and pi in a.do_close and pi == i - 1):
            if a.brace_class(pi) != 'init':
                nl, gap = False, ' '                    # [M07]
        elif t.kind == 'COMMENT' and not nl:
            pass                                        # trailing comment
        elif ptok is not None and is_op(ptok, '{') and \
                a.brace_class(pi) != 'init':
            nl = True                     # statement after '{' ([M07])
        elif ptok is not None and is_op(ptok, '}') and \
                a.brace_class(pi) != 'init':
            attach = is_op(t, ';', ',', ')') or is_ident(t, 'else') or \
                (is_ident(t, 'while') and pi in a.do_close) or \
                a.brace_class(pi) == 'type'
            if not attach:
                nl = True
        elif i in a.case_head and not nl:
            k = prev_code(toks, i)
            if k >= 0 and not (is_op(toks[k], ':') and k in a.case_colon):
                nl = True                               # [M02]
        elif i in a.label_head:
            nl = True                                   # [R03]

        # ---- [M07] violations that only moving a comment can fix -------
        if nl and im is not None and im.kind == 'COMMENT' and \
                i not in a.asm_region:
            if is_op(t, '{') and cls in ('ctrl', 'switch', 'type'):
                a.warnings.append((t.line, '[M07] opening brace cannot be '
                                   'attached to its header: move the '
                                   'comment'))
            elif ptok is not None and is_op(ptok, '}') and \
                    a.brace_class(pi) != 'init' and \
                    (is_ident(t, 'else') or
                     (is_ident(t, 'while') and pi in a.do_close)):
                a.warnings.append((t.line, "[M07] '%s' should follow '}': "
                                   'move the comment' % t.text))

        # ---- a closing brace returns to the outer indent ---------------
        closing = None
        if is_op(t, '}') and st.stack:
            closing = st.stack.pop()
            st.after_case = (closing[1], t.lead) \
                if closing[0] == 'caseblock' else None
            st.last_close = closing[1] if closing[0] != 'init' else None

        # ---- emit -----------------------------------------------------
        text = t.text
        # first token of a (sub)statement; a brace attached to its
        # header belongs to the header's statement
        starts = not st.open and not (is_op(t, '{') and not nl)
        if nl:
            if cur:
                lines.append(''.join(cur).rstrip(' \t'))   # [R05]
                cur = []
                if t.nl_before >= 2:
                    lines.extend([''] * (t.nl_before - 1))
            indent = _indent(toks, i, t, st, a, cls, closing)
            if t.kind == 'COMMENT':
                text = _shift_comment(text, t.lead if t.nl_before else
                                      indent, indent)
            cur.append(indent)
            line_new = indent
        else:
            if gap is None:
                gap = _gap_text(toks, i, pi, im, a)
            if t.kind == 'COMMENT':
                text = _shift_comment(text, '', '')
            cur.append(gap)
        cur.append(text)

        # ---- statement state -----------------------------------------
        if t.kind in ('COMMENT', 'PREPROC'):
            continue
        if starts:
            st.stmt_old = t.lead
            st.stmt_new = line_new
        if not is_op(t, '}'):
            st.after_case = None
            st.last_close = None
        st.body = False
        if is_op(t, '('):
            st.paren += 1
        elif is_op(t, ')'):
            st.paren = max(0, st.paren - 1)
        if is_op(t, '{'):
            if cls == 'init':
                st.stack.append(('init', None, None))
                st.open = True
            elif cls == 'extern':
                st.stack.append((cls, st.indent(), st.indent()))
                st.open = False
            else:
                st.stack.append((cls, st.stmt_new, st.stmt_new + '\t'))
                st.open = False
        elif is_op(t, '}'):
            st.open = cls == 'init'
        elif is_op(t, ';') and st.paren == 0:
            st.open = False
        elif is_op(t, ':') and (i in a.case_colon or i in a.label_colon):
            st.open = False
        elif st.paren == 0 and _ends_header(toks, i, a):
            st.open, st.body = False, True
        else:
            st.open = True

    if cur:
        lines.append(''.join(cur).rstrip(' \t'))
    while lines and lines[-1] == '':
        lines.pop()
    if not lines:
        return ''
    return '\n'.join(lines) + '\n'                   # [R06]


def _gap_text(toks, i, pi, im, a):
    """Blanks written between toks[i] and the token before it on the
    same output line."""
    t = toks[i]
    if i in a.asm_region:
        return t.ws
    g = base_gap(toks, i - 1 if im is not None and im.kind == 'COMMENT'
                 else pi, i, a)
    if g == 'keep':
        if t.kind == 'COMMENT' or (im is not None and im.kind == 'COMMENT'):
            return t.ws                 # e.g. aligned trailing comments
        g = 'space' if t.ws else 'none'
    if g == 'space':
        p = toks[pi] if pi >= 0 else None
        if (len(t.ws) > 1 or '\t' in t.ws) and p is not None and (
                (t.kind == 'OP' and t.text in BINARY_OPS and
                 t.text.endswith('=') and t.text not in
                 ('==', '!=', '<=', '>=')) or
                (p.kind == 'IDENT' and (t.kind == 'IDENT' or
                                        is_op(t, '*'))) or
                is_op(p, ',')):
            return t.ws                 # column alignment in tables,
                                        # declarations and assignments
        return ' '
    return ' ' if im is not None and _would_merge(im, t) else ''


def _indent(toks, i, t, st, a, cls, closing):
    """Indentation of a line starting with toks[i]."""
    if t.kind == 'PREPROC':
        return ''                                    # [R09]
    if i in a.asm_region:
        return t.lead                                # verbatim region
    if i in a.label_head:
        return ''                                    # [R03]
    if closing is not None and closing[0] != 'init':
        return closing[1]                            # [M07]
    block = st.block()
    in_switch = block is not None and block[0] == 'switch'
    if i in a.case_head and in_switch:
        return block[1]                              # [M02]
    if cls == 'caseblock' and in_switch:
        return block[1]                  # 'case X:' / '{' at case level
    if is_op(t, '{') and cls != 'init':
        # a body brace that cannot be attached ('else' / '#endif' / '{')
        # lines up with its header
        return st.stmt_new if st.body else st.indent()
    if st.open or st.body:
        # continuation line or brace-less body: keep its position
        # relative to the first line of the statement
        lead = t.lead if t.nl_before > 0 else st.stmt_old + '\t'
        if st.body:
            # [M01] a brace-less body is one tab deeper than its header
            # (or at the same level when written so, e.g. after #endif)
            deeper = _cols(lead) > _cols(st.stmt_old)
            return st.stmt_new + ('\t' if deeper else '')
        if lead.startswith(st.stmt_old):
            return st.stmt_new + lead[len(st.stmt_old):]
        extra = _cols(lead) - _cols(st.stmt_old)
        if extra <= 0:
            return st.stmt_new
        return st.stmt_new + '\t' * (extra // TAB_WIDTH) + \
            ' ' * (extra % TAB_WIDTH)
    if st.last_close is not None:
        j = i
        while j < len(toks) and toks[j].kind in ('COMMENT', 'PREPROC'):
            j += 1
        if j < len(toks) and is_ident(toks[j], 'else', 'while'):
            return st.last_close         # 'else' below '}' and a comment
    if st.after_case is not None and \
            _cols(t.lead) == _cols(st.after_case[1]):
        # 'break;' after a case block keeps the author's level: with the
        # closing brace or one level deeper
        return st.after_case[0]
    if t.kind == 'COMMENT' and in_switch:
        j = next_sig(toks, i)
        while j < len(toks) and toks[j].kind == 'COMMENT':
            j = next_sig(toks, j)
        if j < len(toks) and j in a.case_head:
            return block[1]                          # comment above case
    return st.indent()                               # [M01]


def format_source(text, warnings=None):
    """One formatting pass over *text*; returns the formatted source.
    Rule violations the formatter cannot fix are appended to *warnings*
    as (line, message) when a list is given."""
    if not text.strip():
        return text
    toks = tokenize(text)
    if not toks:
        return text
    a = Analysis(toks)
    # mark switch bodies in the brace classes used by the layout
    for idx in a.switch_brace:
        a.bclass[idx] = 'switch'
    out = layout(toks, a)
    if warnings is not None:
        warnings.extend(a.warnings)
    return out


def token_signature(text):
    """Token stream of *text* without whitespace information.  Inside
    comments only the blanks at the start and end of lines may
    change."""
    sig = []
    for t in tokenize(text):
        if t.kind == 'COMMENT':
            sig.append((t.kind, '\n'.join(ln.strip(' \t')
                                          for ln in t.text.split('\n'))))
        else:
            sig.append((t.kind, t.text))
    return sig


# --------------------------------------------------------------------------
# File processing and command line interface


class Result(object):
    def __init__(self, path, status, message='', warnings=()):
        self.path = path
        self.status = status
        self.message = message
        self.warnings = list(warnings)


def format_file(path, check=False, backup=False, show_diff=False):
    """Format one file.  Returns a Result with one of:
       'unchanged', 'formatted', 'would-format', 'skipped', 'error'."""
    try:
        with open(path, 'r', encoding='utf-8', errors='surrogateescape',
                  newline='') as f:
            original = f.read()
    except OSError as e:
        return Result(path, 'error', str(e))

    crlf = original.count('\r\n') > original.count('\n') // 2
    source = original.replace('\r\n', '\n')
    if is_cplusplus(tokenize(source)):
        return Result(path, 'skipped', 'C++ source, use cxxformatter.sh')

    warnings = []
    try:
        formatted = format_source(source, warnings)
    except Exception as e:                       # noqa: BLE001
        return Result(path, 'error', 'formatter failed: %s' % e)

    # safety interlock 1: token stream must be unchanged
    if token_signature(source) != token_signature(formatted):
        return Result(path, 'error',
                      'token stream changed; file left untouched')

    # safety interlock 2: formatting must be idempotent
    try:
        twice = format_source(formatted)
    except Exception as e:                       # noqa: BLE001
        return Result(path, 'error', 'second pass failed: %s' % e)
    if twice != formatted:
        return Result(path, 'error',
                      'formatting is not idempotent; file left untouched')

    if crlf:
        formatted = formatted.replace('\n', '\r\n')
    if formatted == original:
        return Result(path, 'unchanged', warnings=warnings)

    if check:
        return Result(path, 'would-format', warnings=warnings)
    if show_diff:
        diff = difflib.unified_diff(
            original.splitlines(True), formatted.splitlines(True),
            fromfile=path, tofile=path + ' (formatted)')
        return Result(path, 'would-format', ''.join(diff), warnings)
    if backup:
        with open(path + '.orig', 'w', encoding='utf-8',
                  errors='surrogateescape', newline='') as f:
            f.write(original)
    with open(path, 'w', encoding='utf-8', errors='surrogateescape',
              newline='') as f:
        f.write(formatted)
    return Result(path, 'formatted', warnings=warnings)


def collect_targets(targets):
    files = []
    for tgt in targets:
        if os.path.isdir(tgt):
            for root, dirs, names in os.walk(tgt):
                dirs[:] = [d for d in sorted(dirs)
                           if d not in ('.git', '.orig')]
                for name in sorted(names):
                    if name.endswith(C_SUFFIX) and not \
                            name.endswith('.orig'):
                        files.append(os.path.join(root, name))
        elif os.path.isfile(tgt):
            files.append(tgt)
        else:
            print('error: no such file or directory: %s' % tgt,
                  file=sys.stderr)
    return files


def main(argv=None):
    parser = argparse.ArgumentParser(
        prog='cformatter.py',
        description='TizenRT C formatter (docs/CodingStyleGuide.md)')
    parser.add_argument('targets', nargs='+', metavar='TARGET',
                        help='.c/.h file or directory')
    parser.add_argument('--check', action='store_true',
                        help='report files needing formatting and rule '
                             'violations to fix manually; exit 1 when '
                             'found (for CI)')
    parser.add_argument('--diff', action='store_true',
                        help='print a unified diff instead of writing')
    parser.add_argument('--backup', action='store_true',
                        help='keep a .orig copy before rewriting')
    parser.add_argument('-v', '--verbose', action='store_true',
                        help='list every processed file')
    args = parser.parse_args(argv)

    files = collect_targets(args.targets)
    if not files:
        print('no C files found', file=sys.stderr)
        return 2

    counts = {}
    failed = 0
    nwarn = 0
    for path in files:
        res = format_file(path, check=args.check,
                          show_diff=args.diff, backup=args.backup)
        counts[res.status] = counts.get(res.status, 0) + 1
        if res.status == 'error':
            failed += 1
            print('%s: %s' % (res.path, res.message), file=sys.stderr)
        elif args.verbose or args.check or args.diff:
            if res.status == 'would-format':
                print(res.message if args.diff else res.path)
            elif args.verbose:
                print('%s: %s' % (res.path, res.status))
        nwarn += len(res.warnings)
        if args.verbose or args.check or args.diff:
            for line, msg in res.warnings:
                print('%s:%d: warning: %s' % (res.path, line, msg))

    if not (args.check or args.diff or args.verbose):
        summary = ', '.join('%d %s' % (v, k)
                            for k, v in sorted(counts.items()))
        if nwarn:
            summary += ', %d warning(s) to fix manually (see -v)' % nwarn
        print('%d file(s): %s' % (len(files), summary or 'nothing'))

    if args.check:
        return 1 if counts.get('would-format') or failed or nwarn else 0
    return 1 if failed else 0


if __name__ == '__main__':
    sys.exit(main())
