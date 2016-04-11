# This is an auto-generated Django model module.
# You'll have to do the following manually to clean this up:
#   * Rearrange models' order
#   * Make sure each model has one field with primary_key=True
#   * Make sure each ForeignKey has `on_delete` set to the desired behavior.
#   * Remove `managed = False` lines if you wish to allow Django to create, modify, and delete the table
# Feel free to rename the models, but don't rename db_table values or field names.
from __future__ import unicode_literals

from django.db import models
from django.contrib.auth.models import User

class CowGeneral(models.Model):
    idcow = models.AutoField(db_column='idCow', primary_key=True)  # Field name made lowercase.
    #idanklet = models.ForeignKey(AnkletGeneral, related_name='cow', on_delete=models.CASCADE, db_column='idAnklet', blank=True, null=True)
    cownum = models.IntegerField(db_column='cowNum', unique=True)  # Field name made lowercase.
    cowname = models.CharField(db_column='cowName', max_length=45, blank=True, null=True)  # Field name made lowercase.
    cowbirthdate = models.DateField(db_column='cowBirthDate', blank=True, null=True)  # Field name made lowercase.
    cowdeathdate = models.DateField(db_column='cowDeathDate', blank=True, null=True)  # Field name made lowercase.
    iduser = models.ForeignKey(User,related_name = 'cow_owner', on_delete=models.CASCADE, db_column='iduser_id', null=True)  # Field name made lowercase.
    
    def __str__(self):
        return self.cowname

    class Meta:
        #managed = False
        db_table = 'cow_general'

class AnkletGeneral(models.Model):
    idanklet = models.AutoField(db_column='idAnklet', primary_key=True)  # Field name made lowercase.
    #iduser = models.ForeignKey(User,related_name = 'owner', on_delete=models.CASCADE, db_column='idUser', blank=True, null=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral,related_name = 'cow_anklet', on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    ankletnum = models.IntegerField(db_column='ankletNum', unique=True, blank=True, null=True)  # Field name made lowercase.
    lastdatareceivedtime = models.DateTimeField(db_column='lastDataReceivedTime', blank=True, null=True)  # Field name made lowercase.
    createddate = models.DateField(db_column='createdDate', blank=True, null=True)  # Field name made lowercase.

    class Meta:
       # managed = False
        db_table = 'anklet_general'





class Accelerometer(models.Model):
    idaccelerometer = models.AutoField(db_column='idAccelerometer', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    idanklet = models.ForeignKey(AnkletGeneral, on_delete=models.CASCADE, db_column='idAnklet', blank=True, default=2)  # Field name made lowercase.
    orientation = models.IntegerField(blank=True, null=True)
    xcoord = models.IntegerField(db_column='xCoord', blank=True, null=True)  # Field name made lowercase.
    ycoord = models.IntegerField(db_column='yCoord', blank=True, null=True)  # Field name made lowercase.
    zcoord = models.IntegerField(db_column='zCoord', blank=True, null=True)  # Field name made lowercase.
    timemeasured = models.DateTimeField(db_column='timeMeasured', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        #managed = False
        db_table = 'accelerometer'


class Activitylevel(models.Model):
    idactivitylevel = models.AutoField(db_column='idactivityLevel', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral,on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    standingpercentage = models.IntegerField(db_column='standingPercentage', blank=True, null=True)  # Field name made lowercase.
    walkingpercentage = models.IntegerField(db_column='walkingPercentage', blank=True, null=True)  # Field name made lowercase.
    runningpercentage = models.IntegerField(db_column='runningPercentage', blank=True, null=True)  # Field name made lowercase.
    lyingdownpercentage = models.IntegerField(db_column='lyingDownPercentage', blank=True, null=True)  # Field name made lowercase.
    sleepingpercentage = models.IntegerField(db_column='sleepingPercentage', blank=True, null=True)  # Field name made lowercase.

    class Meta:
       # managed = False
        db_table = 'activityLevel'




class Cowgroups(models.Model):
    idcowgroups = models.AutoField(db_column='idCowGroups', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    lastupdated = models.DateTimeField(db_column='lastUpdated', blank=True, null=True)  # Field name made lowercase.

    class Meta:
       # managed = False
        db_table = 'cowGroups'

class Staticnode(models.Model):
    idstaticnode = models.AutoField(db_column='idStaticNode', primary_key=True)  # Field name made lowercase.
    iduser = models.ForeignKey(User, on_delete=models.CASCADE,db_column='idUser', blank=True, default=1)  # Field name made lowercase.
    staticnodenum = models.IntegerField(db_column='staticNodeNum', unique=True, blank=True, null=True)  # Field name made lowercase.
    latitude = models.FloatField(blank=True, null=True)
    longitude = models.FloatField(blank=True, null=True)
    xcoord = models.FloatField(db_column='xCoord', blank=True, null=True)  # Field name made lowercase.
    ycoord = models.FloatField(db_column='yCoord', blank=True, null=True)  # Field name made lowercase.
    staticnodecol = models.CharField(db_column='staticNodecol', max_length=45, blank=True, null=True)  # Field name made lowercase.

    class Meta:
       # managed = False
        db_table = 'staticNode'



class Location(models.Model):
    idlocation = models.AutoField(db_column='idLocation', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    idanklet = models.ForeignKey(AnkletGeneral, on_delete=models.CASCADE, db_column='idAnklet', blank=True, default=2)  # Field name made lowercase.
    idstaticnode = models.ForeignKey(Staticnode, on_delete=models.CASCADE, db_column='idStaticNode', blank=True, null=True)  # Field name made lowercase.
    rssivalue = models.IntegerField(db_column='rssiValue', blank=True, null=True)  # Field name made lowercase.
    timemeasured = models.DateTimeField(db_column='timeMeasured', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        #managed = False
        db_table = 'location'

class Cowculatedlocation(models.Model):
    idcowculatedlocation = models.AutoField(db_column='idCowculatedLocation', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral,on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    idanklet = models.ForeignKey(AnkletGeneral, on_delete=models.CASCADE, db_column='idAnklet', blank=True, default=2)  # Field name made lowercase.
    idrssiused = models.ForeignKey(Location, on_delete=models.CASCADE, db_column='idRssiUsed', blank=True, default=1)  # Field name made lowercase.
    latitude = models.FloatField(blank=True, null=True)
    longitude = models.FloatField(blank=True, null=True)
    xcoord = models.FloatField(db_column='xCoord', blank=True, null=True)  # Field name made lowercase.
    ycoord = models.FloatField(db_column='yCoord', blank=True, null=True)  # Field name made lowercase.
    timecalculated = models.DateTimeField(db_column='timeCalculated', blank=True, null=True)  # Field name made lowercase.

    class Meta:
       # managed = False
        db_table = 'cowculatedLocation'


class Friendships(models.Model):
    idfriendships = models.AutoField(db_column='idFriendships', primary_key=True)  # Field name made lowercase.
    idcoworiginal = models.ForeignKey(CowGeneral,on_delete=models.CASCADE,related_name='coworiginal',  db_column='idCowOriginal', blank=True, default=6)  # Field name made lowercase.
    idcowfriend = models.ForeignKey(CowGeneral, on_delete=models.CASCADE,related_name='cowfriend', db_column='idCowFriend', blank=True, default=6)  # Field name made lowercase.
    friendlevel = models.IntegerField(db_column='friendLevel')  # Field name made lowercase.
    friendshipscore = models.IntegerField(db_column='friendshipScore')  # Field name made lowercase.
    lastupdated = models.DateTimeField(db_column='lastUpdated', blank=True, null=True)  # Field name made lowercase.

    class Meta:
       # managed = False
        db_table = 'friendships'





class Microphone(models.Model):
    idmicrophone = models.AutoField(db_column='idMicrophone', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    idanklet = models.ForeignKey(AnkletGeneral,on_delete=models.CASCADE, db_column='idAnklet', blank=True, default=2)  # Field name made lowercase.
    noiselevel = models.IntegerField(db_column='noiseLevel', blank=True, null=True)  # Field name made lowercase.
    timemeasured = models.DateTimeField(db_column='timeMeasured', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        #managed = False
        db_table = 'microphone'


class Pulse(models.Model):
    idpulse = models.AutoField(db_column='idPulse', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    idanklet = models.ForeignKey(AnkletGeneral, on_delete=models.CASCADE, db_column='idAnklet', blank=True, default=2)  # Field name made lowercase.
    pulselevel = models.IntegerField(db_column='pulseLevel', blank=True, null=True)  # Field name made lowercase.
    timemeasured = models.DateTimeField(db_column='timeMeasured', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        #managed = False
        db_table = 'pulse'


class Sociallevel(models.Model):
    idsociallevel = models.AutoField(db_column='idSocialLevel', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    sociallevel = models.IntegerField(db_column='socialLevel', blank=True, null=True)  # Field name made lowercase.
    timecalculated = models.DateTimeField(db_column='timeCalculated', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        #managed = False
        db_table = 'socialLevel'




class Stepcount(models.Model):
    idstepcount = models.AutoField(db_column='idstepCount', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    datastarttime = models.DateTimeField(db_column='dataStartTime', blank=True, null=True)  # Field name made lowercase.
    dataendtime = models.DateTimeField(db_column='dataEndTime', blank=True, null=True)  # Field name made lowercase.
    stepcount = models.IntegerField(db_column='stepCount', blank=True, null=True)  # Field name made lowercase.

    class Meta:
       #managed = False
        db_table = 'stepCount'


class Temperature(models.Model):
    idtemperature = models.AutoField(db_column='idTemperature', primary_key=True)  # Field name made lowercase.
    idcow = models.ForeignKey(CowGeneral, related_name='cow_temperature', on_delete=models.CASCADE, db_column='idCow', blank=True, default=6)  # Field name made lowercase.
    idanklet = models.ForeignKey(AnkletGeneral, on_delete=models.CASCADE, db_column='idAnklet', blank=True, default=1)  # Field name made lowercase.
    temperaturelevel = models.DecimalField(db_column='temperatureLevel',max_digits=5, decimal_places=2, blank=True, null=True)  # Field name made lowercase.
    timemeasured = models.DateTimeField(db_column='timeMeasured', blank=True, null=True)  # Field name made lowercase.

    class Meta:
        #managed = False
        db_table = 'temperature'



