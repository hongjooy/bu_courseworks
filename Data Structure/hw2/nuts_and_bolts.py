# AUTHOR1: Jooyoun Hong hongjooy@bu.edu
# AUTHOR2: Andy Shen shena@bu.edu


# nuts = [n2,n1,n3,n4,n7,n5,n6,n8,n9]
# bolts = [b3,b4,b6,b1,b8,b9,b7,b5,b2]

# sample TEST function
# def TEST(nut,bolt):
# 	if nut > bolt:
# 		return "nut is too big"
# 	elif nut < bolt:
# 		return "nut is too small"
# 	else:
# 		return "nut fits perfectly"



def NutsnBolts(nuts, bolts):
	new_nuts = [0]*len(nuts)
	new_bolts = [0]*len(bolts)
	output_list = [0]*len(bolts)
	for n in nuts: #for this nut
		smaller = 0
		temp = 0
		for b in bolts: # at the end of this for loop, we know its order in the list, and its partner bolt
			if TEST(n,b) == "nut is too big":
				smaller += 1; # the number of nuts and bolts that are smaller than the current!
			elif TEST(n,b) == "nut fits perfectly":
				temp = b
		new_nuts[smaller] = n
		new_bolts[smaller] = temp
		output_list[smaller] = str(n)+ " matches "+str(temp);
	
	return output_list


# #sample demo
# a = [2,1,3,4,7,5,6,8,9]
# b = [3,4,6,1,8,9,7,5,2]


# print(NutsnBolts(a,b))





			




