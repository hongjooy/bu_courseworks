from django import forms
from django.forms import TextInput
#from .models import Users
from django.contrib.auth.models import User
from .models import AnkletGeneral, CowGeneral
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
		



class AnkletForm(forms.ModelForm):
	class Meta:
		model = AnkletGeneral
		fields = [
			"ankletnum",
		]

class CowForm(forms.ModelForm):
	class Meta:
		model = CowGeneral
		fields = [

			"cownum",
			"cowname",
			"cowbirthdate",
		]















































