from django.shortcuts import render
from django.http import HttpResponse
#from rest_framework.decorators import api_view
from rest_framework.response import Response
from rest_framework.renderers import JSONRenderer
from django.views.decorators.csrf import csrf_exempt
from rest_framework.parsers import JSONParser
from django.contrib.auth.models import User
from .serializers import UserSerializer, AnkletSerializer, CowSerializer
from .models import AnkletGeneral, CowGeneral



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
    """
    List all code snippets, or create a new snippet.
    """
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
    """
    Retrieve, update or delete a code snippet.
    """
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
    """
    List all code snippets, or create a new snippet.
    """
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

