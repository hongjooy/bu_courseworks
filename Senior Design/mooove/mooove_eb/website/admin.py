from django.contrib import admin

# Register your models here.
from .models import AnkletGeneral, Temperature, Cowculatedlocation, Activitylevel, Microphone,Sociallevel, Stepcount, CowGeneral,Microphoneaverage

admin.site.register(AnkletGeneral)
admin.site.register(Stepcount)
admin.site.register(Cowculatedlocation)
admin.site.register(Activitylevel)
admin.site.register(Sociallevel)
admin.site.register(CowGeneral)
