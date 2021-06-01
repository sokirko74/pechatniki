from django.shortcuts import render
import os
from .slide_films import SLIDE_FILMS


def get_slide_info(urlpath):
    _, extension = os.path.splitext(urlpath)
    if extension == '.htm':
        urlpath += 'l'
    if urlpath.endswith('/index.html'):
        urlpath = urlpath[:-len('/index.html')]
    key = urlpath.strip("/")
    return key, SLIDE_FILMS.get(key)


def index(request):
    template_path = request.path
    if template_path.startswith('/'):
        template_path = template_path[1:]
    if not template_path.endswith('.html') and not template_path.endswith('.ico') and not template_path.endswith('.htm'):
        template_path = os.path.join(template_path, 'index.html')
    _, extension = os.path.splitext(template_path)
    if extension == '.htm':
        template_path += 'l'

    context = {}
    slide_film_key, slide_film_props = get_slide_info(request.path)
    if slide_film_props is not None:
        context.update(slide_film_props)
        context['slide_film_key'] = slide_film_key
    return render(request, template_path, context)
