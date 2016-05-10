from django.shortcuts import render
from django.http import HttpResponse
from django.contrib.auth.decorators import login_required
from rest_framework.response import Response
from rest_framework.renderers import JSONRenderer
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser
from django.contrib.auth.models import User
from .serializers import UserSerializer, AnkletSerializer, CowSerializer, MicrophoneSerializer, LocationSerializer, SocialLevelSerializer, PulseSerializer, StepcountSerializer, ActivityLevelSerializer, FriendSerializer, CowSimpleSerializer, SocialSimpleSerializer, GroupSerializer
from .models import AnkletGeneral, CowGeneral, Temperature, Microphone, Cowculatedlocation, Pulse, Friendships, Sociallevel, Cowgroups


class JSONResponse(HttpResponse):
    """
    An HttpResponse that renders its content into JSON.
    """
    def __init__(self, data, **kwargs):
        content = JSONRenderer().render(data)
        kwargs['content_type'] = 'application/json'
        super(JSONResponse, self).__init__(content, **kwargs)

#===================== User JSON =========================
@csrf_exempt
def user_list(request):

    if request.method == 'GET':
        users = User.objects.all()
        serializer = UserSerializer(users, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = UserSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)


@csrf_exempt
def user_detail(request, pk):

    try:
        user = User.objects.get(pk=pk)
    except User.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = UserSerializer(user)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = UserSerializer(user, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        user.delete()
        return HttpResponse(status=204)

#========================== Anklet JSON ========================
@csrf_exempt
def anklet_list(request):

    if request.method == 'GET':
        anklets = AnkletGeneral.objects.all()
        serializer = AnkletSerializer(anklets, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = AnkletSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)


@csrf_exempt
def anklet_detail(request, pk):
    """
    Retrieve, update or delete a code snippet.
    """
    try:
        anklet = AnkletGeneral.objects.get(pk=pk)
    except AnkletGeneral.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = AnkletSerializer(anklet)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = AnkletSerializer(anklet, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        anklet.delete()
        return HttpResponse(status=204)


#============================ Cow JSON =======================
@login_required
@csrf_exempt
def cow_list(request):
    """
    List all code snippets, or create a new snippet.
    """
    if request.method == 'GET':
        cows = CowGeneral.objects.all()
        serializer = CowSerializer(cows, many=True)
        return JSONResponse(serializer.data)

    elif request.method == 'POST':
        data = JSONParser().parse(request)
        serializer = CowSerializer(data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data, status=201)
        return JSONResponse(serializer.errors, status=400)

@login_required
@csrf_exempt
def cow_detail(request, pk):
    """
    Retrieve, update or delete a code snippet.
    """
    try:
        cow = CowGeneral.objects.get(pk=pk)
    except CowGeneral.DoesNotExist:
        return HttpResponse(status=404)

    if request.method == 'GET':
        serializer = CowSerializer(cow)
        return JSONResponse(serializer.data)

    elif request.method == 'PUT':
        data = JSONParser().parse(request)
        serializer = CowSerializer(cow, data=data)
        if serializer.is_valid():
            serializer.save()
            return JSONResponse(serializer.data)
        return JSONResponse(serializer.errors, status=400)

    elif request.method == 'DELETE':
        cow.delete()
        return HttpResponse(status=204)


@login_required
@csrf_exempt
def cow_simple(request):
    if request.method == 'GET':
        cow = CowGeneral.objects.all()
        serializer = CowSimpleSerializer(cow, many=True)
        return JSONResponse(serializer.data)

@login_required
@csrf_exempt
def social_simple(request):
    if request.method == 'GET':
        time = Sociallevel.objects.latest('timecalculated');
        friends = Sociallevel.objects.filter(timecalculated=time.timecalculated);
        serializer = SocialSimpleSerializer(friends, many=True)
        return JSONResponse(serializer.data)


@csrf_exempt
def group_list(request):
    if request.method == 'GET':
        time = Cowgroups.objects.latest('lastupdated')
        groups = Cowgroups.objects.filter(lastupdated=time.lastupdated)
        serializer = GroupSerializer(groups, many=True)
        return JSONResponse(serializer.data)

@login_required
@csrf_exempt
def friend_list(request):

    if request.method == 'GET':
       #added
        user = request.user
        owned_cows = CowGeneral.objects.filter(iduser=user);
        owned_cows_id = []
        for cows in owned_cows:
            owned_cows_id.append(cows.idcow);

        time = Friendships.objects.latest('lastupdated');
        friends = Friendships.objects.filter(lastupdated=time.lastupdated)
        friends_own=[]
        for friend in friends:
            #print(friend.idcoworiginal.idcow)
            if friend.idcoworiginal.idcow in owned_cows_id:
                friends_own.append(friend)
        serializer = FriendSerializer(friends_own, many=True)
        return JSONResponse(serializer.data)

@csrf_exempt
def social_list(request):
    if request.method == 'GET':
        socials = Sociallevel.objects.all()
        serializer = SocialLevelSerializer(socials, many=True)
        return JSONResponse(serializer.data)




