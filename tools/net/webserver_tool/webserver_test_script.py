#!/usr/bin/env python3
############################################################################
#
# Copyright 2021 Samsung Electronics All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing,
# software distributed under the License is distributed on an
# "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
# either express or implied. See the License for the specific
# language governing permissions and limitations under the License.
#
############################################################################

import requests
import time
import random
import string

from requests.exceptions import HTTPError
from prettytable import PrettyTable


#############  Chunk generator ####################

def generator(data):
	if(is_chunk == 0):
		return data

	i = 1
	while i * chunk_size < total_data :
		chunkstr = hex(chunk_size)[2:] + "\r\n" + str(i) + ''.join(random.choice(string.ascii_lowercase) for _ in range(chunk_size - len(str(i)))) + "\r\n"
		#print(chunkstr)
		yield chunkstr.encode('utf-8')
		time.sleep(0.000002)
		i+=1

	rem_data = total_data - ((i-1) * chunk_size)
	chunkstr = hex(rem_data)[2:] + "\r\n"
	if(len(str(i)) > rem_data) :
		chunkstr += ''.join(random.choice(string.ascii_lowercase) for _ in range(rem_data)) + "\r\n"
	else :
		chunkstr += str(i) + ''.join(random.choice(string.ascii_lowercase) for _ in range(rem_data - len(str(i)))) + "\r\n"
	#print(chunkstr)
	yield chunkstr.encode('utf-8')
	time.sleep(0.000002)
	if is_trailer :
		yield "0\r\nExpires: Fri, 21 Oct 2022 07:28:00 GMT\r\n\r\n".encode('utf-8')
	else :
		yield "0\r\n\r\n".encode('utf-8')


def check_is_chunked_or_trailer(headers):
	if is_trailer :
		headers['TE'] = 'trailers'
		headers['Trailer'] = 'Expires'
		return
	if(is_chunk):
		headers['TE'] = 'chunked'


def check_is_keep_alive(headers):
	if(is_keep_alive):
		headers['Connection'] = 'keep-alive'
	else :
		headers['Connection'] = 'close'


#############  GET    ####################

def get(total):
	GET_P=0
	GET_F=0
	print("-------- GET test Start -------")
	start_time = time.time()
	for i in range(1,total+1):
		print("--------Test {}/{}-------".format(i,total))
		if(get_request()):
			print("\nTest Case: Pass\n")
			GET_P+=1
		else :
			print("\nTest Case: Fail\n")
			GET_F+=1
	print("---Test took %.6s seconds ---" % (time.time() - start_time))
	print('keep-alive', is_keep_alive, ':: chunk', is_chunk, ' ::')
	myTable = PrettyTable(["Test_Type", "Total_checks", "Pass", "Fail"])
	myTable.add_row(["GET", total, GET_P, GET_F])
	print(myTable)
	return GET_P,GET_F


def get_request():
	headers = {'Accept':'application/json', 'user-agent': 'test_SRID', 'samsung':'Delhi'}
	check_is_keep_alive(headers)
	check_is_chunked_or_trailer(headers)
	s = requests.session()
	print("Headers Sent:")
	for key,value in headers.items():
		print(key+" : "+value)
	P, F = 0, 0
	for i in range(keep_alive_iter):
		try:
			r = s.get(url, headers=headers, verify=cert)
			if(r.status_code==200):
				if(is_keep_alive and (r.headers.get('Connection')=='close' or r.headers.get('Connection')=='close, close')):
					F += 1
					continue
				print("\nResponse Status : "+str(r.status_code)+ "\nResponse Phrase : " +str(r.reason))
				for key,value in r.headers.items():
					print(key+" : "+value)
				print("Content received : "+r.text)
				P += 1
			else :
				F += 1
			r.raise_for_status()
		except requests.exceptions.RequestException as e:
			print(e)
			F += 1
	s.close()
	if(P == keep_alive_iter):
		return True
	return False


####################### POST ##########################

def post(total):
	POST_P=0
	POST_F=0
	print("-------- POST test Start -------")
	start_time = time.time()
	for i in range(1,total+1):
		print("--------Test {}/{}-------".format(i,total))
		if(post_request()):
			print("\nTest Case: Pass\n")
			POST_P+=1
		else :
			print("\nTest Case: Fail\n")
			POST_F+=1
	print("---Test took %.6s seconds ---" % (time.time() - start_time))
	print('keep-alive', is_keep_alive, ':: chunk', is_chunk, ':', str(chunk_size)+'B, '+str(total_data)+'B ::' if is_chunk else '', 'trailer', is_trailer, '::')
	myTable = PrettyTable(["Test_Type", "Total_checks", "Pass", "Fail"])   
	myTable.add_row(["POST", total, POST_P, POST_F]) 		  
	print(myTable)
	return POST_P,POST_F


def post_request():
	payload = {'user-id':'abc','password':'125.00'}
	headers = {'user-agent': 'test_SRID', 'samsung':'Delhi'}
	check_is_keep_alive(headers)
	check_is_chunked_or_trailer(headers)
	s = requests.session()
	print("Headers Sent:")
	for key,value in headers.items():
		print(key+" : "+value)
	P, F = 0, 0
	for i in range(keep_alive_iter):
		try:
			r = s.post(url, headers=headers, data=generator(payload), verify=cert)
			if(r.status_code==200):
				if(is_keep_alive and (r.headers.get('Connection')=='close' or r.headers.get('Connection')=='close, close')):
					F += 1
					continue
				print("\nResponse Status : "+str(r.status_code)+ "\nResponse Phrase : " +str(r.reason))
				for key,value in r.headers.items():
					print(key+" : "+value)
				print("Content received : "+r.text)
				P += 1
			else :
				F += 1
			r.raise_for_status()
		except requests.exceptions.RequestException as e:
			print(e)
			F += 1
	s.close()
	if(P == keep_alive_iter):
		return True
	return False


####################### PUT ##########################

def put(total):
	PUT_P=0
	PuT_F=0
	print("-------- PUT test Start -------")
	start_time = time.time()
	for i in range(1,total+1):
		print("--------Test {}/{}-------".format(i,total))
		if(put_request()):
			print("\nTest Case: Pass\n")
			PUT_P+=1
		else :
			print("\nTest Case: Fail\n")
			PuT_F+=1
	print("---Test took %.6s seconds ---" % (time.time() - start_time))
	print('keep-alive', is_keep_alive, ':: chunk', is_chunk, ':', str(chunk_size)+'B, '+str(total_data)+'B ::' if is_chunk else '', 'trailer', is_trailer, '::')
	myTable = PrettyTable(["Test_Type", "Total_checks", "Pass", "Fail"])
	myTable.add_row(["PUT", total, PUT_P, PuT_F])
	print(myTable)
	return PUT_P,PuT_F

def put_request():
	payload = {'user-id':'abc','password':'125.00'}
	headers = {'user-agent': 'test_SRID', 'samsung':'Delhi'}
	check_is_keep_alive(headers)
	check_is_chunked_or_trailer(headers)
	s = requests.session()
	print("Headers Sent:")
	for key,value in headers.items():
		print(key+" : "+value)
	P, F = 0, 0
	for i in range(keep_alive_iter):
		try:
			r = s.put(url, headers=headers, data=generator(payload), verify=cert)
			if(r.status_code==200):
				if(is_keep_alive and (r.headers.get('Connection')=='close' or r.headers.get('Connection')=='close, close')):
					F += 1
					continue
				print("\nResponse Status : "+str(r.status_code)+ "\nResponse Phrase : " +str(r.reason))
				for key,value in r.headers.items():
					print(key+" : "+value)
				print("Content received : "+r.text)
				P += 1
			else :
				F += 1
			r.raise_for_status()
		except requests.exceptions.RequestException as e:
			print(e)
			F += 1
	s.close()
	if(P == keep_alive_iter):
		return True
	return False


####################### DELETE ##########################

def delt(total):
	DEL_P=0
	DEL_F=0
	print("-------- DELETE test Start -------")
	start_time = time.time()
	for i in range(1,total+1):
		print("--------Test {}/{}-------".format(i,total))
		if(del_request()):
			print("\nTest Case: Pass\n")
			DEL_P+=1
		else :
			print("\nTest Case: Fail\n")
			DEL_F+=1
	print("---Test took %.6s seconds ---" % (time.time() - start_time))
	print('keep-alive', is_keep_alive, ' ::')
	myTable = PrettyTable(["Test_Type", "Total_checks", "Pass", "Fail"])
	myTable.add_row(["DELETE", total, DEL_P, DEL_F])
	print(myTable)
	return DEL_P,DEL_F

def del_request():
	payload = {'user-id':'abc','email':'abc@xyz.com'}
	headers = {'user-agent': 'test_SRID', 'samsung':'Delhi'}
	check_is_keep_alive(headers)
	s = requests.session()
	print("Headers Sent:")
	for key,value in headers.items():
		print(key+" : "+value)
	P, F = 0, 0
	for i in range(keep_alive_iter):
		try:
			r = s.delete(url, headers=headers, data=payload, verify=cert)
			if(r.status_code==200):
				if(is_keep_alive and (r.headers.get('Connection')=='close' or r.headers.get('Connection')=='close, close')):
					F += 1
					continue
				print("\nResponse Status : "+str(r.status_code)+ "\nResponse Phrase : " +str(r.reason))
				for key,value in r.headers.items():
					print(key+" : "+value)
				print("Content received : "+r.text)
				P += 1
			else :
				F += 1
			r.raise_for_status()
		except requests.exceptions.RequestException as e:
			print(e)
			F += 1
	s.close()
	if(P == keep_alive_iter):
		return True
	return False


#############  ALL    ####################

def all(total):
	GET_P,GET_F=get(total)
	print()
	POST_P,POST_F=post(total)
	print()
	PUT_P,PuT_F=put(total)
	print()
	DEL_P,DEL_F=delt(total)
	print("\n\n-------------- SUMMARY ------------------")
	print('keep-alive', is_keep_alive, ':: chunk', is_chunk, ':', str(chunk_size)+'B, '+str(total_data)+'B ::' if is_chunk else '', 'trailer', is_trailer, '::')
	myTable = PrettyTable(["Test_Type", "Total_checks", "Pass", "Fail"]) 
	myTable.add_row(["GET", total, GET_P, GET_F])
	myTable.add_row(["POST", total, POST_P, POST_F])
	myTable.add_row(["PUT", total, PUT_P, PuT_F])
	myTable.add_row(["DELETE", total, DEL_P, DEL_F]) 		  
	print(myTable)


#################   INPUT ###########################

print("Select Connection Type :\n 1>HTTP\t\t 2>HTTPS")
connection_type=int(input())
if(connection_type==1):
	url = 'http://192.168.0.102/'
elif(connection_type==2):
	url = 'https://192.168.0.102/'
else:
	print("Invalid input")
	exit()
print(' --- (server) --- {}  --- '.format(url))


cert='cert.pem'
print("Select request type\n 1>GET\t\t 2>POST\t\t 3>PUT\n 4>DELETE\t 5>ALL")
test_type=int(input())
if(test_type>5 or test_type<1):
	print('Invalid Input')
	exit()
is_chunk = 0
if(test_type == 1 or test_type == 2 or test_type == 3 or test_type == 5):
	print("Select chunk mode: 0/1 :: ")
	is_chunk = int(input() or '0')
if(is_chunk and test_type != 1):
	print("Enter each chunk's size in Bytes (100B to 1024B, default is 1024B): ")
	chunk_size = int(input() or '1024')
	print("Enter whole data size in KBs (64KB to 1024KB, default is 1024KB): ")
	total_data = int(input() or '1024')
	total_data *= 1024
	print("total_data =", total_data, "Bytes, chunk_size =", chunk_size, "Bytes")

is_trailer = 0
if is_chunk :
	print('Enter if trailer header: 0/1 ::')
	is_trailer = int(input() or '0')


print("Select keep-alive mode: 0/1 :: ")
is_keep_alive = int(input())
keep_alive_iter = 1	# useful when keep-alive is off
if(is_keep_alive):
	print("Requests' count for one session of keep-alive (default is 5): ")
	keep_alive_iter = int(input() or '5')


print("Select test iterations : ")
iterations=int(input())


if(test_type==1):
	get(iterations)
elif (test_type==2):
	post(iterations)
elif (test_type==3):
	put(iterations)
elif(test_type==4) :
	delt(iterations)
elif(test_type==5) :
	all(iterations)
