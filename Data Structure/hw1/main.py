# AUTHOR1: Jooyoun Hong hongjooy@bu.edu
import sys

def Analyze (str, size):

	# str = "I like apples. I like oranges."
	# size = 6
	count = 0; #counts until the size; reset when it reaches the size
	list_count = 0; #counts the elements in the list; increases whenever 
	newlist = []
	app_str = ""
	for n in range(0,len(str)):
		for x in range (0, size):
			if (list_count+x) <= (len(str)-1): # to prevent out of range error
				app_str = app_str + str[list_count + x]
		newlist.append(app_str)
		list_count = list_count + 1
		app_str = ""


	if len(str) >= size:
		return((max(set(newlist), key=newlist.count)))

	else:
		return (newlist[0])



inputstr =sys.argv[1]
inputnum = sys.argv[2]


inputnum = int(inputnum) # user input size will be considered as string, and the function won't take it. need to change int


print (Analyze(inputstr,inputnum))

