from django import forms
from django.forms import TextInput
from .models import Fitbit
from django.contrib.auth.models import User
from django.contrib.auth import authenticate


class UserForm(forms.ModelForm):
	password = forms.CharField(widget=forms.PasswordInput()) # overwrite the User.password to make it invisible when user types password ****
	class Meta:
		model = User
		fields = [
			"first_name",
			"last_name",
			"username",
			"email",
			"password",
			
		]

# class UserProfileForm(forms.ModelForm):
# 	#re_password = forms.CharField(widget=forms.PasswordInput())
#     class Meta:
#         model = UserProfile
#         fields = [
        
#         	#"website",

        	

#         ]



class FitbitForm(forms.ModelForm):
	class Meta: 
		model = Fitbit
		fields =[
			"cow_name",
			"cow_age",
			"device_id",

		]











































