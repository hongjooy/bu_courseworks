from rest_framework import serializers
from django.contrib.auth.models import User
from .models import AnkletGeneral, CowGeneral, Microphoneaverage, Cowculatedlocation,Activitylevel, Pulse, Stepcount, Stepcount, Sociallevel, Friendships, Cowgroups


class MicrophoneSerializer(serializers.ModelSerializer):
	micaverage = serializers.FloatField(max_value=None, min_value=None)
	class Meta:
		model = Microphoneaverage
		fields = ('idcow','idmicrophoneaverage','micaverage','timemeasured')

class LocationSerializer(serializers.ModelSerializer):
	latitude = serializers.FloatField(max_value=None, min_value=None)
	longitude = serializers.FloatField(max_value=None, min_value=None)
	class Meta:
		model = Cowculatedlocation
		fields = ('idcowculatedlocation','idcow','latitude','longitude','timecalculated')

class ActivityLevelSerializer(serializers.ModelSerializer):
	standingpercentage = serializers.FloatField(max_value=None, min_value=None)
	walkingpercentage = serializers.FloatField(max_value=None, min_value=None)
	runningpercentage = serializers.FloatField(max_value=None, min_value=None)
	lyingdownpercentage = serializers.FloatField(max_value=None, min_value=None)
	class Meta:
		model = Activitylevel
		fields = ('idcow','idactivitylevel','standingpercentage','walkingpercentage','runningpercentage','lyingdownpercentage','inheat')

class SocialLevelSerializer(serializers.ModelSerializer):
	sociallevel = serializers.IntegerField(max_value=None, min_value=None)
	class Meta:
		model = Sociallevel
		fields = ('idcow','idsociallevel','sociallevel','timecalculated')

class CowSimpleSerializer(serializers.ModelSerializer):
	class Meta:
		model = CowGeneral
		fields = ('idcow','cowname')

class SocialSimpleSerializer(serializers.ModelSerializer):
	idcow = CowSimpleSerializer(read_only=True)
	class Meta:
		model = Sociallevel
		fields = ('idcow','sociallevel')

class GroupSerializer(serializers.ModelSerializer):
	idcow = CowSimpleSerializer(read_only=True)
	class Meta:
		model = Cowgroups
		fields = ('idcow','cowgroupnum')

class FriendSerializer(serializers.ModelSerializer):
	friendshipscore = serializers.IntegerField(max_value=None, min_value=None)
	idcoworiginal = CowSimpleSerializer(read_only=True)
	idcowfriend = CowSimpleSerializer(read_only=True)
	class Meta:
		model = Friendships
		fields = ('idfriendships','idcoworiginal','idcowfriend','friendshipscore')
	
class StepcountSerializer(serializers.ModelSerializer):
	stepcount = serializers.IntegerField(max_value=None, min_value=None)
	class Meta:
		model = Stepcount
		fields = ('idcow','idstepcount','stepcount','datastarttime','dataendtime')

class PulseSerializer(serializers.ModelSerializer):
	pulselevel = serializers.IntegerField(max_value=None, min_value=None)
	class Meta:
		model = Pulse
		fields = ('idcow','idpulse','pulselevel','timemeasured')

class AnkletSerializer(serializers.ModelSerializer):
	class Meta:
		model = AnkletGeneral
		fields = ('idcow','idanklet','ankletnum')

class CowSerializer(serializers.ModelSerializer):
	cow_location = LocationSerializer(many=True, read_only=True)
	cow_noise = MicrophoneSerializer( many=True, read_only=True) 
	cow_anklet = AnkletSerializer(many=True, read_only=True)
	cow_activity = ActivityLevelSerializer(many=True, read_only=True)
	cow_social = SocialLevelSerializer(many=True, read_only=True)
	cow_stepcount = StepcountSerializer(many=True, read_only=True)
	cow_pulse = PulseSerializer(many=True, read_only=True)
	
	class Meta: 
		model = CowGeneral
		fields = (
			'idcow',
			'cownum',
			'cowname',
			'cowbirthdate',
			'cow_location',
			'cow_noise',
			'cow_activity',
			'cow_social',
			'cow_stepcount',
			'cow_pulse',
			'cow_anklet'
			)


class UserSerializer(serializers.ModelSerializer):
	cow_owner = CowSerializer(many=True, read_only=True)
	class Meta:
		model = User
		fields = ('id','first_name','username','cow_owner')




