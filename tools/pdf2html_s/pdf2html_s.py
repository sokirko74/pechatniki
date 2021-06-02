import os

from bs4 import BeautifulSoup
import json
import argparse


def simplify(filename):
    with open (filename) as inp:
        soup = BeautifulSoup(inp, 'html.parser')
    container = soup.find("div", {"id": "page-container"})
    title = soup.find('title').string

    # body = '<table class="simple_presentation">'
    # for c in container.find_all('div', {"class": "pf"}):
    #     text = ""
    #     for text_div in c.find_all('div', {"class": "t"}):
    #         text += text_div.getText().replace("\n", " ")
    #     body += ""
    #     img = c.find('img')
    #     if img is not None:
    #         body += "   <tr><td height=600><img src={} /></td></tr>\n".format(img['src'])
    #     if text != "":
    #         body += "  <tr><td><span>{}</span></td></tr>\n".format(text)
    #     break
    # body += "</table>\n"

    body = ''
    for c in container.find_all('div', {"class": "pf"}):
        text = ""
        for text_div in c.find_all('div', {"class": "t"}):
            text += text_div.getText().replace("\n", " ")
        body += '<figure class=\"simple_presentation_page\">\n'
        img = c.find('img')
        if img is not None:
            body += "   <img src={} />\n".format(img['src'])
        if text != "":
            body += "  <figcaption>{}</figcaption>\n".format(text)
        body += "</figure>\n"



    filename +=  ".html"

    with open(filename, "w") as out:
       out.write("{% extends 'base.html' %}\n")
       out.write("{{% block title %}}{}{{% endblock %}}\n".format(title))
       out.write("{% block content %}\n")
       out.write(body)
       out.write("{% endblock content %}\n")


def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument("--html", dest='file', required=True)
    return parser.parse_args()


if __name__ == "__main__":
    args = parse_args()
    html = args.file
    simplify(args.file)
