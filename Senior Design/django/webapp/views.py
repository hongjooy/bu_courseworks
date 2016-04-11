from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required

from django.views.generic.list import ListView
from django.contrib.auth.models import User
from .forms import UserForm, AnkletForm, CowForm
from .models import CowGeneral, AnkletGeneral

# Create your views here.


# First Page
def index(request): # when the http request comes in..

   
    return render(request,"index.html", {})


# User Register Page 
def register(request):
    registered = False

    if request.method == 'POST':
        user_form = UserForm(data=request.POST)
       

        if user_form.is_valid(): # and profile_form.is_valid():
           
            user = user_form.save()

            user.set_password(user.password)
            user.save()

            registered = True

            context_dict = {
				"user_form": user_form,
				"registered": registered,
			}

            return HttpResponseRedirect("/login")
     
        else:
        	return render(request,"signup.html", {"error": "Sign Up form is invalid, please check!"})
            #print #(user_form.errors, profile_form.errors)

    
    else:
        user_form = UserForm()
       # profile_form = UserProfileForm()

    context_dict = {
		"user_form": user_form,
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



@login_required
def anklet_register(request):
	anklet_registered = False
	
	if request.method == 'POST':
		anklet_form = AnkletForm(data=request.POST)
		cow_form = CowForm(data=request.POST)

		if anklet_form.is_valid() and cow_form.is_valid():
			
			cow = cow_form.save(commit=False)
			
			cownum_clean = cow_form.cleaned_data.get("cownum")
			cow.cownum = cownum_clean
			cow.iduser = request.user
			cow.save()

			cow_current = cow.cownum
			#current_user = auth.get_user(request)

			
			anklet = anklet_form.save(commit=False)
			anklet.idcow = CowGeneral.objects.get(cownum=cow_current)
			anklet.save()
			#anklet_obj = anklet.ankletnum

			anklet_registered = True
			
			return HttpResponseRedirect("/dashboard")
		else:
			return render(request,"anklet.html",{"error": "Anklet Register Form is invalid, please check!"})
	else:
		anklet_form = AnkletForm()
		cow_form = CowForm()

	context_dict = {
				"anklet_form": anklet_form,
				"cow_form": cow_form,
				"anklet_registered": anklet_registered,
			}

	return render(request,"anklet.html",context_dict)

			
@login_required 
def user_logout(request):
    # Since we know the user is logged in, we can now just log them out.
    logout(request)

    # Take the user back to the homepage.
    return HttpResponseRedirect('/')


def dashboard(request):
	if request.user.is_authenticated():
		return render(request,"dashboard.html")
	else:
		return HttpResponse("Get out. Now.")

def required_view(request):
	return HttpResponse("good that you logged in")
	# if request.user.is_authenticated():
	# 	return HttpResponse("You are awesome!")
	# else:
	# 	return HttpResponse("Who the hell are you?! Get out!!")












