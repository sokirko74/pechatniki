import argparse
import re

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('files',  nargs='+',
                        help='input chiwriter files in Russian encoding')
    parser.add_argument('--add-headers',  dest="add_headers",  action="store_true", default=False)
    return parser.parse_args()


DIGITS_BIG = {
    '0': '0', '1':'1', '2':'2', '3': '3', '4':'4', '5':'5',
    '6': '6', '7':'7', '8':'8', '9': '9',
    '*': '*',
    '^': ',',
    '#': '#',
    '$': '$',
    '%': ':',
    '&': '.',
    '@': '"',
    '(': '?',
    ')': '%',

}

LINEDRAW = {
    'u': '⌜',
    'o': '⌝<br/>',
    '-': '-',
    'p': '|',
    'm': '⌞',
    '.':  '⌟<br/>',
    'i': '╂',
    '5': '5',
    '0': '0',
    '^': ',',
    '&': '.',
    '6': '6',
    '7': '7',
    '2': '2'
}

DIGITS_NORM = {
    '0': ')', '1':'!', '2':'"', '3': '???3', '4':'???4', '5':':',
    '6': ',', '7': '.', '8':'"', '9': '(',
    ')': '0',
    '!': '1',
    '@': '2',
    '#': '3',
    '$': '4',
    '%': '5',
    '^': '6',
    '&': '7',
    '*': '8',
    '(': '9',
}

TRANSLATE ={
    '\n': '\n',
    ' ': ' ',
    '-': '-',
    '_': '_',
    '~': '(',
    "`": ')',
    '!': '!',
    '?': '?',
    '=': '=',
    '+': '+',
    '\x0c': '',  # end of page?
    '|': '№',
    '/': '/',
    '\x1a': '', # end of document?
    'f': 'а', 'F': 'A',
    ',': 'б', '<': 'Б',
    'd': 'в', 'D': 'В',
    'u': 'г', 'U': 'Г',
    'l': 'д', 'L': 'Д',
    't': 'е', 'T': 'E',
    ';': 'ж', ':': 'Ж',
    'p': 'з', 'P': 'З',
    'b': 'и', 'B': 'И',
    'q': 'й', 'Q': 'Й',
    'r': 'к', 'R': 'К',
    'k': 'л', 'K': 'Л',
    'v': 'м', 'V': 'М',
    'y': 'н', 'Y': 'Н',
    'h': 'р', 'H': 'Р',
    'j': 'о', 'J': 'О',
    'g': 'п', 'G': 'П',
    'n': 'т', 'N': 'Т',
    'c': 'с', 'C': 'С',
    'a': 'ф', 'A': 'Ф',
    'e': 'у', 'E': 'У',
    'w': 'ц', 'W': 'Ц',
    '[': 'х', '{': 'Х',
    'i': 'ш', 'I': 'Ш',
    'x': 'ч', 'X': 'Ч',
    ']': 'ъ', '}': 'Ъ',
    'o': 'щ', 'O': 'Щ',
    'm': 'ь', 'M': 'Ь',
    "'": 'э', '"': 'Э',
    's': 'ы', 'S': 'Ы',
    '.': 'ю', '>': 'Ю',
    'z': 'я', 'Z': 'Я'
}

TRANSLATE_BIG = dict()
TRANSLATE_BIG.update(TRANSLATE)
TRANSLATE_BIG.update(DIGITS_BIG)

TRANSLATE_NORM = dict()
TRANSLATE_NORM.update(TRANSLATE)
TRANSLATE_NORM.update(DIGITS_NORM)

TRANSLATE_LINE_DRAW = dict()
TRANSLATE_LINE_DRAW.update(TRANSLATE)
TRANSLATE_LINE_DRAW.update(LINEDRAW)


class TTextChunk:
    def __init__(self, text_chunk, centered=None, font="norm_font", paragraph_break=False, is_nbsp=False):
        self.text_chunk = text_chunk
        self.centered = centered
        self.font = font
        self.paragraph_break = paragraph_break
        self.is_nbsp = is_nbsp


class TChiConverter:

    def __init__(self, input_path):
        self.font = "norm_font"
        self.centered = False
        self.input_path = input_path

    def map_char(self, s):
        if self.font == "norm_font":
            tr = TRANSLATE_NORM
        elif self.font == "big_font":
            tr = TRANSLATE_BIG
        elif self.font == "line_draw_font":
            tr = TRANSLATE_LINE_DRAW
        else:
            raise Exception("unknown font {}".format(self.font))
        tr_s = tr.get(s)
        if tr_s is None:
            raise Exception("unknown char \"{}\", font={}".format(s, self.font))
        return TTextChunk(tr_s, self.centered, self.font, paragraph_break=False, is_nbsp=False)

    def convert_special(self, ch):
        if not self.centered and ch == "\\^":
            self.centered = True
        elif self.centered and ch == "\\^":
            self.centered = False
        elif ch == "\\,":
            s = ""
            if self.centered:
                self.centered = False
            return TTextChunk("", centered=False, font=self.font, paragraph_break=True)
        elif ch == "\\ ":
            #return TTextChunk('', centered=self.centered, font=self.font, paragraph_break=False, is_nbsp=True)
            return None
        elif ch == "\\!":
            self.font = "big_font"
        elif ch == "\\9":
            self.font = "line_draw_font"
        elif ch == "\\&":
            self.font = "norm_font"

    def read_text(self):
        start = False
        with open(self.input_path) as inp:
            for l in inp:
                if l.startswith("\\="):
                    start = True
                    continue
                if not start:
                    continue
                if l.startswith('\\+'):
                    continue
                i = 0
                has_line_draw = False
                while i < len(l):
                    if self.font == "line_draw_font":
                        has_line_draw = True
                    if l[i] == '\\':
                        c = self.convert_special(l[i:i+2])
                        if c is not None:
                            yield c
                        i += 2
                    else:
                        mapped = self.map_char(l[i])
                        if has_line_draw and mapped.text_chunk == ' ':
                            mapped.is_nbsp = True
                        yield mapped
                        i += 1
                if has_line_draw:
                    yield TTextChunk("<br/>", self.centered, self.font, paragraph_break=False, is_nbsp=False)
                else:
                    yield TTextChunk("\n", self.centered, self.font, paragraph_break=False, is_nbsp=False)


class THtmlGenerator:
    def __init__(self, add_headers):
        self.current_class = None
        self.current_tag  = None
        self.add_headers = add_headers
        self.out_str = ""
        if self.add_headers:
            out_str += self.default_html_header()

    def end_span(self):
        if self.current_class is not None:
            assert self.current_tag is not None
            self.out_str += "</{}>".format(self.current_tag)
            self.current_tag = None
            self.current_class = None

    def start_span(self, html_tag, class_name):
        if html_tag == self.current_tag and self.current_class == class_name:
            return
        self.end_span()
        if html_tag is not None:
            self.out_str += '<{}>'.format(html_tag)
        self.current_tag = html_tag
        self.current_class = class_name

    @staticmethod
    def default_html_header():
        return """
        <!DOCTYPE html>
        <head>
        <link rel="stylesheet" href="chiwriter.css">
        </head>
        <body>
        """

    @staticmethod
    def get_class_name_and_html_tag(cf: TTextChunk):
        if cf.font == "norm_font" and cf.centered:
            return "centered", "span"
        elif cf.font == "big_font" and not cf.centered:
            return "bigfont", "h5"
        elif cf.font == "big_font" and cf.centered:
            return "centered_big", "h5"
        else:
            return None, None

    def generate_html(self, chunks):
        self.out_str += "<p>\n"
        for c in chunks:
            if c.paragraph_break:
                self.end_span()
                self.out_str += "\n</p>\n<p>"
            else:
                cl, tag = THtmlGenerator.get_class_name_and_html_tag(c)
                self.start_span(tag, cl)
                if c.is_nbsp:
                    self.out_str += "&nbsp;"
                elif len(c.text_chunk) > 0:
                    if (self.current_class == "centered" or self.current_class == "centered_big") and c.text_chunk == " ":
                        self.out_str += "&nbsp;"
                    else:
                        self.out_str += c.text_chunk
        self.end_span()
        if self.add_headers:
            self.out_str += "</p> </body> </html>"
        return self.out_str


def main():
    args = parse_args()
    for f in args.files:
        assert f.lower().endswith('.chi')
        chunks = list()
        out_path = f[:-4]+".html"
        print("write to {}".format(out_path))
        converter = TChiConverter(f)
        for c in converter.read_text():
            chunks.append(c)
        out_str = THtmlGenerator(args.add_headers).generate_html(chunks)
        with open(out_path, "w") as outp:
            outp.write(out_str)


if __name__ == "__main__":
    main()