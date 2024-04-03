from django.urls import path

from . import views

urlpatterns = [
     path('', views.index, name='index'),
    path('T2I', views.T2I, name='T2I'),  
    path('getImageFeature', views.faceFeature, name='getImageFeature'),
    path('camerngetfaceFeature', views.faceFeature, name='camerngetfaceFeature'),
    path('igetfaceFeature', views.igetfaceFeature, name='igetfaceFeature')
]