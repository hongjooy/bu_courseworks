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
from website import views


# class MyRegistrationView(RegistrationView):
    
#     def get_success_url(self,request, user):
#         return '/'
# Create a new class that redirects the user to the index page, if successful at logging

urlpatterns = [
    url(r'^$',views.index, name = 'index'),
    url(r'^add-device/',views.add_device, name='add_device'),
    url(r'^register/',views.register, name='register'),
    url(r'^login/',views.user_login, name='user_login'),
    url(r'^req/',views.required_view, name='required_view'),
    url(r'^test/',views.test, name='test'),
    url(r'^logout/',views.user_logout, name='user_logout'),
    url(r'^admin/', admin.site.urls),
   
    
    #after creating another "urls.py" inside of app called "post"
]
