# -*- coding: utf-8 -*-
"""Tiny zero-dependency .xlsx writer (standard library only).

It implements just the small subset of the openpyxl API used by
gen_regdb_xlsx.py, so that a styled Excel file can be produced even when
openpyxl is not installed. An .xlsx file is only a ZIP archive of XML parts
(OOXML / SpreadsheetML), which the stdlib zipfile + string templating can
build directly.

Supported subset:
    Workbook(), wb.active, wb.save(path)
    ws.title, ws.sheet_view.showGridLines
    ws.cell(row, column[, value]) -> Cell(.value/.font/.fill/.alignment/.border)
    ws.merge_cells(start_row, start_column, end_row, end_column)
    ws.column_dimensions["A"].width, ws.row_dimensions[r].height
    Font(bold, color, size, name)
    PatternFill(patternType, fgColor)
    Alignment(horizontal, vertical, wrap_text)
    Side(style, color), Border(left, right, top, bottom)
    get_column_letter(idx)

Styling fidelity is intentionally limited to what the generator needs:
solid fills, thin/medium borders, bold/size/color fonts and cell alignment.
"""
import zipfile
from xml.sax.saxutils import escape


# ----------------------------- helpers --------------------------------------
def get_column_letter(idx):
    s = ""
    while idx > 0:
        idx, r = divmod(idx - 1, 26)
        s = chr(65 + r) + s
    return s


def column_index_from_string(s):
    n = 0
    for ch in s:
        n = n * 26 + (ord(ch.upper()) - 64)
    return n


def _argb(color, default="FF000000"):
    if not color:
        return default
    c = str(color).upper()
    if len(c) == 6:
        return "FF" + c
    if len(c) == 8:
        return c
    return default


def _attr(s):
    return escape(str(s), {'"': "&quot;"})


# ----------------------------- style objects --------------------------------
class Font:
    def __init__(self, bold=False, color=None, size=11, name="Calibri", **_):
        self.bold = bool(bold)
        self.color = color
        self.size = size
        self.name = name

    def key(self):
        return (self.bold, self.color, self.size, self.name)


class PatternFill:
    def __init__(self, patternType=None, fgColor=None, fill_type=None, **_):
        self.patternType = patternType or fill_type
        self.fgColor = fgColor

    def key(self):
        return (self.patternType, self.fgColor)


class Side:
    def __init__(self, style=None, color=None, border_style=None, **_):
        self.style = style or border_style
        self.color = color

    def key(self):
        return (self.style, self.color)


class Border:
    def __init__(self, left=None, right=None, top=None, bottom=None, **_):
        self.left = left
        self.right = right
        self.top = top
        self.bottom = bottom

    def key(self):
        def sk(s):
            return s.key() if s else None
        return (sk(self.left), sk(self.right), sk(self.top), sk(self.bottom))


class Alignment:
    def __init__(self, horizontal=None, vertical=None, wrap_text=False, **_):
        self.horizontal = horizontal
        self.vertical = vertical
        self.wrap_text = bool(wrap_text)

    def key(self):
        return (self.horizontal, self.vertical, self.wrap_text)


# ----------------------------- data model -----------------------------------
class Cell:
    __slots__ = ("value", "font", "fill", "alignment", "border")

    def __init__(self):
        self.value = None
        self.font = None
        self.fill = None
        self.alignment = None
        self.border = None


class _Dim:
    __slots__ = ("width", "height")

    def __init__(self):
        self.width = None
        self.height = None


class _DimDict(dict):
    def __missing__(self, k):
        d = _Dim()
        self[k] = d
        return d


class _SheetView:
    def __init__(self):
        self.showGridLines = True


class Worksheet:
    def __init__(self, title="Sheet1"):
        self.title = title
        self._cells = {}
        self._merges = []
        self.column_dimensions = _DimDict()
        self.row_dimensions = _DimDict()
        self.sheet_view = _SheetView()
        self._maxr = 1
        self._maxc = 1

    def cell(self, row, column, value=None):
        c = self._cells.get((row, column))
        if c is None:
            c = Cell()
            self._cells[(row, column)] = c
        if value is not None:
            c.value = value
        if row > self._maxr:
            self._maxr = row
        if column > self._maxc:
            self._maxc = column
        return c

    def merge_cells(self, start_row=None, start_column=None,
                    end_row=None, end_column=None):
        self._merges.append((start_row, start_column, end_row, end_column))
        self.cell(start_row, start_column)


class Workbook:
    def __init__(self):
        self.active = Worksheet()

    def save(self, path):
        _save(self.active, path)


# ----------------------------- serialization --------------------------------
def _save(ws, path):
    # --- style registries ---
    fonts, font_map = [], {}
    fills, fill_map = ["none", "gray125"], {}   # reserved fill ids 0,1
    borders, border_map = [], {}
    xfs, xf_map = [], {}

    def reg_font(f):
        f = f or Font()
        k = f.key()
        if k not in font_map:
            font_map[k] = len(fonts)
            fonts.append(f)
        return font_map[k]

    def reg_fill(fl):
        if fl is None or fl.patternType is None:
            return 0
        k = fl.key()
        if k not in fill_map:
            fill_map[k] = len(fills)
            fills.append(fl)
        return fill_map[k]

    def reg_border(b):
        b = b or Border()
        k = b.key()
        if k not in border_map:
            border_map[k] = len(borders)
            borders.append(b)
        return border_map[k]

    def reg_xf(fid, flid, bid, align):
        ak = align.key() if align else None
        k = (fid, flid, bid, ak)
        if k not in xf_map:
            xf_map[k] = len(xfs)
            xfs.append((fid, flid, bid, align))
        return xf_map[k]

    reg_font(Font())          # font 0 (default)
    reg_border(Border())      # border 0 (default)
    reg_xf(0, 0, 0, None)     # xf 0 (default)

    # assign a style id to every cell
    cell_style = {}
    for coord, c in ws._cells.items():
        s = reg_xf(reg_font(c.font), reg_fill(c.fill),
                   reg_border(c.border), c.alignment)
        cell_style[coord] = s

    # --- styles.xml ---
    def font_xml(f):
        parts = ["<font>"]
        if f.bold:
            parts.append("<b/>")
        parts.append('<sz val="%s"/>' % f.size)
        parts.append('<color rgb="%s"/>' % _argb(f.color))
        parts.append('<name val="%s"/>' % _attr(f.name))
        parts.append("</font>")
        return "".join(parts)

    def fill_xml(fl):
        if fl == "none":
            return '<fill><patternFill patternType="none"/></fill>'
        if fl == "gray125":
            return '<fill><patternFill patternType="gray125"/></fill>'
        return ('<fill><patternFill patternType="solid">'
                '<fgColor rgb="%s"/><bgColor indexed="64"/>'
                '</patternFill></fill>' % _argb(fl.fgColor))

    def side_xml(tag, side):
        if side and side.style:
            return '<%s style="%s"><color rgb="%s"/></%s>' % (
                tag, side.style, _argb(side.color, "FF000000"), tag)
        return "<%s/>" % tag

    def border_xml(b):
        return ("<border>%s%s%s%s<diagonal/></border>" % (
            side_xml("left", b.left), side_xml("right", b.right),
            side_xml("top", b.top), side_xml("bottom", b.bottom)))

    def align_xml(a):
        bits = []
        if a.horizontal:
            bits.append('horizontal="%s"' % a.horizontal)
        if a.vertical:
            bits.append('vertical="%s"' % a.vertical)
        if a.wrap_text:
            bits.append('wrapText="1"')
        return "<alignment %s/>" % " ".join(bits) if bits else ""

    def xf_xml(fid, flid, bid, align):
        a = ""
        applyal = ""
        if align:
            a = align_xml(align)
            applyal = ' applyAlignment="1"'
        return ('<xf numFmtId="0" fontId="%d" fillId="%d" borderId="%d" '
                'xfId="0" applyFont="1" applyFill="1" applyBorder="1"%s>%s</xf>'
                % (fid, flid, bid, applyal, a))

    styles = [
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>',
        '<styleSheet xmlns="http://schemas.openxmlformats.org/'
        'spreadsheetml/2006/main">',
        '<fonts count="%d">' % len(fonts),
        "".join(font_xml(f) for f in fonts), "</fonts>",
        '<fills count="%d">' % len(fills),
        "".join(fill_xml(fl) for fl in fills), "</fills>",
        '<borders count="%d">' % len(borders),
        "".join(border_xml(b) for b in borders), "</borders>",
        '<cellStyleXfs count="1"><xf numFmtId="0" fontId="0" fillId="0" '
        'borderId="0"/></cellStyleXfs>',
        '<cellXfs count="%d">' % len(xfs),
        "".join(xf_xml(*x) for x in xfs), "</cellXfs>",
        '<cellStyles count="1"><cellStyle name="Normal" xfId="0" '
        'builtinId="0"/></cellStyles>',
        "</styleSheet>",
    ]
    styles_xml = "".join(styles)

    # --- sheet1.xml ---
    rows = {}
    for (r, c), cell in ws._cells.items():
        rows.setdefault(r, {})[c] = cell

    def cell_xml(r, c, cell):
        ref = "%s%d" % (get_column_letter(c), r)
        s = cell_style[(r, c)]
        v = cell.value
        if v is None or v == "":
            if s == 0:
                return ""
            return '<c r="%s" s="%d"/>' % (ref, s)
        if isinstance(v, bool):
            v = str(v)
        if isinstance(v, (int, float)):
            return '<c r="%s" s="%d"><v>%s</v></c>' % (ref, s, v)
        txt = escape(str(v))
        return ('<c r="%s" s="%d" t="inlineStr"><is>'
                '<t xml:space="preserve">%s</t></is></c>' % (ref, s, txt))

    body = []
    for r in sorted(rows):
        dim = ws.row_dimensions.get(r)
        attr = ' r="%d"' % r
        if dim and dim.height:
            attr += ' ht="%s" customHeight="1"' % dim.height
        cells = rows[r]
        inner = "".join(cell_xml(r, c, cells[c]) for c in sorted(cells))
        body.append("<row%s>%s</row>" % (attr, inner))

    cols_xml = ""
    col_entries = []
    for letter, dim in ws.column_dimensions.items():
        if dim.width:
            idx = column_index_from_string(letter)
            col_entries.append((idx, dim.width))
    if col_entries:
        col_entries.sort()
        cols_xml = "<cols>%s</cols>" % "".join(
            '<col min="%d" max="%d" width="%s" customWidth="1"/>' % (i, i, w)
            for i, w in col_entries)

    merges_xml = ""
    if ws._merges:
        refs = []
        for (r1, c1, r2, c2) in ws._merges:
            refs.append('<mergeCell ref="%s%d:%s%d"/>' % (
                get_column_letter(c1), r1, get_column_letter(c2), r2))
        merges_xml = '<mergeCells count="%d">%s</mergeCells>' % (
            len(refs), "".join(refs))

    grid = "" if ws.sheet_view.showGridLines else ' showGridLines="0"'
    sheet_xml = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<worksheet xmlns="http://schemas.openxmlformats.org/'
        'spreadsheetml/2006/main">'
        '<sheetViews><sheetView%s workbookViewId="0"/></sheetViews>'
        '<sheetFormatPr defaultRowHeight="15"/>'
        '%s<sheetData>%s</sheetData>%s</worksheet>'
        % (grid, cols_xml, "".join(body), merges_xml))

    # --- package parts ---
    title = _attr(ws.title[:31])
    content_types = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<Types xmlns="http://schemas.openxmlformats.org/package/2006/'
        'content-types">'
        '<Default Extension="rels" ContentType="application/vnd.'
        'openxmlformats-package.relationships+xml"/>'
        '<Default Extension="xml" ContentType="application/xml"/>'
        '<Override PartName="/xl/workbook.xml" ContentType="application/vnd.'
        'openxmlformats-officedocument.spreadsheetml.sheet.main+xml"/>'
        '<Override PartName="/xl/worksheets/sheet1.xml" ContentType='
        '"application/vnd.openxmlformats-officedocument.spreadsheetml.'
        'worksheet+xml"/>'
        '<Override PartName="/xl/styles.xml" ContentType="application/vnd.'
        'openxmlformats-officedocument.spreadsheetml.styles+xml"/>'
        '</Types>')

    root_rels = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<Relationships xmlns="http://schemas.openxmlformats.org/package/'
        '2006/relationships">'
        '<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/'
        'officeDocument/2006/relationships/officeDocument" '
        'Target="xl/workbook.xml"/></Relationships>')

    workbook_xml = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<workbook xmlns="http://schemas.openxmlformats.org/spreadsheetml/'
        '2006/main" xmlns:r="http://schemas.openxmlformats.org/'
        'officeDocument/2006/relationships">'
        '<sheets><sheet name="%s" sheetId="1" r:id="rId1"/></sheets>'
        '</workbook>' % title)

    workbook_rels = (
        '<?xml version="1.0" encoding="UTF-8" standalone="yes"?>'
        '<Relationships xmlns="http://schemas.openxmlformats.org/package/'
        '2006/relationships">'
        '<Relationship Id="rId1" Type="http://schemas.openxmlformats.org/'
        'officeDocument/2006/relationships/worksheet" '
        'Target="worksheets/sheet1.xml"/>'
        '<Relationship Id="rId2" Type="http://schemas.openxmlformats.org/'
        'officeDocument/2006/relationships/styles" Target="styles.xml"/>'
        '</Relationships>')

    with zipfile.ZipFile(path, "w", zipfile.ZIP_DEFLATED) as z:
        z.writestr("[Content_Types].xml", content_types)
        z.writestr("_rels/.rels", root_rels)
        z.writestr("xl/workbook.xml", workbook_xml)
        z.writestr("xl/_rels/workbook.xml.rels", workbook_rels)
        z.writestr("xl/styles.xml", styles_xml)
        z.writestr("xl/worksheets/sheet1.xml", sheet_xml)
