from django.http import HttpResponse, HttpResponseRedirect
from django.shortcuts import render
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required
from django.views.generic.list import ListView
from django.contrib.auth.models import User
from .forms import UserForm, AnkletForm, CowForm
from .models import CowGeneral, AnkletGeneral, Friendships, Temperature, Activitylevel, Pulse, Stepcount, Sociallevel, Microphoneaverage
from django.db.models import Count, Avg


def index(request): # when the http request comes in..    
    dic = {}
    if request.user.is_authenticated():
        user = request.user
        defaultcow=CowGeneral.objects.filter(iduser=user)
        if defaultcow:
            defaultcownow=defaultcow[0]
            dic['defaultcow'] = defaultcownow.idcow
        else:
            dic['defaultcow'] = 0 #don't have cow yet   
    return render(request,"index.html", dic)

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
            #print ("hello")
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
			anklet = anklet_form.save(commit=False)
			anklet.idcow = CowGeneral.objects.get(cownum=cow_current)
			anklet.save()
			anklet_registered = True
			return HttpResponseRedirect("/")
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



def dashboard(request,pk):
    if request.user.is_authenticated():
    	# gets the cowid as pk
        thecow = {}
        itsfriends={} # nested dictionary 
        itsfriends_social=[]
     
        try:
        	#----- list of user owned cows -------
        	user = request.user
        	thecow['user'] = user
        	thecow['first_name'] = user.first_name
        	owned_cows = CowGeneral.objects.filter(iduser=user)
        	thecow['cow_list'] = owned_cows
        	#----- general info of cow ------------  
        	cow = CowGeneral.objects.get(pk=pk)
        	thecow['cowid'] = pk
        	thecow['cownum'] = cow.cownum
        	thecow['cowname'] = cow.cowname
        	anklet = AnkletGeneral.objects.get(idcow=cow)
        	thecow['ankletnum'] = anklet.ankletnum
        	time = Friendships.objects.latest('lastupdated')
        	friendships = Friendships.objects.filter(idcoworiginal=pk).filter(lastupdated=time.lastupdated).order_by('friendlevel').order_by('-lastupdated')
        	thecow['friendlist'] = []

        	for friendship in friendships[0:3]:
        		friend = friendship.idcowfriend
        		itsfriends['friend_id'] = friend.idcow
        		itsfriends['friend_name'] = friend.cowname
        		itsfriends['friend_number'] = friend.cownum
        		itsfriends['friendship_rank'] = friendship.friendlevel
        		itsfriends['friendship_score'] = friendship.friendshipscore
        		thecow['friendlist'].append(itsfriends.copy())

        except CowGeneral.DoesNotExist or User.DoesNotExist:
        	pass

        # --------- Health Status Aggregation -------------------
        try:
            ave_stand = Activitylevel.objects.filter(idcow=pk).aggregate(Avg('standingpercentage'))
            ave_walk = Activitylevel.objects.filter(idcow=pk).aggregate(Avg('walkingpercentage'))
            ave_run = Activitylevel.objects.filter(idcow=pk).aggregate(Avg('runningpercentage'))
            ave_lying = Activitylevel.objects.filter(idcow=pk).aggregate(Avg('lyingdownpercentage'))
            ave_noise = Microphoneaverage.objects.filter(idcow=pk).aggregate(Avg('micaverage'))
            ave_stepcount = Stepcount.objects.filter(idcow=pk).aggregate(Avg('stepcount'))
            ave_sociallevel = Sociallevel.objects.filter(idcow=pk).aggregate(Avg('idsociallevel'))
            ave_temperature = Temperature.objects.filter(idcow=pk).aggregate(Avg('temperaturelevel'))
            ave_pulse = Pulse.objects.filter(idcow=pk).aggregate(Avg('pulselevel'))
            thecow['ave_stand'] = ave_stand
            thecow['ave_walk'] = ave_walk
            thecow['ave_run'] = ave_run
            thecow['ave_lying'] = ave_lying
            thecow['ave_noise'] = ave_noise
            thecow['ave_stepcount'] = ave_stepcount
            thecow['ave_sociallevel'] = ave_sociallevel
            thecow['ave_temperature'] = ave_temperature
            thecow['ave_pulse'] = ave_pulse
            in_heat = Activitylevel.objects.filter(idcow=pk).latest('idactivitylevel')   
            thecow['in_heat'] = in_heat.inheat
        except Activitylevel.DoesNotExist or Temperature.DoesNotExist or Pulse.DoesNotExist or Microphoneaverage.DoesNotExist or Sociallevel.DoesNotExist or Stepcount.DoesNotExist:
            pass
        return render(request,"dashboard.html", thecow)
    else:
        return HttpResponseRedirect('/login')
        
def required_view(request):
	return HttpResponse("good that you logged in")
	











