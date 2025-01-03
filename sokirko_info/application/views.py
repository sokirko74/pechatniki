
from .slide_films import SLIDE_FILMS
from .book_navigaton import NAVIGATION

from django.shortcuts import render
import os
from django.http import HttpResponse, HttpResponsePermanentRedirect
from django.conf import settings

def get_slide_info(urlpath, context):
    if urlpath.endswith('/index.html'):
        urlpath = urlpath[:-len('/index.html')]
    key = urlpath.strip("/")
    info = SLIDE_FILMS.get(key)
    if info is not None:
        context.update(info)
        context['pechatniki_js'] = os.path.join(urlpath, "slide-viewer.html")
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
    if urlpath.strip('/') == 'favicon.ico':  #for debug, in prod favicons are managed by nginx
        return HttpResponse(open(os.path.join(settings.STATIC_FOLDER, 'favicon.ico'), "rb").read(), content_type='image/x-icon')
    if '_' in urlpath:
        urlpath = urlpath.replace('_', '-')
        return HttpResponsePermanentRedirect(urlpath)
    template_path = urlpath

    if template_path.startswith('/'):
        template_path = template_path[1:]
    if not template_path.endswith('.html') and not template_path.endswith('.ico') and not template_path.endswith('.htm'):
        template_path = os.path.join(template_path, 'index.html')

    context = {}
    get_slide_info(urlpath, context)
    get_navigation_info(urlpath, context)
    return render(request, template_path, context)



