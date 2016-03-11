from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required

from django.views.generic.list import ListView

from .forms import FitbitForm, UserForm#, UserProfileForm
from .models import Fitbit

# Create your views here.


# First Page
def index(request): # when the http request comes in..

   
    return render(request,"index.html", {})


# User Register Page 
def register(request):
    registered = False

    if request.method == 'POST':
        user_form = UserForm(data=request.POST)
        #profile_form = UserProfileForm(data=request.POST)

        if user_form.is_valid(): # and profile_form.is_valid():
            # Save the user's form data to the database.
            user = user_form.save()

            user.set_password(user.password)
            user.save()

         
           # profile = profile_form.save(commit=False)
            #profile.user = user

            # Now we save the UserProfile model instance.
            #profile.save()

            # Update our variable to tell the template registration was successful.
            registered = True

            return HttpResponseRedirect("/login")
     
        else:
        	return render(request,"signup.html", {"error": "Sign Up form is invalid, please check!"})
            #print #(user_form.errors, profile_form.errors)

    
    else:
        user_form = UserForm()
       # profile_form = UserProfileForm()

    context_dict = {
	"user_form": user_form,
	#"profile_form": profile_form,
	"registered": registered,
	}
    return render(request,"signup.html", context_dict)


def user_login(request):

	error = "Invalid Login. Try again!"
	if request.method == 'POST':
		username = request.POST.get('username')
		password = request.POST.get('password')


		user = authenticate(username = username, password = password)

		if user: #if the user exists
			if user.is_active:
				login(request,user)
				return HttpResponseRedirect("/")
			else:
				return HttpResponse("Your account is disabled.")


		else:

			return render(request,"login.html",{"error":error})

		
			#return HttpResponseRedirect("/login")

	else: 
		return render(request,"login.html",{"error":""})



def add_device(request):
	form = FitbitForm(request.POST or None)
	if form.is_valid(): 
		form.save()
		return index(request)

	context_dict = {
		"form": form
	}
	return render(request,"add_device.html", context_dict)

def test(request):
	return render(request,"index.html",{})


@login_required 
def user_logout(request):
    # Since we know the user is logged in, we can now just log them out.
    logout(request)

    # Take the user back to the homepage.
    return HttpResponseRedirect('/')


def required_view(request):
	return HttpResponse("good that you logged in")
	# if request.user.is_authenticated():
	# 	return HttpResponse("You are awesome!")
	# else:
	# 	return HttpResponse("Who the hell are you?! Get out!!")
@login_required
def user_logout(request):
	logout(request)
	return HttpResponseRedirect("/")















