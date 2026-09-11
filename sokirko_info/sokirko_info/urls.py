import os.path
from django.contrib import admin
from django.urls import path, include, re_path
from django.views.generic.base import TemplateView
from django.http import HttpResponse
from django.conf import settings
from django.views.static import serve

SLIDE_FILMS_ROOT = os.path.join(settings.STATIC_FOLDER, "SlideFilms")


def static_file(request):
    file_path =  os.path.join(settings.STATIC_FOLDER, request.path.strip('/'))
    with open(file_path) as inp:
        ct = "text/plain"
        if file_path.endswith('.xml'):
            ct = "text/xml"
        return HttpResponse(inp.read(), content_type=ct)


urlpatterns = [
    path('admin/', admin.site.urls),
    path('robots.txt', static_file),
    path('sitemap.xml', static_file),
    re_path('google', static_file),
    re_path('yandex', static_file),
    re_path(r'^browse-films/SlideFilms/(?P<path>.*)$', serve, {
        'document_root': SLIDE_FILMS_ROOT,
        'show_indexes': True,
    }),
    re_path('', include('application.urls')),
]
