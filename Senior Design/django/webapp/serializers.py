from rest_framework import serializers
from django.contrib.auth.models import User
from .models import AnkletGeneral, CowGeneral, Temperature


class TemperatureSerializer(serializers.ModelSerializer):
	temperaturelevel = serializers.DecimalField(max_digits=5, decimal_places=2, coerce_to_string= False)
	class Meta:
		model = Temperature
		fields = ('idtemperature','idcow','temperaturelevel','timemeasured')

class AnkletSerializer(serializers.ModelSerializer):
	
	#temperature = TemperatureSerializer(many=True, read_only=True)
	class Meta:
		model = AnkletGeneral
		fields = ('idanklet','ankletnum')


class CowSerializer(serializers.ModelSerializer):
	cow_temperature = TemperatureSerializer(many=True, read_only=True)
	cow_anklet = AnkletSerializer(many=True, read_only=True)
	class Meta: 
		model = CowGeneral
		fields = ('idcow','cownum','cowname','cowbirthdate','cow_temperature','cow_anklet')


class UserSerializer(serializers.ModelSerializer):
	#anklet = serializers.PrimaryKeyRelatedField(many=True, read_only=True) # "related_name" in AnkletGeneral in UserField 
	cow_owner = CowSerializer(many=True, read_only=True)
	class Meta:
		model = User
		fields = ('id','first_name','username','cow_owner')










































# from django.contrib.auth.models import User
# from rest_framework import serializers
# from .models import CowGeneral, AnkletGeneral, Activitylevel, Location, Friendships, Microphone, Sociallevel, Stepcount, Temperature


# class TemperatureSerializer(serializers.ModelSerializer):
# 	temperature = serializers.DecimalField(max_digits=5, decimal_places=2, coerce_to_string= False)
# 	class Meta:
# 		model = Temperature
# 		fields = ('temperaturelevel','timemeasured')

# class AnkletSerializer(serializers.ModelSerializer):
# 	temperature = TemperatureSerializer(many=True)
# 	class Meta:
# 		model = AnkletGeneral
# 		fields = ('idanklet','temperature','iduser','idcow','ankletnum','lastdatareceivedtime','createddate')



# class UserSerializer(serializers.ModelSerializer):
#     anklet = serializers.PrimaryKeyRelatedField(many=True, queryset=AnkletGeneral.objects.all())
#     anklet_data = AnkletSerializer(many=True)
#     class Meta:
#         model = User
#         fields = ('id','first_name', 'username', 'anklet', 'anklet_data')








































# from django.contrib.auth.models import User
# from rest_framework import serializers
# from .models import Anklet, Temperature, Noise



# class TemperatureSerializer(serializers.ModelSerializer):
# 	temperature = serializers.DecimalField(max_digits=5, decimal_places=2, coerce_to_string= False)
# 	timestamp = serializers.DateTimeField()
# 	class Meta:
# 		model = Temperature
# 		field = ('temperature','timestamp')
# 		exclude = ('id', 'anklet')	



# class NoiseSerializer(serializers.ModelSerializer):
# 	noise = serializers.DecimalField(max_digits=5, decimal_places=2, coerce_to_string= False)
# 	timestamp = serializers.DateTimeField()
# 	class Meta:
# 		model = Noise
# 		field = ('noise','timestamp')
# 		exclude = ('id', 'anklet')	



# class AnkletSerializer(serializers.ModelSerializer):
# 	temperature = TemperatureSerializer(many=True) #many True == essential
# 	noise = NoiseSerializer(many = True)
# 	class Meta:
# 		model = Anklet
# 		fields = ('anklet_id', 'temperature','noise')
# 		depth = 1
