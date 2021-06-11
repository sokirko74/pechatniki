from .slide_films import SLIDE_FILMS

from django.shortcuts import render
from django.http import Http404
from bs4 import BeautifulSoup
import os
from django.conf import settings
import re

SLIDE_FILMS_FOLDER = 'SlideFilms'
SLIDE_FILMS_ROOT = os.path.join(settings.STATIC_FOLDER, SLIDE_FILMS_FOLDER)


def duration_to_minutes(seconds):
    return "{:02}:{}".format(int(seconds / 60), (seconds % 60))


def build_slide_collection(url_path, soup, context):
    index = 1
    whole_dur_in_secs = 0
    slides = list()
    for img in soup.find_all("img"):
        img_src = os.path.join(url_path, img['src'])
        dur = img['dur']
        if dur.endswith('s'):
            dur = dur[:-1]
        dur = int(dur)

        slides.append((index, img_src, duration_to_minutes(whole_dur_in_secs)))
        index += 1
        whole_dur_in_secs += int(dur)
    context['slides'] = slides
    context['whole_duration'] = duration_to_minutes(whole_dur_in_secs)


def slide_viewer(request):
    film_key = os.path.dirname(request.path)
    if film_key.startswith('/'):
        film_key = film_key[1:]

    local_path = os.path.join(SLIDE_FILMS_ROOT, film_key)
    if not os.path.exists(local_path):
        raise Http404("Slide film does not exist")
    url_path = os.path.join(settings.STATIC_URL, SLIDE_FILMS_FOLDER, film_key)

    context = dict()
    with open(os.path.join(local_path, "smil.smil")) as inp:
        soup = BeautifulSoup(inp, 'xml')
        build_slide_collection(url_path, soup, context)
        context['audio_mp3'] = os.path.join(url_path, "audio.mp3")
        html_heading = SLIDE_FILMS.get(film_key, {}).get('heading')
        context['title'] = re.sub('<[^>]+>', '', html_heading)
    template_path = os.path.join(os.path.dirname(__file__), "templates", "slide_film.html")
    return render(request, template_path, context)