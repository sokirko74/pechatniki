from bs4 import BeautifulSoup
import sys



def process_file(filename):
    with open (filename) as inp:
        soup = BeautifulSoup(inp, 'html.parser')
    for a in soup.find_all("a"):
        print (a['href'])


if __name__ == "__main__":
    if not process_file(sys.argv[1]):
        sys.exit(1)