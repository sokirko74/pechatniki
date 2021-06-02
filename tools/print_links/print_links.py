import os

from bs4 import BeautifulSoup
import json
import argparse


def get_links(filename, print_links, print_image_links):
    with open (filename) as inp:
        soup = BeautifulSoup(inp, 'html.parser')
    if print_links:
        for a in soup.find_all("a"):
            yield a['href']
    if print_image_links:
        for a in soup.find_all("img"):
            yield a['src']


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--html", dest='file', required=True)
    parser.add_argument("--a", dest='a', action="store_true", default=False)
    parser.add_argument("--img", dest='img', action="store_true", default=False)
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    html = args.file
    for x in get_links(args.file, args.a, args.img):
        print (x)
