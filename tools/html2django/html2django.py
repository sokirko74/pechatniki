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
    body = soup.find("body")
    if body is None:
        return False
    title = soup.find('title').string
    title = normalize(title).strip()
    for x in soup.find_all("script"):
        x.decompose()

    #shutil.copy(filename, filename+ ".sav")
    filename +=  ".new"
    with open (filename, "w") as out:
        out.write("{% extends 'base.html' %}\n")
        out.write("<title>{}</title>\n".format(title))
        out.write("{% block content %}\n")
        out.write(str(body).replace('<body>','').replace('</body>','') + "\n")
        #out.write(str(body) + "\n")
        out.write("{% endblock content %}\n")
    return True


if __name__ == "__main__":
    if not process_file(sys.argv[1]):
        sys.exit(1)