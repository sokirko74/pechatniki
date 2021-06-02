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
    contents_links_base_names = [os.path.basename(x) for x in get_links(contents_file)]
    assert len(contents_links_base_names) > 0
    found_files = [x for x in os.listdir(args.folder) if x != "index.html"]
    print(found_files)
    print(contents_links_base_names)

    if not set(found_files).issubset(set(contents_links_base_names)):
        for x in found_files:
            if x not in contents_links_base_names:
                print("{} is not in contents_links".format(x))
        if set(found_files) != set(contents_links_base_names):
            raise Exception("contents_links != html files in {}".format(args.folder))
    url_paths = []
    for x in contents_links:
        if os.path.basename(x) in found_files:
            if x.startswith('/'):
                url_paths.append(x)
            else:
                url_paths.append("/"+os.path.join(args.folder, x))
    rec = {
        "/" + args.folder: {
            "main": "/" + contents_file,
            "subpages": url_paths,
        }
    }
    print (json.dumps(rec, indent=4))
