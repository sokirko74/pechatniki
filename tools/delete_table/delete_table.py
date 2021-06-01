from bs4 import BeautifulSoup
import sys
import shutil


def normalize(text):
    if text is not None:
        text = " ".join(text.split())
        text = text.strip(' \n\t\r')
        return text
    return ""


def process_file(filename):
    with open (filename) as inp:
        soup = BeautifulSoup(inp, 'html.parser')

    for t in soup.find_all("table"):
        if t.get('width') == "600" and 'оглавление' in str(t):
            t.decompose()
            #print (t.get('width'))

    #filename +=  ".new"

    with open (filename, "w") as out:
        out.write(str(soup))
    return True


if __name__ == "__main__":
    if not process_file(sys.argv[1]):
        sys.exit(1)