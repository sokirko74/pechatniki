import os

from bs4 import BeautifulSoup
import json
import argparse


def get_links(filename):
    with open (filename) as inp:
        soup = BeautifulSoup(inp, 'html.parser')
    for a in soup.find_all("a"):
        yield a['href']


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--folder", dest='folder', required=True)
    parser.add_argument("--contents-file", dest='contents_file')
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    contents_file = args.contents_file
    if contents_file is None:
        contents_file = os.path.join(args.folder, "index.html")
    contents_links = [x for x in get_links(contents_file)]
    assert len(contents_links) > 0
    files = [x for x in os.listdir(args.folder) if x != "index.html"]
    if set(files) != set(contents_links):
        raise Exception ("contents_links != html files in {}".format(args.folder))
    url_paths = []
    for x in contents_links:
        url_paths.append("/"+os.path.join(args.folder, x))
    rec = {
        "/" + args.folder: {
            "main": "/" + contents_file,
            "subpages": url_paths,
        }
    }
    print (json.dumps(rec, indent=4))
