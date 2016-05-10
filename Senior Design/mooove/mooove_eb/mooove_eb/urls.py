"""mooove_eb URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.9/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import include, url # "include" will allow to use urls.py inside of apps
from django.contrib import admin
from website import views, rest_views
from django.contrib.auth.models import User
from rest_framework.urlpatterns import format_suffix_patterns



urlpatterns = [
    url(r'^$',views.index, name = 'index'),
    url(r'^dashboard/0',views.anklet_register, name='anklet'),
    url(r'^dashboard/(?P<pk>[0-9]+)$',views.dashboard, name='dashboard'),
    url(r'^register/',views.register, name='register'),
    url(r'^anklet_register/',views.anklet_register, name='anklet_register'),
    url(r'^login/',views.user_login, name='user_login'),
    url(r'^req/',views.required_view, name='required_view'),
    url(r'^logout/',views.user_logout, name='user_logout'),
    url(r'^json/users/$', rest_views.user_list, name='user_list'),
    url(r'^json/users/(?P<pk>[0-9]+)$', rest_views.user_detail, name='user_detail'),
    url(r'^json/cows/$', rest_views.cow_list, name='cow_list'),
    url(r'^json/cows/(?P<pk>[0-9]+)$', rest_views.cow_detail, name='cow_detail'),
    url(r'^json/friends/$', rest_views.friend_list, name='friend_list'),
    url(r'^json/socials/$', rest_views.social_simple, name='social_simple'),
    url(r'^json/groups/$', rest_views.group_list, name='group_list'),
    url(r'^json/social/$', rest_views.social_list, name='social_list'),
    url(r'^json/cowsimple/$', rest_views.cow_simple, name='cow_simple'),
    url(r'^admin/', admin.site.urls),

]



