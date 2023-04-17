"""sokirko_info URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
import os.path

from django.contrib import admin
from django.urls import path, include, re_path
from django.views.generic.base import TemplateView
from django.http import HttpResponse
from django.conf import settings

def static_file(request):
    file_path =  os.path.join(settings.STATIC_FOLDER, request.path.strip('/'))
    with open(file_path) as inp:
        return HttpResponse(inp.read(), content_type="text/plain")


urlpatterns = [
    path('admin/', admin.site.urls),
    path('robots.txt', static_file),
    re_path('google', static_file),
    re_path('yandex', static_file),
    re_path('', include('application.urls')),
]
