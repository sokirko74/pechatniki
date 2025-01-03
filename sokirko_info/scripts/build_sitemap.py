import urllib
from pathlib import Path
import os

def build_sitemap_xml_by_local_files(static_folder):
    folder = os.path.join(os.path.dirname(__file__), "../application/templates")
    urls = list(os.path.relpath(path.absolute(), start=folder) for path in Path(folder).rglob('*.html'))
    sitemap = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    sitemap += "<urlset xmlns=\"https://www.sitemaps.org/schemas/sitemap/0.9\">\n"

    folder1 = static_folder
    cnt = 0
    for p in urls:
        if p != "base.html":
            cnt += 1
            url = urllib.parse.urljoin("https://sokirko.info", p)
            sitemap += "<url><loc>{}</loc></url>\n".format(url)
    static = list(os.path.relpath(path.absolute(), start=folder1) for path in Path(folder1).rglob('*.pdf'))
    for p in static:
        cnt += 1
        url = urllib.parse.urljoin("https://sokirko.info/static/", p)
        sitemap += "<url><loc>{}</loc></url>\n".format(url)
    print("found {} urls".format(cnt))
    sitemap += "</urlset>\n"
    return sitemap


def main():
    static_folder = os.path.join(os.path.dirname(__file__), "../sokirko_info/static")
    p = os.path.join(static_folder, "sitemap.xml")
    print("write to {}".format(p))
    with open (p, "w") as outp:
        xml = build_sitemap_xml_by_local_files(static_folder)
        outp.write(xml)


if __name__ == "__main__":
    main()
