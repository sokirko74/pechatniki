from django.urls import path, re_path
from . import views
from .slide_viewer import slide_viewer


urlpatterns = [
    re_path('.*/slide-viewer.html', slide_viewer, name='slide_viewer'),
    re_path('', views.index, name='index'),
]