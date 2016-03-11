from django.contrib import admin

# Register your models here.
from .models import Fitbit#, UserProfile

# admin.site.register(Member) 
admin.site.register(Fitbit)
#admin.site.register(UserProfile)