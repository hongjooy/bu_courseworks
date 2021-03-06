# -*- coding: utf-8 -*-
# Generated by Django 1.9.2 on 2016-04-08 02:45
from __future__ import unicode_literals

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('website', '0022_remove_ankletgeneral_iduser'),
    ]

    operations = [
        migrations.AlterField(
            model_name='accelerometer',
            name='idaccelerometer',
            field=models.AutoField(db_column='idAccelerometer', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='accelerometer',
            name='idanklet',
            field=models.ForeignKey(blank=True, db_column='idAnklet', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.AnkletGeneral'),
        ),
        migrations.AlterField(
            model_name='accelerometer',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='activitylevel',
            name='idactivitylevel',
            field=models.AutoField(db_column='idactivityLevel', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='activitylevel',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='ankletgeneral',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, related_name='cow_anklet', to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='cowculatedlocation',
            name='idanklet',
            field=models.ForeignKey(blank=True, db_column='idAnklet', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.AnkletGeneral'),
        ),
        migrations.AlterField(
            model_name='cowculatedlocation',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='cowculatedlocation',
            name='idcowculatedlocation',
            field=models.AutoField(db_column='idCowculatedLocation', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='cowculatedlocation',
            name='idrssiused',
            field=models.ForeignKey(blank=True, db_column='idRssiUsed', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.Location'),
        ),
        migrations.AlterField(
            model_name='cowgroups',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='cowgroups',
            name='idcowgroups',
            field=models.AutoField(db_column='idCowGroups', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='friendships',
            name='idcowfriend',
            field=models.ForeignKey(blank=True, db_column='idCowFriend', default=1, on_delete=django.db.models.deletion.CASCADE, related_name='cowfriend', to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='friendships',
            name='idcoworiginal',
            field=models.ForeignKey(blank=True, db_column='idCowOriginal', default=1, on_delete=django.db.models.deletion.CASCADE, related_name='coworiginal', to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='friendships',
            name='idfriendships',
            field=models.AutoField(db_column='idFriendships', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='location',
            name='idanklet',
            field=models.ForeignKey(blank=True, db_column='idAnklet', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.AnkletGeneral'),
        ),
        migrations.AlterField(
            model_name='location',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='location',
            name='idlocation',
            field=models.AutoField(db_column='idLocation', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='microphone',
            name='idanklet',
            field=models.ForeignKey(blank=True, db_column='idAnklet', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.AnkletGeneral'),
        ),
        migrations.AlterField(
            model_name='microphone',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='microphone',
            name='idmicrophone',
            field=models.AutoField(db_column='idMicrophone', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='pulse',
            name='idanklet',
            field=models.ForeignKey(blank=True, db_column='idAnklet', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.AnkletGeneral'),
        ),
        migrations.AlterField(
            model_name='pulse',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='pulse',
            name='idpulse',
            field=models.AutoField(db_column='idPulse', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='sociallevel',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='sociallevel',
            name='idsociallevel',
            field=models.AutoField(db_column='idSocialLevel', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='staticnode',
            name='idstaticnode',
            field=models.AutoField(db_column='idStaticNode', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='staticnode',
            name='iduser',
            field=models.ForeignKey(blank=True, db_column='idUser', default=1, on_delete=django.db.models.deletion.CASCADE, to=settings.AUTH_USER_MODEL),
        ),
        migrations.AlterField(
            model_name='stepcount',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='stepcount',
            name='idstepcount',
            field=models.AutoField(db_column='idstepCount', primary_key=True, serialize=False),
        ),
        migrations.AlterField(
            model_name='temperature',
            name='idanklet',
            field=models.ForeignKey(blank=True, db_column='idAnklet', default=1, on_delete=django.db.models.deletion.CASCADE, to='website.AnkletGeneral'),
        ),
        migrations.AlterField(
            model_name='temperature',
            name='idcow',
            field=models.ForeignKey(blank=True, db_column='idCow', default=1, on_delete=django.db.models.deletion.CASCADE, related_name='cow_temperature', to='website.CowGeneral'),
        ),
        migrations.AlterField(
            model_name='temperature',
            name='idtemperature',
            field=models.AutoField(db_column='idTemperature', primary_key=True, serialize=False),
        ),
    ]
