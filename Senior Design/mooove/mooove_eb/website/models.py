from django.db import models
from django.contrib.auth.models import User

# class UserProfile(models.Model):
#     # This line is required. Links UserProfile to a User model instance.
#     user = models.OneToOneField(User)

#     # The additional attributes we wish to include.
#     #website = models.URLField(blank=True)

    

   
#     def __str__(self):
#         return self.user.username

class Fitbit(models.Model):

	user = models.ForeignKey(User, on_delete=models.CASCADE)
	cow_name = models.CharField(max_length = 20)
	cow_age = models.IntegerField()
	device_id = models.IntegerField()
	
	def __str__(self):
		return self.device_id
