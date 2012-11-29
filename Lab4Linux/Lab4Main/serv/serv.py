#!/usr/bin/env python

port=8000

# Don't edit below this line... unless you want to.

import BaseHTTPServer
import re
import cgi
import urllib
import datetime
from authfile import VerifyGroup


class GroupEntry:
	def __init__(self, ID=0, name="", status="", data=""):
		self.ID=ID
		self.name=cgi.escape(name)
		self.status=cgi.escape(status)
		self.data=cgi.escape(data)
		self.date=datetime.datetime.now()
	def __str__(self):
		return ("<tr>"+
				"<td class=\"thinborder\">" + str(self.ID) + "</td>" +
				"<td class=\"thinborder\">" + str(self.name) + "</td>" +
				"<td class=\"thinborder\">" + str(self.data) + "</td>" +
				"<td class=\"thinborder\">" + str(self.status) + "</td>" +
				#"<td class=\"thinborder\">" + str(self.date) + "</td>" +
				"<td class=\"thinborder\">" + str(self.date.strftime("%a, %b %d, %Y at %I:%M %P")) +"</td>"+
				"</tr>")

groupdata={}
def GenGroupTable():
	# Creates a table of all collected group information
	result="<table cellspacing=\"0\">"
	result+="<tr><th class=\"thinborder\">ID</th><th class=\"thinborder\">Group Name</th><th class=\"thinborder\">Value</th><th class=\"thinborder\">Status</th><th class=\"thinborder\">Last Update</th></tr>"
	for i in groupdata:
		result+=str(groupdata[i])+"\n"
	result+="</table>"
	return result

def ParseQuery(query):
	# Creates a dictionary of name/value parse from a URL query
	try:
		query=re.sub(".*\?", '', query)
		querysplit=re.split("&", query)
		params={}
		for el in querysplit:
			elsplit=re.split("=",el)
			params[elsplit[0]]=urllib.unquote_plus(elsplit[1])
		return params
	except:
		return {}

def ParseQuery_AddGroup(query):
	# Throws exception at any sign of failure (unparsable URL query, incorrect parameters, incorrect credentials)
	params=ParseQuery(query)
	g=GroupEntry(params['id'],params['name'],params['status'],params['data'])
	if(VerifyGroup(int(params['id']),params['password'])):
		groupdata[g.ID]=g
	else:
		raise Exception('No. Stop that. Go away.')

class Handler(BaseHTTPServer.BaseHTTPRequestHandler):
	def do_GET(self):
		print str(self.client_address[0])+ " GET \"" + self.path + "\""
		realpath=re.sub("\?.*",'',self.path)
		if(realpath=="/"):
			realpath="/index.html"
		elif(realpath=="/update"):
			try:
				ParseQuery_AddGroup(self.path)
				realpath='/submitted.html'
			except:
				realpath='/failed.html'
		try:
			root="html"
			HTTP_OK= "HTTP/1.1 200 OK\nContent-type:text/html\n\n"
			HTTP_404="HTTP/1.1 404 Not Found\nContent-type:text/html\n\nError: 404"
			f=open(root+realpath,'r')
			self.wfile.write(HTTP_OK)
			table=GenGroupTable()
			for line in f:
				self.wfile.write(re.sub('--GROUPTABLE--', table, line))
			f.close()
		except:
			self.wfile.write(HTTP_404)

# How do I shot web?
BaseHTTPServer.HTTPServer(('',port), Handler).serve_forever()
