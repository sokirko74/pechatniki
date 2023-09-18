
from .slide_films import SLIDE_FILMS
from .book_navigaton import NAVIGATION

from django.shortcuts import render
import os
import urllib
from pathlib import Path
from django.http import HttpResponse
from django.conf import settings


def build_sitemap_xml_by_local_files():
    folder = os.path.join(os.path.dirname(__file__), "templates")
    urls = list(os.path.relpath(path.absolute(), start=folder) for path in Path(folder).rglob('*.html'))
    sitemap = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    sitemap += "<urlset xmlns=\"https://www.sitemaps.org/schemas/sitemap/0.9\">\n"
    for p in urls:
        if p != "base.html":
            url = urllib.parse.urljoin("https://sokirko.info", p)
            sitemap += "<url><loc>{}</loc></url>\n".format(url)
    sitemap += "</urlset>\n"
    return sitemap


SITEMAP = build_sitemap_xml_by_local_files()
IMAGES_ROOT = os.path.join(settings.STATIC_FOLDER, 'images')


def get_slide_info(urlpath, context):
    if urlpath.endswith('/index.html'):
        urlpath = urlpath[:-len('/index.html')]
    key = urlpath.strip("/")
    info = SLIDE_FILMS.get(key)
    if info is not None:
        context.update(info)
        context['pechatniki_js'] = os.path.join(urlpath, "slide_viewer.html")
        context['slide_film_key'] = key


def get_navigation_info(urlpath, context):
    folder = os.path.dirname(urlpath)
    info = NAVIGATION.get(folder, NAVIGATION.get(os.path.dirname(folder)))
    if info is None:
        return
    subpages = info.get('subpages', [])
    try:
        index = subpages.index(urlpath)
    except ValueError:
        return
    context['contents_page'] = info['main']
    if index > 0:
        context['prev_page'] = subpages[index - 1]
    if index + 1 < len(subpages):
        context['next_page'] = subpages[index + 1]


def index(request):
    urlpath = request.path
    _, extension = os.path.splitext(urlpath)
    if extension == '.htm':
        urlpath += 'l'
    if urlpath.strip('/') == 'sitemap.xml':
        return HttpResponse(SITEMAP, content_type='application/xml')
    if urlpath.strip('/') == 'favicon.ico':  #for debug, in prod favicons are managed by nginx
        return HttpResponse(open(os.path.join(settings.STATIC_FOLDER, 'favicon.ico'), "rb").read(), content_type='image/x-icon')
    template_path = urlpath
    if template_path.startswith('/'):
        template_path = template_path[1:]
    if not template_path.endswith('.html') and not template_path.endswith('.ico') and not template_path.endswith('.htm'):
        template_path = os.path.join(template_path, 'index.html')

    context = {}
    get_slide_info(urlpath, context)
    get_navigation_info(urlpath, context)
    return render(request, template_path, context)



